#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int compare = 0;

// this is based on week 5's sfrob.c program
int frobcmp(char const *a, char const *b){
  //compare++;
  do{
    if(*a == ' ' && *b == ' '){
      return 0;
    }
    else if(*b == ' ' || ((*a^42) > (*b^42))){
      return 1;
    }
    else if(*a == ' ' || ((*a^42) < (*b^42))){
      return -1;
    }
    a++;
    b++;
  }
  while ((*a != 32) && (*b != 32));
  return 0;
}

int do_frobcmp(const void* a, const void* b){
  char const* a_in = *(char const**)a;
  char const* b_in = *(char const**)b;
  compare++;
  return frobcmp(a_in, b_in);
}

int main(){
  struct stat fileSTAT;
  fstat(0, &fileSTAT);
  int input_char;
  int file_size = fileSTAT.st_size;
  char* input_arr;
  int wordcount = 0;
  int i = 0;
  if (!S_ISREG(fileSTAT.st_mode)){
    int allocatesize = 1000;
    if(read(0, &input_char, 1) == 0){
      exit(1);
    }
    input_arr = (char*)malloc(sizeof(char)*allocatesize);
    if(input_arr == NULL){
      fprintf(stderr, "Error");
      exit(1);
    }
    input_arr[i] = input_char;
    i++;
    while (read(0, &input_char, 1) > 0){
      if(i == allocatesize){
	input_arr = (char*)realloc(input_arr, allocatesize*2);
	if (input_arr == NULL){
	  fprintf(stderr, "Error");
	  exit(1);
	}
	allocatesize *=2;
      }
      // determine space(SP)
      if((char)input_char == ' '){
	wordcount++;
      }
      if((char)input_char == '\n'){
	wordcount++;
	input_arr[i] = ' ';
	break;
      }
      input_arr[i] = (char)input_char;
      i++;
    }
  }
  else if (S_ISREG(fileSTAT.st_mode)){
    if(read(0, &input_char, 1) == 0){
      return 0;
    }
    input_arr = (char*)malloc(sizeof(char)*file_size);
    if (input_arr == NULL){
      fprintf(stderr, "Error");
      exit(1);
    }
    input_arr[i] = input_char;
    i++;
    int num;
    int size = file_size;
    while(read(0, &input_char, 1)>0){
      if(i == size){
	input_arr = (char*)realloc(input_arr, size*2);
	if (input_arr == NULL){
	  fprintf(stderr, "Error");
	  exit(1);
	}
	size *=2;
      }
      if((char)input_char == ' '){
	wordcount++;
      }
      if((char)input_char == '\n'){
	wordcount++;
	input_arr[i] = ' ';
	i++;
	continue;
      }
      input_arr[i] = (char)input_char;
      i++;
      num++;
      fstat(0, &fileSTAT);
      size = fileSTAT.st_size;
    }
  }
  int array_size = i;
  char** input_words = (char**)malloc(sizeof(char*)*wordcount);
  if (input_words == NULL){
    fprintf(stderr, "Error");
    exit(1);
  }
  int l;
  char* firstptr = input_arr;
  for(l = 0; l < wordcount; l++){
    input_words[l] = firstptr;
    if(l == (wordcount-1)){
      break;
    }
    while(*firstptr != ' '){
      firstptr++;
    }
    firstptr++;
  }
  qsort(input_words, wordcount, sizeof(char*), do_frobcmp);
  int n;
  char* output;
  for(n = 0; n < wordcount; n++){
    output = input_words[n];
    write(1, &(*output), 1);
    while (*output != ' '){
      output++;
      write(1, &(*output), 1);
    }
  }
  free(input_arr);
  free(input_words);
  fprintf(stderr, "Comparisons: %d\n", compare);
  return 0;
}

  
