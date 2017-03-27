Range Sum Query - Mutable My Submissions Question
Total Accepted: 2101 Total Submissions: 14332 Difficulty: Medium
Given an integer array nums, find the sum of the elements between indices i and j (i ≤ j), inclusive.

The update(i, val) function modifies nums by updating the element at index i to val.
Example:
Given nums = [1, 3, 5]

sumRange(0, 2) -> 9
update(1, 2)
sumRange(0, 2) -> 8


https://leetcode.com/problems/range-sum-query-mutable/






//my code:


typedef struct  {
    //下表都从1开始，为了好求lowbit
    int*nums;//原始数据
    int*sums;//树状数组
    int size;
}NumArray;


/*
线性数组的运用：
C1=A1
C2=A1+A2
C3=A3
C4=A1+A2+A3+A4
C5=A5
C6=A5+A6
C7=A7
C8=A1+A2+A3+A4+A5＋A6+A7+A8

Cn= 的个数为: n的最后一个1代表几。如
C1的1，最后一个1代表的就是1，那么C1就只有一个数字A1
C6的6，最后一个1代表的是2（110），那么C6就有两个数字A5＋A6

lowbit（n）就是来快速求出n的最后一个1代表多少的。
*/

/** Initialize your data structure here. */
int lowbit(int n)
{
    return n&(-n);
}
NumArray* NumArrayCreate(int* nums, int numsSize) {
    NumArray    *a;
    int          i=0,j=0;
    a=malloc(sizeof(*a));
    a->nums=malloc(sizeof(int)*(numsSize+1));
    a->sums=malloc(sizeof(int)*(numsSize+1));
    a->size=numsSize;
    while(i<numsSize){
        a->nums[i+1]=nums[i];        //为了简单，起始下表从1开始
        i++;
    }
    for(i=1;i<=numsSize;i++){
        a->sums[i]=a->nums[i];
        for(j=i-1;j>i-lowbit(i);j--){              //这里的a->sums[i]就是上面的Ci.这里求出每个Ci的值
            a->sums[i]+=a->nums[j];
        }
    }
    return a;
}

void update( NumArray* numArray, int i, int val) {
    int j,p=i+1;//下表从1开始
    int temp=val;
    val=val-numArray->nums[p];                      //秋初差值         
    numArray->nums[p]=temp;
    for(j=p;j<=numArray->size;j+=lowbit(j))          //更改了一个Ai，那么后面的很多C（涉及到Ai的）都要改变，
    numArray->sums[j]+=val;                         //  因为是更改和，那么把差值求出来，然后加上即可。
}

int sumRange( NumArray* numArray, int i, int j) {
    i++;j++;
    int p=i,ans=0,sum1=0,sum2=0;
    for(;j>=1;j-=lowbit(j)) sum1+=numArray->sums[j];            //从i到j的和可以用从求1～j的和－1～i的和然后加上nums[i]的的值
    for(;i>=1;i-=lowbit(i)) sum2+=numArray->sums[i];            //注意，这里求1-i的和是从Ci开始逐步向C1回溯的，
    ans=sum1-sum2+numArray->nums[p];
    return ans;
}

/** Deallocates memory previously allocated for the data structure. */
void NumArrayFree( NumArray* numArray) {
    free(numArray->nums);
    free(numArray->sums);
    free(numArray);
}
