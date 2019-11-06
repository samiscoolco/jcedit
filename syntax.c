#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syntax.h"
//this is for C
//char *keywords[] = {"if","else","while","for","enum","switch","break","return","continue","case","#define","#include","signed","unsigned","int","char","short","long","float","double","void","struct"};
//char *colors[] = {GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,RED,RED,RED,RED,DARKBLUE,DARKBLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE};
//


char **keywords = NULL;
char **colors = NULL;

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
	for(int i = 0;keywords[i]!=NULL;i++){
		equiv = 1;
		if(full[pos] == keywords[i][0]){
			for(int b = 0;b<strlen(keywords[i]);b++){
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
		if (test!=-1){
			printf("%s",colors[test]);
			printf("%s",keywords[test]);
			printf("\x1b[0m");
			i+=strlen(keywords[test]);
		}
		printf("%c",inp[i]);
	}
	printf("\n");

}
