%option noyywrap

%{
  // A lot of this lexer is based off the ANSI C grammar:
  // https://www.lysator.liu.se/c/ANSI-C-grammar-l.html#MUL-ASSIGN
  // Avoid error "error: `fileno' was not declared in this scope"
  extern "C" int fileno(FILE *stream);
  #include "parser.tab.hpp"
  std::unordered_map<std::string, bool> typedef_ids  = {};
%}

D	  [0-9]
L	  [a-zA-Z_]
H   [a-fA-F0-9]
E	  [Ee][+-]?{D}+
FS  (f|F|l|L)
IS  (u|U|l|L)*

%%



"/*"([^*]|(\*+[^*/]))*"*/"   {
  // printf("Block comment found:\n%s\n", yytext);
}
"//".*\n   {
  // printf("%s\n", yytext);
  // return(COMMENT);
}

"auto"			{return(AUTO);}
"break"			{return(BREAK);}
"case"			{return(CASE);}
"char"			{return(CHAR);}
"const"			{return(CONST);}
"continue"  {return(CONTINUE);}
"default"		{return(DEFAULT);}
"do"			  {return(DO);}
"double"		{return(DOUBLE);}
"else"			{return(ELSE);}
"enum"			{return(ENUM);}
"extern"		{return(EXTERN);}
"float"			{return(FLOAT);}
"for"			  {return(FOR);}
"goto"			{return(GOTO);}
"if"			  {return(IF);}
"int"			  {return(INT);}
"long"			{return(LONG);}
"register"	{return(REGISTER);}
"return"		{return(RETURN);}
"short"			{return(SHORT);}
"signed"		{return(SIGNED);}
"sizeof"		{return(SIZEOF);}
"static"		{return(STATIC);}
"struct"		{return(STRUCT);}
"switch"		{return(SWITCH);}
"typedef"		{return(TYPEDEF);}
"union"			{return(UNION);}
"unsigned"	{return(UNSIGNED);}
"void"			{return(VOID);}
"volatile"	{return(VOLATILE);}
"while"			{return(WHILE);}

{L}({L}|{D})*		{
  yylval.string = new std::string(yytext);
  if(typedef_ids.find(*yylval.string) != typedef_ids.end()) return(TYPE_NAME);
  return(IDENTIFIER);
}

0[xX]{H}+{IS}?		{yylval.number_int = (int)strtol(yytext, NULL, 0); return(INT_CONSTANT);}
0{D}+{IS}?		    {yylval.number_int = (int)strtol(yytext, NULL, 0); return(INT_CONSTANT);}
{D}+{IS}?		      {yylval.number_int = (int)strtol(yytext, NULL, 0); return(INT_CONSTANT);}
L?'(\\.|[^\\'])+'	{
  // handles escape sequences
  if(yytext[1] == '\\' && yytext[2] != '\''){
    switch(yytext[2]){
      case 'a': yylval.number_int = 7; break;
      case 'b': yylval.number_int = 8; break;
      case 't': yylval.number_int = 9; break;
      case 'n': yylval.number_int = 10; break;
      case 'v': yylval.number_int = 11; break;
      case 'f': yylval.number_int = 12; break;
      case 'r': yylval.number_int = 13; break;
      case '\\': yylval.number_int = 92; break;
      case '\'': yylval.number_int = 39; break;
      case '"': yylval.number_int = 34; break;
      case '0': yylval.number_int = 0; break;
      default: yylval.number_int = (int)yytext[2]; break;
    }
  } else {
    yylval.number_int = (int)yytext[1];
  }
  return(INT_CONSTANT);
}

{D}+{E}{FS}?		        {yylval.number_float = strtod(yytext, NULL); return(FLOAT_CONSTANT);}
{D}*"."{D}+({E})?{FS}?	{yylval.number_float = strtod(yytext, NULL); return(FLOAT_CONSTANT);}
{D}+"."{D}*({E})?{FS}?	{yylval.number_float = strtod(yytext, NULL); return(FLOAT_CONSTANT);}


L?\"(\\.|[^\\"])*\"	{yylval.string = new std::string(yytext); return(STRING_LITERAL);}

"..."      {return(ELLIPSIS);}
">>="			 {yylval.string = new std::string(yytext); return(RIGHT_ASSIGN);}
"<<="      {yylval.string = new std::string(yytext); return(LEFT_ASSIGN);}
"+="			 {yylval.string = new std::string(yytext); return(ADD_ASSIGN);}
"-="       {yylval.string = new std::string(yytext); return(SUB_ASSIGN);}
"*="       {yylval.string = new std::string(yytext); return(MUL_ASSIGN);}
"/="			 {yylval.string = new std::string(yytext); return(DIV_ASSIGN);}
"%="			 {yylval.string = new std::string(yytext); return(MOD_ASSIGN);}
"&="       {yylval.string = new std::string(yytext); return(AND_ASSIGN);}
"^="			 {yylval.string = new std::string(yytext); return(XOR_ASSIGN);}
"|="       {yylval.string = new std::string(yytext); return(OR_ASSIGN);}
">>"       {return(RIGHT_OP);}
"<<"       {return(LEFT_OP);}
"++"			 {return(INC_OP);}
"--"			 {return(DEC_OP);}
"->"			 {return(PTR_OP);}
"&&"			 {return(AND_OP);}
"||"			 {return(OR_OP);}
"<="			 {return(LE_OP);}
">="			 {return(GE_OP);}
"=="			 {return(EQ_OP);}
"!="			 {return(NE_OP);}
";"			   {return(';');}
("{"|"<%") {return('{');}
("}"|"%>") {return('}');}
","			   {return(',');}
":"			   {return(':');}
"="			   {return('=');}
"("		     {return('(');}
")"			   {return(')');}
("["|"<:") {return('[');}
("]"|":>") {return(']');}
"."			   {return('.');}
"&"			   {return('&');}
"!"			   {return('!');}
"~"			   {return('~');}
"-"			   {return('-');}
"+"			   {return('+');}
"*"			   {return('*');}
"/"			   {return('/');}
"%"			   {return('%');}
"<"			   {return('<');}
">"			   {return('>');}
"^"			   {return('^');}
"|"			   {return('|');}
"?"			   {return('?');}

[\a\b\t\v\f\n\r]		{/* ignore new lines and special sequences */}
.			              {/* ignore bad characters */}

%%

void yyerror (char const *s)
{
  fprintf(stderr, "Lexing error: %s\n", s);
  exit(1);
}
