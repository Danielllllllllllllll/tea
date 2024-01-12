#include <bits/stdc++.h>

using namespace std;
typedef pair<int, int> PII;

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
#define F first
#define S second
#define mkp make_pair

const int INF = 1e9;
bool debugMode = 0;

enum Action{
    UP, LEFT, DOWN, RIGHT
};

// class Random{
//    public:
//     int genRandom(int from, int to) {
//         int range = to - from + 1;
//         return (rand() % range) + from;
//     }
//     void setRandom() {
//         srand(time(NULL));
//     }
// };

class Timer{
   private:
    clock_t start_time;
    clock_t stop_time;

   public:
    void startTimer() {
        start_time = clock();
    }
    void stopTimer() {
        stop_time = clock();
    }
    double getTime() {
        return (double(stop_time) - double(start_time)) / CLOCKS_PER_SEC;
    }
};

struct Pri{
    int diff, depth, high;
    pair< PII, int > cmp;
    Pri(int di, int de, int hi){
        diff = di;
        depth = de;
        high = hi;
        cmp = mkp(mkp(di, hi), de);
    }
    bool operator == (Pri b){
        return this->diff == b.diff && this->depth == b.depth && this->high == b.high;
    }
    // bool operator < (Pri b){
    //     if(this->diff == b.diff){
    //         if(this->depth == b.depth){
    //             return this->high < b.high;
    //         }
    //         return this->depth < depth;
    //     }
    //     return this->diff < b.diff;
    // }
    // bool operator <= (Pri b){
    //     if(this->diff == b.diff){
    //         if(this->depth == b.depth){
    //             return this->high <= b.high;
    //         }
    //         return this->depth <= depth;
    //     }
    //     return this->diff <= b.diff;
    // }
    // bool operator > (Pri b){
    //     if(this->diff == b.diff){
    //         if(this->depth == b.depth){
    //             return this->high > b.high;
    //         }
    //         return this->depth > depth;
    //     }
    //     return this->diff > b.diff;
    // }
    // bool operator >= (Pri b){
    //     if(this->diff == b.diff){
    //         if(this->depth == b.depth){
    //             return this->high >= b.high;
    //         }
    //         return this->depth >= depth;
    //     }
    //     return this->diff >= b.diff;
    // }
    bool operator < (Pri b){
        return this->cmp < b.cmp;
    }
    bool operator <= (Pri b){
        return this->cmp <= b.cmp;
    }
    bool operator > (Pri b){
        return this->cmp > b.cmp;
    }
    bool operator >= (Pri b){
        return this->cmp >= b.cmp;
    }
};

class Bot : public Timer{
   public:
    Bot(int round, int height, int width);
    void init();
    string makeDecision();

   private:
    Pri minimax(int depth, Pri alpha, Pri beta, bool mxp, int scoreHi, int dMax);
    PII doAction(PII pos, Action act);
    string actToS(Action act);
    int Eat(int score, PII pos, bool mxp);
    bool vPos(PII pos);
    void setAct();
    void isBomb(bool mxp, PII pos);

    vector< Action > actALL;
    PII posMe, posOp;
    int rd, wid, hei, scoreMe, scoreOp, cdMe, cdOp, steps, weightB;
    vector< vector< char > > grid;
    char Me, Op;
    vector< vector< int > > test, test2;
    // vector< int > scoreMax, dMax;
};

Bot::Bot(int round, int height, int width){
    rd = round;
    wid = width;
    hei = height;
}

void Bot::setAct(){
    double dir[4] = {};
    for(int i=0; i<hei; i++){
        for(int j=0; j<wid; j++){
            double tmp = abs(i - posMe.F) + abs(j - posMe.S);
            double tmp2 = abs(i - posOp.F) + abs(j - posOp.S);
            double r = ((i - posMe.F) * (i - posOp.F) <= 0 && (j - posMe.S) * (j - posOp.S) <= 0 ? 2 : 1);
            r *= (tmp > tmp2 ? 0.5 : 1);
            if(i < posMe.F) dir[0] += (double) (scoreMe * (grid[i][j] == 's') - scoreMe * (grid[i][j] == 't' && (abs(i - posMe.F) + abs(j - posMe.S)) == 1) / 2 + (grid[i][j] == 'm')) * r / tmp;
            if(i > posMe.F) dir[2] += (double) (scoreMe * (grid[i][j] == 's') - scoreMe * (grid[i][j] == 't' && (abs(i - posMe.F) + abs(j - posMe.S)) == 1) / 2 + (grid[i][j] == 'm')) * r / tmp;
            if(j < posMe.S) dir[1] += (double) (scoreMe * (grid[i][j] == 's') - scoreMe * (grid[i][j] == 't' && (abs(i - posMe.F) + abs(j - posMe.S)) == 1) / 2 + (grid[i][j] == 'm')) * r / tmp;
            if(j > posMe.S) dir[3] += (double) (scoreMe * (grid[i][j] == 's') - scoreMe * (grid[i][j] == 't' && (abs(i - posMe.F) + abs(j - posMe.S)) == 1) / 2 + (grid[i][j] == 'm')) * r / tmp;
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(dir[i] > dir[j]){
                swap(dir[i], dir[j]);
                Action tmp = actALL[i];
                actALL[i] = actALL[j];
                actALL[j] = tmp;
            }
        }
    }
    if(debugMode)
        for(int i=0; i<4; i++)
            cout << actToS(actALL[i]) << " " << dir[i] << '\n';
}

