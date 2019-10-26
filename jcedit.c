/* jcedit.c */

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jcedit.h"

#define VERNO "3.5.2"

#define DBGS(s) printf("%s\n", s)

enum keys {
	BACKSPACE = 127,
	ARROW_LEFT = 1000,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN,
	DEL_KEY,
	HOME_KEY,
	END_KEY,
	PAGE_UP,
	PAGE_DOWN,
	INSERT_KEY
};


struct editorData{
	int linemax;
	int clinenum;
	int cmd;
	int disp;
	int dispLength;
	char *filename;
};

char header[50];
struct editorData ED;
char **full_file = NULL;
char clinetext[100];
char lineJump[5];

/*** etc ***/
void show_help(void){
	clear();
	printf("JCEdit Version %s\nWritten by sam0s & jdedmondt\n\n",VERNO);
	printf("Commands:\n\t..? - Show this screen\n\t.qt - Close JCEdit\n\t.sv - Save currently open file\n\t.ls - List lines into display window\n\t.ln - Set current line number\n\t.mv - Use W and S to scroll around the display window\n\nPress enter to return...");
	getchar();
}


/*** i/o ***/

void print_file(char **file, int i, int x) {
	for (i;i<x;i++) {
		printf("%3d| %s\n", i, file[i]);
	}
}

void clear(void) {    
	printf("\x1b[?251\x1b[2J\x1b[H\x1b[?25h");
}

void print_top(void) {
	printf("%s \n",header);
	printf("FILENAME: %s | LINEMAX: %d\n\n",ED.filename,ED.linemax);
}

void refresh_screen(void) {
	clear();
	print_top();
	//maybe going to add ls call here once its functionalized.
}

int process_keypress(int no_esc) {
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1) != 1))
		; /* TODO: ADD ERROR CHECKING HERE */
	
	
	
	if(c == '\x1b') { /* \x1b is 27 in hex */
		DBGS("we have an escape sequence");
		char seq[3];

		if (read(STDIN_FILENO, &seq[0], 1) != 1) { return '\x1b'; }
		if (read(STDIN_FILENO, &seq[1], 1) != 1) { return '\x1b'; }

		if (seq[0] == '[') {
			DBGS("SECOND CHAR IS [");
			if (seq[1] >= '0' && seq[1] <= '9') {
				if (read(STDIN_FILENO, &seq[2], 1) != 1) {
					return '\x1b';
				}
				if (seq[2] == '~'){
					switch(seq[1]) {	
					case '1': return HOME_KEY;
					case '2': return INSERT_KEY;
					case '3': goto ret; /* del_key */
					case '4': return END_KEY;
					case '5': return PAGE_UP;
					case '6': return PAGE_DOWN;
					case '7': return HOME_KEY;
					case '8': return END_KEY;
					}	
			 	}					
			} else {
				switch (seq[1]) {
				case 'A': return ARROW_UP;
				case 'B': return ARROW_DOWN;
				case 'C': return ARROW_RIGHT;
				case 'D': return ARROW_LEFT;
				case 'H': return HOME_KEY;
				case 'F': return END_KEY;
				case 'P': return DEL_KEY;
				}
			}	
		} else if (seq[0] == 0) {			
			switch (seq[1]) {
			case 'H': return HOME_KEY;
			case 'F': return END_KEY;
			}
		} 
		return '\x1b';
	} else {
		ret:
		return c;	
	}
}

int getch(int b, int no_esc) {
	struct termios oldattr, newattr;
	
	int ch;
	tcgetattr(STDIN_FILENO, &oldattr);
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);
	newattr.c_cc[VMIN] = b; /*	Read requires 0 bytes to return */
	newattr.c_cc[VTIME] = 0; /* Set timeout of 0 seconds */
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

	getchar();
	ch = process_keypress(no_esc);

	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

	return ch;
}

/*** utility ***/

int calc_maxdisp(void) {
	return (ED.disp+ED.dispLength > ED.linemax) ? ED.linemax : ED.disp+ED.dispLength;
}

int file_exist(const char * filename) {
	/* try to open file to read */
	FILE *file;
	if (file = fopen(filename, "r")) {
		fclose(file);
		return 1;
	}
	return 0;
}

