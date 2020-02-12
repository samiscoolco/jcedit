/* jcedit.c */

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "jcedit.h"
#include "syntax.h"
#include "config.h"

#define VERNO "4.1"

#define DBGS(s) printf("%s\n", s)
#define DBGI(i) printf("%d\n", i)


/*** utility ***/

//forced force flush :)
void ffflush(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}


void die(char *msg, int ret) {
	if (msg != NULL) {
		DBGS(msg);
	}
	exit(ret);
}

int calc_maxdisp(void) {
	return (ED.disp+ED.dispLength > ED.linemax) ? ED.linemax : ED.disp+ED.dispLength; // change
}


int file_exist(const char* filename) {
	/* try to open file to read */
	FILE *file;
	if (file = fopen(filename, "r")) {
		fclose(file);
		return 1;
	}
	return 0;
}


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


char header[50];
struct editorData ED;
char clinetext[1001];
//char lineJump[8];

/*** etc ***/
void show_help(void){
	clear();
	printf("%s\nWritten by sam0s & jdedmondt\n\n",header);
	printf("Commands:\n\t..? - Show this screen\n\t.qt - Close JCEdit\n\t.sv - Save currently open file\n\t.ln - Jump to certain line by number\n\t.mv - Scroll around the file while in command mode\n\t.i - Enter insert mode (press escape to return to command mode)\n\nPress enter to return...");
	getchar();ffflush();
}

//SAFE GETS
char *sgets(char *line, size_t size)
{
   size_t i;
   for ( i = 0; i < size - 1; ++i )
   {
      int ch = fgetc(stdin);
      if ( ch == '\n' || ch == EOF )
      {
         break;
      }
      line[i] = ch;
   }
   line[i] = '\0';
   return line;
}

int digits_in_int(int i) {
	int digits = 0;
	while (i) {
		i -= i % 10;
		i /= 10;
		digits++;
	}
	return digits;
}

/*** i/o ***/

void print_file(char **file, int i, int x) {
	char* printed = NULL;
	for (i;i<x;i++) {
		if (ED.clinenum == i){
			printf("\x1b[33;1m%3d\x1b[0m| ",i);
		}else{printf("%3d| ", i);}
		
		//i represents current row
		highlight_syntax(file[i],i);
	}
	if (ED.clinenum==ED.linemax){
		//if we are editing the newest line
		printf("\x1b[33;1m%3d\x1b[0m| \n",ED.linemax);
	}
}

void clear(void) {    
	printf("\x1b[?251\x1b[2J\x1b[H\x1b[?25h");
}

void print_top(void) {
	printf(CYAN);
	printf("%s \n",header);
	printf("FILENAME: %s | LINEMAX: %d | CUR_LINE: %d\n\n",ED.filename,ED.linemax,ED.clinenum);
	printf("\x1b[0m");
}

void refresh_screen(void) {
	clear();
	print_top();
	//maybe going to add ls call here once its functionalized.
}

int process_keypress(void) {
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1) != 1))
		; /* TODO: ADD ERROR CHECKING HERE */
	
	if(c == '\x1b') { /* \x1b is 27 in hex */
		//DBGS("we have an escape sequence");
		char seq[3];
		if (read(STDIN_FILENO, &seq[0], 1) != 1) { return '\x1b'; }
		if (read(STDIN_FILENO, &seq[1], 1) != 1) { return '\x1b'; }

		if (seq[0] == '[') {
			//DBGS("SECOND CHAR IS [");
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
					case '8': return END_KEY;					}	
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

int getch(void) {
	struct termios oldattr, newattr;
	
	int ch;

	tcgetattr(STDIN_FILENO, &oldattr);
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);
	newattr.c_cc[VMIN] = 0; /*	Read requires 0 bytes to return */
	newattr.c_cc[VTIME] = 0; /* Set timeout of 0 seconds */
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
	
	ch = process_keypress();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

	return ch;
}

