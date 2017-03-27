#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int*create_array(int n)
{
    srand(time(NULL));
    int *arr=malloc(sizeof(int)*n);
    int i;
    for(i=0;i<n;i++){
	arr[i]=random()%100;
    }
    return arr;
}
void print(int*arr,int n)
{
    int i;
    for(i=0;i<n;i++){
	printf("%d ",arr[i]);
    }
    printf("\n");
}
int partition(int*a,int low,int high)
{
    int pivot=a[low];
    while(low<high){
	while(low<high&&a[high]>=pivot) high--;
	a[low]=a[high];
	while(low<high&&a[low]<=pivot) low++;
	a[high]=a[low];
    }
    a[low]=pivot;
    return low;
}
   


void quick_sort(int*a,int low,int high)
{
    int pos;
    if(low<high){
	pos=partition(a,low,high);
	quick_sort(a,low,pos);
	quick_sort(a,pos+1,high);
    }
}

void quicksort(int*a,int n)
{
    quick_sort(a,0,n-1);
}
void merger_two(int*a,int left,int mid,int high)
{
    int temp[1000];
    int i=0,j=0,k=0;

    for(i=left;i<=mid;i++) temp[i]=a[i];
    for(j=mid+1;j<=high;j++) temp[j]=a[j];

    for(i=left,j=mid+1,k=left;k<=high;k++){
	if(i>mid){a[k]=temp[j++];continue;}
	if(j>high){a[k]=temp[i++];continue;}
	a[k]=temp[i]<temp[j]?temp[i++]:temp[j++];
    }
}



void merger_sort(int*a,int low,int high)
{
    if(low>=high) return;
    int mid=(low+high)/2;
    merger_sort(a,low,mid);
    merger_sort(a,mid+1,high);
    merger_two(a,low,mid,high);
}

void mergersort(int*a,int n)
{
    merger_sort(a,0,n-1);
}

int main(int argc,char**argv)
{
    int n=atoi(argv[1]);
    int*arr=create_array(n);
    print(arr,n);
    mergersort(arr,n);
//    sortarray(arr,n);
    print(arr,n);
    
}
