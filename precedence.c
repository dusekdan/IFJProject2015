/***********************precedence.c*****************************/
/* Soubor: precednce.c - Syntaktická analýza výrazů 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/*					Matúš Bútora (xbutor01)						*/
/****************************************************************/

#include "precedence.h"

bool isReal = false;	// pokud najdeme hodnotu reálné hodnoty

int countID = 0;		// počítání instrukcí
int matusOp;			// instrukce pro interpret

/* Načtení dalšího tokenu */
token gib_tok() {
	
	getNextToken(fd, tok);
	return tok;
}

/* Inicializace zásobníku */
void stackInit(tStack *stack) {

	stack->top = NULL;
}

/* Kontrola, zda je zásobník prázdná nebo ne */
bool stackEmpty(tStack *stack) {

	if(stack->top == NULL)
		return true;

	else
		return false;
}

/* Přídání prvku na vrchol zásobníku, vrácím boolean hodnotu podle toho, zda se povedlo či ne */
bool stackPush(tStack *stack, tOpData element) {

	tElement newElem;


	if((newElem = malloc(sizeof(struct tElement))) == NULL) {
		{InsertLastMarius(&Smetisko, newElem);
			errorHandler(errInt);
				return false;}
	}
	//InsertLastMarius (&Smetisko, newElem);

	newElem->data = element;
	newElem->pointer = stack->top;
	stack->top = newElem;

	return true;
}

/* Smazání prvku z vrcholu zásobníku */
void stackPop(tStack *stack, tOpData *data) {

	if(stack->top != NULL) {

		tElement temp;

		temp = stack->top;
		*data = stack->top->data;
		stack->top = stack->top->pointer;

		free(temp);
	}
}

/* Vrácení prvku z vrcholu zásobníku  */
tOpData stackTop(tStack *stack) {

	return (stack->top->data);
}

/* Smazání celého zásobníku */
void stackDispose(tStack *stack) {

	tOpData temp;

	while(stackEmpty(stack) != true) {

		stackPop(stack, &temp);
	}

	stack->top = NULL;
}

/* Precedenční tabulka */
tPriority precedenceTable [14][14] = {

	{pMORE, pMORE, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pMORE, pMORE, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pMORE, pMORE, pMORE, pMORE, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pMORE, pMORE, pMORE, pMORE, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pEQUAL, pLESS, pLESS, pLESS, pLESS, pLESS, pLESS, pLESS, pEMPTY},
	{pMORE, pMORE, pMORE, pMORE, pEMPTY,pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pEMPTY, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pMORE, pMORE, pMORE, pMORE, pEMPTY, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pEMPTY, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pEMPTY, pLESS, pLESS, pLESS, pLESS, pLESS, pLESS, pLESS, pEMPTY},
};