string Bot::actToS(Action act){
    switch(act){
        case UP:
            return "UP";
        case LEFT:
            return "LEFT";
        case DOWN:
            return "DOWN";
        case RIGHT:
            return "RIGHT";
    }
    return "FALSE";
}

void Bot::init(){
    grid = vector< vector< char > > (wid, vector< char > (hei, 0));
    test = vector< vector< int > > (wid, vector< int > (hei, 0));
    test2 = vector< vector< int > > (wid, vector< int > (hei, 0));
    // scoreMax = vector< int > (4, 0);
    // dMax = vector< int > (4, 0);
    for(int i=0; i<hei; i++){
        for(int j=0; j<wid; j++){
            cin >> grid[i][j];
        }
    }
    int scoreA, scoreB;
    cin >> scoreA >> scoreB >> Me;
    Op = (Me == 'A' ? 'B' : 'A');
    scoreMe = (Me == 'A' ? scoreA : scoreB);
    scoreOp = (Me == 'A' ? scoreB : scoreA);
    for(int i=0; i<hei; i++){
        for(int j=0; j<wid; j++){
            if(grid[i][j] == Me) posMe = mkp(i, j);
            if(grid[i][j] == Op) posOp = mkp(i, j);
        }
    }
    int dEdge = 4;
    steps = (posMe.F < dEdge ? dEdge - posMe.F : 0) + (hei - posMe.F < dEdge ? dEdge - (hei - posMe.F) : 0) + (posMe.S < dEdge ? dEdge - posMe.S : 0) + (wid - posMe.S < dEdge ? dEdge - (wid - posMe.S) : 0);
    steps += (posOp.F < dEdge ? dEdge - posOp.F : 0) + (hei - posOp.F < dEdge ? dEdge - (hei - posOp.F) : 0) + (posOp.S < dEdge ? dEdge - posOp.S : 0) + (wid - posOp.S < dEdge ? dEdge - (wid - posOp.S) : 0);
    steps = steps / 6 * 2 + 14;
    steps = min((1000 - rd + 1) * 2, steps) - (Me == 'B'); // (wid + hei);
    if(debugMode)
        cout << "steps = " << steps << '\n';
    cdMe = cdOp = 0;
    // setRandom();
    actALL = vector< Action > (4);
    actALL[0] = UP;
    actALL[1] = LEFT;
    actALL[2] = DOWN;
    actALL[3] = RIGHT;

    weightB = 4;
    // queue< pair< PII, int > > que;
    // dis = vector< vector< int > > (hei, vector< int > (wid, -1));
    // que.push(mkp(posMe, 0));
    // while(que.size()){
    //     pair< PII, int > tmp = que.front();
    //     que.pop();
    //     if((tmp.F.S < 0 || tmp.F.S >= wid || tmp.F.F < 0 || tmp.F.F >= hei || grid[tmp.F.F][tmp.F.S] == 'B' || grid[tmp.F.F][tmp.F.S] == 'x') || dis[tmp.F.F][tmp.F.S] > -1) continue;
    //     dis[tmp.F.F][tmp.F.S] = tmp.S;
    //     for(int i=0; i<4; i++){
    //         que.push(mkp(doAction(tmp.F, actALL[i]), tmp.S+1));
    //     }
    // }
    // if(debugMode){
    //     for(int i=0; i<hei; i++){
    //         for(int j=0; j<wid; j++){
    //             cout << dis[i][j] << " ";
    //         }
    //         cout << '\n';
    //     }
    //     cout << '\n';
    // }
    setAct();
}

PII Bot::doAction(PII pos, Action act){
    switch (act) {
        case UP:
            return mkp(pos.F - 1, pos.S);
        case LEFT:
            return mkp(pos.F, pos.S - 1);
        case DOWN:
            return mkp(pos.F + 1, pos.S);
        case RIGHT:
            return mkp(pos.F, pos.S + 1);
    }
    return pos;
}

