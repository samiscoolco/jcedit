/* jcedit.h */

#ifndef JCEDIT_H
#define JCEDIT_H

/*** functions ***/

/* i/o */
void clear(void);
int getch(void);

/* utility */
int file_exist(const char *filename);
void init(int argc, char **argv);

#endif