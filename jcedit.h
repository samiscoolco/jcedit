/* jcedit.h */

#ifndef JCEDIT_H
#define JCEDIT_H

/*** defines ***/

#define DISP_HEIGHT 20


/*** functions ***/

/* i/o */
void clear(void);
void print_top(void);
void refresh_screen(void);
int getch(int b,int no_esc);
char* text_entry(void);
/* utility */
int calc_maxdisp(void);
int file_exist(const char *filename);
void init(int argc, char **argv);

#endif