bool Bot::vPos(PII pos){
    return ((pos.S < 0 || pos.S >= wid || pos.F < 0 || pos.F >= hei || grid[pos.F][pos.S] == 'A' || grid[pos.F][pos.S] == 'B' || grid[pos.F][pos.S] == 'x') ? false : true);
}

int Bot::Eat(int score, PII pos, bool mxp){
    if(!vPos(pos))
        return -INF;
    switch (grid[pos.F][pos.S]) {
        case 'm':
            return score + 1;
        case 'n':
            return score - 1;
        case 's':
            return score * 2;
        case 't':
            return score / 2;
        case 'b':
            return score + weightB * ((abs(posMe.F + posMe.S - posOp.F - posOp.S) % 2) != mxp ? -1 : 1);
        default:
            return score;
    }
}

void Bot::isBomb(bool mxp, PII pos){
    if(grid[pos.F][pos.S] == 'b'){
        if(mxp) cdMe = 3;
        else cdOp = 3;
    }
}

Pri Bot::minimax(int depth, Pri alpha, Pri beta, bool mxp, int scoreHi, int dMax){
    // bool flag = true;
    if(depth <= 0){
        if(debugMode){
            test[posMe.F][posMe.S] = max(test[posMe.F][posMe.S], scoreMe);
            test2[posOp.F][posOp.S] = max(test2[posOp.F][posOp.S], scoreOp);
        }
        // if(scoreMe > scoreMax[k]){
        //     scoreMax[k] = scoreMe;
        //     dMax[k] = 0;
        // }
        return Pri(scoreMe - scoreOp, dMax, scoreHi);
    }
    if(mxp){
        Pri maxEval = Pri(-INF, depth, -INF);
        PII posTmp = posMe;
        int scoreTmp = scoreMe;
        if(cdMe){
            cdMe--;
            Pri eval = minimax(depth - 1, alpha, beta, false, scoreHi, dMax);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            cdMe++;
        }
        else{
            grid[posMe.F][posMe.S] = '.';
            for(int i=0; i<4; i++){
                Action act = actALL[i];
                posMe = doAction(posTmp, act);
                if(!vPos(posMe)) continue;
                // flag = false;
                scoreMe = Eat(scoreTmp, posMe, false);
                if(scoreMe > scoreHi){
                    scoreHi = scoreMe;
                    dMax = depth;
                }
                else if(scoreMe == scoreHi){
                    dMax = max(dMax, depth);
                }
                isBomb(mxp, posMe);
                char it = grid[posMe.F][posMe.S];
                grid[posMe.F][posMe.S] = Me;
                Pri eval = minimax(depth - 1, alpha, beta, false, scoreHi, dMax);
                // if(scoreMe == scoreTmp){
                //     maxEval = max(maxEval, Pri(eval.diff, depth, eval.high));
                // }
                // else 
                    maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                grid[posMe.F][posMe.S] = it;
                cdMe = 0;
                if(beta <= alpha) break;
            }
        }
        posMe = posTmp;
        scoreMe = scoreTmp;
        grid[posMe.F][posMe.S] = Me;
        // if(debugMode && maxEval.diff == INF){
        //     cout << posMe.F << " " << posMe.S << " " << posOp.F << " " << posOp.S << " " << maxEval.depth << '\n';
        // }
        if(debugMode){
            test[posMe.F][posMe.S] = max(test[posMe.F][posMe.S], scoreMe);
            test2[posOp.F][posOp.S] = max(test2[posOp.F][posOp.S], scoreOp);
        }
        return maxEval;
    }
    else{
        Pri minEval = Pri(INF, depth, INF);
        PII posTmp = posOp;
        int scoreTmp = scoreOp;
        if(cdOp){
            cdOp--;
            Pri eval = minimax(depth - 1, alpha, beta, true, scoreHi, dMax);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            cdOp++;
            // flag = false;
        }
        else{
            grid[posOp.F][posOp.S] = '.';
            for(int i=0; i<4; i++){
                Action act = actALL[i];
                posOp = doAction(posTmp, act);
                if(!vPos(posOp)) continue;
                // flag = false;
                scoreOp = Eat(scoreTmp, posOp, true);
                if(scoreMe > scoreHi){
                    scoreHi = scoreMe;
                    dMax = depth;
                }
                else if(scoreMe == scoreHi){
                    dMax = max(dMax, depth);
                }
                isBomb(mxp, posOp);
                char it = grid[posOp.F][posOp.S];
                grid[posOp.F][posOp.S] = Op;
                Pri eval = minimax(depth - 1, alpha, beta, true, scoreHi, dMax);
                // if(scoreMe == scoreTmp){
                //     minEval = min(minEval, Pri(eval.diff, depth, eval.high));
                // }
                // else 
                    minEval = min(minEval, eval);
                beta = min(beta, eval);
                grid[posOp.F][posOp.S] = it;
                cdOp = 0;
                if(beta <= alpha) break;
            }
        }
        posOp = posTmp;
        scoreOp = scoreTmp;
        grid[posOp.F][posOp.S] = Op;
        // if(debugMode){ // && minEval.diff == INF){
        //     cout << posMe.F << " " << posMe.S << " " << posOp.F << " " << posOp.S << " " << minEval.diff << " " << minEval.depth << " " << minEval.high << '\n';
        // }
        // if(flag){ // !vPos(doAction(posOp, Action::UP)) && !vPos(doAction(posOp, Action::LEFT)) && !vPos(doAction(posOp, Action::DOWN)) && !vPos(doAction(posOp, Action::RIGHT))){
        //     if(INF > scoreMax[k]){
        //         scoreMax[k] = INF;
        //         dMax[k] = depth;
        //         cout << posMe.F << " " << posMe.S << " " << posOp.F << " " << posOp.S << " " << minEval.diff << '\n';
        //     }
        //     else if(INF == scoreMax[k]){
        //         dMax[k] = max(dMax[k], depth);
        //     }
        // }
        if(debugMode){
            test[posMe.F][posMe.S] = max(test[posMe.F][posMe.S], scoreMe);
            test2[posOp.F][posOp.S] = max(test2[posOp.F][posOp.S], scoreOp);
        }
        return minEval;
    }
}

