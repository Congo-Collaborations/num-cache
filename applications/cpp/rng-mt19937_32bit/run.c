#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include "mt32.h"

void run(int argc, char **argv){
  long int population = 10;

  if(argc >= 2){
    char *p;
    errno = 0;
    long conv = strtol(argv[1], &p, 10);
    if (errno != 0 || *p != '\0' || conv > INT_MAX) {
        printf("parameter as to be an integer.\n");
        exit(1);
    } else {
        population = conv;
    }
  }

  long int i;
  unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
  init_by_array(init, length);

  FILE *fptr;
  fptr=fopen("genrand_real1.txt","a+");
  if(fptr==NULL){
      printf("Error! Could not write into data.txt.");
      exit(2);
  }
  printf("\n%ld outputs of genrand_real1()\n", population);
  for (i=0; i<population; i++) {
    fprintf(fptr,"%10.8f ", genrand_real1().value);
    if (i%5==4) fprintf(fptr,"\n");
  }
  fprintf(fptr,"+++ END +++\n");
  fclose(fptr);

  fptr=fopen("genrand_real2.txt","a+");
  if(fptr==NULL){
      printf("Error! Could not write into data.txt.");
      exit(2);
  }
  printf("\n%ld outputs of genrand_real2()\n", population);
  for (i=0; i<population; i++) {
    fprintf(fptr,"%10.8f ", genrand_real2().value);
    if (i%5==4) fprintf(fptr,"\n");
  }
  fprintf(fptr,"+++ END +++\n");
  fclose(fptr);

  fptr=fopen("genrand_real3.txt","a+");
  if(fptr==NULL){
      printf("Error! Could not write into data.txt.");
      exit(2);
  }
  printf("\n%ld outputs of genrand_real3()\n", population);
  for (i=0; i<population; i++) {
    fprintf(fptr,"%10.8f ", genrand_real3().value);
    if (i%5==4) fprintf(fptr,"\n");
  }
  fprintf(fptr,"+++ END +++\n");
  fclose(fptr);

  fptr=fopen("genrand_res53.txt","a+");
  if(fptr==NULL){
      printf("Error! Could not write into data.txt.");
      exit(2);
  }
  printf("\n%ld outputs of genrand_res53()\n", population);
  for (i=0; i<population; i++) {
    fprintf(fptr,"%10.8f ", genrand_res53().value);
    if (i%5==4) fprintf(fptr,"\n");
  }
  fprintf(fptr,"+++ END +++\n");
  fclose(fptr);
}

int main(int argc, char **argv)
{
  printf("+++ Generating cache1.");
  Numb::setup("cache1", "", 15, "ignore-cache");
  run(argc, argv);
  printf("+++ Generating cache2 with cache1 as witness.");
  Numb::setup("cache2", "cache1", 15, "ignore-cache");
  run(argc, argv);
  return 0;
}
