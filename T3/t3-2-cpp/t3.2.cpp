#include<iostream>
#include<cstdlib>
#include<cstring>
#include<random>
#define y 4 //pieces per plot
int no_step_print=0;
char p2op[13];
char op2p[30];
//pos
//13 12 11 10  9  8  7  6
//13  0  1  2  3  4  5  6

//operation
// x 26 25 24 23 22 21  x
// x 11 12 13 14 15 16  x
void init(char* base){//initialize game set, p2op, op2p
    for(int i = 0;i<14;++i)base[i] = y;
    base[6] = 0;
    base[13] = 0;
    for(int i=0;i<6;i++){
        p2op[i]=i+11;
        op2p[i+11]=i;
        p2op[i+7]=i+21;
        op2p[i+21]=i+7;
    }
}
void copy_board(char* src,char* dst){
    for(int i = 0;i<14;++i)dst[i]=src[i];
}
void print(char* base){
    if(no_step_print==1){return;}
    printf("     26  25  24  23  22  21\n");
    for(int i=13;i>=6;i--)printf("%3d ",base[i]);
    printf("\n");
    for(int i=-1;i<=6;i++)printf("%3d ",base[(i==-1?13:i)]);
    printf("\n     11  12  13  14  15  16\n");
}

char terminate(char* base){ //return -1 or (if game ends) final score of first player
    /*
    putchar('{\n');
    for(int i=13;i<=6;i--)printf("%3d ",base[i]);
    putchar(10);
    for(int i=-1;i<=6;i++)printf("%3d ",base[(i==-1?13:i)]);
    printf("\n}\n");
    */
    if(base[0]+base[1]+base[2]+base[3]+base[4]+base[5] == 0)return base[6];
    if(base[7]+base[8]+base[9]+base[10]+base[11]+base[12] == 0)return 12*y-base[13];
    return -1;
}
char perform(char* base, char* now, char op){//-1 = illegal
    if(terminate(base)!=-1)return -1;//game ends
    if(p2op[op2p[op]]!=op)return -1;//op valid
    if(op/10!=*now)return -1;//player's turn
    char pos = op2p[op];
    char free = base[pos];//pieces to be move
    if(free==0)return -1;
    base[pos]=0;
    while(free>0){//put pieces
        pos++;pos%=14;//next pos
        if(pos==6&&*now==2)continue;
        if(pos==13&&*now==1)continue;
        base[pos]++;
        free--;
    }
    if(pos==6||pos==13)return 0;
    if(base[pos]==1&&base[12-pos]!=0&&pos/7==*now-1){//only one piece, opposite position contains pieces, player's turn
        base[*now*7-1]+=base[pos]+base[12-pos];//collect
        base[pos]=0;
        base[12-pos]=0;
    }
    *now = 3-*now; //change player
    return 0;
}
int mancala_result(int flag,int* seq,int size){ //Q2
    char now = flag;
    char array[14];
    init(array);

    for(int i=0;i<size;i++){
        if(perform(array,&now,seq[i])==-1)return 30000+i;
    }
    int t = terminate(array);
    if(t==-1)return 20000+array[6];
    return 15000+t-(12*y-t);
}
int mancala_result_ptr(int* flag,int* seq,int* size){
    return mancala_result(*flag,seq,*size);
}

double randd(){
    return (rand()%10000)/10000.0;
}

struct nodest{
    int id;
    double q[14];
};
typedef struct nodest node;

struct playerst{
    int depth;//5,6
    int node_max;//5,7
    node* enemy[8];
    node* all[8];
    double all_percent;
    int turn;//1,2
};
typedef struct playerst player;
void print_player(player p){
    printf("dep/max/alp = [%d, %d, %f]\n",p.depth,p.node_max,p.all_percent);
    for(int i=0;i<=7;i++)if(p.enemy[i]!=NULL){
        printf("en %d: ",p.enemy[i]->id);
        for(int j=0;j<=13;j++)printf("%lf%s",p.enemy[i]->q[j],j==13?"\n":", ");
    }
    for(int i=0;i<=7;i++)if(p.all[i]!=NULL){
        printf("al %d: ",p.all[i]->id);
        for(int j=0;j<=13;j++)printf("%f%s",p.all[i]->q[j],j==13?"\n":", ");
    }
}

