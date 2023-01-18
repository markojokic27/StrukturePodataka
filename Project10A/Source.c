#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE 1024
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum ERROR_CODE {
	FILE_NOT_OPEN = -1,
	USPJESNO_ZAVRSEN_PROGRAM,
	USPJESNO_IZVRSENO,
	NEUSPJESNO_IZVRSENO,
};


typedef struct Grad* P_Grad;
typedef struct P_Grad* PP_Grad;
typedef struct Grad {
	char Naziv_Grad[MAX_LINE];
	int broj_stanovnika;
	P_Grad L;
	P_Grad D;
}Grad;

typedef struct Drzava* P_Drzava;
typedef struct Drzava {
	char Naziv_Drzava[MAX_LINE];
	P_Drzava next;
	P_Grad root;
}Drzava;

int CitaDatDrzave(P_Drzava);
int NovaDrzava(char*, P_Drzava, P_Drzava);
int CitaDatGradovi(P_Drzava, char*);
int NoviGrad(char*, int, P_Drzava);
int UnosGrada(P_Grad, P_Grad);
int PrintAll(P_Drzava);
int PrintSome(P_Drzava, char*, int);
int ispisStabla(P_Grad);
int FiltriraniIspisStabla(P_Grad, int);



int main(void) {

	Drzava Head_Drzava = {
		.Naziv_Drzava = {0},
		.next = NULL,
		.root = NULL
	};

	if (CitaDatDrzave(&Head_Drzava) == USPJESNO_IZVRSENO)
	{
		printf("Uspjesno uneseno. \n");
	}


	char drzava[MAX_LINE] = { 0 };
	int broj_st = 0;
	PrintAll(&Head_Drzava);

	printf("Unesi drzavu pa minimalan broj stanovnika: ");
	scanf(" %s %d", drzava, &broj_st);

	PrintSome(&Head_Drzava, &drzava, broj_st);

	return 0;
}

int CitaDatDrzave(P_Drzava head) {
	FILE* fp = NULL;
	char buffer[MAX_LINE] = { 0 };
	char* p = buffer;
	char drzava[MAX_LINE] = { 0 };
	char file[MAX_LINE] = { 0 };
	fp = fopen("drzave.txt", "r");
	if (fp == NULL) {
		printf("Dokument se nije otvorio.\n");
		return FILE_NOT_OPEN;
	}

	while (!feof(fp)) {
		fgets(buffer, MAX_LINE, fp);
		sscanf(p, " %s %s", drzava, file);
		P_Drzava Nova_Drzava = malloc(sizeof(Drzava));
		Nova_Drzava->root = NULL;
		Nova_Drzava->next = NULL;
		if (Nova_Drzava == NULL) {
			printf("Memorija nije alocirana.");
			return NEUSPJESNO_IZVRSENO;
		}
		strcpy(Nova_Drzava->Naziv_Drzava, drzava);
		NovaDrzava(&file, Nova_Drzava, head);
	}

	fclose(fp);
}

int NovaDrzava(char* file, P_Drzava Nova_Drzava, P_Drzava Head) {


	if (Head->next == NULL) {
		Head->next = Nova_Drzava;
	}

	else {
		while (Head->next != NULL && strcmp(Head->next->Naziv_Drzava, Nova_Drzava->Naziv_Drzava) < 0) {
			Head = Head->next;
		}
		Nova_Drzava->next = Head->next;
		Head->next = Nova_Drzava;
	}

	CitaDatGradovi(Nova_Drzava, file);

	return USPJESNO_IZVRSENO;

}

int CitaDatGradovi(P_Drzava Nova_Drzava, char* file) {
	FILE* fp = NULL;
	char buffer[MAX_LINE] = { 0 };
	char* p = buffer;
	char naziv_grada[MAX_LINE] = { 0 };
	int broj_stanovnika = 0;

	fp = fopen(file, "r");
	if (fp == NULL) {
		printf("Dokument se nije otvorio.\n");
		return FILE_NOT_OPEN;
	}
	while (!feof(fp)) {
		fgets(buffer, MAX_LINE, fp);
		sscanf(p, " %s %d", naziv_grada, &broj_stanovnika);
		NoviGrad(&naziv_grada, broj_stanovnika, Nova_Drzava);
	}

	return USPJESNO_IZVRSENO;

}

int NoviGrad(char* Naziv_Grada, int broj_stanovnika, P_Drzava drzava) {
	P_Grad Novi_Grad = malloc(sizeof(Grad));
	Novi_Grad->L = NULL;
	Novi_Grad->D = NULL;
	Novi_Grad->broj_stanovnika = broj_stanovnika;
	if (Novi_Grad == NULL) {
		printf("Memorija nije alocirana.");
		return NEUSPJESNO_IZVRSENO;
	}
	strcpy(Novi_Grad->Naziv_Grad, Naziv_Grada);

	UnosGrada(&(drzava->root), Novi_Grad);

	return USPJESNO_IZVRSENO;
}

int UnosGrada(P_Grad* root, P_Grad Novi_Grad) {
	if (*root == NULL)
		*root = Novi_Grad;
	else if (Novi_Grad->broj_stanovnika > (*root)->broj_stanovnika)
		UnosGrada(&(*root)->L, Novi_Grad);
	else if (Novi_Grad->broj_stanovnika < (*root)->broj_stanovnika)
		UnosGrada(&(*root)->D, Novi_Grad);
	else {
		if (strcmp(Novi_Grad->Naziv_Grad, (*root)->Naziv_Grad) > 0)
			UnosGrada(&(*root)->L, Novi_Grad);
		else if (strcmp(Novi_Grad->Naziv_Grad, (*root)->Naziv_Grad) < 0)
			UnosGrada(&(*root)->D, Novi_Grad);
	}

	return USPJESNO_IZVRSENO;
}


int PrintAll(P_Drzava head) {
	while (head != NULL) {
		if (head->root != NULL) {
			printf("Gradovi iz %s: ", head->Naziv_Drzava);
			ispisStabla(head->root);
			printf("\n");
		}

		head = head->next;
	}
	return USPJESNO_IZVRSENO;

}
int ispisStabla(P_Grad root) {
	if (root->L != NULL) {
		ispisStabla(root->L);
	}
	printf("%s ", root->Naziv_Grad);
	while (root->D != NULL) {
		printf("%s ", root->D);
		root = root->D;
	}
	return USPJESNO_IZVRSENO;
}

int PrintSome(P_Drzava head_drzava, char* naziv_drzave, int broj_stanovnika) {
	while (strcmp(head_drzava->Naziv_Drzava, naziv_drzave) != 0 && head_drzava != NULL) {
		head_drzava = head_drzava->next;
	}
	if (head_drzava == NULL) {
		printf("Nema te drzave.");
	}
	else {
		printf("Gradovi iz %s: sa vise od %d stanovnika: ", head_drzava->Naziv_Drzava, broj_stanovnika);
		FiltriraniIspisStabla(head_drzava->root, broj_stanovnika);
		printf("\n");
	}
	return USPJESNO_IZVRSENO;

}
int FiltriraniIspisStabla(P_Grad root, int broj_stanovnika) {

	if (root->broj_stanovnika >= broj_stanovnika) {
		printf("%s ", root);
		if (root->L != NULL) {
			ispisStabla(root->L);
		}
		if (root->D != NULL) {
			FiltriraniIspisStabla(root->D, broj_stanovnika);
		}

	}
	else {
		if (root->L != NULL) {
			FiltriraniIspisStabla(root->L, broj_stanovnika);
		}

	}

	return USPJESNO_IZVRSENO;

}