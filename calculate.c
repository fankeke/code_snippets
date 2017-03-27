#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 1000

typedef struct {
    char *base,*top;
    int  size;
}stack_op;

typedef struct {
    double *base,*top;
    int     size;
}stack_val;

void stack_op_init(stack_op *sop)
{
    sop->base=(char*)malloc(N*sizeof(char));
    sop->top=sop->base;
    sop->size=N;
}
void stack_val_init(stack_val *sval)
{
    sval->base=(double*)malloc(N*sizeof(double));
    sval->top=sval->base;
    sval->size=N;
}

char stack_op_top(stack_op *sop)
{
    char 	e;
    e=*(sop->top);
    return e;
}
double stack_val_top(stack_val *sval)
{
    double 	e;
    e=*(sval->top);
    return e;
}

void stack_op_push(stack_op *sop,char e)
{
    sop->top++;
    *(sop->top)=e;
}
void stack_val_push(stack_val *sval,double e)
{
    sval->top++;
    *(sval->top)=e;
}

void stack_op_pop(stack_op *sop)
{
    sop->top--;
}
void stack_val_pop(stack_val *sval)
{
    sval->top--;
}

//符号栈顶的符号c1优先级和str扫描到的c2优先级别比较，
char compare(char c1,char c2)
{
    if(c1=='+'||c1=='-'){
	if(c2=='+'||c2=='-'||c2==')'||c2=='=')
	    return '>';
	else
	    return '<';
    }else if(c1=='*'||c1=='/'){
	if(c2=='*'||c2=='/'||c2=='+'||c2=='-'||c2==')'||c2=='=')
	    return '>';
	else 
	    return '<';
    }else if(c1=='('||c1=='#'){
	if((c1=='('&&c2==')')||(c1=='#'&&c2=='='))
	    return '=';
	else
	    return '<';
    }
}

double compute(double x,char c,double y)
{
    if(c=='+') return x+y;
    if(c=='-') return x-y;
    if(c=='*') return x*y;
   
    return x/y;
}


int main(int argc,char**argv)
{
    int i,t,end,loop,len;
    double x,y,temp;
    char str[2000],val[200],c;
    stack_op *sop;
    stack_val*sval;

    sop=malloc(sizeof *sop);
    sval=malloc(sizeof *sval);

    scanf("%d",&t);
    while(t--){

	memset(str,0,sizeof(str));
	memset(val,0,sizeof(val));

	end=loop=0;

	stack_op_init(sop);
	stack_op_push(sop,'#');
	stack_val_init(sval);

	scanf("%s",str);
	len=strlen(str);

	for(i=0;i<len;){

	    if(str[i]=='='&&stack_op_top(sop)=='#')
		break;
	    
	    if(str[i]>='0'&&str[i]<='9'||str[i]=='.'){
		loop=1;
		val[end++]=str[i++];
		continue;
	    }

	    if(loop){
		val[end]='\0';
		temp=atof(val);
		stack_val_push(sval,temp);
		end=loop=0;
	    }

	    switch(compare(stack_op_top(sop),str[i])){
		case '<':
		    stack_op_push(sop,str[i++]);
		    break;
		case '=':
		    stack_op_pop(sop);
		    i++;
		    break;
		case '>':
		    y=stack_val_top(sval);stack_val_pop(sval);
		    x=stack_val_top(sval);stack_val_pop(sval);
		    c=stack_op_top(sop);stack_op_pop(sop);
		    stack_val_push(sval,compute(x,c,y));
		    break;
	    }
	}

	printf("%.2f\n",stack_val_top(sval));
    }
    return 0;
}
			    