/* Zpracování tokenů z lexikální analýzy */
int zpracuj(token tok, tOpData *column) {

	char *key;
	tNodePtr node;

	switch(tok->type) {

		case 31:
			column->element = PLUS;
			break;
		
		case 32:
			column->element = MINUS;
			break;

		case 33:
			column->element = MUL;
			break;

		case 34:
			column->element = DIV;
			break;

		case 4:
			column->element = LEFT;
			break;

		case 5:
			column->element = RIGHT;
			break;

		case 35:
			column->element = LESS;
			break;

		case 36:
			column->element = MORE;
			break;

		case 37:
			column->element = LESSEQUAL;
			break;

		case 38:
			column->element = MOREEQUAL;
			break;

		case 39:
			column->element = EQUAL;
			break;

		case 40:
			column->element = NONEQUAL;
			break;

		case 3:								// strednik
		case 14:							// then
		case 17:							// do
		case 9:								// end

			column->element = DOLAR;
			break;

		case 20:							// var

			column->element = ID;

			if((node = malloc(sizeof(struct tNodePtr))) == NULL) {
				InsertLastMarius(&Smetisko, node);
				errorHandler(errInt);
				return -1;
			}

			if((key = malloc(sizeof(char)*(strlen(tok->val_str) + 1))) == NULL) {
				InsertLastMarius(&Smetisko, key);
				errorHandler(errInt);
				return -1;
			}

			memset (key, 0, strlen(key)); //Treba key pred prvým strcatom vynulovať, inak ak je tam bordel, pripája sa až zaň.
			strcat(key, "V");
			strcat(key, tok->val_str);
		
			if((node = searchSymbol(&localTS, key)) != 0);
				//printf("Nasel jsem %s v lokalni tabulce symbolu.\n",key);

			else {

				if((node = searchSymbol(&rootTS, key)) != 0);
					//printf("Nasel jsem %s v globalni tabulce symbolu.\n",key);

				else {
					//free(key);
					//free(node);
					fprintf(stderr, "Promenna nebyla nalezena.\n");
					return -1;
				}
			}

			column->key = key;

			//free(key);

			if((column->symbol = malloc(sizeof(struct tData))) != NULL) {
				InsertLastMarius(&Smetisko, column->symbol);

				if(node->data->type == sym_var_rea)
					column->symbol->type = t_expr_dou;
			
				if(node->data->type == sym_var_int)
					column->symbol->type = t_expr_int;
			
				if(node->data->type == sym_var_str)
					column->symbol->type = t_expr_str;
			
				if(node->data->type == sym_var_boo)
					column->symbol->type = t_expr_boo;
			}
			else
				errorHandler(errInt);

			break;

		case 41:	// int, real, bool, string
		case 42:
		case 43:						
		case 44:	

			if((column->symbol = malloc(sizeof(struct tData))) != NULL) {
				InsertLastMarius(&Smetisko, column->symbol);
				column->element = ID;
				
				if(tok->type == t_expr_int)
					column->symbol->content.integer = tok->val_int;
				if(tok->type == t_expr_boo)
					column->symbol->content.boolean = tok->val_int;
				if(tok->type == t_expr_str)
					column->symbol->content.string = tok->val_str;
				if(tok->type == t_expr_dou)
				{
					column->symbol->content.real = tok->val_flo;
					isReal=true;
				}

				column->symbol->content.initialized = true;
				column->symbol->used = false;
				column->symbol->type = tok->type;
			}
			else
				errorHandler(errInt);

			char *tempKey = randstring(20);

			if((key = malloc(sizeof(char)*(strlen(tempKey)))) == NULL) {
				InsertLastMarius(&Smetisko, key);
				errorHandler(errInt);
				return -1;
			}

			strcpy(key, tempKey);

			if((node = malloc(sizeof(struct tNodePtr))) == NULL) {
				InsertLastMarius(&Smetisko, node);
				errorHandler(errInt);
				return -1;
			}

			node = insertSymbol(&rootTS, key, column->symbol);
			column->key = key;

			//free(key);
			//free(column->symbol);	
			break;

		default:

			errorHandler(errSyn);
			return -1;	
	}

	return 0;
}

