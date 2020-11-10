%{
	#include "comum.c"
	#include "lex.yy.c"
	#include "semantic.c"

	char* code;
%}

%union {
	Exp* exp;
};

%token <exp> IDENTIFIER STRING_LITERAL SIZEOF
%token <exp> NEW TRUE FALSE INTEGER_LITERAL CHAR_LITERAL DECIMAL_LITERAL
%token <exp> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token <exp> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <exp> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <exp> XOR_ASSIGN OR_ASSIGN TYPE_NAME
%token <exp> TYPEDEF EXTERN STATIC AUTO
%token <exp> CHAR SHORT INT LONG UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token <exp> STRUCT UNION ENUM ELLIPSIS
%token <exp> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%type <exp> ';' '{' '}' ',' ':' '=' '(' ')' '[' ']' '.' '&' '!' '~' '-' '+' '*' '/' '%' '<' '>' '^' '|' '?'
%type <exp> unary_op binary_op assignment_op primary_exp unary_exp binary_exp complex_exp assignment_exp
%type <exp> exp exp_list declaration declaration_specifiers init_declarator_list init_declarator
%type <exp> storage_class_specifier type_specifier struct_or_union_specifier struct_or_union
%type <exp> struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator_list
%type <exp> struct_declarator enum_specifier enumerator_list enumerator type_qualifier declarator
%type <exp> direct_declarator pointer type_qualifier_list parameter_type_list parameter_list
%type <exp> parameter_declaration identifier_list type_name abstract_declarator direct_abstract_declarator
%type <exp> initializer initializer_list stm labeled_stm case_stm default_stm case_stm_list switch_stm
%type <exp> compound_stm declaration_list stm_list for_exp selection_stm iteration_stm jump_stm
%type <exp> translation_unit external_declaration function_def

%start translation_unit

%left '<' '>' LE_OP GE_OP EQ_OP NE_OP '&' '^' '|' AND_OP OR_OP
%left '+' '-'
%left '*' '/' '%'
%right '=' MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN
%right LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%left ','

%%

unary_op
	: '&'
	| '*'
	| '~'
	| '!'
	;

binary_op
	: '*'
	| '/'
	| '%'
	| '+'
	| '-'
	| LEFT_OP
	| RIGHT_OP
	| '<'
	| '>'
	| LE_OP
	| GE_OP
	| EQ_OP
	| NE_OP
	| '&'
	| '^'
	| '|'
	| AND_OP
	| OR_OP
	;

assignment_op
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

primary_exp
	: IDENTIFIER
	| '(' exp ')'																							{ primary_exp2(&$$, $1, $2, $3) }
	| primary_exp '[' exp ']' 																{ primary_exp3(&$$, $1, $2, $3, $4) }
	| primary_exp '(' ')'																			{ primary_exp4(&$$, $1, $2, $3) }
	| primary_exp '(' exp_list ')'														{ primary_exp5(&$$, $1, $2, $3, $4) }
	| primary_exp '.' IDENTIFIER
	| primary_exp PTR_OP IDENTIFIER
	;

unary_exp
	: primary_exp
	| INTEGER_LITERAL
	| CHAR_LITERAL
	| DECIMAL_LITERAL
	| STRING_LITERAL
	| INC_OP IDENTIFIER																				{ unary_exp67(&$$, $1, $2) }
	| DEC_OP IDENTIFIER																				{ unary_exp67(&$$, $1, $2) }
	| IDENTIFIER INC_OP																				{ unary_exp89(&$$, $1, $2) }
	| IDENTIFIER DEC_OP																				{ unary_exp89(&$$, $1, $2) }
	| '+' INTEGER_LITERAL																			{ unary_exp10(&$$, $1, $2) }
	| '-' INTEGER_LITERAL																			{ unary_exp10(&$$, $1, $2) }
	| '+' DECIMAL_LITERAL																			{ unary_exp10(&$$, $1, $2) }
	| '-' DECIMAL_LITERAL																			{ unary_exp10(&$$, $1, $2) }
	;

binary_exp
	: unary_exp
	| binary_exp binary_op unary_exp													{ binary_exp2(&$$, $1, $2, $3) }
	;

complex_exp
	: binary_exp
	| unary_op binary_exp																			{ complex_exp2(&$$, $1, $2) }
	| SIZEOF binary_exp
	| SIZEOF '(' type_name ')'
	| '(' type_name ')' binary_exp
	| binary_exp '?' exp ':' exp															{ complex_exp6(&$$, $1, $2, $3, $4, $5) }
	;

assignment_exp
	: primary_exp assignment_op complex_exp										{ assignment_exp(&$$, $1, $2, $3) }
	| primary_exp assignment_op assignment_exp
	;

exp
	: assignment_exp
	| complex_exp
	;

exp_list
	: exp
	| exp_list ',' exp																				{ exp_list($1, $3) }
	;

declaration
	: declaration_specifiers init_declarator_list ';'					{ declaration(&$$, $1, $2, $3) }
	;

declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier
	| type_specifier declaration_specifiers
	| type_qualifier
	| type_qualifier declaration_specifiers
	;

init_declarator_list
	: init_declarator 																				{ init_declarator_list1(&$$, $1) }
	| init_declarator_list ',' init_declarator 								{ init_declarator_list2($1, $2, $3) }
	;

