%{
	#include "lex.yy.c"
	#include "semantic.c"
%}

%token IDENTIFIER STRING_LITERAL SIZEOF
%token NEW TRUE FALSE INTEGER_LITERAL CHAR_LITERAL DECIMAL_LITERAL
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME
%token TYPEDEF EXTERN STATIC AUTO
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

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
	: IDENTIFIER											{ s_identifier(); }
	| '(' exp ')'
	| primary_exp '[' exp ']'
	| primary_exp '(' ')'
	| primary_exp '(' exp_list ')'
	| primary_exp '.' IDENTIFIER
	| primary_exp PTR_OP IDENTIFIER
	;

unary_exp
	: primary_exp
	| INTEGER_LITERAL
	| CHAR_LITERAL
	| DECIMAL_LITERAL
	| STRING_LITERAL
	| INC_OP IDENTIFIER
	| DEC_OP IDENTIFIER
	| IDENTIFIER INC_OP
	| IDENTIFIER DEC_OP
	;

binary_exp
	: unary_exp
	| binary_exp binary_op unary_exp
	;

complex_exp
	: binary_exp
	| unary_op binary_exp
	| SIZEOF binary_exp
	| SIZEOF '(' type_name ')'
	| '(' type_name ')' binary_exp
	| binary_exp '?' exp ':' exp
	;

assignment_exp
	: primary_exp assignment_op complex_exp
	| primary_exp assignment_op assignment_exp
	;

exp
	: assignment_exp
	| complex_exp
	;

exp_list
	: exp
	| exp_list ',' exp

declaration
	: declaration_specifiers init_declarator_list ';'
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
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator
	| declarator '=' initializer
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
	| SIGNED
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
	: IDENTIFIER
	| '(' declarator ')'
	| direct_declarator '[' complex_exp ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
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
	| parameter_list ',' ELLIPSIS
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
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
	| exp ';'
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
	: '{' '}'
	| '{' stm_list '}'
	| '{' declaration_list '}'
	| '{' declaration_list stm_list '}'
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

stm_list
	: stm
	| stm_list stm
	;

for_exp
	: ';'
	| exp_list ';'
	;

selection_stm
	: IF '(' exp ')' stm
	| IF '(' exp ')' stm ELSE stm
	| switch_stm
	;

iteration_stm
	: WHILE '(' exp ')' stm
	| DO stm WHILE '(' exp ')' ';'
	| FOR '(' for_exp for_exp ')' stm
	| FOR '(' for_exp for_exp exp_list ')' stm
	;

jump_stm
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN exp ';'
	;

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_def
	| declaration
	;

function_def
	: declaration_specifiers declarator declaration_list compound_stm
	| declaration_specifiers declarator compound_stm
	| declarator declaration_list compound_stm
	| declarator compound_stm
	;

%%

int main(int argc, char* argv[]) {
	if (!argv[1]) {
		printf("Enter the file path in the first parameter\n");
		exit(1);
	}
	yyin = fopen(argv[1], "r");
	if (!yyin) {
		printf("Could not use file \"%s\"\n", argv[1]);
		exit(1);
	}
	return yyparse();
}
