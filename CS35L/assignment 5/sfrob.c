#include <stdio.h>
#include <stdlib.h>
// well, finally this program works, which took longer than I expected...
// this c program reads stdin byte-by-byte and each byte is frobnicated
// which each input is XORed with dec 42 (*^42)

// frobcmp takes two arguments a and b as input and returns an int
// that result in negative, zero, or positive depending on whether a is
// less than, equal to, or greater than b.
int frobcmp(char const *a, char const *b){
  do{
    // equal results in zero
    if(*a == ' ' && *b == ' '){
      return 0;
    }
    // greater than b results in positive
    else if(*b == ' ' || ((*a^42) > (*b^42))){
      return 1;
    }
    // less than b results in negative
    else if(*a == ' ' || ((*a^42) < (*b^42))){
      return -1;
    }
    // increment
    a++;
    b++;
  }
  // this is where I encountered an issue for hours/days and couldn't solve
  // but now it seems the increaed array size is twice the size of the original
  // size of the array 16, so I limited the size to 2*16, which now makes
  // the correct output
  // without this condition, it compiles but outputs the wrong result
  // to make sure this is right, I tried different numbers from 25 to 40
  // just in case and as expected, 32 produced the correct output
  while ((*a != 32) && (*b != 32));
  return 0;
}

int do_frobcmp(const void* a, const void* b){
  char const* a_in = *(char const**)a;
  char const* b_in = *(char const**)b;
  return frobcmp(a_in, b_in);
}

int main(){
  int allocsize = 1000;
  int wordcount = 0;
  char get_char;
  char* input_arr = (char*)malloc(sizeof(char)*allocsize);
  // check
  if (input_arr == NULL){
    fprintf(stderr, "error to allocate memory");
    exit(1);
  }
  // I checked the GNU c library and stackoverflow for handling EOF 
  // and figured out to use getchar()
  int i=0;
  for(i; (get_char=getchar())!=EOF;i++){
    if(i == allocsize){
      char* check = (char*)realloc(input_arr, allocsize);
      if(check != NULL){
	input_arr = check;
      }
      // check
      if(input_arr == NULL){
	fprintf(stderr, "error to reallocate memory");
	exit(1);
      }
      // twice the size and increase the mem allocation size
      allocsize = allocsize*2;
    }
    // determine space(SP)
    if(get_char == ' '){
      wordcount++;
    }
    // end of line
    if(get_char == '\n'){
      break;
    }
    input_arr[i] = get_char;
  }
  if(input_arr[i] != ' '){
    wordcount++;
    input_arr[i] = ' ';
  }
  char** input_words = (char**)malloc(sizeof(char*)*wordcount);
  // check
  if (input_words == NULL){
    fprintf(stderr, "error");
    exit(1);
  }
  int l=0;
  char* firstptr = input_arr;
  for (l; l < wordcount; l++){
    input_words[l] = firstptr;
    if(l == (wordcount-1)){
      break;
    }
    while(*firstptr != ' '){
      firstptr++;
    }
    firstptr++;
  }
  // use qsort to sort the pointers
  qsort(input_words, wordcount, sizeof(char*), do_frobcmp);
  
  int n=0;
  for (n; n < wordcount; n++){
    char* output = input_words[n];
    // check
    if (output == NULL){
      fprintf(stderr, "error");
      exit(1);
    }
    putchar(*output);
    while (*output != ' '){
      output++;
      putchar(*output);
    }
  }
  // free
  free(input_arr);
  free(input_words);
}