node* copy_of(node n){
    node* ret = (node*)malloc(sizeof(node));
    memset(ret,0,sizeof(node));
    ret->id = n.id;
    for(int i=0;i<=13;i++)ret->q[i]=n.q[i];
    return ret;
}

//pos
//13 12 11 10  9  8  7  6
//13  0  1  2  3  4  5  6
player* new_player(int dep, int max, double alp, int diva, int dive){
    player* p = (player*)malloc(sizeof(player));
    memset(p,0,sizeof(player));
    p->depth = dep;
    p->node_max = max;
    node* e0 = (node*)malloc(sizeof(node));
    node* ef = (node*)malloc(sizeof(node));
    memset(ef,0,sizeof(node));
    memset(e0,0,sizeof(node));
    e0->id = 0;
    ef->id = 12*y;
    for(int i=0;i<=6;i++){
        e0->q[i] = 1;
        ef->q[i] = 0;
    }
    for(int i=7;i<=13;i++){
        e0->q[i] = -1;
        ef->q[i] = 0;
    }
    ef->q[6]=1;ef->q[13]=-1;
    node* a0 = copy_of(*e0);
    node* am = copy_of(*ef);
    node* em = copy_of(*ef);
    node* af = copy_of(*ef);
    am->id = diva*y;
    em->id = dive*y;
    af->id = 12*y;
    p->all_percent = alp;
    p->turn = 1;
    p->enemy[0] = e0;
    p->enemy[1] = em;
    p->enemy[2] = ef;
    p->all[0] = a0;
    p->all[1] = am;
    p->all[2] = af;
    return p;
}

void release(player* p){
    for(int i=0;i<=7;i++){
        if(p->all[i]!=NULL)free(p->all[i]);
        if(p->enemy[i]!=NULL)free(p->enemy[i]);
    }
    free(p);
}

void make_opponent(player p,player*dst){
    player* r = dst;
    r->all_percent=p.all_percent;
    r->node_max=p.node_max;
    r->depth=p.depth;
    r->turn=3-p.turn;
    for(int i=0;i<=7;i++)if(p.enemy[i]!=NULL){
        node* n = (node*)malloc(sizeof(node));
        memset(n,0,sizeof(node));
        n->id=p.enemy[i]->id;
        for(int j=0;j<=13;j++){
            n->q[j]=p.enemy[i]->q[(j+7)%14];
        }
        r->enemy[i]=n;
    }
    for(int i=0;i<=7;i++)if(p.all[i]!=NULL){
        node* n = (node*)malloc(sizeof(node));
        memset(n,0,sizeof(node));
        n->id=p.all[i]->id;
        for(int j=0;j<=13;j++){
            n->q[j]=p.all[i]->q[(j+7)%14];
        }
        r->all[i]=n;
    }
}

double value(player p, char* base){
    int en=0,al=0,enx=0,eny=0;
    double ren=0,ral=0;
    for(int i=7;i<=12;i++)enx+=base[i];
    for(int i=0;i<=5;i++)eny+=base[i];
    en=(enx>eny?eny:enx);
    for(int i=7;i<=12;i++)al+=base[i];
    for(int i=0;i<=5;i++)al+=base[i];
    for(int i=0;i<=7;i++){
        if(p.enemy[i]->id==en){
            for(int j=0;j<=13;j++){
                ren+=p.enemy[i]->q[j]*base[j];
            }
            break;
        }
        if(p.enemy[i+1]->id>en){
            double en1=0,en2=0;
            for(int j=0;j<=13;j++){
                en1+=p.enemy[i]->q[j]*base[j];
            }
            for(int j=0;j<=13;j++){
                en2+=p.enemy[i+1]->q[j]*base[j];
            }
            ren = ((en-p.enemy[i]->id)*(en2)+(p.enemy[i+1]->id-en)*(en1))/(p.enemy[i+1]->id-p.enemy[i]->id);
            break;
        }
    }
    for(int i=0;i<=7;i++){
        if(p.all[i]->id==al){
            for(int j=0;j<=13;j++){
                ral+=p.all[i]->q[j]*base[j];
            }
            break;
        }
        if(p.all[i+1]->id>al){
            double al1=0,al2=0;
            for(int j=0;j<=13;j++){
                al1+=p.all[i]->q[j]*base[j];
            }
            for(int j=0;j<=13;j++){
                al2+=p.all[i+1]->q[j]*base[j];
            }
            ral = ((al-p.all[i]->id)*(al2)+(p.all[i+1]->id-al)*(al1))/(p.all[i+1]->id-p.all[i]->id);
            break;
        }
    }
    return p.all_percent*ral+(1-p.all_percent)*ren;
}

