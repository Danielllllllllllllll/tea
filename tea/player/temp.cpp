#include <bits/stdc++.h>
#include <cstring>

using namespace std;
typedef pair<int, int> PII;

#define F first
#define S second
#define mkp make_pair

const int INF = 1e9;

enum Action {
    UP, LEFT, DOWN, RIGHT
};

Action actALL[4] = {LEFT, UP, DOWN, RIGHT};

class Bot {
	public:
		Bot(int round, int height, int width);
		void init();
		string makeDecision();

	private:
		PII minimax(int depth, PII alpha, PII beta, int mxp, string mpath, string opath);
		PII doAction(PII pos, Action act);
		string actToS(Action act);
		int Eat(int score, PII pos);
		bool vPos(PII pos);
		PII posMe, posOp;
		int rd, wid, hei, scoreMe, scoreOp, cdA, cdB, x;
		vector< vector< char > > grid;
		char Me, Op;
};

Bot::Bot(int round, int height, int width) {
	rd = round;
	wid = width;
	hei = height;
}

string Bot::actToS(Action act) {
	switch(act) {
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

void Bot::init() {
	grid = vector< vector< char > > (wid, vector< char > (hei, 0));
	for(int i=0; i<hei; i++) {
		for(int j=0; j<wid; j++) {
			cin >> grid[i][j];
		}
	}
	int scoreA, scoreB;
	cin >> scoreA >> scoreB >> Me;
	Op = (Me == 'A' ? 'B' : 'A');
	scoreMe = (Me == 'A' ? scoreA : scoreB);
	scoreOp = (Me == 'A' ? scoreB : scoreA);
	for(int i=0; i<hei; i++) {
		for(int j=0; j<wid; j++) {
			if(grid[i][j] == Me) posMe = mkp(i, j);
			if(grid[i][j] == Op) posOp = mkp(i, j);
		}
	}
	x = 16;
	cdA = cdB = 0;
	//cout << scoreA << scoreB << Me << endl;
}

PII Bot::doAction(PII pos, Action act) {
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

bool Bot::vPos(PII pos) {
	return ((pos.S < 0 || pos.S >= wid || pos.F < 0 || pos.F >= hei || grid[pos.F][pos.S] == 'A' || grid[pos.F][pos.S] == 'B' || grid[pos.F][pos.S] == 'x') ? false : true);
}

int Bot::Eat(int score, PII pos) {
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
		case '.':
		case 'b':
		default:
			return score;
	}
}

PII Bot::minimax(int depth, PII alpha, PII beta, int mxp, string mpath, string opath) {
	// if(i<0 || i>=m || j<0 || j>=n || grid[i][j] == 'x' || (mxp ? grid[i][j] + me == 'A' + 'B' : grid[i][j] == me))
	//     return -INF;
	
	if(depth == 0) {
		//cout << depth << " " << alpha.F << " " << beta.F << " " << mxp << " " << mpath << " " << opath << endl;
		//cout << scoreMe << " " << scoreOp << " " << scoreMe - scoreOp << endl;
		return mkp(scoreMe - scoreOp, 0);
	}	
	if(mxp != 0) {
		PII maxEval = mkp(-INF, depth);
		PII posTmp = posMe;
		int scoreTmp = scoreMe;
		string tmpath;		
		grid[posMe.F][posMe.S] = '.';
		for(int i=0; i<4; i++) { // int act = Action::UP; act != Action::RIGHT; act++){
			Action act = actALL[i];
			posMe = doAction(posTmp, act);
			if(!vPos(posMe)) continue;
			tmpath = mpath + actToS(act)[0];
			scoreMe = Eat(scoreTmp, posMe);
			char it = grid[posMe.F][posMe.S];
			grid[posMe.F][posMe.S] = Me;
			PII eval = minimax(depth - 1, alpha, beta, 0, tmpath, opath);
			if (eval.F > maxEval.F || (eval.F == maxEval.F && eval.S > maxEval.S)) {
				maxEval = eval;
				//cout << "Selected" << endl;
			}
			if (eval.F > alpha.F || (eval.F == alpha.F && eval.S > alpha.S)) {
				alpha = eval;
			}
			grid[posMe.F][posMe.S] = it;
			if(alpha.F > beta.F || (alpha.F == beta.F && alpha.S > beta.S)) break;
		}
		posMe = posTmp;
		scoreMe = scoreTmp;
		grid[posMe.F][posMe.S] = Me;
		//cout << depth << " " << alpha.F << " " << beta.F << " " << mxp << " " << mpath << " " << opath << endl;
		//cout << scoreMe << " " << scoreOp << " " << scoreMe - scoreOp << endl;

		return maxEval;
	} else {

		PII minEval = mkp(INF, depth);
		PII posTmp = posOp;
		int scoreTmp = scoreOp;
		string topath;
		grid[posOp.F][posOp.S] = '.';
		for(int i=0; i<4; i++) { // int act = Action::UP; act != Action::RIGHT; act++){
			Action act = actALL[i];
			posOp = doAction(posTmp, act);
			if(!vPos(posOp)) continue;
			topath = opath + actToS(act)[0];
			scoreOp = Eat(scoreTmp, posOp);
			char it = grid[posOp.F][posOp.S];
			grid[posOp.F][posOp.S] = Op;
			PII eval = minimax(depth - 1, alpha, beta, 1, mpath, topath);
			if (eval.F < minEval.F || (eval.F == minEval.F && eval.S < minEval.S)) {
				minEval = eval;
				//cout << "Selected" << endl;
			}
			if (eval.F < beta.F || (eval.F == beta.F && eval.S < beta.S)) {
				beta = eval;
			}
			grid[posMe.F][posMe.S] = it;
			if(alpha.F > beta.F || (alpha.F == beta.F && alpha.S > beta.S)) break;
		}
		posOp = posTmp;
		scoreOp = scoreTmp;
		grid[posOp.F][posOp.S] = Op;
		//cout << depth << " " << alpha.F << " " << beta.F << " " << mxp << " " << mpath << " " << opath << endl;
		//cout << scoreMe << " " << scoreOp << " " << scoreMe - scoreOp << endl;
		return minEval;
	}
}

string Bot::makeDecision() {
	Action dir;
	PII maxEval = mkp(-INF, x+1);

	PII alpha = mkp(-INF, x+1), beta = mkp(INF, x+1);
	PII posTmp = posMe;
	int scoreTmp = scoreMe;
	string mpath, opath;
	grid[posMe.F][posMe.S] = '.';
	for(int i=0; i<4; i++) { // int act = Action::UP; act != Action::RIGHT; act++){
		Action act = actALL[i];		
		posMe = doAction(posTmp, act);
		if(!vPos(posMe)) continue;
		//cout << posMe.first << posMe.second << endl;
		mpath = actToS(act)[0];
		opath = "";
		scoreMe = Eat(scoreTmp, posMe);
		char it = grid[posMe.F][posMe.S];
		grid[posMe.F][posMe.S] = Me;
		//int eval = minimax(wid+hei, alpha, beta, 0, mpath, opath);
		PII eval = minimax(x, alpha, beta, 0, mpath, opath);
		if(eval.F > maxEval.F || (eval.F == maxEval.F && eval.S > maxEval.S)) {
			maxEval = eval;
			dir = act;			
		}
		//cout << actToS(act) << " " << eval.F << " " << eval.S << endl; 
		grid[posMe.F][posMe.S] = it;
	}
	posMe = posTmp;
	scoreMe = scoreTmp;
	grid[posMe.F][posMe.S] = Me;
	return actToS(dir);
}

signed main(void) {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	int T, m, n;
	cin >> T >> m >> n;
	Bot bot(T, m, n);
	// bot.startTimer();
	bot.init();
	cout << bot.makeDecision() << endl;
	// bot.stopTimer();

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
// minimax(currentPosition, 3, -¡Û, +¡Û, true)
