#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syntax.h"

char **keywords = NULL;
char **colors = NULL;

//For reading the syntax file!!
char* get_color(char a){
	switch(a){
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
		
		
		//block highlighting to do
		// if(inp[i]=='\"'){
		// 	printf("%s",YELLOW);
		// 	do {
		// 		printf("%c",inp[i++]);
		// 	}while(inp[i]!='\"' && inp[i]!='\0');
		// 	printf("%c",inp[i++]);
		// 	printf("\x1b[0m");
		// }
		
		if (test>0){
			printf("%s",colors[test]);
			printf("%s",keywords[test]);
			printf("\x1b[0m");
			i+=strlen(keywords[test]);
		}
		printf("%c",inp[i]);
	}
	printf("\n");
}
