%{
	#include "syntactic.tab.h"

	void count();
%}

%option noinput
%option nounput
%option noyywrap
%option never-interactive

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			((u|U)?(l|ll|L|LL)|(l|ll|L|LL)?(u|U))

%%

"/*"([^*]|\*+[^/])*"*/"|"//".*	{ count(); }

"auto"							{ count(); return AUTO; }
"break"							{ count(); return BREAK; }
"case"							{ count(); return CASE; }
"char"							{ count(); return CHAR; }
"const"							{ count(); return CONST; }
"continue"						{ count(); return CONTINUE; }
"default"						{ count(); return DEFAULT; }
"do"							{ count(); return DO; }
"double"						{ count(); return DOUBLE; }
"else"							{ count(); return ELSE; }
"enum"							{ count(); return ENUM; }
"extern"						{ count(); return EXTERN; }
"false"							{ count(); return FALSE; }
"float"							{ count(); return FLOAT; }
"for"							{ count(); return FOR; }
"goto"							{ count(); return GOTO; }
"if"							{ count(); return IF; }
"int"							{ count(); return INT; }
"long"							{ count(); return LONG; }
"new"							{ count(); return NEW; }
"return"						{ count(); return RETURN; }
"short"							{ count(); return SHORT; }
"signed"						{ count(); return SIGNED; }
"sizeof"						{ count(); return SIZEOF; }
"static"						{ count(); return STATIC; }
"struct"						{ count(); return STRUCT; }
"switch"						{ count(); return SWITCH; }
"true"							{ count(); return TRUE; }
"typedef"						{ count(); return TYPEDEF; }
"union"							{ count(); return UNION; }
"unsigned"						{ count(); return UNSIGNED; }
"void"							{ count(); return VOID; }
"volatile"						{ count(); return VOLATILE; }
"while"							{ count(); return WHILE; }

{L}({L}|{D})*					{ count(); return IDENTIFIER; }

0[xX]{H}+{IS}?					{ count(); return INTEGER_LITERAL; }
0[0-7]+{IS}?					{ count(); return INTEGER_LITERAL; }
{D}+{IS}?						{ count(); return INTEGER_LITERAL; }

L?'(\\.|[^\\'])+'				{ count(); return CHAR_LITERAL; }

{D}+{E}{FS}?					{ count(); return DECIMAL_LITERAL; }
({D}+".")?{D}+({E})?{FS}?		{ count(); return DECIMAL_LITERAL; }

L?\"(\\.|[^\\"])*\"				{ count(); return STRING_LITERAL; }

"..."							{ count(); return ELLIPSIS; }
">>="							{ count(); return RIGHT_ASSIGN; }
"<<="							{ count(); return LEFT_ASSIGN; }
"+="							{ count(); return ADD_ASSIGN; }
"-="							{ count(); return SUB_ASSIGN; }
"*="							{ count(); return MUL_ASSIGN; }
"/="							{ count(); return DIV_ASSIGN; }
"%="							{ count(); return MOD_ASSIGN; }
"&="							{ count(); return AND_ASSIGN; }
"^="							{ count(); return XOR_ASSIGN; }
"|="							{ count(); return OR_ASSIGN; }
">>"							{ count(); return RIGHT_OP; }
"<<"							{ count(); return LEFT_OP; }
"++"							{ count(); return INC_OP; }
"--"							{ count(); return DEC_OP; }
"->"							{ count(); return PTR_OP; }
"&&"							{ count(); return AND_OP; }
"||"							{ count(); return OR_OP; }
"<="							{ count(); return LE_OP; }
">="							{ count(); return GE_OP; }
"=="							{ count(); return EQ_OP; }
"!="							{ count(); return NE_OP; }
";"								{ count(); return ';'; }
("{"|"<%")						{ count(); return '{'; }
("}"|"%>")						{ count(); return '}'; }
","								{ count(); return ','; }
":"								{ count(); return ':'; }
"="								{ count(); return '='; }
"("								{ count(); return '('; }
")"								{ count(); return ')'; }
("["|"<:")						{ count(); return '['; }
("]"|":>")						{ count(); return ']'; }
"."								{ count(); return '.'; }
"&"								{ count(); return '&'; }
"!"								{ count(); return '!'; }
"~"								{ count(); return '~'; }
"-"								{ count(); return '-'; }
"+"								{ count(); return '+'; }
"*"								{ count(); return '*'; }
"/"								{ count(); return '/'; }
"%"								{ count(); return '%'; }
"<"								{ count(); return '<'; }
">"								{ count(); return '>'; }
"^"								{ count(); return '^'; }
"|"								{ count(); return '|'; }
"?"								{ count(); return '?'; }

[ \t\v\n\f]						{ count(); }
.								{ count(); return yylval; }

%%

int column = 0;
int row = 1;

void count() {
	int i;
	for (i = 0; yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n'){
			column = 0;
			row++;
		} else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;
	}
	if (strlen(yytext) < 2 || yytext[0] != '/' || !(yytext[1] == '*' || yytext[1] == '/'))
		ECHO;
}

int yyerror(char* s) {
	fprintf(stderr, "\n%s, linha: %d, coluna: %d\n", s, row, column);
	return 0;
}
