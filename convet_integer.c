
Given a positive integer, return its corresponding column title as appear in an Excel sheet.

For example:

    1 -> A
    2 -> B
    3 -> C
    ...
    26 -> Z
    27 -> AA
    28 -> AB 
    
    
    

void revers(char*s)
{
    int len=strlen(s);
    int i=0,j=len-1;
    while(i<j){
        char c=s[i];
        s[i]=s[j];
        s[j]=c;
        i++;j--;
    }
}

//如果有对应的0-25对应A－Z的话，那么就是这种常规的进制转化。
//但是题目中的是1－26对应A-Z,那么就需要下面的先－1的操作。
char* convertToTiltle(int n)
{
    while(n){
        ans[count++]=n%26+'A'-1;
        n=n/26;
    }
    revers(ans);
}
char* convertToTitle(int n) {
    char ans[10000]
    int count=0,s=0;
    bzero(ans,10000);
    while(n){
        n-=1;
        ans[count++]=n%26+'A';
        n/=26;
    }
    ans[count]='\0';
    revers(ans);
    return ans;
}


Related to question Excel Sheet Column Title

Given a column title as appear in an Excel sheet, return its corresponding column number.

For example:

    A -> 1
    B -> 2
    C -> 3
    ...
    Z -> 26
    AA -> 27
    AB -> 28 

int titleToNumber(char* s) {
    int ans,i;
    
    ans=0;
    while(s[i]!='\0'){
        ans=ans*26+s[i]-'A'+1;
        i++;
    }
    return ans;
}
