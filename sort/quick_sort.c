#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

typedef int Item;
#define RADN_MAX 1000000
#define key(a) (a)
#define less(a,b) (key(a)<key(b))
#define more(a,b) (key(a)>key(b))
#define exch(a,b)  {Item t=a;a=b;b=t;}
#define compexch(a,b) if(less(b,a)) exch(a,b)

//注：上面的exch都是用宏来表示，所以在传递的时候直接传递
//值就可以，没有函数调用过程（宏直接展开），故可行


void sort(Item*a,int low,int high)
{
    int	i,j;
    for(i=low+1;i<=high;i++){
	for(j=i;j>low;j--)
	    compexch(a[j-1],a[j]);
    }
}

//插入排序，即把当前元素t放入到前面的某个正确的位置
void insert_sort(Item*a,int low,int high)
{
    int i,j;
    Item t;
    for(i=low+1;i<=high;i++){
	t=a[i];
	j=i-1;
	while(j>=0&&less(t,a[j])){
	    a[j+1]=a[j];	//把大于t的都向后面移动
	    j--;
	}
	a[j+1]=t;
    }
}

int partition(Item*a,int low,int high)
{
    Item pivot=a[low];
    while(low<high){
	while(low<high&&!less(a[high],pivot)) high--;
	//while(low<high&&a[high]>=pivot) high--;
	a[low]=a[high];
	while(low<high&&!more(a[low],pivot)) low++;
//	while(low<high&&a[low]<=pivot) low++;
	a[high]=a[low];
    }
    a[low]=pivot;
    return low;
}
void quick_sort(Item*a,int low,int high)
{
    int pos;
    if(low<high){
	pos=partition(a,low,high);
	quick_sort(a,low,pos);
	quick_sort(a,pos+1,high);
    }
}
void print(Item*a,int n)
{
    int i;
    for(i=0;i<n;i++){
	printf("%d ",a[i]);
    }
    printf("\n");
}

//选择排序，即依次序把元素放入第一个，第二个位置
void select_sort(Item*a,int low,int high)
{
    int	i,j,min;
    for(i=low;i<high;i++){//位置
	min=i;
	for(j=i+1;j<=high;j++){
	    if(less(a[j],a[min])) min=j;
	}
	exch(a[i],a[min]);
    }
}

//冒泡排序，本质上也是一种选择排序（把最小的放入第一位置，然后把次小的
//放入第二个位置
void bubbler_sort(Item*a,int low,int high)
{
    int i,j;
    for(i=low;i<high;i++){//位置
	for(j=high;j>i;j--)//每次从底层往上面冒泡
	    compexch(a[j-1],a[j]);//这样一次下来，最小的就放入了第i位置
    }

}

void getcost(struct timeval*tv1,struct timeval*tv2,long*sec,long*usec)
{
    *sec=tv2->tv_sec-tv1->tv_sec;
    *usec=tv2->tv_usec-tv1->tv_usec;
    if(*usec<0){
	(*sec)--;
	(*usec)+=1000000;
    }
}

int main(int argc,char**argv)
{
    int i,n;
    struct timeval tv1,tv2;
    long sec,usec;

    n=atoi(argv[1]);
    srand(time(NULL));
    Item*a=(Item*)malloc(n*sizeof(Item));
    Item*b=(Item*)malloc(n*sizeof(Item));
    for(i=0;i<n;i++)
    	b[i]=a[i]=random()%RADN_MAX;

    /*
    print(a,n);
    gettimeofday(&tv1,NULL);
    quick_sort(a,0,n-1);
    gettimeofday(&tv2,NULL);
    print(a,n);
    getcost(&tv1,&tv2,&sec,&usec);
    printf("sort:cost:%ld sec %ld usec\n",sec,usec);
    */

    
    print(a,n);
    gettimeofday(&tv1,NULL);
    quick_sort(a,0,n-1);
    gettimeofday(&tv2,NULL);
    print(a,n);
    getcost(&tv1,&tv2,&sec,&usec);
    printf("quick_sort:cost:%ld sec %ld usec\n",sec,usec);
    return 0;
}




