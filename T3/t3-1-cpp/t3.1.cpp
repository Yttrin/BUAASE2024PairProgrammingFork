#include<iostream>
#include<cstdlib>
#include<cstring>
#define y 4 //pieces per plot
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
char terminate(char* base){ //return -1 or (if game ends) final score of first player
    //putchar('(\n');
    /*
    for(int i=0;i<=6;i++)printf("%d ",base[i]);
    putchar(10);
    for(int i=7;i<=13;i++)printf("%d ",base[i]);
    printf("\n)\n");
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
    if(t==-1)return 20000+(flag==1?array[6]:array[13]);
    return 15000+(t-(12*y-t))*(flag==1?1:-1);
}
int mancala_result_ptr(int* flag,int* seq,int* size){
    return mancala_result(*flag,seq,*size);
}

//t3.1
extern "C" {
int mancala_board(int* flag,int* seq,int* size,int* return_array/*[15]*/){
free(malloc(sizeof(int)));
    char now = seq[0]/10;
    char array[14];
    init(array);

    int ret=0;
    for(int i=0;i<*size;i++){
        if(perform(array,&now,seq[i])==-1){
            now=-1*now;
            break;
        }
    }
    if(now==-1){
        ret=200+2*array[6]-48;
    }
    else if(now==-2){
        ret=200+48-2*array[13];
    }
    else{//legal
        int t=terminate(array);
        if(t!=-1){
            ret=200+(t-(12*y-t))*((seq[0]/10)==1?1:-1);
        }
        else ret=now;
    }

    for(int i=0;i<14;i++){
        return_array[i]=array[i];
    }
    return_array[14]=ret;
    return 0;
}
};

//int main(){
//    int op1[3]={11,12};
//    int op2[5]={13,16,22,26};
//    int test;
//    test = mancala_result(1,op2,4);
//    std::cout<<test<<std::endl;
//    test = mancala_result(1,op1,2);
//    std::cout<<test<<std::endl;
//    test = mancala_result(2,op1,2);
//    std::cout<<test<<std::endl;
//    return 0;
//}