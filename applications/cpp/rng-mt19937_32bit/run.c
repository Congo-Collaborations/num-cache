#include <stdio.h>
#include "mt32.h"

int main(void)
{
    // Numb::setup("cache2", "cache1", "ignore-cache");
    Numb::setup("cache2", "", "ignore-cache");
    int i;
    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length);
    printf("1000 outputs of genrand_int32()\n");
    for (i=0; i<1000; i++) {
      printf("%10lu ", genrand_int32());
      if (i%5==4) printf("\n");
    }
    printf("\n1000 outputs of genrand_real2()\n");
    for (i=0; i<1000; i++) {
      printf("%10.8f ", genrand_real2().value);
      if (i%5==4) printf("\n");
    }
    return 0;
}