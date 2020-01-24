/* jcedit.h */

#ifndef JCEDIT_H
#define JCEDIT_H

/*** functions ***/

/* i/o */
void clear(void);
void print_top(void);
void refresh_screen(void);
int getch(void);

/* utility */
int calc_maxdisp(void);
int file_exist(const char *filename);
void init(int argc, char **argv);


struct editorData{
	int linemax;
	int clinenum;
	int cmd;
	int disp;
	int pos;
	int dispLength;
	char *filename;
};


extern struct editorData ED;

#endif