void init(int argc, char** argv){

	clear();

	/* syntax file selection menu */
	DIR *d;
	struct dirent *dir;
	char **syntax_files = malloc(sizeof(char*));
	
	d = opendir(SYNTAX_PATH);
	
	int s = 0;
	
	if (d != NULL) {
		while ((dir = readdir(d)) != NULL) {
			if (!strcmp(dir->d_name, "..") || !strcmp(dir->d_name, ".")) { continue; }
			
			char *ext = strchr(dir->d_name, '.');
			if (!ext) {
				continue;
			} else if (strcmp(ext, ".syntax")) {
				continue;
			}
			
			syntax_files[s++] = strdup(dir->d_name);
			syntax_files = realloc(syntax_files, sizeof(char *)*(s+1));
		}
		
		closedir(d);
	}
	
	if (s == 0) {
		die("input: syntax menu: no syntax files found", 1);
	}
	
	printf("Available syntax files in: ");
	printf(SYNTAX_PATH);
	printf("\n");
	for (int j = 0; j < s; j++) {
		printf("%d %s\n", j, syntax_files[j]);
	}
	
	printf("Please enter the number of the syntax file you would like to use (default=0)\n");
	
	char *input = malloc(sizeof(char)*10);
	int choice;
	fgets(input, 10, stdin);
	if (strlen(input) > 0) {
		choice = atoi(input);
		if (choice >= s) {
			die ("input: syntax menu: invalid file number entered.", 1);
		}
	}

	char *syn_path = strdup(SYNTAX_PATH);
	
	syn_path = realloc(syn_path, strlen(syn_path) + strlen(syntax_files[choice]));
	
	syn_path = strcat(syn_path, syntax_files[choice]);

	FILE *syn = fopen(syn_path, "r");
	if (syn == NULL) {
		die("error: init: could not open syntax file", 1);
	}
	char *keyw = malloc(32);
	char  c = ' ';
	int i=0;
	keywords = NULL;
	colors = NULL;
	while(fscanf(syn,"%c %s\n",&c,keyw)!=-1){
		keywords=realloc(keywords,sizeof(char*)*(i+1));
		colors=realloc(colors,sizeof(char*)*(i+1));
		keywords[i] = malloc(strlen(keyw));
		colors[i] = malloc(strlen(RED));
		keywords[i] = strdup(keyw);
		colors[i] = strdup(get_color(c));
		i++;
	}
	free(keyw);
	keywords=realloc(keywords,sizeof(char*)*(i+1));
	fclose(syn);
	
	int lim_display = LIMIT_DISPLAY;
	struct winsize w;
	
	if (!lim_display) {
	
		/* attempt to get screen size with ioctl */
		if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
			lim_display = 1;
		}
		
	}
	
	//init variables
	ED.pos=0;
	ED.linemax = 0;
	ED.clinenum = 0;
	ED.cmd = 0;
	ED.mode = 0;
	ED.disp = 0;
	ED.dispLength = lim_display ? DISPLAY_LENGTH : w.ws_row-7;
	ED.filename = NULL;
	ED.full_file = NULL;

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
			ED.full_file = realloc(ED.full_file, sizeof(char*)*(ED.linemax+1));
			ED.full_file[ED.linemax] = malloc(len+1);
			if(line[len-1]=='\n'){line[len-1]='\0';}	//strip newline chars if they exist
			ED.full_file[ED.linemax] = strndup(line,len);
			ED.linemax+=1;
		}
		
		fclose(fp);
	} else {
		ED.full_file=malloc(sizeof(char*));
		//ED.full_file[0] = malloc(sizeof(char));
	}
	
}

void remove_cur_line(void) {
	//free(ED.full_file[ED.clinenum]);
	memmove(ED.full_file + ED.clinenum, ED.full_file + ED.clinenum + 1, sizeof(char*)*(ED.linemax-ED.clinenum-1));
	--ED.linemax;
}

void remove_cur_char(void) {
	memmove(ED.full_file[ED.clinenum] + ED.pos, ED.full_file[ED.clinenum] + ED.pos + 1, sizeof(char)*strlen(ED.full_file[ED.clinenum]+ED.pos));
}

void insert_line_after_cur_line(void) {

}

