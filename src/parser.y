// Adapted from: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html

// TODO: you can either continue adding to this grammar file or
// rename parser_full.y to parser.y once you're happy with
// how this example works.

%code requires{
    #include "ast.hpp"

    extern Node *g_root;
    extern FILE *yyin;
    int yylex(void);
    void yyerror(const char *);
}

// Represents the value associated with any kind of AST node.
%union{
	Node         *node;
	NodeList     *nodes;
	int          number_int;
	double       number_float;
	std::string* string;
	yytokentype  token;

	// New
	CompoundStatement* compound_statement;
}

%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token TYPE_NAME TYPEDEF EXTERN STATIC AUTO REGISTER SIZEOF
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN COMMENT


/* Declare the data type for each grammar rule */
%type <node> translation_unit external_declaration function_definition primary_expression postfix_expression
%type <node> unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <node> conditional_expression assignment_expression constant_expression declaration declaration_specifiers
%type <node> init_declarator type_specifier struct_specifier struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator_list
%type <node> struct_declarator enum_specifier enumerator_list enumerator declarator direct_declarator pointer parameter_list parameter_declaration
%type <node> identifier_list type_name abstract_declarator direct_abstract_declarator statement labeled_statement // initializer initializer_list
%type <node> expression_statement selection_statement iteration_statement jump_statement

/* Moved declaration_list, argument_expression_list, compound_statement to nodes */
%type <nodes> statement_list declaration_list argument_expression_list init_declarator_list expression
%type <nodes> initializer,initializer_list

/* New */
%type <compound_statement> compound_statement

%type <string> unary_operator assignment_operator storage_class_specifier

%type <number_int> INT_CONSTANT STRING_LITERAL
%type <number_float> FLOAT_CONSTANT
%type <string> IDENTIFIER


%start ROOT
%%

ROOT
    : translation_unit { g_root = $1; }

/* Contains a series of external declarations (e.g. global functions & variables) */
translation_unit
	: external_declaration {
		// Modify to hold a list of external declarations?
		$$ = $1;
	}
	| translation_unit external_declaration { }
	;

/* Top level external declarations can be function definitions OR global vars */
external_declaration
	: function_definition { $$ = $1; }
	| declaration
	;

function_definition
	: declaration_specifiers declarator compound_statement {
		std::cout<<"Printing function contents:"<<std::endl;
		$3->Print(std::cout);
		$$ = new FunctionDefinition($1, $2, $3);
	}
	;


/* In C90, all declarations have to come before statements */
compound_statement
	: '{' '}' { $$ = new CompoundStatement(nullptr, nullptr); }
	| '{' statement_list '}' { $$ = new CompoundStatement(nullptr, $2); }
	| '{' declaration_list '}' { $$ = new CompoundStatement($2, nullptr); }
	| '{' declaration_list statement_list '}'  { $$ = new CompoundStatement($2, $3);}
	;


/* DECLARATION PARSING (Should declaration_list be a NodeList?) */
declaration_list
	: declaration { $$ = new NodeList($1); }
	| declaration_list declaration {
		$1->PushBack($2);
		$$ = $1;
	}
	;

declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';' {
		$$ = new Declaration($1, $2);
	}

/* Parent of type_specifier and storage_class_specifier
(includes e.g. int, static, int statc, static int) */
declaration_specifiers
	: type_specifier
	/* | storage_class_specifier { $$ = new DeclarationSpecifiers(nullptr, $1); } */
	/* | storage_class_specifier declaration_specifiers { $2->AddStorageClassSpecifier($1); $$ = $2;} */
	/* | type_specifier declaration_specifiers { $2->AddTypeSpecifier($1); $$ = $2; } */
	;

type_specifier
	: CHAR 		{ 	$$ = new TypeSpecifier("char");		}
	| INT 		{	$$ = new TypeSpecifier("int");		}
	| SHORT 	{	$$ = new TypeSpecifier("short");	}
	| LONG 		{	$$ = new TypeSpecifier("long");		}
	| FLOAT 	{	$$ = new TypeSpecifier("float");	}
	| DOUBLE	{	$$ = new TypeSpecifier("double");	}
	| VOID 		{	$$ = new TypeSpecifier("void");		}
	;

/* Parent of init_declarator & initialiser (e.g. x = 5, y = 10) */
init_declarator_list
	: init_declarator { $$ = new NodeList($1); }
	| init_declarator_list ',' init_declarator {
		std::cout<<"Handling init declarator list..."<<std::endl;
		$1->PushBack($3);
		$$ = $1;
	}
	;

init_declarator
	: declarator { $$ = new InitDeclarator($1, nullptr); }
	| declarator '=' initializer { $$ = new InitDeclarator($1, $3);}
	;

/* parent of direct declarator (to include pointer syntax) */
declarator
	: direct_declarator {
		$$ = $1;
	}
	;

direct_declarator
	: IDENTIFIER {
		$$ = new Identifier(*$1);
		delete $1;
	}
	| '(' declarator ')' { $$ = $2; }
	| direct_declarator '(' ')' { $$ = new DirectDeclarator($1); }
	| direct_declarator '[' constant_expression ']' { $$ = new ArrayDeclarator($1, $3);}
	| direct_declarator '[' ']' { { $$ = new ArrayDeclarator($1, nullptr); } }
	;

