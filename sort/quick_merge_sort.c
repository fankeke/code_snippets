#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int partition(int*a,int left,int right)
{
    int pivot=a[left];
    while(left<right){
	while(left<right&&a[right]>=pivot) right--;
	a[left]=a[right];
	while(left<right&&a[left]<=pivot) left++;
	a[right]=a[left];
    }
    a[left]=pivot;
    return left;
}
void quick_sort(int*a,int left,int right)
{
    if(left<right){
	int pos=partition(a,left,right);
	quick_sort(a,left,pos);
	quick_sort(a,pos+1,right);
    }
}
void merge_two(int*a,int left,int mid,int right)
{
    int temp[1000];
    int i,j,k;
    for(i=left;i<=mid;i++) temp[i]=a[i];
    for(j=mid+1;j<=right;j++) temp[j]=a[j];
    for(i=left,j=mid+1,k=left;k<=right;k++){
	if(i>mid) 		a[k]=temp[j++];
	else if(j>right) 	a[k]=temp[i++];
	else       		a[k]=temp[i]<temp[j]?temp[i++]:temp[j++];
    } 

}
void merge_sort(int*a,int left,int right)
{
    if(left<right){
	int mid=(left+right)/2;
	merge_sort(a,left,mid);
	merge_sort(a,mid+1,right);
	merge_two(a,left,mid,right);
    }
}

int main(int argc,char**argv)
{
    int i, n,arr[20];
    srand(time(NULL));

    n=20;
    for(i=0;i<n;i++){
	arr[i]=random()%100;
    }


    for(i=0;i<n;i++){
	printf("%d ",arr[i]);
    }
//    quick_sort(arr,0,n-1);
    merge_sort(arr,0,n-1);
    printf("\nafter sort:\n");
    for(i=0;i<n;i++){
	printf("%d ",arr[i]);
    }
    printf("\n");
}

