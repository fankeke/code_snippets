void sortColors(int* nums, int numsSize) {
    int         num0,num1,num2,i;
    
    num0=num1=num2=0;
    i=0;
    
    //总的来说，就是插入排序，给每个不同的值都给一个下标
    //然后就是插入，如果插入的值比较小，则需要把比它大的数全部都要移向后面，且移动顺序要从大的开始，
    
    while(i<numsSize){
        if(nums[i]==2){
            nums[num2++]=2;
        }else if(nums[i]==1){
            nums[num2++]=2;
            nums[num1++]=1;
        }else{
            nums[num2++]=2;
            nums[num1++]=1;
            nums[num0++]=0;
        }
        i++;
    }
    
}

//这种排序对于只存在几个不同的值但是数据量有非常大的数组是非常高效的。
//补充一个有四个不同的0，1，2，3的排序
void sort4nums(int*nums,int numssize)
{
    int n0,n1,n2,n3,i;
    n0=n1=n2=n3=i=0;
    
    while(i<numsSize){
        if(nums[i]==3){
            nums[n3++]=nums[i];
        }else if(nums[i]==2){
            nums[n3++]=nums[i];
            nums[n2++]=nums[i];
        }else if(nums[i]==1){
            nums[n3++]=nums[i];
            nums[n2++]=nums[i];
            nums[n1++]=nums[i];
        }else {
            nums[n3++]=nums[i];
            nums[n2++]=nums[i];
            nums[n1++]=nums[i];
            nums[n0++]=nums[i];
        }
        i++;
    }
    
}
