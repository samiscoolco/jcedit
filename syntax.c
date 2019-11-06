#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syntax.h"

#define RED "\033[91m"
#define GREEN "\033[92m"
#define BLUE "\033[96m"
#define DARKBLUE "\033[94m"
#define YELLOW "\033[93m"

//this is for C
char *keywords[] = {"if","else","while","for","enum","switch","break","return","continue","case","#define","#include","signed","unsigned","int","char","short","long","float","double","void","struct"};
char *colors[] = {GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,RED,RED,RED,RED,DARKBLUE,DARKBLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE};

int check_syntax(char* full,int pos){
	int equiv = 1;
	for(int i = 0;i<sizeof(keywords)/sizeof(keywords[0]);i++){
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
