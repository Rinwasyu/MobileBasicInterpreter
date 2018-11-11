#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer/lexer.c"

int main(int argc, char **argv) {
	FILE *fp;
	
	if (argc < 2) {
		return 1;
	}
	
	if ((fp = fopen(argv[1], "r")) == NULL) {
		return 1;
	}
	
	lexer.init(&lexer);
	
	printf("-----source-----\n");
	for (char c; (c = fgetc(fp)) != EOF;) {
		putchar(c);
		lexer.push(&lexer, c);
	}
	
	printf("\n\n-----lexer------\n");
	for (lexer.c_token = lexer.f_token; lexer.c_token != NULL; ) {
		printf("[%s] ", lexer.c_token->value);
		lexer.c_token = lexer.c_token->next;
	}
	
	printf("\n\n-----parser------\n");
	
	printf("\n");
	
	return 0;
}