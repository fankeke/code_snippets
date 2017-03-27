
#include <stdio.h>
#define OUT 0
#define IN 1
int main(int argc,char**argv)
{
    int c,nl,nw,nc,state;

    state=OUT;
    nl=nw=nc=0;

    while((c=getchar())!=EOF){
	nc++;
	if(c=='\n') nl++;
	if(c==' '||c=='\t'||c=='\n') state=OUT;
	else if(state==OUT) { state=IN;nw++;}
    }
    printf("%d %d %d\n",nc,nl,nw);
    return 0;
}

    

    


