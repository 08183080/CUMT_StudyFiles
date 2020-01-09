%{
#include <stdlib.h>
#include <stdio.h>

#include "node.h"
#define YYSTYPE node

#include "siyuanshi.h"

int yylex(void);
void yyerror(char *);
void print_siyuanshi();

%}
%token ID NUMBER RELOP1 RELOP2 WHILE MAIN
%left '+' '-'	//�涨���������ȼ�
%left '*' '/'
%left '{' '}' '(' ')' '=' ';'
%%
program:
program MAIN block '\n' {print_xvhao();printf("program -> main block \n");
			print_siyuanshi();init(); }
|
; 

block:
'{' stmts '}'  {print_xvhao();printf("block -> { stmts }\n");}
;

stmts:
stmt stmts  {print_xvhao();printf("stmts -> stmt stmts\n");}
|
;

stmt:
ID '=' E ';'	 {print_xvhao();printf("stmt -> ID = E\n");emit_ID($1.text,$3.T);}
| WHILE '(' bool ')' stmt  {print_xvhao();printf("stmt -> while(bool) stmt\n");}
| block			{print_xvhao();printf("stmt -> block\n");}
;
 //��ΪYacc�Դ����ȼ��������Ż���ʦ��������������ķ���ͬʱΪ�˿���ʵ�֣�ȥ�����ŵ�ʵ��
E:
E '+' E  {print_xvhao();printf("E -> E + E\n");newtemp(&$$);emit_3addr($$.T,"+",$1.T,$3.T);}
|E '*' E  {print_xvhao();printf("E -> E * E\n");newtemp(&$$);emit_3addr($$.T,"*",$1.T,$3.T);}
|T	{print_xvhao();printf("E-> T\n");$$.T=$1.T;}//������Ҫ���������������ʶ����ӣ��ܷ���ȷʶ��
; 

bool:		
T RELOP1 T	{print_xvhao();printf("bool -> T<=T\n");newtemp(&$$);emit_relop("<=",$$.T,$1.T,$2.T);}
| T RELOP2 T	{print_xvhao();printf("bool -> T>=T\n");newtemp(&$$);emit_relop(">=",$$.T,$1.T,$2.T);}
| T		{print_xvhao();printf("bool -> T\n");}//�����йر�ʶ���������ֿ���ͬ��
;

T:
ID		{print_xvhao();printf("T -> ID: %s\n",$1.text);}
| NUMBER	{print_xvhao();printf("T -> NUM: %d \n",$1.a);newtemp(&$$);emit_num($$.T,$1.a);}
;

%%

int main(void) {
  yyparse();
  system("pause");
  return 0;
}

void print_siyuanshi(){
	printf("��Ԫʽ����Ϊ��\n%s����������\n-------------------\n",code_buff);
}

void yyerror(char *s) {
  printf("�﷨��������%s\n", s);
}
