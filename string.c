#include "string.h"

/*MUDULOS PUBLICOS QUE YO AGREGUE*/

void limpiar_buffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

#define fflush(stdin) limpiar_buffer()

/*PUBLICOS*/

str create(){/*CREA NODO*/
	str nvo;
	nvo=NULL;
	nvo=(str)malloc(sizeof(tnodo));
	nvo->dato = '\0';
	nvo->sig = NULL;
	return nvo;
}

str load(){/*CARGA CADENA*/
	str head=NULL, aux=NULL, act=NULL;
	char c = getchar();
	while(c!=EOF && c!='\n'){
		aux=create();
		aux->dato=c;

		if(head==NULL)
			head = aux;
		else
			act->sig = aux;
		act = aux;

		c=getchar();
	}
	return head;
}

str load2(const char* cadena){

	str Cab=NULL; str act=NULL;
	int i=0;

	while(cadena[i]!='\0'){
		str nvo=create();
		nvo->dato = cadena[i];

		if(Cab==NULL)
			Cab = nvo;
		else
			act->sig = nvo;
		act = nvo;

		i++;
	}
	return Cab;
}

void print(str cad){
	if(cad!=NULL){
		printf("'");
		while(cad!=NULL){
			printf("%c",cad->dato);
			cad=cad->sig;
		}
		printf("'");
	}
}
str concat(str a,str b){
	str head=NULL, aux=NULL, act=NULL;
	while (a!=NULL) {
		aux=create();
		aux->dato = a->dato;

		if (head==NULL)
			head = aux;
		else
			act->sig = aux;
		act = aux;

		a = a->sig;
	}
	while (b!=NULL) {
		aux=create();
		aux->dato = b->dato;

		if (head==NULL)
			head = aux;
		else
			act->sig = aux;
		act = aux;

		b = b->sig;
	}
	return head;
}

str beforeToken(str origen, char token){/*PARTE UNA CADENA*/
	str aux=NULL, head=NULL, act=NULL;

	while(origen!=NULL && origen->dato!=token){
		aux = create();
		aux->dato = origen->dato;

		if(head == NULL)
			head = aux;
		else
			act->sig = aux;
		act = aux;

		origen = origen->sig;
	}
	return head;
}
str afterToken (str origen, char token){
	str aux=NULL, head=NULL, act=NULL;

	while (origen != NULL && origen->dato!=token) {
		origen = origen->sig;
	}
	if (origen) origen = origen->sig;

	while (origen) {
		aux = create();
		aux->dato = origen->dato;

		if(head == NULL)
			head = aux;
		else
			act->sig = aux;
		act = aux;
		origen = origen->sig;
	}
	return head;
}

void freeString(str A){
	str temp=NULL;

	while(A){
		temp=A;
		A=A->sig;
		free(temp);
	}
}
int compStr(str a, str b) {
	int flag = 0;							/*Funcion usada en TADSet para contains, devuelve 0 si ambos son iguales*/
	while (a && b && flag == 0) {
		if (a->dato != b->dato)
			flag = 1;
		a = a->sig;
		b = b->sig;
	}
	if (a || b)
		flag = 1;

	return flag;
}
str copyStr(str origen){
	str aux=NULL, head=NULL, act=NULL;
	while(origen!=NULL){
		aux = create();
		aux->dato = origen->dato;

		if(head == NULL)
			head = aux;
		else
			act->sig = aux;
		act = aux;

		origen = origen->sig;
	}
	return head;
}
int isin (str a, str b){						/*devuelve si la cadena a está contenida en la cadena b, 0 si es verdadero*/
	int flag = 0;
	while (a && b && a->dato != b->dato) {
		a = a->sig;
	}

	while (a && b && flag == 0) {
		if (a->dato != b->dato)
			flag = 1;
		a = a->sig;
		b = b->sig;
	}
	if (a)
		flag = 1;

	return flag;
}