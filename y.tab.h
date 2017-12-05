/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INTEGER = 258,
    IDENTIFIER = 259,
    STRING = 260,
    INT_ARRAY = 261,
    PLUS = 262,
    MINUS = 263,
    MULTIPLY = 264,
    DIVIDE = 265,
    ASSIGNMENT = 266,
    COMMA = 267,
    SEMICOLON = 268,
    INCREASE = 269,
    DECREASE = 270,
    PRINT_STRING = 271,
    PRINT_INTEGER = 272,
    LEFT_ROUND_BRACKET = 273,
    RIGHT_ROUND_BRACKET = 274,
    LEFT_BRACE = 275,
    RIGHT_BRACE = 276,
    BOOL_EQ = 277,
    BOOL_NE = 278,
    BOOL_GT = 279,
    BOOL_LT = 280,
    BOOL_GE = 281,
    BOOL_LE = 282,
    BOOL_AND = 283,
    BOOL_OR = 284,
    BOOL_NOT = 285,
    IF = 286,
    ELSE = 287,
    FOR = 288,
    WHILE = 289,
    TYPE_INT = 290,
    TYPE_INT_ARRAY = 291,
    TYPE_STENCIL = 292
  };
#endif
/* Tokens.  */
#define INTEGER 258
#define IDENTIFIER 259
#define STRING 260
#define INT_ARRAY 261
#define PLUS 262
#define MINUS 263
#define MULTIPLY 264
#define DIVIDE 265
#define ASSIGNMENT 266
#define COMMA 267
#define SEMICOLON 268
#define INCREASE 269
#define DECREASE 270
#define PRINT_STRING 271
#define PRINT_INTEGER 272
#define LEFT_ROUND_BRACKET 273
#define RIGHT_ROUND_BRACKET 274
#define LEFT_BRACE 275
#define RIGHT_BRACE 276
#define BOOL_EQ 277
#define BOOL_NE 278
#define BOOL_GT 279
#define BOOL_LT 280
#define BOOL_GE 281
#define BOOL_LE 282
#define BOOL_AND 283
#define BOOL_OR 284
#define BOOL_NOT 285
#define IF 286
#define ELSE 287
#define FOR 288
#define WHILE 289
#define TYPE_INT 290
#define TYPE_INT_ARRAY 291
#define TYPE_STENCIL 292

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 19 "stenc.y" /* yacc.c:1909  */

        int integer_value;
        char *string;
        int *array;
        enum relop_enum {EQ, NE, GT, LT, GE, LE} relop_code;
        enum for_iterator_enum {INCR, DECR} for_iterator_code;
        struct codegen {
                struct symbol *result;
                struct list *code;
                struct list *truelist;
                struct list *falselist;
                struct list *nextlist;
        }gencode;

#line 143 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