/* Parent of initializer_list. Can be expression or initializer*/
/* NOTE: int x = {1} is allowed in ANSI C...? */
initializer
	: assignment_expression { $$ = new NodeList($1); }
	| '{' initializer_list '}' { $$ = $2; }
	| '{' initializer_list ',' '}'
	;

/* Allows for array initialisers: e.g. { 5, 10, 12 }, without the { }*/
initializer_list
	: initializer { $$ = $1; }
	| initializer_list ',' initializer { $1->PushBack($3); $$ = $1; }
	;

/* STATEMENT PARSING */
statement_list
	: statement { $$ = new NodeList($1); }
	| statement_list statement { $1->PushBack($2); $$=$1; }
	;

/* Handle if statements, switch, while, for */
statement
	: expression_statement
	| jump_statement { $$ = $1; }
	;

jump_statement
	: RETURN ';' {
		$$ = new ReturnStatement(nullptr);
	}
	| RETURN expression ';' {
		$$ = new ReturnStatement($2);
	}
	;

expression_statement
	: ';'
	| expression ';'
	;

/* EXPRESSION PARSING */

/* Parent of all expressions */
expression
	: assignment_expression { $$ = new NodeList($1); }
	| expression ',' assignment_expression {
		$1->PushBack($3);
		$$ = $1;
	}
	;

constant_expression
	: additive_expression { $$ = $1; } // TODO: Change to conditional expression later on
	;

assignment_expression
	: additive_expression /* : replace later with conditional_expression */
	| unary_expression assignment_operator assignment_expression {

		// Should unary_expression be replaced with primary_expression?
		$$ = new AssignmentExpression($1, *$2, $3);
		delete $2;
	}
	;

assignment_operator
	: '=' { $$ = new std::string("="); }
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

/* Binary Expressions (Parent of cast expression / unary expression) */
additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression { $$ = new BinaryExpression($1, "+", $3);}
	| additive_expression '-' multiplicative_expression { $$ = new BinaryExpression($1, "-", $3);}
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression { $$ = new BinaryExpression($1, "*", $3);}
	| multiplicative_expression '/' cast_expression { $$ = new BinaryExpression($1, "/", $3);}
	| multiplicative_expression '%' cast_expression { $$ = new BinaryExpression($1, "%", $3);}
	;

/* Unary Expressions */
cast_expression
	: unary_expression
	/* | '(' type_name ')' cast_expression */
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression { $$ = new UnaryExpression("++", $2); }
	| DEC_OP unary_expression { $$ = new UnaryExpression("--", $2);}
	| unary_operator cast_expression {
		$$ = new UnaryExpression(*$1, $2);
		delete $1;
	}
	/* | SIZEOF unary_expression { $$ = new UnaryExpression("sizeof", $2); } */
	/* | SIZEOF '(' type_name ')' */
	;

unary_operator
	: '&' { $$ = new std::string("&"); } // Address of
	| '*' { $$ = new std::string("*"); } // Dereference
	| '+' { $$ = new std::string("+"); } // Unary plus
	| '-' { $$ = new std::string("-"); } // Unary minus
	| '~' { $$ = new std::string("~"); } // Bitwise NOT
	| '!' { $$ = new std::string("!"); } // Logical NOT
	;

argument_expression_list
	: assignment_expression { $$ = new NodeList($1); }
	| argument_expression_list ',' assignment_expression {
		$1->PushBack($3);
		$$ = $1;
	}
	;

postfix_expression
	: primary_expression
	| postfix_expression INC_OP { $$ = new PostfixExpression($1, "++"); }
	| postfix_expression DEC_OP { $$ = new PostfixExpression($1, "--"); }
	| postfix_expression '(' argument_expression_list ')' { $$ = new FunctionCall($1, $3); }
	/* To add the rest for arrays, functions & structs */
	;

primary_expression
	: IDENTIFIER {
		$$ = new Identifier(*$1);
		delete $1;
	}
	| INT_CONSTANT { $$ = new IntConstant($1); }
	/* | FLOAT_CONSTANT { $$ = new FloatConstant($1); } */
	| '(' expression ')' { $$ = $2; }
	;


/* For function calls */

/* argument_expression_list
	: assignment_expression
	;

cast_expression
	: unary_expression
	;

multiplicative_expression
	: cast_expression
	;

additive_expression
	: multiplicative_expression
	;

shift_expression
	: additive_expression
	;

relational_expression
	: shift_expression
	;

equality_expression
	: relational_expression
	;

and_expression
	: equality_expression
	;

exclusive_or_expression
	: and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	;

conditional_expression
	: logical_or_expression
	;

 */



%%

Node *g_root;

Node *ParseAST(std::string file_name)
{
  yyin = fopen(file_name.c_str(), "r");
  if(yyin == NULL){
    std::cerr << "Couldn't open input file: " << file_name << std::endl;
    exit(1);
  }
  g_root = nullptr;
  yyparse();
  return g_root;
}






