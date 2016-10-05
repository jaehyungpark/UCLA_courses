#include <stdio.h>
#include <stdlib.h>

// the main program takes two operands from and to that are byte string
// of the same length, and that copies standard input to standard output
int main(int argc, char *argv[]){
  // error handling
  if (argc != 3){
    fprintf(stderr, "Error: not enough arguments");
    exit(1);
  }
  // the main program takes two operands from and to
  char *from = argv[1];
  char *to = argv[2];

  int i = 0;
  // check to see if two arguments have the same length
  // check where char ends with \0 and  increment i at the same rate
  while(from[i] != '\0' && to[i] != '\0'){
    i++;
  }
  
  // check to see if two argument does not have the same length
  if((from[i] != '\0' && to[i] == '\0') || (from[i] == '\0' && to[i] != '\0')){
    fprintf(stderr, "Error");
    exit(1);
  }

  // use getchar and putchar
  char buffer;
  while(buffer = getchar() != EOF){
    int j = 0;
    while(from[j] != '\0'){
      if(buffer == from[j]){
	break;
      }
      j++;
    }
    buffer = to[j];
    putchar(buffer);
  }
  return 0;
}
