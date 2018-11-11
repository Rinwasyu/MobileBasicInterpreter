#define LEXER_TOKEN_VALUE_LENGTH 100

enum LexerMode {
	skip,
	token,
	text,
	comment
};

struct LexerToken {
	struct LexerToken *next;
	char *value;
};

struct Lexer {
	enum LexerMode mode;
	struct LexerToken *f_token;
	struct LexerToken *c_token;
	void (*init)(struct Lexer *);
	void (*push)(struct Lexer *, char);
	void (*newtoken)(struct Lexer *);
};

void lexer_init(struct Lexer *lexer) {
}

void lexer_push(struct Lexer *lexer, char c) {
	if (lexer->mode == skip) {
		if (c == ' ' || c == '	') {
			return;
		} else if (c == '\'') {
			lexer->mode = comment;
			return;
		} else if (c == '\n' || c == ';') {
			lexer->newtoken(lexer);
			strcpy(lexer->c_token->value, ";");
			lexer->mode = skip;
			return;
		} else {
			lexer->newtoken(lexer);
			lexer->mode = token;
		}
	}
	
	if (lexer->mode == token) {
		if (c == '"') {
			lexer->mode = text;
		} else if (c == '(' || c == ')' || c == ' ' || c == '	') {
			lexer->newtoken(lexer);
			lexer->mode = skip;
		} else if (c == '\n' || c == ';') {
			lexer->newtoken(lexer);
			strcpy(lexer->c_token->value, ";");
			lexer->mode = skip;
			return;
		}
	} else if (lexer->mode == text) {
		if (c == '"') {
			lexer->mode = token;
		}
	} else if (lexer->mode == comment) {
		if (c == '\n' || c == ';') {
			lexer->mode = skip;
		}
		return;
	}
	
	lexer->c_token->value[strlen(lexer->c_token->value)] = c;
}

void lexer_newtoken(struct Lexer *lexer) {
	struct LexerToken *token = (struct LexerToken *)malloc(sizeof(struct LexerToken));
	token->next = NULL;
	token->value = (char *)malloc(sizeof(char) * LEXER_TOKEN_VALUE_LENGTH);
	if (lexer->f_token == NULL) {
		lexer->f_token = token;
	} else {
		lexer->c_token->next = token;
	}
	lexer->c_token = token;
}

struct Lexer lexer = {
	skip,
	NULL,
	NULL,
	lexer_init,
	lexer_push,
	lexer_newtoken
};