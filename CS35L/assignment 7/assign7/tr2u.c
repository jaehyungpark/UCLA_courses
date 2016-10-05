#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// basically, tr2u.c is uses the same code from tr2b.c, but with read and 
// write instead of getchar and putchar
int main(int argc, char *argv[]){
  // error handling
  if (argc != 3){
    fprintf(stderr, "Error: not enough arguments");
    exit(1);
  }
  // the main program takes two oeprands form and to
  char *from = argv[1];
  char *to = argv[2];

  int i = 0;
  // check to see if two arguments have the same length
  // check where char ends with \0 and increment i at the same rate
  while(from[i] != '\0' && to[i] != '\0'){
    i++;
  }

  // check to see if two argument does not have the same length
  if((from[i] != '\0' && to[i] == '\0') || (from[i] == '\0' && to[i] != '\0')){
    fprintf(stderr, "Error: wrong length of arguments");
    exit(1);
  }

  // use read and write
  char buffer;
  while(read(0, &buffer, 1)>0){
    int j = 0;
    while(from[j] != '\0'){
      if(buffer == from[j]){
	break;
      }
      j++;
    }
    buffer = to[j];
    write(1, &buffer, 1);
  }
  return 0;
}

