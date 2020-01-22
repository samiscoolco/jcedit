#ifndef SYNTAX_H
#define SYNTAX_H

#define RED "\033[91m"
#define GREEN "\033[92m"
#define CYAN "\033[96m"
#define BLUE "\033[94m"
#define YELLOW "\033[93m"
#define PURPLE "\033[1;35m"

/*** functions ***/
void highlight_syntax(char* inp);
int check_syntax(char* full,int pos);
char* get_color(char a);
extern char **keywords;
extern char **colors;
extern int keywordlen;

#endif
