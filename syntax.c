#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "syntax.h"

char **keywords = NULL;
char **colors = NULL;
int keywordlen = 0;
int hi = 0;
//For reading the syntax file!!
char* get_color(char a){
	if (a < 97){
		char* ret = malloc(2);
		ret[0] = tolower(a);
		ret[1] = 'h';
		return ret;
	}
	switch(a){
		case 'p':
		return PURPLE;
		break;
		case 'r':
		return RED;
		break;
		case 'g':
		return GREEN;
		break;
		case 'b':
		return BLUE;
		break;
		case 'c':
		return CYAN;
		break;
		case 'y':
		return YELLOW;
		break;
		return YELLOW;
	}	
}

int check_syntax(char* full,int pos){
	int equiv = 1;
	int b=0;
	for(int i = 0;keywords[i]!=NULL;i++){
		equiv = 1;
		if(full[pos] == keywords[i][0]){
			for(b = 0;b<strlen(keywords[i]);b++){
				if (full[pos+b]!=keywords[i][b]){
					equiv=0;break;}
			}
			if (equiv==1){return i;}
		}
	}
	return -1;
}

void highlight_syntax(char* inp){
	for(int i=0;i<strlen(inp);i++){
		int test = check_syntax(inp,i);
		
		if (test>0){
			if(colors[test][1] == 'h'){
				if(hi==1){
					hi=0;
					printf("%c",inp[i]);
					printf("\x1b[0m");
					continue;
				}
				else{
					hi=1;
					printf("%s",get_color(colors[test][0]));
				}
			}
			
			if(hi==0){
			printf("%s",colors[test]);
			printf("%s",keywords[test]);
			printf("\x1b[0m");
			}
			else{printf("%s",keywords[test]);}

			i+=strlen(keywords[test]);
		}
		printf("%c",inp[i]);
	}
	printf("\x1b[0m");
	printf("\n");
	hi=0;
}