/* Hlavní funkce precedeční analýzy */
int precedenceParser() {

	tStack stack1;
	stackInit(&stack1);

	tStack stack2;
	stackInit(&stack2);

	tOpData column;	// indexy do precedencni tabulky 
	tOpData row;
	tOpData temp;
	tOpData change;
	tOpData theEnd;

	numberOfExprInsts = 0;
	int error;
	int count = 0;
	int conStep = 1;
	int returnType = -1;
	bool skipGib=true;
	isReal = false;


	temp.element = DOLAR;
	stackPush(&stack1, temp);		// pushnuti dolaru na vrchol zasobniku

	do {

		if(conStep == 1) {
		
				if (skipGib==false)
					tok = gib_tok();
			
				skipGib=false;
		
				if((error = zpracuj(tok, &column)) != 0) {	// pokud skoncime s chybou, breakujeme

					stackDispose(&stack1);
					stackDispose(&stack2);
					errorHandler(errSyn);
				}

		}

		row = stackTop(&stack1);		// precteni tokenu na vrcholu zasobniku

		while(row.element == NETERM || row.element == SHIFT) { // preskakujeme na zasobniku neterminaly
			
			stackPop(&stack1, &change);
			stackPush(&stack2, change);
			row = stackTop(&stack1);
		}

		while(stackEmpty(&stack2) != true) {			// navraceni neterminalu na hlavni zasobnik

			stackPop(&stack2, &change);
			stackPush(&stack1, change);
		}

		switch(precedenceTable[row.element][column.element]) {		// Rozdělení podle precedenční tabulky

			case pEQUAL:				// equal
				
				conStep = 1;
				stackPush(&stack1, column);
				break;

			case pLESS:					// shift

				conStep = 1;
				while(stackTop(&stack1).element == NETERM || stackTop(&stack1).element == SHIFT) {	// preskakujeme na zasobniku neterminaly
			
					stackPop(&stack1, &change);
					stackPush(&stack2, change);
				}

				temp.element = SHIFT;						// pushnuti SHIFTu pred neterminal
				stackPush(&stack1, temp);

				while(stackEmpty(&stack2) != true) {		// vraceni neterminalu na zasobnik

					stackPop(&stack2, &change);
					stackPush(&stack1, change);
				}

				stackPush(&stack1, column);

				break;

			case pMORE:					// redukce

				conStep = 0;

				if((returnType = reduction(&stack1, &stack2)) < 0) {

					stackDispose(&stack1);
					stackDispose(&stack2);
					errorHandler(errSyn);	
				}

				break;

			case pEMPTY:							// empty, syntax error

				stackDispose(&stack1);
				stackDispose(&stack2);
				errorHandler(errSyn);
				break;
		}

		while(stackTop(&stack1).element == NETERM || stackTop(&stack1).element == SHIFT) {

			stackPop(&stack1, &change);
			stackPush(&stack2, change);
		}

		theEnd = stackTop(&stack1);

		while(stackEmpty(&stack2) != true) {

			stackPop(&stack2, &change);
			stackPush(&stack1, change);
		}

	} while((column.element != DOLAR) || (theEnd.element != DOLAR));
	
	if(countID == 1 && column.element == DOLAR) {

		if((matusOp = myOp2matousOp(PLUS, column.symbol->type, 41)) != -1) {

			numberOfExprInsts++;
			if(localIL == NULL) {
							
				insertInst(&IL, matusOp, searchData(column.key), NULL, NULL);
				//printf("Vlozil jsem instrukci %d s ukazatelem %u do listu %u\n", matusOp, &column.symbol, &IL);
			}
			else {

				insertInst(localIL, matusOp, searchData(column.key), NULL, NULL);
				//printf("Vlozil jsem instrukci %d s ukazatelem %u do listu %u\n", matusOp, &column.symbol, localIL);
			}
					
			returnType = column.symbol->type;
		}

	}

	stackDispose(&stack1);
	stackDispose(&stack2);
	countID = 0;
	
	if (returnType == 41 && isReal == true) 
		returnType = t_expr_dou;
	
	return returnType;
}

