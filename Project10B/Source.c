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
typedef struct Grad {
	char Naziv_Grad[MAX_LINE];
	int broj_stanovnika;
	P_Grad next;
}Grad;

typedef struct Drzava* P_Drzava;
typedef struct Drzava {
	char Naziv_Drzava[MAX_LINE];
	P_Drzava L;
	P_Drzava D;
	P_Grad head;
}Drzava;

P_Drzava CitaDatDrzave(P_Drzava);
int NovaDrzava(P_Drzava, P_Drzava);
int CitaDatGradovi(P_Drzava, char*);
int NoviGrad(char*, int, P_Drzava);
int UnosGrada(P_Grad, P_Grad);
int PrintAll(P_Drzava);
int PrintSome(P_Drzava, char*, int);
int ispisListe(P_Drzava);
int FiltriraniIspisListe(P_Grad, int);
int UsporedbaGradova(P_Grad, P_Grad);



int main(void) {

	P_Drzava root = NULL;

	printf("Uspjesno uneseno. \n");
	root = CitaDatDrzave(root);

	char drzava[MAX_LINE] = { 0 };
	int broj_st = 0;
	PrintAll(root);

	printf("Unesi drzavu pa minimalan broj stanovnika: ");
	scanf(" %s %d", drzava, &broj_st);

	PrintSome(root, &drzava, broj_st);

	return 0;
}

P_Drzava CitaDatDrzave(P_Drzava root) {
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
		Nova_Drzava->L = NULL;
		Nova_Drzava->D = NULL;
		Nova_Drzava->head = malloc(sizeof(Grad));
		Nova_Drzava->head->broj_stanovnika = 0;
		strcpy(Nova_Drzava->head->Naziv_Grad, "head");
		Nova_Drzava->head->next = NULL;



		if (Nova_Drzava == NULL) {
			printf("Memorija nije alocirana.");
			return NEUSPJESNO_IZVRSENO;
		}
		strcpy(Nova_Drzava->Naziv_Drzava, drzava);
		NovaDrzava(Nova_Drzava, &root);
		CitaDatGradovi(Nova_Drzava, &file);
	}

	fclose(fp);

	return root;
}

int NovaDrzava(P_Drzava Nova_Drzava, P_Drzava* root) {


	if ((*root) == NULL)
		*root = Nova_Drzava;
	else if (strcmp(Nova_Drzava->Naziv_Drzava, (*root)->Naziv_Drzava) < 0)
		NovaDrzava(Nova_Drzava, &(*root)->L);
	else if (strcmp(Nova_Drzava->Naziv_Drzava, (*root)->Naziv_Drzava) > 0)
		NovaDrzava(Nova_Drzava, &(*root)->D);


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
	Novi_Grad->next = NULL;
	Novi_Grad->broj_stanovnika = broj_stanovnika;
	if (Novi_Grad == NULL) {
		printf("Memorija nije alocirana.");
		return NEUSPJESNO_IZVRSENO;
	}
	strcpy(Novi_Grad->Naziv_Grad, Naziv_Grada);

	UnosGrada(&(drzava->head), Novi_Grad);

	return USPJESNO_IZVRSENO;
}

int UnosGrada(P_Grad* head, P_Grad Novi_Grad) {

	P_Grad temp = (*head);
	if ((*head)->next == NULL) {
		(*head)->next = Novi_Grad;
	}

	else {
		while ((*head)->next != NULL && UsporedbaGradova((*head)->next, Novi_Grad)) {
			(*head) = (*head)->next;
		}
		Novi_Grad->next = (*head)->next;
		(*head)->next = Novi_Grad;

	}
	*head = temp;


	return USPJESNO_IZVRSENO;
}

int UsporedbaGradova(P_Grad A, P_Grad B) {
	if (A->broj_stanovnika > B->broj_stanovnika) {
		return 1;
	}
	else if (A->broj_stanovnika < B->broj_stanovnika) {
		return 0;
	}
	else {
		if (strcmp(A->Naziv_Grad, B->Naziv_Grad) < 0) {
			return 0;
		}
		if (strcmp(A->Naziv_Grad, B->Naziv_Grad) > 0) {
			return 1;
		}

	}

}


int PrintAll(P_Drzava root) {
	if (root->L != NULL) {
		ispisListe(root->L);
	}
	ispisListe(root);
	while (root->D != NULL) {
		PrintAll(root->D);
		root = root->D;
	}
	printf("\n");
	return USPJESNO_IZVRSENO;

}
int ispisListe(P_Drzava drzava) {


	P_Grad temp = drzava->head;
	if (temp->next->broj_stanovnika == 0) {
		printf("\n %s nema gradova. ", drzava->Naziv_Drzava);
	}
	else {
		temp = temp->next;
		printf("\n%s: ", drzava->Naziv_Drzava);
		while (temp != NULL) {
			printf("%s ", temp->Naziv_Grad);
			temp = temp->next;
		}
	}
	return USPJESNO_IZVRSENO;
}

P_Drzava trazi_drzavu(P_Drzava root_drzava, char* naziv_drzave) {
	if (strcmp(root_drzava->Naziv_Drzava, naziv_drzave) == 0) {
		return root_drzava;
	}

	else if (strcmp(root_drzava->Naziv_Drzava, naziv_drzave) > 0) {
		trazi_drzavu(root_drzava->L, naziv_drzave);
	}
	else if (strcmp(root_drzava->Naziv_Drzava, naziv_drzave) < 0) {
		trazi_drzavu(root_drzava->D, naziv_drzave);
	}
}

int PrintSome(P_Drzava root_drzava, char* naziv_drzave, int broj_stanovnika) {

	P_Drzava Trazena = trazi_drzavu(root_drzava, naziv_drzave);

	printf("\nGradovi iz %s sa vise od %d stanovnika: ", Trazena->Naziv_Drzava, broj_stanovnika);
	FiltriraniIspisListe(Trazena->head, broj_stanovnika);

	return USPJESNO_IZVRSENO;

}
int FiltriraniIspisListe(P_Grad head, int broj_stanovnika) {
	P_Grad temp = head->next;
	while ((temp->broj_stanovnika > broj_stanovnika) && temp != NULL) {
		printf(" %s", temp->Naziv_Grad);
		temp = temp->next;
	}
	return USPJESNO_IZVRSENO;
}