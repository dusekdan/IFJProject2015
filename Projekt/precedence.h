/***********************precedence.h*****************************/
/* Soubor: precedence.h - Syntaktická analýza výrazů 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/*					Matúš Bútora (xbutor01)						*/
/****************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "ial.c"
#include "errorHandler.c"


#define STACKSIZE 50;


typedef enum {

	pMORE,		// > redukce
	pLESS,		// < shift
	pEQUAL,		// = equal
	pEMPTY		// syntakticka chyba - prazdne misto v precedencni tabulce

} tPriority;

typedef enum {

	PLUS,
	MINUS,
	MUL,
	DIV,
	LEFT,
	RIGHT,
	LESS,
	MORE,
	LESSEQUAL,
	MOREEQUAL,
	EQUAL,
	NONEQUAL,
	ID,
	DOLAR,
	NETERM,
	SHIFT

} tOperators;

static const int rule_1[3] = {NETERM, PLUS, NETERM};
static const int rule_2[3] = {NETERM, MINUS, NETERM};
static const int rule_3[3] = {NETERM, MUL, NETERM};
static const int rule_4[3] = {NETERM, DIV, NETERM};
static const int rule_5[3] = {NETERM, LESS, NETERM};
static const int rule_6[3] = {NETERM, MORE, NETERM};
static const int rule_7[3] = {NETERM, LESSEQUAL, NETERM};
static const int rule_8[3] = {NETERM, MOREEQUAL, NETERM};
static const int rule_9[3] = {NETERM, EQUAL, NETERM};
static const int rule_10[3] = {NETERM, NONEQUAL, NETERM};
static const int rule_11[3] = {RIGHT, NETERM, LEFT};
static const int rule_12[3] = {0, 0, ID};



typedef struct {

	tData symbol;
	tOperators element;

} tOpData;

typedef struct tElement {

	tOpData data;
	struct tElement *pointer;

} *tElement;

typedef struct {

	tElement top;

} tStack;


void stackInit(tStack *stack);		// inicializace zasobniku
//bool stackFull();					// kontrola plneho zasobniku
bool stackEmpty(tStack *stack);		// kontrola prazdneho zasobniku
tOpData stackTop(tStack *stack);
void stackPop(tStack *stack, tOpData *data);
bool stackPush(tStack *stack, tOpData element);

int precedenceParser();
int zpracuj(token tok, tOpData *column);
int reduction(tStack *stack1, tStack *stack2);