void init(int argc, char **argv){
	//init variables
	ED.linemax = 0;
	ED.clinenum = 0;
	ED.cmd = 0;
	ED.disp = 0;
	ED.dispLength = DISP_HEIGHT;
	ED.filename = NULL;

	/* write header */
	sprintf(header, "JCEdit Version %s", VERNO);

	//load file if it exists
 	if (argc > 1) {
		ED.filename = argv[1];
	} else {
  		//file name prompt
  		system("cls||clear");
		printf("%s \nEnter file name: ",header);
		ED.filename = malloc(50);
		fgets(ED.filename,50,stdin);
		ED.filename[strcspn(ED.filename, "\n")] = '\0';
	}
	
	//load file if it exists
	if (file_exist(ED.filename)) {
		size_t cap = 0;
		ssize_t len;
		char *line = NULL;
		FILE *fp = fopen(ED.filename, "r");
		
		while ((len = getline(&line, &cap, fp)) != -1) {
			full_file = realloc(full_file, sizeof(char*)*(ED.linemax+1));
			full_file[ED.linemax] = malloc(len+1);
			full_file[ED.linemax] = strndup(line,len-1);
			ED.linemax+=1;
		}
		
		fclose(fp);
	} else {
		full_file=malloc(sizeof(char*));
	}
	
}

char* text_entry(void){
	int cur_char = 0;
	char *output = malloc(1);
	char entry = ' ';
	while(entry!='\n'){
		entry = getch(0,1);
		printf("%c",entry);
		output[cur_char++]=entry;	
		//resize string (change after advanced manipulation is added)
		output = realloc(output,cur_char+1);
	}
	output[cur_char] = '\0';
	return output;
}

/*** main ***/

int main(int argc, char *argv[]) {
	init(argc,argv);
	int run = 1;
	refresh_screen();
	
	char a = ' ';
	char *aa = malloc(32);
	int i = 0;
	while (run) {
		print_file(full_file, ED.disp, calc_maxdisp());
		printf("\n%3d| ", ED.clinenum);
		
		strcpy(clinetext,text_entry());
		clinetext[strcspn(clinetext, "\n")] = '\0';

  		if (strcmp(clinetext, "..?") == 0){
			ED.cmd=1;
			show_help();
			strcpy(clinetext,".ls");
		}
		if (strcmp(clinetext, ".qt") == 0){
			run=0;ED.cmd=1;
		}
		
		if (strcmp(clinetext, ".mv") == 0) {
			int a;
			ED.cmd = 1;
			while ((a = getch(0,1)) != 27) {
				switch (a) {
				case 119: /* w */
				case ARROW_UP:
					if (ED.disp > 0) {
						ED.disp--;
					}
					break;
				case 115: /* s */
				case ARROW_DOWN:
					if (ED.disp+ED.dispLength < ED.linemax) {
						ED.disp++;						
					}
					break;
				case PAGE_UP:
					if (ED.disp - ED.dispLength < 0) {
						ED.disp = 0;
					} else {
						ED.disp -= ED.dispLength;					
					}
					break;
				case PAGE_DOWN:
					if (ED.disp+ED.dispLength < ED.linemax) {
						ED.disp += ED.dispLength;
					}
					break;
				default:
					break;
				}

				refresh_screen();
				
				
				if (ED.linemax > 0) {
					int maxdisp = calc_maxdisp();
					//printf("%d, %d\n", ED.disp, maxdisp); /* show file coords (make optional or add to the header at top of screen) */
					print_file(full_file, ED.disp, maxdisp);
				}
				
			}
		}
		
		if (strcmp(clinetext, ".ln") == 0){
			fgets(lineJump,5,stdin);
			ED.clinenum = atoi(lineJump);
			if (ED.clinenum > ED.linemax) {ED.clinenum = ED.linemax;}
			if (ED.clinenum < 0) {ED.clinenum = 0;}
			fflush(stdin);
			strcpy(clinetext,".ls");
		}
  
		if (strcmp(clinetext, ".sv") == 0){
			FILE* file = fopen(ED.filename, "w");
			
			for(int i=0;i<ED.linemax;i+=1){
				fprintf(file,"%s\n",full_file[i]);fflush(file);
			}
			
			fclose(file);
			ED.cmd=1;
		}
  
		if (strcmp(clinetext, ".ls") == 0) {
			system("cls||clear");printf("%s \n",header);ED.cmd=1;
			printf("FILENAME: %s | LINEMAX: %d \n",ED.filename,ED.linemax);
  
			if(ED.linemax>=1) {
				printf("\n");
				int maxdisp = calc_maxdisp();						
				
				for(int i=ED.disp;i<maxdisp;i+=1){
        				printf("%d| %s\n",i,full_file[i]);
				}
			}
      			printf("\n");
		}
  
  
		if (ED.cmd == 0) {
    			full_file[ED.clinenum] = malloc(strlen(clinetext)+1);
			full_file[ED.clinenum] = strdup(clinetext);
			ED.clinenum+=1;
			
			if (ED.clinenum>ED.linemax){
      				ED.linemax+=1;
      
			}
    
			full_file = realloc(full_file, sizeof(char*)*(ED.linemax+1));
		}
  
		ED.cmd = 0;
		fflush(stdin);
		refresh_screen();
	}

	return 0;
}