/* Funkce zprostředkovávající redukci pravidel dle precedenční tabulky */
int reduction(tStack *stack1, tStack *stack2) {

	tOpData help = stackTop(stack1);
	tOpData change;
	tOpData temp1;
	tOpData temp2;
	tOpData temp3;
	int concat = 0;
	bool boolean = false;
	int returnType = -1;
	int control = 0;
	int checkRule;

	//printf("HELP: %d\n", help.element);

	while(help.element != SHIFT/* && stackEmpty(stack1) != true*/) {			//	dostaneme se az na SHIFT, vse na druhem zasobniku pouzijeme pro jedno z pravidel

		stackPop(stack1, &change);
		stackPush(stack2, change);
		help = stackTop(stack1);
	}


	//help = stackTop(stack1);
	if(help.element == SHIFT) {

		stackPop(stack2, &temp1);	// nacteme si dalsi oper z druheho zasobniku
		//printf("%d\n", temp1.element);
		
		if(temp1.element == ID && stackEmpty(stack2) == true) {		// zacneme od nejjednodusiho - E->ID
			
			countID++;

			returnType = temp1.symbol->type;
			stackPop(stack1, &change);		// popneme SHIFT, ze zasobniku, uz neni potreba
			change = temp1;
			change.element = NETERM;
			stackPush(stack1, change);		// pushneme dle pravidla - E (neterminal)
		}

		else if(temp1.element == NETERM) {	// nyni vsechna pravidla pro neterminaly

			stackPop(stack2, &temp2);
			//printf("%d\n", temp2.element);

			switch(temp2.element) {
				//printf("%d\n", temp2.symbol->type);

				case PLUS:
					checkRule = PLUS;
					control = 1;
					break;
				case MINUS:
				
					checkRule = MINUS;
					control = 1;
					break;
				case MUL:
					checkRule = MUL;
					control = 1;
					break;
				case DIV:
					checkRule = DIV;
					control = 1;
					break;
				case LESS:
					checkRule = LESS;
					control = 1;
					boolean = true;
					break;
				case MORE:
					checkRule = MORE;
					boolean = true;
					control = 1;
					break;
				case MOREEQUAL:
					checkRule = MOREEQUAL;
					boolean = true;
					control = 1;
					break;
				case LESSEQUAL:
					checkRule = LESSEQUAL;
					boolean = true;
					control = 1;
					break;
				case EQUAL:
					checkRule = EQUAL;
					boolean = true;
					control = 1;
					break;
				case NONEQUAL:
					checkRule = NONEQUAL;
					boolean = true;
					control = 1;
					break;

				default:
					control = 0;
					errorHandler(errSyn);
					break;
			}

			if(control == 0)
				return -1;

			else {

				stackPop(stack2, &temp3);
				//printf("%d\n", temp3.element);

				if(temp3.element == NETERM) {

					if(temp1.symbol->type == temp3.symbol->type) {

						if(checkRule == DIV && temp1.symbol->type == t_expr_int)
							returnType = t_expr_dou;

						if(boolean == true)
							returnType = t_expr_boo;
						
						else if(checkRule == DIV && temp1.symbol->type == t_expr_int) {
							returnType = t_expr_dou;
						}
						else
							returnType = temp1.symbol->type;
						

						numberOfExprInsts++;

						if((matusOp = myOp2matousOp(checkRule, temp1.symbol->type, temp3.symbol->type)) != -1) {
							
							if(localIL == NULL) {
								
								insertInst(&IL, matusOp, searchData(temp1.key), searchData(temp3.key), NULL);
								//printf("Vlozil jsem instrukci %d s ukazateli %d a %d do listu %u\n", matusOp, &temp1.symbol, &temp3.symbol, &IL);

							}
							else {

								insertInst(localIL, matusOp, searchData(temp1.key), searchData(temp3.key), NULL);
								//printf("Vlozil jsem instrukci %d s ukazateli %u a %u do listu %u\n", matusOp, temp1.symbol->content.integer, temp3.symbol->content.integer, localIL);
							}
						}
					}
					else if((temp1.symbol->type == t_expr_int && temp3.symbol->type == t_expr_dou) || (temp3.symbol->type == t_expr_int && temp1.symbol->type == t_expr_dou)) {

						if(boolean == 1)							// pokud mame logickou operaci, musime vracet boolean hodnotu
							returnType = t_expr_boo;
						else
							returnType = t_expr_dou;

						numberOfExprInsts++;

						if((matusOp = myOp2matousOp(checkRule, temp1.symbol->type, temp3.symbol->type)) != -1) {
							
							if(localIL == NULL) {
								
								insertInst(&IL, matusOp, searchData(temp1.key), searchData(temp3.key), NULL);
								//printf("Vlozil jsem instrukci %d s ukazateli %u a %u do listu %u\n", matusOp, &temp1.symbol, &temp3.symbol, &IL);
							}
							else {

								insertInst(localIL, matusOp, searchData(temp1.key), searchData(temp3.key), NULL);
								//printf("Vlozil jsem instrukci %d s ukazateli %u a %u do listu %u\n", matusOp, &temp1.symbol, &temp3.symbol, localIL);
							}
						}
					}

					else {
						//printf("Ve vyrazu nejsou stejne typy.\n");
						errorHandler(errSemTypArg);
					}


					if(stackEmpty(stack2) == true) {

						stackPop(stack1, &change);	// odstraneni <
						//change.symbol->type = temp1.symbol->type;
						if(temp1.symbol->type == t_expr_dou || temp3.symbol->type == t_expr_dou) {

							change = temp1;
							change.symbol->type = t_expr_dou;
						} 
						else
							change = temp1;
						
						change.element = NETERM;
						stackPush(stack1, change);	
					}
				}

				else {

					errorHandler(errSyn);
					return -1;
				} 
			}
		}

		else if(temp1.element == LEFT) {

			stackPop(stack2, &temp2);

			if(temp2.element == NETERM) {

				stackPop(stack2, &temp3);

				if(temp3.element == RIGHT && stackEmpty(stack2) == true) {

					stackPop(stack1, &change);
					change.element = NETERM;
					change = temp2;
					stackPush(stack1, change);
					returnType = temp2.symbol->type;
				}
			}
		}

		else {

			errorHandler(errSyn);
			return -1;
		}
	}

	return returnType;

}

