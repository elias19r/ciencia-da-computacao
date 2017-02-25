/* Definicoes */
%{
	#include <stdlib.h>
	#include <stdio.h>
%}

M1  jan(eiro)?|0?1
M2  fev(ereiro)?|0?2
M3  mar((c|ç|Ç)o)?|0?3
M4  abr(il)?|0?4
M5  mai(o)?|0?5
M6  jun(ho)?|0?6
M7  jul(ho)?|0?7
M8  ago(sto)?|0?8
M9  set(embro)?|0?9
M10 out(ubro)?|10
M11 nov(embro)?|11
M12 dez(embro)?|12

MONTH (?i:{M1}|{M2}|{M3}|{M4}|{M5}|{M6}|{M7}|{M8}|{M9}|{M10}|{M11}|{M12})

D1  0?1|um|primeiro|1º
D2  0?2|dois
D3  0?3|tr(e|ê|Ê)s
D4  0?4|quatro
D5  0?5|cinco
D6  0?6|seis
D7  0?7|sete
D8  0?8|oito
D9  0?9|nove
D10 10|dez
D11 11|onze
D12 12|doze
D13 13|treze
D14 14|quatorze|catorze
D15 15|quinze
D16 16|dezesseis
D17 17|dezessete
D18 18|dezoito
D19 19|dezenove
D20 20|vinte
D21 21|vinte\ e\ um
D22 22|vinte\ e\ dois
D23 23|vinte\ e\ tr(e|ê|Ê)s
D24 24|vinte\ e\ quatro
D25 25|vinte\ e\ cinco
D26 26|vinte\ e\ seis
D27 27|vinte\ e\ sete
D28 28|vinte\ e\ oito
D29 29|vinte\ e\ nove
D30 30|trinta
D31 31|trinta\ e\ um

DAY (?i:{D1}|{D2}|{D3}|{D4}|{D5}|{D6}|{D7}|{D8}|{D9}|{D10}|{D11}|{D12}|{D13}|{D14}|{D15}|{D16}|{D17}|{D18}|{D19}|{D20}|{D21}|{D22}|{D23}|{D24}|{D25}|{D26}|{D27}|{D28}|{D29}|{D30}|{D31})

DOTW1 dom(ingo)?
DOTW2 seg(unda(\ ?-\ ?feira)?)?
DOTW3 ter((c|ç|Ç)a(\ ?-\ ?feira)?)?
DOTW4 qua(rta(\ ?-\ ?feira)?)?
DOTW5 qui(nta(\ ?-\ ?feira)?)?
DOTW6 sex(ta(\ ?-\ ?feira)?)?
DOTW7 s(a|á|Á)b(ado)?

DAYOFTHEWEEK (?i:{DOTW1}|{DOTW2}|{DOTW3}|{DOTW4}|{DOTW5}|{DOTW6}|{DOTW7})

YEAR [0-9]{2,4}

DAY_LITERAL (do\ )dia|(de\ )hoje|(da\ )semana
MONTH_LITERAL (de\ |para\ ){MONTH}
YEAR_LITERAL (de\ |para\ )[0-9]{4}

DATE_LITERAL (?i:{DAY_LITERAL}|{MONTH_LITERAL}|{YEAR_LITERAL})

SYMBOL (\"|\'|\ |:|;|\/|\\|\?|!|\.|,|\*|\(|\)|\[|\]|\{|\}|@|#|$|%|\^|-)

SEPARATOR (?i:\ de\ |a|à|À|\/|-|\ |\(|\))

S1  (a|á|Á)ries
S2  touro
S3  g(e|ê|Ê)meos
S4  c(a|â|Â)ncer
S5  le(a|ã|Ã)o
S6  virgem
S7  libra
S8  escorpi(a|ã|Ã)o
S9  sagit(a|á|Á)rio
S10 capric(o|ó|Ó)rnio
S11 aqu(a|á|Á)rio
S12 peixes

SIGN (?i:{S1}|{S2}|{S3}|{S4}|{S5}|{S6}|{S7}|{S8}|{S9}|{S10}|{S11}|{S12})

KEYWORD (?i:(no\ )?amor|sexo|trabalho|perfil|profiss(o|õ|Õ)es|profiss(a|ã|Ã)o|voca(c|ç|Ç)(a|ã|Ã)o|(na\ )amizade|(no\ )estilo|(nos?\ )?estudos?)

NAME ([A-ZÁÃÂÀÉÊÍÓÕÔÚÜ][a-záãâàéêíóõôúüçÇ]+)

WORD (?i:[a-z0-9áÁãÃâÂàÀéÉêÊíÍóÓõÕôÔúÚüÜçÇ]+)

BY (?i:por)

/* Regras */
%%

{SEPARATOR}{SIGN}{SEPARATOR} { printf("[SIGN:%s]", yytext); }
{SIGN}{SEPARATOR} { printf("[SIGN:%s]", yytext); }
{SIGN}$                      { printf("[SIGN:%s]", yytext); }

({DAYOFTHEWEEK}{SEPARATOR}{SEPARATOR}?)?{DAY}{SEPARATOR}{MONTH}({SEPARATOR}{YEAR})?{SEPARATOR} { printf("[DATE:%s]", yytext); }
({DAYOFTHEWEEK}{SEPARATOR}{SEPARATOR}?)?{DAY}{SEPARATOR}{MONTH}({SEPARATOR}{YEAR})?$ { printf("[DATE:%s]", yytext); }
{MONTH}{SEPARATOR}{YEAR}{SEPARATOR} { printf("[DATE:%s]", yytext); }
{MONTH}{SEPARATOR}{YEAR}$ { printf("[DATE:%s]", yytext); }
{DAY}{SEPARATOR}{MONTH}{SEPARATOR}({DAYOFTHEWEEK}{SEPARATOR})? { printf("[DATE:%s]", yytext); }
{DAY}{SEPARATOR}{MONTH}{SEPARATOR}({DAYOFTHEWEEK})?$ { printf("[DATE:%s]", yytext); }
{DATE_LITERAL}{SEPARATOR} { printf("[DATE:%s]", yytext); }
{DATE_LITERAL}$ { printf("[DATE:%s]", yytext); }

^{KEYWORD}$ { printf("[KEYWORD:%s]", yytext); }

({WORD}{SYMBOL}+){8,}{WORD}? { printf("[ADVICE:%s]", yytext); }

^{BY}?({SYMBOL}?{NAME}){2,}$ { printf("[AUTHOR:%s]", yytext); }

%%
/* Codigo do usuario */

int main(int argc, char **argv) {

	yylex();
	
	return EXIT_SUCCESS;
}

