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

int main(int argc, char *argv[]) {
	//init variables
	const char header[50] = "JCEdit Version 3 - Written by sam0s";
	
  char full_file[9999][100];
	char clinetext[100];
	char *lineJump = malloc(sizeof(char)*5);
	
  int linemax = 0;
	int clinenum = 0;
	
  int cmd = 0;
  
  int dispmin = 0;
  int dispmax = 7;
  
	char *filename = NULL;

	//load file if it exists
	if(argc>1)
	{
		filename = argv[1];
	}
	else{
	//file name prompt
	system("cls||clear");
	printf("%s \nEnter file name: ",header);
	filename = malloc(sizeof(char)*50);
	fgets(filename,50,stdin);
	filename[strcspn(filename, "\n")] = '\0';

	}

	//load file if it exists
	if(file_exist(filename))
	{
		FILE *fp = fopen(filename, "r");
		while(!feof(fp)){
		fgets(full_file[linemax], 500, fp);
		full_file[linemax][strcspn(full_file[linemax], "\n")] = 0;
		linemax+=1;
		}
		fclose(fp);

	}

	system("cls||clear");
	printf("%s \n",header);
	printf("FILENAME: %s | LINEMAX: %d \n\n",filename,linemax);

	int run = 1;

	while(run>0){

		printf("%d| ", clinenum);
		fgets(clinetext,100,stdin);
		clinetext[strcspn(clinetext, "\n")] = '\0';
    
		if(strcmp(clinetext, ".qt") == 0){run=0;cmd=1;}
		if(strcmp(clinetext, ".mv") == 0){
			cmd=1;
			int a=119;
			
			while(a == 119 || a == 115){
				a=getch();
			}
			
			clear();
			
		}
		if(strcmp(clinetext, ".ln") == 0){
			
			fgets(lineJump,5,stdin);
			clinenum = atoi(lineJump);
			if (clinenum > linemax) {clinenum = linemax;}
			if (clinenum < 0) {clinenum = 0;}
			fflush(stdin);
			strcpy(clinetext,".ls");
		}

		if(strcmp(clinetext, ".sv") == 0){
			FILE* file = fopen(filename, "w");
			for(int i=0;i<linemax;i+=1){
				fprintf(file,"%s\n",full_file[i]);fflush(file);
				}
			fflush(file);
			fclose(file);
			cmd=1;
		}

		if(strcmp(clinetext, ".ls") == 0){
			system("cls||clear");printf("%s \n",header);cmd=1;
			printf("FILENAME: %s | LINEMAX: %d \n",filename,linemax);

			if(linemax>=1){
			printf("\n");
			for(int i=0;i<linemax;i+=1){
				printf("%d| %s\n",i,full_file[i]);
				}
			}

			printf("\n");
		}

		if(cmd==0){
		strcpy(full_file[clinenum], clinetext);
		clinenum+=1;
		if (clinenum>linemax){linemax+=1;}
		}
	
		cmd = 0;
		strcpy(clinetext, "f");
		fflush(stdin);
	}

	return 0;
}
