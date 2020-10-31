%{
	#include "syntactic.tab.h"
	#include "symbol-table.c"

	int tokenUp(const int);
%}

%option noinput
%option nounput
%option noyywrap
%option never-interactive

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS		(f|F|l|L)
IS		((u|U)?(l|ll|L|LL)|(l|ll|L|LL)?(u|U))

%%

"/*"([^*]|\*+[^/])*"*/"|"//".*	{ tokenUp(0); }

"auto"											{ return tokenUp(AUTO); }
"break"											{ return tokenUp(BREAK); }
"case"											{ return tokenUp(CASE); }
"char"											{ return tokenUp(CHAR); }
"const"											{ return tokenUp(CONST); }
"continue"									{ return tokenUp(CONTINUE); }
"default"										{ return tokenUp(DEFAULT); }
"do"												{ return tokenUp(DO); }
"double"										{ return tokenUp(DOUBLE); }
"else"											{ return tokenUp(ELSE); }
"enum"											{ return tokenUp(ENUM); }
"extern"										{ return tokenUp(EXTERN); }
"false"											{ return tokenUp(FALSE); }
"float"											{ return tokenUp(FLOAT); }
"for"												{ return tokenUp(FOR); }
"goto"											{ return tokenUp(GOTO); }
"if"												{ return tokenUp(IF); }
"int"												{ return tokenUp(INT); }
"long"											{ return tokenUp(LONG); }
"new"												{ return tokenUp(NEW); }
"return"										{ return tokenUp(RETURN); }
"short"											{ return tokenUp(SHORT); }
"sizeof"										{ return tokenUp(SIZEOF); }
"static"										{ return tokenUp(STATIC); }
"struct"										{ return tokenUp(STRUCT); }
"switch"										{ return tokenUp(SWITCH); }
"true"											{ return tokenUp(TRUE); }
"typedef"										{ return tokenUp(TYPEDEF); }
"union"											{ return tokenUp(UNION); }
"unsigned"									{ return tokenUp(UNSIGNED); }
"void"											{ return tokenUp(VOID); }
"volatile"									{ return tokenUp(VOLATILE); }
"while"											{ return tokenUp(WHILE); }

{L}({L}|{D})*								{ return tokenUp(IDENTIFIER); }

0[xX]{H}+{IS}?							{ return tokenUp(INTEGER_LITERAL); }
0[0-7]+{IS}?								{ return tokenUp(INTEGER_LITERAL); }
{D}+{IS}?										{ return tokenUp(INTEGER_LITERAL); }

L?'(\\.|[^\\'])+'						{ return tokenUp(CHAR_LITERAL); }

{D}+{E}{FS}?								{ return tokenUp(DECIMAL_LITERAL); }
({D}+".")?{D}+({E})?{FS}?		{ return tokenUp(DECIMAL_LITERAL); }

L?\"(\\.|[^\\"])*\"					{ return tokenUp(STRING_LITERAL); }

"..."												{ return tokenUp(ELLIPSIS); }
">>="												{ return tokenUp(RIGHT_ASSIGN); }
"<<="												{ return tokenUp(LEFT_ASSIGN); }
"+="												{ return tokenUp(ADD_ASSIGN); }
"-="												{ return tokenUp(SUB_ASSIGN); }
"*="												{ return tokenUp(MUL_ASSIGN); }
"/="												{ return tokenUp(DIV_ASSIGN); }
"%="												{ return tokenUp(MOD_ASSIGN); }
"&="												{ return tokenUp(AND_ASSIGN); }
"^="												{ return tokenUp(XOR_ASSIGN); }
"|="												{ return tokenUp(OR_ASSIGN); }
">>"												{ return tokenUp(RIGHT_OP); }
"<<"												{ return tokenUp(LEFT_OP); }
"++"												{ return tokenUp(INC_OP); }
"--"												{ return tokenUp(DEC_OP); }
"->"												{ return tokenUp(PTR_OP); }
"&&"												{ return tokenUp(AND_OP); }
"||"												{ return tokenUp(OR_OP); }
"<="												{ return tokenUp(LE_OP); }
">="												{ return tokenUp(GE_OP); }
"=="												{ return tokenUp(EQ_OP); }
"!="												{ return tokenUp(NE_OP); }
";"													{ return tokenUp(';'); }
("{"|"<%")									{ return tokenUp('{'); }
("}"|"%>")									{ return tokenUp('}'); }
","													{ return tokenUp(','); }
":"													{ return tokenUp(':'); }
"="													{ return tokenUp('='); }
"("													{ return tokenUp('('); }
")"													{ return tokenUp(')'); }
("["|"<:")									{ return tokenUp('['); }
("]"|":>")									{ return tokenUp(']'); }
"."													{ return tokenUp('.'); }
"&"													{ return tokenUp('&'); }
"!"													{ return tokenUp('!'); }
"~"													{ return tokenUp('~'); }
"-"													{ return tokenUp('-'); }
"+"													{ return tokenUp('+'); }
"*"													{ return tokenUp('*'); }
"/"													{ return tokenUp('/'); }
"%"													{ return tokenUp('%'); }
"<"													{ return tokenUp('<'); }
">"													{ return tokenUp('>'); }
"^"													{ return tokenUp('^'); }
"|"													{ return tokenUp('|'); }
"?"													{ return tokenUp('?'); }

[ \t\v\r\n\f]								{ tokenUp(0); }
.														{ return tokenUp(*yytext); }

%%

int column = 0;
int row = 1;

Exp* getType(const int token) {
	Exp* exp = NULL;
	switch (token) {
		case CHAR_LITERAL:
			exp = getAll("char");
			break;
		case INTEGER_LITERAL:
			exp = getAll("int");
			break;
		case DECIMAL_LITERAL:
			exp = getAll("float");
			break;
		case STRING_LITERAL:
			exp = getAll("string");
	}
	return exp;
}

int tokenUp(const int token) {
	int i;
	for (i = 0; yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n'){
			column = 0;
			row++;
		} else
			column++;
	}
	yylval.exp = NULL;
	if(token == IDENTIFIER)
		yylval.exp = getAll(yytext);
	if(token > 0 && !yylval.exp)
		yylval.exp = newExp3(strdup(yytext), token, getType(token));
	return token;
}

void show(char* s) {
	fprintf(stderr, "%s, row: %d, column: %d\n", s, row, column);
}

int yyerror(char* s) {
	show(s);
	abort();
	return 1;
}