string Bot::makeDecision(){
    Action dir;
    Pri maxEval = Pri(-INF, steps, scoreMe);
    int k;

    Pri alpha = Pri(-INF, steps, -INF), beta = Pri(INF, steps, INF);
    PII posTmp = posMe;
    int scoreTmp = scoreMe;
    grid[posMe.F][posMe.S] = '.';
    for(int i=0; i<4; i++){
        Action act = actALL[i];
        posMe = doAction(posTmp, act);
        if(!vPos(posMe)) continue;
        if(actToS(dir) == "FALSE"){
            dir = act;
            k = i;
        }
        scoreMe = Eat(scoreTmp, posMe, false);
        // if(scoreMe > scoreMax[i]){
        //     scoreMax[i] = scoreMe;
        //     dMax[i] = steps;
        // }
        // else if(scoreMe == scoreMax[i]){
        //     dMax[i] = max(dMax[i], steps);
        // }
        isBomb(1, posMe);
        char it = grid[posMe.F][posMe.S];
        grid[posMe.F][posMe.S] = Me;
        Pri eval = minimax(steps-1, alpha, beta, false, scoreMe, steps);
        if(eval > maxEval){
            maxEval = eval;
            dir = act;
            k = i;
        }
        // scoreMax[i] = eval.high;
        // dMax[i] = eval.depth;
        grid[posMe.F][posMe.S] = it;
        cdMe = 0;
        // cout << '\n';
        if(debugMode){
            cout << eval.diff << " " << eval.depth << " " << eval.high << '\n';
        }
    }
    posMe = posTmp;
    scoreMe = scoreTmp;
    grid[posMe.F][posMe.S] = Me;

    if(debugMode){
        // for(int i=0; i<4; i++){
        //     cout << scoreMax[i] << " " << dMax[i] << '\n';
        // }
        cout << maxEval.diff << '\n';
        for(int i=0; i<hei; i++){
            for(int j=0; j<wid; j++){
                cout << test[i][j] << " ";
            }
            cout << '\n';
        }
        cout << '\n';
        for(int i=0; i<hei; i++){
            for(int j=0; j<wid; j++){
                cout << test2[i][j] << " ";
            }
            cout << '\n';
        }
    }

    return actToS(dir);
}

signed main(void){
    ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);

    int T, m, n; cin >> T >> m >> n;
    Bot bot(T, m, n);
    if(debugMode)
        bot.startTimer();
    bot.init();
    cout << bot.makeDecision() << '\n';
    if(debugMode){
        bot.stopTimer();
        cout << bot.getTime() << '\n';
    }

    return 0;
}





// function minimax(position, depth, alpha, beta, maximizingPlayer)
//     if depth == 0 or game over in position
//         return static evaluation of position
 
//     if maximizingPlayer
//         maxEval = -infinity
//         for each child of position
//             eval = minimax(child, depth - 1, alpha, beta, false)
//             maxEval = max(maxEval, eval)
//             alpha = max(alpha, eval)
//             if beta <= alpha
//                 break
//         return maxEval
 
//     else
//         minEval = +infinity
//         for each child of position
//             eval = minimax(child, depth - 1, alpha, beta, true)
//             minEval = min(minEval, eval)
//             beta = min(beta, eval)
//             if beta <= alpha
//                 break
//         return minEval
 
 
// // initial call
// minimax(currentPosition, 3, -∞, +∞, true)
