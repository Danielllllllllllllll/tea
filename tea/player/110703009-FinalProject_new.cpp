#include<stdio.h>

int main(void){
        int round,n1,n2,score1,score2;
        char ab;
        int map[25][25]={};
        int object[25][25]={};
        int bfsA[25][25]={};
        int bfsB[25][25]={};
        int undeveloped[100000][2]={};
        int memAB[2][2];
        int attack[25][25]={};
        scanf("%d",&round);
        scanf("%d%d",&n1,&n2);
        for(int xx=0 ; xx<n1+2 ; ++xx){
                for(int xxx=0 ; xxx<n2+2 ; ++xxx){
                        if(xx==0 || xxx==0 || xx==n2+1 || xxx==n2+1){
                                map[xx][xxx]=-1;
                                object[xx][xxx]=-1;
                                bfsA[xx][xxx]=-1;
                                bfsB[xx][xxx]=-1;
                                attack[xx][xxx]=-1;
                        }
                }
        }
        for(int a=1 ; a<n1+1 ; ++a){
                for(int b=1 ; b<n2+1 ; ++b){
                        char c;
                        scanf("%c",&c);
                        while(c=='\0' || c=='\n' || c==' ')
                                scanf("%c",&c);
                        if(c=='x' || c=='A' || c=='B'){
                                map[a][b]=-1;
                                bfsA[a][b]=-1;
                                bfsB[a][b]=-1;
                        }
                        else{
                                map[a][b]=-2;
                                bfsA[a][b]=-2;
                                bfsB[a][b]=-2;
                        }

                        if(c=='x'){
                                object[a][b]=-1;
                                attack[a][b]=-1;
                        }
                        else if(c=='m')
                                object[a][b]=1;
                        else if(c=='n')
                                object[a][b]=2;
                        else if(c=='s')
                                object[a][b]=3;
                        else if(c=='t'){
                                object[a][b]=4;
                        }
                        else if(c=='b'){
                                object[a][b]=5;
                                bfsA[a][b]=-3;
                                bfsB[a][b]=-3;
                                attack[a][b]=-3;
                        }
                        else if(c=='.')
                                object[a][b]=6;
                        else if(c=='A'){
                                object[a][b]=7;
                                memAB[0][0]=a;
                                memAB[0][1]=b;
                        }
                        else if(c=='B'){
                                object[a][b]=8;
                                memAB[1][0]=a;
                                memAB[1][1]=b;
                        }
                }
        }
        int scoreA,scoreB;
        char who;
        scanf("%d %d ",&scoreA,&scoreB);
        scanf("%c",&who);
        int sx,sy;
        if(who=='A'){
                sx=memAB[0][0];
                sy=memAB[0][1];
        }
        else{
                sx=memAB[1][0];
                sy=memAB[1][1];
        }
        int path[25][25][25][2]={};
        path[sx][sy][0][0]=sx;
        path[sx][sy][0][1]=sy;
        int score[25][25]={};
        if(who=='A')
                score[sx][sy]=scoreA;
        else
                score[sx][sy]=scoreB;
        int direction[25][25]={};
        int step[25][25]={};
        int max=0;
        int wheremax[2]={sx,sy};
        int stepofmax=0;
        int dx[5]={0,-1,1,0,0};
        int dy[5]={0,0,0,-1,1};
        int waitingA=0,waitingB=0,nextA=0,nextB=0;
        int sxA=memAB[0][0],syA=memAB[0][1],sxB=memAB[1][0],syB=memAB[1][1];
        int undevelopedA[100000][2]={},undevelopedB[100000][2]={};
        undevelopedA[waitingA][0]=sxA;
        undevelopedA[waitingA][1]=syA;
        ++waitingA;
        bfsA[sxA][syA]=0;
        bfsA[memAB[1][0]][memAB[1][1]]=-2;
        for(; nextA<waitingA ; ++nextA){
                sxA=undevelopedA[nextA][0];
                syA=undevelopedA[nextA][1];
                for(int xy=1 ; xy<5 ; ++xy){
                        if(bfsA[sxA+dx[xy]][syA+dy[xy]]==-2){
                                bfsA[sxA+dx[xy]][syA+dy[xy]]=bfsA[sxA][syA]+1;
                                undevelopedA[waitingA][0]=sxA+dx[xy];
                                undevelopedA[waitingA][1]=syA+dy[xy];
                                ++waitingA;
                        }
                        else if(bfsA[sxA+dx[xy]][syA+dy[xy]]==-3){
                                bfsA[sxA+dx[xy]][syA+dy[xy]]=bfsA[sxA][syA]+4;
                                undevelopedA[waitingA][0]=sxA+dx[xy];
                                undevelopedA[waitingA][1]=syA+dy[xy];
                                ++waitingA;
                        }
                }
        }
        undevelopedB[waitingB][0]=sxB;
        undevelopedB[waitingB][1]=syB;
        ++waitingB;
        bfsB[sxB][syB]=0;
        bfsB[memAB[0][0]][memAB[0][1]]=-2;
        for(; nextB<waitingB ; ++nextB){
                sxB=undevelopedB[nextB][0];
                syB=undevelopedB[nextB][1];
                for(int xy=1 ; xy<5 ; ++xy){
                        if(bfsB[sxB+dx[xy]][syB+dy[xy]]==-2){
                                bfsB[sxB+dx[xy]][syB+dy[xy]]=bfsB[sxB][syB]+1;
                                undevelopedB[waitingB][0]=sxB+dx[xy];
                                undevelopedB[waitingB][1]=syB+dy[xy];
                                ++waitingB;
                        }
                        else if(bfsB[sxB+dx[xy]][syB+dy[xy]]==-3){
                                bfsB[sxB+dx[xy]][syB+dy[xy]]=bfsB[sxB][syB]+4;
                                undevelopedB[waitingB][0]=sxB+dx[xy];
                                undevelopedB[waitingB][1]=syB+dy[xy];
                                ++waitingB;
                        }
                }
        }
        int distanceAB;
        distanceAB=bfsA[memAB[1][0]][memAB[1][1]];
        int whereshit[100000][2]={};
        int shitcount=0;
        for(int q=0 ; q<n1+2 ; ++q){
                for(int qq=0 ; qq<n2+2 ; ++qq){
                        int ifshit=0;
                        for(int xy=1 ; xy<5 ; ++xy){
                                if(attack[q+dx[xy]][qq+dy[xy]]==-1 && attack[q][qq]!=-1)
                                        ifshit+=1;
                        }
                        if(ifshit==3){
                                whereshit[shitcount][0]=q;
                                whereshit[shitcount][1]=qq;
                                shitcount+=1;
                        }
                }
        }
        int bfsATT[25][25]={};
        int ban[5]={};
        for(int gg=0 ; gg<shitcount ; ++gg){
                int att[25][25]={};
                for(int p=0 ; p<n1+2 ; ++p){
                        for(int pp=0 ; pp<n2+2 ; ++pp){
                                att[p][pp]=attack[p][pp];
                        }
                }
                int undevelopedATT[100000][2]={};
                int sxATT,syATT;
                int waitingATT=0,nextATT=0;
                sxATT=whereshit[gg][0];
                syATT=whereshit[gg][1];
                att[sxATT][syATT]=3;
                undevelopedATT[waitingATT][0]=sxATT;
                undevelopedATT[waitingATT][1]=syATT;
                ++waitingATT;
                for(; nextATT<waitingATT ; ++nextATT){
                        sxATT=undevelopedATT[nextATT][0];
                        syATT=undevelopedATT[nextATT][1];
                        for(int xy=1 ; xy<5 ; ++xy){
                                if(att[sxATT+dx[xy]][syATT+dy[xy]]==0){
                                        int count=0;
                                        for(int xxyy=1 ; xxyy<5 ; ++xxyy){
                                                if(att[sxATT+dx[xy]+dx[xxyy]][syATT+dy[xy]+dy[xxyy]]==-1)
                                                        count+=1;
                                        }
                                        if(count==1 || count==0){
                                                count=0;
                                                break;
                                        }
                                        att[sxATT+dx[xy]][syATT+dy[xy]]=count;
                                        undevelopedATT[waitingATT][0]=sxATT+dx[xy];
                                        undevelopedATT[waitingATT][1]=syATT+dy[xy];
                                        ++waitingATT;
                                }
                        }
                }
                int maybeA;
                maybeA=bfsA[whereshit[gg][0]][whereshit[gg][0]];
                int maybeB;
                maybeB=bfsB[whereshit[gg][0]][whereshit[gg][0]];
                if(who=='A'){
                        if(att[memAB[1][0]][memAB[1][1]]!=0){
                                if(distanceAB%2==0){
                                        int min=100000;
                                        for(int y=0 ; y<n1+2 ; ++y){
                                                for(int yy=0 ; yy<n2+2 ; ++yy){
                                                        if(att[y][yy]==0){
                                                                if(bfsB[y][yy]<min)
                                                                        min=bfsB[y][yy];
                                                        }
                                                }
                                        }
                                        if(min+2*maybeB>=distanceAB-min && 1001-round>maybeA){
                                                for(int z=0 ; z<n1+2 ; ++z){
                                                        for(int zz=0 ; zz<n2+2 ; ++zz){
                                                                if(bfsA[z][zz]==1 && bfsA[z][zz]+bfsB[z][zz]==distanceAB){
                                                                        if(z-memAB[0][0]==-1)
                                                                                printf("UP\n");
                                                                        else if(z-memAB[0][0]==1)
                                                                                printf("DOWN\n");
                                                                        else if(zz-memAB[0][1]==-1)
                                                                                printf("LEFT\n");
                                                                        else if(zz-memAB[0][1]==1)
                                                                                printf("RIGHT\n");
                                                                        return 0;
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
                }
                else if(who=='B'){
                        if(att[memAB[0][0]][memAB[0][1]]!=0){
                                if(distanceAB%2==0){
                                        int min=100000;
                                        for(int y=0 ; y<n1+2 ; ++y){
                                                for(int yy=0 ; yy<n2+2 ; ++yy){
                                                        if(att[y][yy]==0){
                                                                if(bfsA[y][yy]<min)
                                                                        min=bfsA[y][yy];
                                                        }
                                                }
                                        }
                                        if(min+maybeA*2>=distanceAB-min && 1001-round>maybeB){
                                                for(int z=0 ; z<n1+2 ; ++z){
                                                        for(int zz=0 ; zz<n2+2 ; ++zz){
                                                                if(bfsB[z][zz]==1 && bfsB[z][zz]+bfsA[z][zz]==distanceAB){
                                                                        if(z-memAB[1][0]==-1)
                                                                                printf("UP\n");
                                                                        else if(z-memAB[1][0]==1)
                                                                                printf("BOWN\n");
                                                                        else if(zz-memAB[1][1]==-1)
                                                                                printf("LEFT\n");
                                                                        else if(zz-memAB[1][1]==1)
                                                                                printf("RIGHT\n");
                                                                        return 0;
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
                }
                if(distanceAB%2==1){
                        if(who=='A'){
                                for(int xy=1 ; xy<5 ; ++xy){
                                        if(att[memAB[0][0]+dx[xy]][memAB[0][1]+dy[xy]]>0){
                                                int bfsa[25][25]={};
                                                for(int a=0 ; a<n1+2 ; ++a){
                                                        for(int aa=0 ; aa<n2+2 ; ++aa){
                                                                if(bfsA[a][aa]==-1)
                                                                        bfsa[a][aa]=-1;
                                                                else
                                                                        bfsa[a][aa]=-2;
                                                                if( (a==memAB[0][0] && aa==memAB[0][1]) || (a==memAB[1][0] && aa==memAB[1][1]) )
                                                                        bfsa[a][aa]=-2;
                                                        }
                                                }
                                                int waitinga=0,nexta=0;
                                                int sxa=memAB[0][0]+dx[xy],sya=memAB[0][1]+dy[xy];
                                                if(bfsa[sxa][sya]==-1)
                                                        break;
                                                int undevelopeda[100000][2]={};
                                                undevelopeda[waitinga][0]=sxa;
                                                undevelopeda[waitinga][1]=sya;
                                                ++waitinga;
                                                bfsa[sxa][sya]=0;
                                                bfsa[memAB[1][0]][memAB[1][1]]=-2;
                                                for(; nexta<waitinga ; ++nexta){
                                                        sxa=undevelopeda[nexta][0];
                                                        sya=undevelopeda[nexta][1];
                                                        for(int xya=1 ; xya<5 ; ++xya){
                                                                if(bfsa[sxa+dx[xya]][sya+dy[xya]]==-2){
                                                                        bfsa[sxa+dx[xya]][sya+dy[xya]]=bfsa[sxa][sya]+1;
                                                                        undevelopeda[waitinga][0]=sxa+dx[xya];
                                                                        undevelopeda[waitinga][1]=sya+dy[xya];
                                                                        ++waitinga;
                                                                }
                                                                else if(bfsa[sxa+dx[xya]][sya+dy[xya]]==-3){
                                                                        bfsa[sxa+dx[xya]][sya+dy[xya]]=bfsa[sxa][sya]+4;
                                                                        undevelopeda[waitinga][0]=sxa+dx[xy];
                                                                        undevelopeda[waitinga][1]=sya+dy[xy];
                                                                        ++waitinga;
                                                                }
                                                        }
                                                }
                                                int min=100000;
                                                for(int y=0 ; y<n1+2 ; ++y){
                                                        for(int yy=0 ; yy<n2+2 ; ++yy){
                                                                if(att[y][yy]==0){
                                                                        if(bfsa[y][yy]<min)
                                                                                min=bfsa[y][yy];
                                                                }
                                                        }
                                                }
                                                if(distanceAB+1-(min)<=(min) && 1001-round>=maybeB){
                                                        ban[xy]=-1;
                                                        break;
                                                }
                                        }
                                }
                        }
                        else if(who=='B'){
                                for(int xy=1 ; xy<5 ; ++xy){
                                        if(att[memAB[1][0]+dx[xy]][memAB[1][1]+dy[xy]]>0){
                                                int bfsb[25][25]={};
                                                for(int b=0 ; b<n1+2 ; ++b){
                                                        for(int bb=0 ; bb<n2+2 ; ++bb){
                                                                if(bfsB[b][bb]==-1)
                                                                        bfsb[b][bb]=-1;
                                                                else
                                                                        bfsb[b][bb]=-2;
                                                                if( (b==memAB[0][0] && bb==memAB[0][1]) || (b==memAB[1][0] && bb==memAB[1][1]) )
                                                                        bfsb[b][bb]=-2;
                                                        }
                                                }
                                                int waitingb=0,nextb=0;
                                                int sxb=memAB[1][0]+dx[xy],syb=memAB[1][1]+dy[xy];
                                                if(bfsb[sxb][syb]==-1)
                                                        break;
                                                int undevelopedb[100000][2]={};
                                                undevelopedb[waitingb][0]=sxb;
                                                undevelopedb[waitingb][1]=syb;
                                                ++waitingb;
                                                bfsb[sxb][syb]=0;
                                                bfsb[memAB[0][0]][memAB[0][1]]=-2;
                                                for(; nextb<waitingb ; ++nextb){
                                                        sxb=undevelopedb[nextb][0];
                                                        syb=undevelopedb[nextb][1];
                                                        for(int xyb=1 ; xyb<5 ; ++xyb){
                                                                if(bfsb[sxb+dx[xyb]][syb+dy[xyb]]==-2){
                                                                        bfsb[sxb+dx[xyb]][syb+dy[xyb]]=bfsb[sxb][syb]+1;
                                                                        undevelopedb[waitingb][0]=sxb+dx[xyb];
                                                                        undevelopedb[waitingb][1]=syb+dy[xyb];
                                                                        ++waitingb;
                                                                }
                                                                else if(bfsb[sxb+dx[xyb]][syb+dy[xyb]]==-3){
                                                                        bfsb[sxb+dx[xyb]][syb+dy[xyb]]=bfsb[sxb][syb]+4;
                                                                        undevelopedb[waitingb][0]=sxb+dx[xy];
                                                                        undevelopedb[waitingb][1]=syb+dy[xy];
                                                                        ++waitingb;
                                                                }
                                                        }
                                                }
                                                int min=100000;
                                                for(int y=0 ; y<n1+2 ; ++y){
                                                        for(int yy=0 ; yy<n2+2 ; ++yy){
                                                                if(att[y][yy]==0){
                                                                        if(bfsB[y][yy]<min)
                                                                                min=bfsB[y][yy];
                                                                }
                                                        }
                                                }
                                                if(distanceAB+1-(min)<=(min) && 1001-round>=maybeA){
                                                        ban[xy]=-1;
                                                        break;
                                                }
                                        }
                                }
                        }
                }
        }
        int newmap[25][25][25][25]={};
        for(int w=0 ; w<n1+2 ; ++w){
                for(int ww=0 ; ww<n2+2 ; ++ww){
                        newmap[sx][sy][w][ww]=object[w][ww];
                        if(object[w][ww]>=6)
                                newmap[sx][sy][w][ww]=6;
                }
        }
        int waiting=0;
        undeveloped[waiting][0]=sx;
        undeveloped[waiting][1]=sy;
        int next=0;
        ++waiting;
        for(; next<waiting ; ++next){
                sx=undeveloped[next][0];
                sy=undeveloped[next][1];
                for(int xy=1 ; xy<5 ; ++xy){
                        if(ban[xy]==-1 && next==0)
                                break;
                        if(map[sx+dx[xy]][sy+dy[xy]]==-2){
                                int distanceA,distanceB;
                                distanceA=bfsA[sx+dx[xy]][sy+dy[xy]];
                                distanceB=bfsB[sx+dx[xy]][sy+dy[xy]];
                                if(distanceA<0)
                                        distanceA=10000;
                                if(distanceB<0)
                                        distanceB=10000;
                                int ffllaagg=0;
                                if(who=='A' && (step[sx][sy]+1>1001-round || step[sx][sy]>40))
                                        ffllaagg=1;
                                else if(who=='B' && (step[sx][sy]+1>1001-round || step[sx][sy]>40))
                                        ffllaagg=1;
                                int temp=0;
                                int flag=0;
                                if(newmap[sx][sy][sx+dx[xy]][sy+dy[xy]]==1 && ffllaagg==0){
                                        temp=score[sx][sy]+1;
                                        if(score[sx+dx[xy]][sy+dy[xy]]<temp){
                                                score[sx+dx[xy]][sy+dy[xy]]=temp;
                                                flag=1;
                                        }
                                }
                                else if(newmap[sx][sy][sx+dx[xy]][sy+dy[xy]]==2 && ffllaagg==0){
                                        temp=score[sx][sy]-1;
                                        if(step[sx+dx[xy]][sy+dy[xy]]==0){
                                                score[sx+dx[xy]][sy+dy[xy]]=temp-1;
                                                flag=1;
                                        }
                                        else if(step[sx+dx[xy]][sy+dy[xy]]!=0){
                                                if(score[sx+dx[xy]][sy+dy[xy]]<temp){
                                                        score[sx+dx[xy]][sy+dy[xy]]=temp;
                                                        flag=1;
                                                }
                                        }
                                }
                                else if(newmap[sx][sy][sx+dx[xy]][sy+dy[xy]]==3 && ffllaagg==0){
                                        temp=score[sx][sy]*2;
                                        if(score[sx+dx[xy]][sy+dy[xy]]<temp){
                                                score[sx+dx[xy]][sy+dy[xy]]=temp;
                                                flag=1;
                                        }
                                }
                                else if(newmap[sx][sy][sx+dx[xy]][sy+dy[xy]]==4 && ffllaagg==0){
                                        temp=score[sx][sy]/2;
                                        if(score[sx+dx[xy]][sy+dy[xy]]<temp){
                                                score[sx+dx[xy]][sy+dy[xy]]=temp;
                                                flag=1;
                                        }
                                }
                                else if(newmap[sx][sy][sx+dx[xy]][sy+dy[xy]]==5 && ffllaagg==0){
                                        temp=score[sx][sy];
                                        if(score[sx+dx[xy]][sy+dy[xy]]<=temp){
                                                score[sx+dx[xy]][sy+dy[xy]]=temp;
                                                flag=1;
                                        }
                                }
                                else if(newmap[sx][sy][sx+dx[xy]][sy+dy[xy]]==6 && ffllaagg==0){
                                        temp=score[sx][sy];
                                        if(score[sx+dx[xy]][sy+dy[xy]]<=temp){
                                                score[sx+dx[xy]][sy+dy[xy]]=temp;
                                                flag=1;
                                        }
                                }
                                if(flag==1 && ffllaagg==0){
                                        int ii=0;
                                        for(; ii<step[sx][sy]+1 ; ++ii){
                                                path[sx+dx[xy]][sy+dy[xy]][ii][0]=path[sx][sy][ii][0];
                                                path[sx+dx[xy]][sy+dy[xy]][ii][1]=path[sx][sy][ii][1];
                                        }
                                        path[sx+dx[xy]][sy+dy[xy]][ii][0]=sx+dx[xy];
                                        path[sx+dx[xy]][sy+dy[xy]][ii][1]=sy+dy[xy];
                                        if(step[sx][sy]==0)
                                                direction[sx+dx[xy]][sy+dy[xy]]=xy;
                                        else
                                                direction[sx+dx[xy]][sy+dy[xy]]=direction[sx][sy];
                                        if(newmap[sx][sy][sx+dx[xy]][sy+dy[xy]]==5)
                                                step[sx+dx[xy]][sy+dy[xy]]=step[sx][sy]+3;
                                        else
                                                step[sx+dx[xy]][sy+dy[xy]]=step[sx][sy]+1;
                                        if(max<score[sx+dx[xy]][sy+dy[xy]]){
                                                max=score[sx+dx[xy]][sy+dy[xy]];
                                                wheremax[0]=sx+dx[xy];
                                                wheremax[1]=sy+dy[xy];
                                                stepofmax=step[sx+dx[xy]][sy+dy[xy]];
                                        }
                                        else if(max==score[sx+dx[xy]][sy+dy[xy]] && stepofmax>step[sx+dx[xy]][sy+dy[xy]] && stepofmax!=0){
                                                max=score[sx+dx[xy]][sy+dy[xy]];
                                                wheremax[0]=sx+dx[xy];
                                                wheremax[1]=sy+dy[xy];
                                                stepofmax=step[sx+dx[xy]][sy+dy[xy]];
                                        }
                                        for(int h=0 ; h<n1+2 ; ++h){
                                                for(int hh=0 ; hh<n2+2 ; ++hh){
                                                        newmap[sx+dx[xy]][sy+dy[xy]][h][hh]=newmap[sx][sy][h][hh];
                                                }
                                        }
                                        newmap[sx+dx[xy]][sy+dy[xy]][sx+dx[xy]][sy+dy[xy]]=6;
                                        if(flag==1 && waiting<99999){
                                                undeveloped[waiting][0]=sx+dx[xy];
                                                undeveloped[waiting][1]=sy+dy[xy];
                                                ++waiting;
                                        }
                                }
                        }
                }
        }
        int xxxxx,yyyyy;
        if(who=='A'){
                xxxxx=memAB[0][0];
                yyyyy=memAB[0][1];
        }
        else{
                xxxxx=memAB[1][0];
                yyyyy=memAB[1][1];
        }
        if(direction[wheremax[0]][wheremax[1]]==1)
                printf("UP\n");
        else if(direction[wheremax[0]][wheremax[1]]==2)
                printf("DOWN\n");
        else if(direction[wheremax[0]][wheremax[1]]==3)
                printf("LEFT\n");
        else if(direction[wheremax[0]][wheremax[1]]==4)
                printf("RIGHT\n");
        else{
                for(int z=1 ; z<5 ; ++z){
                        if(map[xxxxx+dx[z]][yyyyy+dy[z]]!=-1 && ban[z]!=-1){
                                if(z==1)
                                        printf("UP\n");
                                else if(z==2)
                                        printf("DOWN\n");
                                else if(z==3)
                                        printf("LEFT\n");
                                else
                                        printf("RIGHT\n");
                                return 0;
                        }
                }
        }
        return 0;
}
