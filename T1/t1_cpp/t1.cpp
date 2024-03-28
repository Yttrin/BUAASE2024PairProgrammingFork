#include <emscripten/emscripten.h>
#include <iostream>
#include <cstdlib>
extern "C" {
int EMSCRIPTEN_KEEPALIVE bocchi_shut_up(int* flag, int* seq, int* size) {
    printf("%d %d %d\n",*flag,*seq,*size);
    int tmp[30]={},i;
    for(i=0;i<*size;i++){
        tmp[seq[i]]++;
    }
    int l,r,max=-1,ans=0;
    if(*flag==1)l=11,r=16;
    else if(*flag==2)l=21,r=26;
    for(i=l;i<=r;i++){
        if(tmp[i]>max)max=tmp[i];
    }
    for(i=l;i<=r;i++){
        if(tmp[i]==max)ans=(ans==0)?i:10;
    }
    return ans;
}
}