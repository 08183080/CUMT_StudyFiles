%{
#include <stdlib.h>
void yyerror(char *);
#include "y.tab.h"
%}

letter			[A-Za-z_]
digit			[0-9]
id			({letter}|\_)(\_|{letter}|{digit})*

%%

while		{return WHILE;}
main		return MAIN;
{id}		return ID;

"<="	          {return RELOP1;}
">="	          {return (RELOP2);}
[0-9]+       { yylval = atoi(yytext); return NUMBER; }
[-+*/\n(){}=;]     return *yytext;
[ \t\n]         ;/* ȥ���ո� */
.            yyerror("��Ч�ַ�");
%%
int yywrap(void) {
  return 1;
}
