#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void clear(void) {    
  while ( getchar() != '\n' );
}

int file_exist(const char * filename) {
    /* try to open file to read */
    FILE *file;
    if (file = fopen(filename, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}


int main(int argc, char **argv) {
  char *full_file[] = {"abc","q23","asdasda"};
  printf("%s",full_file[])
  char *filename = "hello.txt";
  int linemax = 0;
 	FILE *fp = fopen(filename, "r");
 	while(!feof(fp)){
 	fgets(full_file[linemax], 500, fp);
 	full_file[linemax][strcspn(full_file[linemax], "\n")] = 0;
 	linemax+=1;
 	}
 	fclose(fp);

	return 0;
}
