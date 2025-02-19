%{
#include <stdio.h>

int num_integers = 0;
int num_decimals = 0;
int num_operators = 0;
int num_lparens = 0;
int num_rparens = 0;
int total_tokens = 0;
%}

DIGIT [0-9]
EXPONENT [Ee][+-]?{DIGIT}+
DECIMAL {DIGIT}+"."{DIGIT}+({EXPONENT})?
INTEGER {DIGIT}+
OPERATOR [+\-*/=]

%%

{INTEGER}        { printf("NUMBER\t\t%s\n", yytext); num_integers++; total_tokens++; }
{DECIMAL}        { printf("DECIMAL NUMBER\t%s\n", yytext); num_decimals++; total_tokens++; }
{OPERATOR}       { printf("OPERATOR\t%s\n", yytext); num_operators++; total_tokens++; }
"("              { printf("LPAREN\t\t%s\n", yytext); num_lparens++; total_tokens++; }
")"              { printf("RPAREN\t\t%s\n", yytext); num_rparens++; total_tokens++; }
[ \t\n]          { /* Ignore whitespace */ }
.                { printf("UNKNOWN\t\t%s\n", yytext); total_tokens++; } /* Catch any other unknown token */

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Could not open file: %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }

    yylex();

    printf("\nSummary:\n");
    printf("Number of Integers: %d\n", num_integers);
    printf("Number of Decimal Numbers: %d\n", num_decimals);
    printf("Number of Operators: %d\n", num_operators);
    printf("Number of Left Parentheses: %d\n", num_lparens);
    printf("Number of Right Parentheses: %d\n", num_rparens);
    printf("Total Tokens: %d\n", total_tokens);

    return 0;
}

