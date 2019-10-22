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

int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

struct editorData{
  int linemax;
  int clinenum;
  int cmd;
  int disp;
  int dispLength;
  char *filename;
};

const char header[50] = "JCEdit Version 3.2 - Written by sam0s";
struct editorData ED;
char full_file[9999][100];
char clinetext[100];
char lineJump[5];

void init(int argc, char **argv){
  //init variables
  ED.linemax = 0;
  ED.clinenum = 0;
  ED.cmd = 0;
  ED.disp = 0;
  ED.dispLength = 20;
  ED.filename = NULL;

  //load file if it exists
  if(argc>1)
  {
    ED.filename = argv[1];
  }
  else{
  //file name prompt
  system("cls||clear");
  printf("%s \nEnter file name: ",header);
  ED.filename = malloc(sizeof(char)*50);
  fgets(ED.filename,50,stdin);
  ED.filename[strcspn(ED.filename, "\n")] = '\0';

  }

  //load file if it exists
  if(file_exist(ED.filename))
  {
    FILE *fp = fopen(ED.filename, "r");
    while(!feof(fp)){
    fgets(full_file[ED.linemax], 500, fp);
    full_file[ED.linemax][strcspn(full_file[ED.linemax], "\n")] = 0;
    ED.linemax+=1;
    }
    fclose(fp);
  }

  system("cls||clear");
  printf("%s \n",header);
  printf("FILENAME: %s | LINEMAX: %d \n\n",ED.filename,ED.linemax);
  
}

int main(int argc, char *argv[]) {

  init(argc,argv);
	int run = 1;
  
	while(run>0){
		printf("%d| ", ED.clinenum);
		fgets(clinetext,100,stdin);
		clinetext[strcspn(clinetext, "\n")] = '\0';
  
		if(strcmp(clinetext, ".qt") == 0){run=0;ED.cmd=1;}
		if(strcmp(clinetext, ".mv") == 0){
			ED.cmd=1;
			int a=119;
  
			while(a == 119 || a == 115){
				a=getch();
        if(a==113){continue;}
        ED.disp += (a==119) ? -1 : 1;
        system("cls||clear");printf("%s \n",header);ED.cmd=1;
        printf("FILENAME: %s | LINEMAX: %d \n",ED.filename,ED.linemax);
  
        if(ED.linemax>=1){
        printf("\n");
        for(int i=ED.disp;i<ED.disp+ED.dispLength;i+=1){
          printf("%d| %s\n",i,full_file[i]);
          }
        }
        printf("\n");
  
			}
			clear();
		}
		if(strcmp(clinetext, ".ln") == 0){
			fgets(lineJump,5,stdin);
			ED.clinenum = atoi(lineJump);
			if (ED.clinenum > ED.linemax) {ED.clinenum = ED.linemax;}
			if (ED.clinenum < 0) {ED.clinenum = 0;}
			fflush(stdin);
			strcpy(clinetext,".ls");
		}
  
		if(strcmp(clinetext, ".sv") == 0){
			FILE* file = fopen(ED.filename, "w");
			for(int i=0;i<ED.linemax;i+=1){
				fprintf(file,"%s\n",full_file[i]);fflush(file);
				}
			fclose(file);
			ED.cmd=1;
		}
  
    if(strcmp(clinetext, ".ls") == 0){
      system("cls||clear");printf("%s \n",header);ED.cmd=1;
      printf("FILENAME: %s | LINEMAX: %d \n",ED.filename,ED.linemax);
  
      if(ED.linemax>=1){
      printf("\n");
      for(int i=ED.disp;i<ED.disp+ED.dispLength;i+=1){
        printf("%d| %s\n",i,full_file[i]);
        }
      }
      printf("\n");
    }
  
  
		if(ED.cmd==0){
		strcpy(full_file[ED.clinenum], clinetext);
		ED.clinenum+=1;
		if (ED.clinenum>ED.linemax){ED.linemax+=1;}
		}
  
		ED.cmd = 0;
		strcpy(clinetext, "");
		fflush(stdin);
	}

	return 0;
}
