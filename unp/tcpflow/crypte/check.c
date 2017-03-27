#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "gsl_sf_gamma.h"
#include "gsl_cdf.h"

int CumulativeSum(unsigned char* data,int len) {
  if (data == NULL || len < 16) {
    return -1;
  }
  int maxZ = 0;
  int cum = 0;
  int i,j;
  for ( i = 0; i < len; ++i) {
    for ( j = 7; j >= 0; --j) {
        if ( data[i]&(1<<j) ) {
            cum++;
        }
        else {
            cum--;
        }
        if (abs(cum) > maxZ) {
            maxZ = abs(cum);
        }
    }
  }
  int bitlen = len*8;
  double pValue = 1.0;
  for ( i = (-bitlen/maxZ+1)/4; i <= (bitlen/maxZ-1)/4; ++i) {
    pValue -= gsl_cdf_ugaussian_P((4*i+1)*maxZ/sqrt(bitlen*1.0))-
        gsl_cdf_ugaussian_P((4*i-1)*maxZ/sqrt(bitlen*1.0));
  }
  for ( i = (-bitlen/maxZ-3)/4; i <= (bitlen/maxZ-1)/4; ++i) {
    pValue += gsl_cdf_ugaussian_P((4*i+3)*maxZ/(sqrt(bitlen*1.0)))-
		gsl_cdf_ugaussian_P((4*i+1)*maxZ/sqrt(bitlen*1.0));
  }
  if (pValue < 0.01) {
	  return 0;
  }
  return 1;
}


int BlockFrequency(unsigned char* data, int len) {
    if (data == NULL || len < 16) {
        return -1;
    }
    int sub_bit_len=100;
    int bit_len = len*8;
    int block_num = bit_len/sub_bit_len;
    double x_obs = 0;
    int sum;
    int byte_index,bit_index;
    int i,j;
    for ( i = 0; i < block_num; ++i) {
         sum = 0;
        for ( j = 0; j < sub_bit_len; ++j) {
             byte_index = (i*sub_bit_len+j)/8;
             bit_index = 7-((i*sub_bit_len+j)%8);
            if ( data[byte_index]&(1<<bit_index)) {
                sum++;
            }
        }
        double tmp = (sum*1.0/sub_bit_len-0.5);
        x_obs += 4*sub_bit_len*tmp*tmp;
    }
    double pValue = gsl_sf_gamma_inc_Q(block_num/2.0,x_obs/2.0);
    if (pValue < 0.01) {
        return 0;
    }
    return 1;
}

inline char GetBit(unsigned char* data, int len,int bit_pos) {
    int byte_pos = (bit_pos/8)%len;
    bit_pos %= 8;
    if ( data[byte_pos]&(1<<(7-bit_pos)) ) {
        return 1;
    }
    return 0;
}

double FrequencyEntropy(unsigned char*data,int len,int sub_bit_len) {
    int len_new=1<<sub_bit_len;
    int* freq = malloc(sizeof(int)*len_new);
    int bit_len = len*8;
    memset(freq,0,sizeof(int)*(1<<sub_bit_len));
    int cur = 0;
    int i;
    double g;
    for ( i = 0; i < sub_bit_len-1; ++i) {
        cur = (cur<<1)+GetBit(data,len,i);
    }

    for ( i = 0; i < bit_len; ++i) {
        cur = (cur<<1)+GetBit(data,len,i+sub_bit_len-1);
        freq[cur]++;
        cur &= (1<<(sub_bit_len-1))-1;
    }
    double ret = 0;
    for ( i = 0; i < (1<<sub_bit_len); ++i) {
        if (freq[i] != 0) {
             g = freq[i]*1.0/bit_len;
            ret += g*log(g);
        }
    }
    free(freq);
    return ret;
}

int ApproximateEntropy(unsigned char*data, int len) {
    if (data == NULL || len < 16) {
        return -1;
    }
    int sub_bit_len=6;
    int bit_len = len*8;
    double x1 = FrequencyEntropy(data,len,sub_bit_len);
    double x2 = FrequencyEntropy(data,len,sub_bit_len+1);
    double apEn =x1-x2;
    double x_test = 2.0*bit_len*(log(2.0)-apEn);
    double pValue = gsl_sf_gamma_inc_Q(pow(2,sub_bit_len-1),x_test/2);
    if (pValue < 0.01) return 0;
    return 1;
}

int detect_encrypted_stream(unsigned char* data,int length) {
    if (data == NULL || length < 32 || length > 65536) {
        return -1;
    }
    if (CumulativeSum(data,length) && BlockFrequency(data,length) &&
         ApproximateEntropy(data,length) ) {
         return 1;
    }
    return 0;
}