/* Převod precedenčních instrukcí na interpretové instrukce */
int myOp2matousOp(int myOp, int type1, int type2) {

	int matusOp;

	switch(myOp) {

		case PLUS:

			if(type1 == t_expr_int && type2 == t_expr_int)
				matusOp = I_ADDI;

			else if(type1 == t_expr_dou || type2 == t_expr_dou) 
				matusOp = I_ADDR;

			else if(type1 == t_expr_str)
				matusOp = I_CONCATE;

			else
				return -1;

			break;

		case MINUS:

			if(type1 == t_expr_int && type2 == t_expr_int)
				matusOp = I_SUBI;

			else if(type1 == t_expr_dou || type2 == t_expr_dou) 
				matusOp = I_SUBR;

			else
				return -1;

			break;
		case MUL:

			if(type1 == t_expr_int && type2 == t_expr_int)
				matusOp = I_MULI;

			else if(type1 == t_expr_dou || type2 == t_expr_dou) 
				matusOp = I_MULR;

			else
				return -1;

			break;
		case DIV:
			matusOp = I_DIVR;
			break;
		case LESS:
			matusOp = I_LESS;
			break;
		case MORE:
			matusOp = I_MORE;
			break;
		case MOREEQUAL:
			matusOp = I_EMORE;
			break;
		case LESSEQUAL:
			matusOp = I_ELESS;
			break;
		case EQUAL:
			matusOp = I_EQUAL;
			break;
		case NONEQUAL:
			matusOp = I_NEQUAL;
			break;
	}

	return matusOp;
}

tNodePtr searchData(char *key) {

	tNodePtr node;

	if((node = searchSymbol(&localTS, key)) != 0);
				//printf("Nasel jsem %s v lokalni tabulce symbolu.\n",key);
	else {

		if((node = searchSymbol(&rootTS, key)) != 0);
					//printf("Nasel jsem %s v globalni tabulce symbolu.\n",key);
		else {
			//free(key);
			//free(node);
			fprintf(stderr, "Promenna nebyla nalezena.\n");
			return NULL;
		}
	}

	return node;
}
