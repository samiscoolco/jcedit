#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "syntax.h"

char *keywords[] = {"if","else","while","for","break","continue","signed","unsigned","int","char","short","long","float","double"};
char *colors[] = {"\033[92m","\033[92m","\033[92m","\033[92m","\033[91m","\033[94m","\033[96m","\033[96m","\033[96m","\033[96m","\033[85m","\033[96m","\033[96m","\033[96m"};

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


// if (inp[i] == 'i'){
// 	if (inp[i+1] == 'f'){
// 		printf("\033[91m");
// 		printf("if");
// 		printf("\x1b[0m");
// 		i+=1;continue;
// 	}