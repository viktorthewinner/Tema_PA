#include <stdio.h>
#include <string.h>
#include "lanPartyLib.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	printf("Number of input files: %d\n", argc);
	printf("Exec name: %s\n", argv[0]);

	// argv[1] = "./date/t15/c.in";
	// argv[2] = "./date/t15/d.in";
	// argv[3] = "./out/out_test.out";
	printf("File1: %s\n", argv[1]); /// c.in
	printf("File2: %s\n", argv[2]); /// d.in
	printf("File3: %s\n", argv[3]); /// r.out

	// FILE *f_in = fopen( argv[1], "r" );

	echipa *head = NULL;
	int nr, v[10];
	FILE *fisier, *output, *cerinta;

	if ((cerinta = fopen(argv[1], "r")) == NULL) /// fisierul c.in
	{
		exit(1);
	}
	if ((fisier = fopen(argv[2], "r")) == NULL) /// fisierul d.in
	{
		exit(1);
	}
	if ((output = fopen(argv[3], "w")) == NULL) /// fisierul r.out
	{
		exit(1);
	}

	fscanf(fisier, "%d", &nr); /// citesc nr de echipe din d.in
	for (int i = 0; i < 5; i++)
	{ /// are rol sa citesc cele 5 cerinte din c.in
		fscanf(cerinta, "%d", &v[i]);
	}

	readTeams(fisier, nr, &head);

	if (v[0] == 1 && v[1] == 0)
	{
		afisare1(output, head);
	}

	int copie = nr;
	putere2(&copie); /// aflu maxim putere 2

	for (int i = copie; i < nr; i++) /// elimin
		eliminare(&head);

	if (v[1] == 1)
	{
		afisare2(output, head);
	}

	echipa *top=NULL;
	if (v[2] == 1)
	{
		coada *headCoada = malloc(sizeof(coada)); /// coada de meciuri
		headCoada->primul = headCoada->ultimul = NULL;

		queue(headCoada, head);
		int k = 1;

		while (copie > 8)
		{

			stiva *castigator = NULL, *pierzator = NULL;
			realizareMeci(headCoada, &castigator, &pierzator);

			pop_stiva(&pierzator);

			afisare3(headCoada, k, output, copie);

			afisareWinner(castigator, output, k);
			deallocCoada(headCoada);
			headCoada = malloc(sizeof(coada)); /// coada de meciuri
			headCoada->primul = NULL;
			headCoada->ultimul = NULL;

			repopulareQueue(headCoada, castigator);
			pop_stiva(&castigator);
			copie /= 2;
			k++;
		}
		if (copie == 8)
		{
			top8(headCoada, &top);
		}
		while (copie > 1)
		{

			stiva *castigator = NULL, *pierzator = NULL;
			realizareMeci(headCoada, &castigator, &pierzator);

			pop_stiva(&pierzator);

			afisare3(headCoada, k, output, copie);

			afisareWinner(castigator, output, k);
			deallocCoada(headCoada);
			headCoada = malloc(sizeof(coada)); /// coada de meciuri
			headCoada->primul = NULL;
			headCoada->ultimul = NULL;

			if (copie > 2)
				repopulareQueue(headCoada, castigator);

			pop_stiva(&castigator);

			copie /= 2;
			k++;
		}
	}

	arbore *root = NULL;
	if (v[3] == 1)
	{
		aranjareBST(top, &root);

		fprintf(output, "\nTOP 8 TEAMS:\n");
		afisareTOP(root, output);
	}

	arboreAVL *avl = NULL;
	if (v[4] == 1)
	{
		aranjareAVL(top, &avl);

		fprintf(output, "\nTHE LEVEL 2 TEAMS ARE: \n");
		afisareAVL(avl, output);
	}

	while (top != NULL)
	{
		echipa *parc = top;
		parc = top->urm;
		deallocTeam(top);
		top=parc;
	}
	while (head != NULL)
	{
		echipa *parc = head;
		parc = head->urm;
		deallocTeam(head);
		head=parc;
	}

	freeBST(root);
	freeAVL(avl);
	return 0;
}