/*** main ***/
int main(int argc, char *argv[]) {
	init(argc,argv);
	int run = 1;
	refresh_screen();
	while (run) {	
		refresh_screen();
		print_file(ED.full_file, ED.disp, calc_maxdisp());
		printf("\n%3d| ", ED.clinenum);
		ED.mode=0;
		fgets(clinetext,1001,stdin);
		clinetext[strcspn(clinetext, "\n")] = '\0';

  		if (strcmp(clinetext, "..?") == 0){
			ED.cmd=1;
			show_help();
			strcpy(clinetext,".ls");
		}
		if (strcmp(clinetext, ".qt") == 0){
			run=0;ED.cmd=1;
		}
		
		if (strcmp(clinetext, ".i") == 0) {
			ED.mode=1;
			int a;
			ED.cmd = 1;
			refresh_screen();
			print_file(ED.full_file, ED.disp, calc_maxdisp());
			char *cline = ED.full_file[ED.clinenum];
			ED.pos = 0;
			while ((a = getch()) != 27) {
				switch (a) {
				case '\n': ;
					/* create room for new line */
					char *temp = strdup(ED.full_file[ED.clinenum] + ED.pos);
					ED.full_file[ED.clinenum][ED.pos] = '\0';
					ED.full_file = realloc(ED.full_file, sizeof(char*)*(++ED.linemax));
					memmove(ED.full_file + ED.clinenum + 2, ED.full_file + ED.clinenum + 1, sizeof(char*)*(ED.linemax-ED.clinenum-2));
					ED.full_file[++ED.clinenum] = temp;
					ED.pos = 0;
					break;
				case ARROW_UP:
					if (ED.clinenum > ED.disp) {
						ED.clinenum--;
						ED.pos = ED.pos > strlen(ED.full_file[ED.clinenum]) ? strlen(ED.full_file[ED.clinenum]) : ED.pos;
					} else {
						int val = 0;
						val = (ED.disp)?1:0;
						ED.disp-=val;ED.clinenum-=val;
					}
					cline = ED.full_file[ED.clinenum];
					break;
				case ARROW_DOWN:
					if (ED.clinenum < ED.linemax-1) {
						if (ED.clinenum<ED.disp+ED.dispLength -1 ) {
							ED.clinenum +=1;
						} else { 
							ED.disp+=1;
							ED.clinenum+=1;
						}
						ED.pos = ED.pos > strlen(ED.full_file[ED.clinenum]) ? strlen(ED.full_file[ED.clinenum]) : ED.pos;
						cline = ED.full_file[ED.clinenum];
					}
					break;
				case ARROW_RIGHT:
					if (ED.pos < strlen(ED.full_file[ED.clinenum])) {
						ED.pos++;
					}
					break;
				case ARROW_LEFT:
					if (ED.pos > 0) {
						ED.pos--;
					}
					break;
				case BACKSPACE:
					if (ED.pos > 0) {
						ED.pos--;
						remove_cur_char();
					} else if (ED.clinenum > 0) {
						ED.pos = strlen(ED.full_file[ED.clinenum-1]);
						if (strlen(ED.full_file[ED.clinenum]) > 0) {
							ED.full_file[ED.clinenum-1] = strcat(ED.full_file[ED.clinenum-1], ED.full_file[ED.clinenum]);
						}
						remove_cur_line();
						ED.clinenum--;
						break;
					} 
					break;
				case DEL_KEY:
					if (strlen(ED.full_file[ED.clinenum]) > 0) {
            					if (ED.pos == strlen(ED.full_file[ED.clinenum]) && a == DEL_KEY) { 
							break; 
						}
						remove_cur_char();
					} else if (ED.linemax > 1) {
						remove_cur_line();
					}
					break;
				default:
					ED.full_file[ED.clinenum] = realloc(ED.full_file[ED.clinenum], strlen(ED.full_file[ED.clinenum])+2);
					memmove(ED.full_file[ED.clinenum] + ED.pos + 1, ED.full_file[ED.clinenum] + ED.pos, strlen(ED.full_file[ED.clinenum])-ED.pos+1);
					ED.full_file[ED.clinenum][ED.pos++] = a;
					break;
				}
				refresh_screen();
				//printf("linelen %ld\n", strlen(cline));
				print_file(ED.full_file, ED.disp, calc_maxdisp());
			}
		}
		
		if (strcmp(clinetext, ".mv") == 0) {
			int a;
			ED.cmd = 1;
			while ((a = getch()) != 27) {
				switch (a) {
				case 119: /* w */
				case ARROW_UP:
					if (ED.clinenum > ED.disp) {
						ED.clinenum -=1;
					}else{
						int val = 0;
						val = (ED.disp)?1:0;
						ED.disp-=val;ED.clinenum-=val;
					}
					break;
				case 115: /* s */
				case ARROW_DOWN:
					if (ED.linemax-1>=ED.clinenum){
					if (ED.clinenum<ED.disp+ED.dispLength -1 ) {
						ED.clinenum +=1;
					}else{
						ED.disp+=1;
						ED.clinenum+=1;
						}
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
					//printf("%d, %d\n", ED.disp, maxdisp);
					print_file(ED.full_file, ED.disp, maxdisp);
				}
			}
		}
		
		if (strcmp(clinetext, ".ln") == 0){
			ED.cmd=1; 	
			char* lineJump = malloc(sizeof(char*)*9999999);
			sgets(lineJump,9999999);
			//fgets(lineJump,999999,stdin);
			ED.clinenum = atoi(lineJump);
			if (ED.clinenum > ED.linemax || strlen(lineJump)>7) {ED.clinenum = ED.linemax;}
			if (ED.clinenum < 0) {ED.clinenum = 0;}
			ED.disp = ED.clinenum;
			free(lineJump);
		}
  
		if (strcmp(clinetext, ".sv") == 0){
			FILE* file = fopen(ED.filename, "w");
			
			for(int i=0;i<ED.linemax;i+=1){
				fprintf(file,"%s\n",ED.full_file[i]);fflush(file);
			}
			
			fclose(file);
			ED.cmd=1;
		}
  
		if (ED.cmd == 0) {
			if (ED.clinenum==ED.linemax){
				ED.full_file = realloc(ED.full_file, sizeof(char*)*(++ED.linemax));
				if(ED.disp+ED.dispLength<=ED.linemax){ED.disp++;}
			}
		
    	ED.full_file[ED.clinenum] = malloc(strlen(clinetext)+1);
			ED.full_file[ED.clinenum] = strdup(clinetext);
			ED.clinenum+=1;
		}

		ED.cmd = 0;

	}

	return 0;
}






// we ever gonna free tha memory doe?
// ok i know what i said before but once LINE-INSERTINg works
// then jcedit will actually work great
// syntax highlighting is still a meme