int minmax(player p,char* base,int d,int turn, double*score){
    if(d==0||terminate(base)!=-1){
        player* tmp=&p;
        if(turn!=p.turn){
            tmp=(player*)malloc(sizeof(player));
            memset(tmp,0,sizeof(player));
            make_opponent(p,tmp);
        }
        *score=value(*tmp,base);
        if(p.turn!=tmp->turn)release(tmp);
        return 0;
    }
    double max_score=-999;
    int best_op=0;
    for(int i=0;i<=5;i++){
        char op=turn*10+i+1;
        if(base[op2p[op]]==0)continue;
        double this_score;
        char tmp[14];char now=turn;
        copy_board(base,tmp);
        perform(tmp,&now,op);
        minmax(p,tmp,d-(turn==now?0:1),now,&this_score);
        if(turn!=now)this_score*=-1;
        if(this_score>max_score){
            max_score=this_score;
            best_op=op;
        }
    }
    *score=max_score;
    return best_op;
}

int autogame(player p1,player p2){
    char base[14];
    init(base);
    char turn=1;
    print(base);
    while(terminate(base)==-1){
        double tmp;
        int op = minmax(turn==1?p1:p2,base,(turn==1?p1:p2).depth,turn,&tmp);
        perform(base,&turn,op);
        if(no_step_print==0)printf("op=%d,score=%.2f\n",op,tmp);
        print(base);
    }
    printf("final result: %d-%d\n",terminate(base),12*y-terminate(base));
    return 2*terminate(base)-12*y;
}
int human_game(player p1){
    printf("move first(1), move second(2)\n");
    int humanturn=0;
    while(humanturn!=1&&humanturn!=2)scanf("%d",&humanturn);
    printf("start, please input legal operation between[%d1,%d6]\n",humanturn,humanturn);
    char base[14];
    init(base);
    char turn=1;
    print(base);
    while(terminate(base)==-1){
        double tmp=-999;
        int op=0;
        if(turn == humanturn)
            printf("your turn.\n"),
            scanf("%d",&op);
        else
            op = minmax(turn==1?p1:p1,base,(turn==1?p1:p1).depth,turn,&tmp);
        perform(base,&turn,op);
        if(tmp!=-999)printf("op=%d,score=%.2f\n",op,tmp);
        print(base);
    }
    printf("final result: %d-%d\n",terminate(base),12*y-terminate(base));
    return 2*terminate(base)-12*y;
}

int main(){
    int n=8;
    player* p[n];
    double t1[n]={},t2[n]={};
    int index[n];
    //no_step_print=1;
    p[4]=new_player(6,2,1.0,12,12);
    human_game(*p[4]);
    return 0;
    /*
    //for(int i=0;i<n/2;i++)p[i]=new_player(4,2);
    //for(int i=n/2;i<n;i++)p[i]=new_player(5,2);
    for(int i=n-1;i>=0;i--)for(int j=0;j<n;j++){
        printf("%d vs %d\n",i,j);
        double tmp=autogame(*p[i],*p[j]);
        t1[i]+=tmp/n/n;
        t2[j]-=tmp/n/n;
    }
    for(int i=0;i<n;i++)index[i]=i;
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)
        if(t1[index[i]]<t1[index[j]]){
            int tmp=index[i];index[i]=index[j];index[j]=tmp;
        }
    for(int i=0;i<(n>=3?3:n);i++){
        printf("[score=%f/%f]\n",t1[index[i]],t2[index[i]]);
        print_player(*p[index[i]]);
    }
    return 0;
    */
}