/***********************ial.c************************************/
/* Soubor: ial.h - Hlavičkový soubor pro ial.c 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/****************************************************************/

//#include "parser.h"


#ifndef max
	#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

typedef struct {			// data promenne v symbolu

	int integer;
	double real;
	char *string;
	bool boolean;
	bool initialized;

} tContent;
/*  LEX ČÁST */
typedef struct tData {		// symbol

	char *name;				// nazev symbolu
	int type; 			    // datovy typ symbolu
	int argCount;			// pocet argumentu funkce, pro promennou hodnota NULL
	struct tNodePtr *localTSadr; //Adresa lokalnej tabulky
	struct tNodePtr *nextArg;			// ukazatel na dalsi argument funkce
	tInsList *localILadr;        //ukazatel na adresu lokalnych instrukcii
	tContent content;		// obsah promenne	
	bool used;	
} *tData;


typedef struct tNodePtr {		// uzel tabulky symbolu

	char *key;
	tData data;
	struct tNodePtr *rptr;
	struct tNodePtr *lptr;
} *tNodePtr;

extern tNodePtr rootTS;		// ukazatel na zacatek tabulky symbolu


char *quickSort(char *text, int l, int r);		// quicksort, nejspise jen cast
char *allocQuickSort(char *text, int l, int r);
void partition(char *text, int *i, int *j);		// rozdeleni pro quickSort

void init(tNodePtr *rootTS);
void initTable(tNodePtr *rootTS);
void disposeTable(tNodePtr *rootTS);
tNodePtr createNode(char *key, tData data);
tNodePtr insertSymbol(tNodePtr *rootTS, char *key, tData data);
tNodePtr searchSymbol(tNodePtr *rootTS, char* key);

int BMASearch();
void BMACountBadChar();
void BMACountGoodSuffix();
void BMASuffixes();

