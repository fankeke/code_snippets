Given two sorted integer arrays nums1 and nums2, merge nums2 into nums1 as one sorted array.

Note:
You may assume that nums1 has enough space (size that is greater or equal to m + n) to hold additional elements from nums2. The number of elements initialized in nums1 and nums2 are m and n respectively.


https://leetcode.com/problems/merge-sorted-array/



void merge(int* nums1, int m, int* nums2, int n) {
    int         *end,*nums1_end,*nums2_end;
    
   nums1_end=nums1+m-1;
   nums2_end=nums2+n-1;
   end=nums1+m+n-1;
    
    while(nums1_end>=nums1&&nums2_end>=nums2){
        if(*nums1_end>*nums2_end){
            *end=*nums1_end;
            end--;nums1_end--;
        }else{
            *end=*nums2_end;
            end--;nums2_end--;
        }
    }
    
    while(nums2_end>=nums2){
        *end=*nums2_end;
        end--;
        nums2_end--;
    }
}



 void merge(int A[], int m, int B[], int n) {
        int i=m-1;
        int j=n-1;
        int k = m+n-1;
        while(i >=0 && j>=0)
        {
            if(A[i] > B[j])
                A[k--] = A[i--];
            else
                A[k--] = B[j--];
        }
        while(j>=0)
            A[k--] = B[j--];
    }
