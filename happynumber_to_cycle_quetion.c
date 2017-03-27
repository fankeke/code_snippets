https://leetcode.com/problems/happy-number/


Happy Number My Submissions Question
Total Accepted: 44119 Total Submissions: 128446 Difficulty: Easy
Write an algorithm to determine if a number is "happy".

A happy number is a number defined by the following process: Starting with any positive integer, replace the number by the sum of the squares of its digits, and repeat the process until the number equals 1 (where it will stay), or it loops endlessly in a cycle which does not include 1. Those numbers for which this process ends in 1 are happy numbers.

Example: 19 is a happy number

12 + 92 = 82
82 + 22 = 68
62 + 82 = 100
12 + 02 + 02 = 1



//my code:
// it's simple ,stroe every sqare value in a list and then chenck if duplicate .
typedef struct node{
    int          val;
    struct node *next;
}node;
int getsq(int n)
{
    int sum=0;
    while(n){
        int sq=n%10;
        sum+=sq*sq;
        n=n/10;
    }
    return sum;
}
int isduplicate(int n,node**head)
{
    node *p=*head;
    while(p){
        if(p->val==n) return 1;
        p=p->next;
    }
    node *i=malloc(sizeof(node));
    i->val=n;
    i->next=*head;
    *head=i;
    return 0;
}
bool isHappy(int n) {
    int k;
    node*p=NULL;
    while(n!=1){
        n=getsq(n);
        if(isduplicate(n,&p)) return false;
    }
    return true;
}


//best code :
//since this problem have the taste of a "cycle style", so we could use the linklist cycle's ideal, use slow and fast to solver this problm.
NOTE:        every "cycle style" problem could use the fast and slow idea.

int getsq(int n)
{
    int sum=0;
    while(n){
        int sq=n%10;
        sum+=sq*sq;
        n=n/10;
    }
    return sum;
}

bool isHappy(int n) {
    int slow,fast;
    slow=fast=n;
    while(1){
      slow=getsq(slow);
      fast=getsq(fast);fast=getsq(fast);
      if(slow==fast) break;
    }
    return fast==1;
}
