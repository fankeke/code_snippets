Count Primes My Submissions Question
Total Accepted: 42450 Total Submissions: 192609 Difficulty: Easy
Description:

Count the number of prime numbers less than a non-negative number, n.

Credits:
Special thanks to @mithmatt for adding this problem and creating all test cases.


//BEST CODE;
int countPrimes(int n) {
    if(n<3) return 0;
    int *dp=malloc(sizeof(int)*n);
    int i,j;
    for(i=2;i<n;i++) dp[i]=1;
    for(i=2;i*i<n;i++){
        j=i*i;                                       /*值得注意的是，每次j都从i的平方开始，例如，i＝3求完后，不必再从4开始检查，直接从4*4开始
                                                      4-4*4-1之间的数都被判断过了，不需要再次判读啊*/
        if(dp[j]==1){
          while(j<n){
            dp[j]=0;
            j=j+i;
         }                  
        }
    }
    int ans=0;
    for(i=2;i<n;i++)
      if(dp[i]==1) ans+=1;
    
    return ans;
}