init_declarator
	: declarator
	| declarator '=' initializer 															{ init_declarator(&$$, $1, $2, $3) }
	;

storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	;

type_specifier
	: VOID
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| UNSIGNED
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' complex_exp
	| declarator ':' complex_exp
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: IDENTIFIER
	| IDENTIFIER '=' complex_exp
	;

type_qualifier
	: CONST
	| VOLATILE
	;

declarator
	: pointer direct_declarator
	| direct_declarator
	;

direct_declarator
	: IDENTIFIER																											{ direct_declarator1(&$$, $1) }
	| '(' declarator ')'
	| direct_declarator '[' complex_exp ']'														{ direct_declarator3(&$$, $1, $2, $3, $4) }
	| direct_declarator '[' ']'																				{ direct_declarator4(&$$, $1, $2, $3) }
	| direct_declarator '(' parameter_type_list ')'										{ direct_declarator5($1, $2, $3, $4) }
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')' 																			{ direct_declarator7($1, $2, $3) }
	;

pointer
	: '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;

parameter_type_list
	: parameter_list
	| parameter_list ',' ELLIPSIS																			{ parameter_list2($1, $2, $3) }
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration												{ parameter_list2($1, $2, $3) }
	;

parameter_declaration
	: declaration_specifiers declarator 															{ paramater_declaration(&$$, $1, $2) }
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' complex_exp ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' complex_exp ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: exp
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

stm
	: labeled_stm
	| compound_stm
	| exp ';'																													{ concatExp(&$$, $1, $2) }
	| selection_stm
	| iteration_stm
	| jump_stm
	;

labeled_stm
	: IDENTIFIER ':' stm
	;

case_stm
	: CASE complex_exp ':' stm
	| CASE complex_exp ':' case_stm
	;

default_stm
	: DEFAULT ':' stm
	;

case_stm_list
	: case_stm
	| case_stm_list case_stm
	;

switch_stm
	: SWITCH '(' exp ')' '{' case_stm_list '}'
	| SWITCH '(' exp ')' '{' case_stm_list default_stm '}'
	;

compound_stm
	: '{' '}'																										{ concatExp(&$$, $1, $2) }
	| '{' stm_list '}'																					{ concatExp(&$$, $1, $2); concatExp(&$$, $$, $3) }
	| '{' declaration_list '}'																	{ concatExp(&$$, $1, $2); concatExp(&$$, $$, $3) }
	| '{' declaration_list stm_list '}'													{ concatExp(&$$, $1, $2); concatExp(&$$, $$, $3); concatExp(&$$, $$, $4) }
	;

declaration_list
	: declaration
	| declaration_list declaration															{ concatExp(&$$, $1, $2) }
	;

stm_list
	: stm
	| stm_list stm																							{ concatExp(&$$, $1, $2) }
	;

for_exp
	: ';'
	| exp_list ';'																							{ concatExp(&$$, $1, $2) }
	;

selection_stm
	: IF '(' exp ')' stm																				{ selection_stm1(&$$, $3, $5) }
	| IF '(' exp ')' stm ELSE stm																{ selection_stm2(&$$, $3, $5, $7) }
	| switch_stm
	;

iteration_stm
	: WHILE '(' exp ')' stm													{ iteration_stm1(&$$, $3, $5) }
	| DO stm WHILE '(' exp ')' ';'
	| FOR '(' for_exp for_exp ')' stm								{ iteration_stm34(&$$, $3, $4, NULL, $6) }
	| FOR '(' for_exp for_exp exp_list ')' stm			{ iteration_stm34(&$$, $3, $4, $5, $7) }
	;

jump_stm
	: GOTO IDENTIFIER ';'														{ concatExp(&$$, $1, $2) }
	| CONTINUE ';'																	{ concatExp(&$$, $1, $2) }
	| BREAK ';'																			{ concatExp(&$$, $1, $2) }
	| RETURN ';'																		{ concatExp(&$$, $1, $2) }
	| RETURN exp ';'																{ concatExp(&$$, $1, newExp(" ", 0)); concatExp(&$$, $$, $2); concatExp(&$$, $$, $3) }
	;

external_declaration
	: function_def
	| declaration
	;

function_def
	: declaration_specifiers declarator { function_def_declaration($1, $2) } compound_stm { function_def(&$$, $1, $2, $4) }
	| declarator compound_stm
	;

	translation_unit
	: external_declaration													{ code = $1->value }
	| translation_unit external_declaration					{ concatExp(&$$, $1, $2); code = $$->value }
	;

%%

int main(int argc, char* argv[]) {
	int result;
	if (!argv[1]) {
		printf("Enter the path of the cn file in the first parameter\n");
		return 1;
	}
	yyin = fopen(argv[1], "r");
	if (!yyin) {
		printf("Could not use the file \"%s\"\n", argv[1]);
		return 1;
	}
	initSymbolTable();
	result = yyparse();
	fclose(yyin);
	if (!result) {
		if (argv[2])
			yyout = fopen(argv[2], "w");
		else
			yyout = stdout;
		fprintf(yyout, "#include <stdio.h>\n#include <stdlib.h>\n%s\n", code);
		if (yyout != stdout)
			printf("OK");
		fclose(yyout);
	}
	return result;
}
