#include "lanPartyLib.h"

void readTeams(FILE *fisier, int nr, echipa **head)
{
    for (int i = 0; i < nr; i++)
    {
        echipa *newNode = malloc(sizeof(echipa)); /// alocare de memorie
        newNode->urm = *head;                     /// noul nod este in fata head

        fscanf(fisier, "%d", &newNode->nrJ); /// nr jucatori
        fgetc(fisier);
        char numeEchipa[256] = {}; /// buffer pt numele echipei

        fscanf(fisier, "%[^\n]s", numeEchipa); /// nume echipa

        newNode->numeEchipa = malloc((strlen(numeEchipa) + 1) * sizeof(char));
        strcpy(newNode->numeEchipa, numeEchipa);
        /// este necesara alocarea si dupa copierea
        eliminaSpatiileGoale(newNode->numeEchipa);

        newNode->jucator = NULL;               /// fiind primul jucator el trebuie sa fie NULL
        for (int j = 0; j < newNode->nrJ; j++) /// citesc jucatorii
        {
            Player *nouNod = malloc(sizeof(Player)); /// aloc urmatorul jucator

            char firstName[30]; /// citesc numele
            fscanf(fisier, "%s", firstName);
            nouNod->firstName = malloc((strlen(firstName) + 1) * sizeof(char));
            strcpy(nouNod->firstName, firstName);

            char secondName[30]; /// citesc prenumele
            fscanf(fisier, "%s", secondName);
            nouNod->secondName = malloc((strlen(secondName) + 1) * sizeof(char));
            strcpy(nouNod->secondName, secondName);

            fscanf(fisier, "%d", &nouNod->points); /// citesc exp lui
            nouNod->next = NULL;                   /// momentan urmatorul jucator nu exista

            if (newNode->jucator == NULL) /// daca e primul din echipa
                newNode->jucator = nouNod;
            else /// altfel il adaug la capat;
            {
                Player *curent = newNode->jucator;
                while (curent->next)
                    curent = curent->next;

                curent->next = nouNod;
            }
        }

        mediePunctaj(newNode); /// fac o medie a pct
        *head = newNode;       /// noul nod devine primul
    }
}

void eliminaSpatiileGoale(char *s)
{
    int lungime = strlen(s);

    while (lungime > 0 && s[lungime - 2] == ' ')
    {
        lungime = lungime - 2;
        s[lungime] = '\0'; // Înlocuiește ultimul spațiu gol cu terminatorul de șir
    }
}

void mediePunctaj(echipa *team)
{
    team->medie = 0; /// e important sa incep cu media 0

    Player *coada = team->jucator;

    while (coada) /// adaug mai intai punctele
    {
        team->medie = team->medie + coada->points;
        coada = coada->next;
    }

    team->medie = (team->medie) / (team->nrJ); /// termin media
}

void afisare1(FILE *output, echipa *head)
{
    echipa *afis = head;
    while (afis) /// cat timp exista o continuare
    {
        // romanian si dont ball...

        fprintf(output, "%s\n", afis->numeEchipa); /// afisez nume de echipe
        /*
        fprintf(output, "%d\n", afis->nrJ);
        Player *curent = afis->jucator;
        for (int i = 0; i < afis->nrJ; i++)
        {
            fprintf(output, "%s %s ", curent->firstName, curent->secondName);
            fprintf(output, "%d\n", curent->points);
            curent = curent->next;
        }
        fprintf(output, "\n");
        */

        afis = afis->urm; /// trec la urmatorul
    }
}

void putere2(int *nr)
{
    int k = 1;
    while (k * 2 <= (*nr)) /// practic cat timp 2*k e mai mic k creste
        k = k * 2;
    *nr = k; /// cand while ul se opreste am ajuns la cea mai mare putere
}

void eliminare(echipa **head)
{
    // fprintf(stdout, ".\n");
    echipa *curent = *head;
    float minim = 100; /// va fi mereu media minima
    while (curent)
    {
        if (curent->medie < minim) /// parcurg lista si compar media
        {
            minim = curent->medie;
        }
        curent = curent->urm;
    }

    echipa *nodElim = *head; /// parcurg lista
    echipa *nodPrev = NULL;

    while (nodElim != NULL && nodElim->medie != minim)
    {
        nodPrev = nodElim;
        nodElim = nodElim->urm;
    }

    if (nodElim == NULL)
        return; /// conditie fata de care nu merge problema chiar daca nu ajung la null niciodata

    if (nodPrev == NULL) /// daca primul nod e si cel minim
    {
        *head = nodElim->urm;
    }
    else
        nodPrev->urm = nodElim->urm; /// daca e prin lista il scot

    deallocTeam(nodElim); /// free
}

void deallocTeam(echipa *head)
{
    deallocPlayer(head->jucator);
    free(head->numeEchipa);
    free(head);
}

void deallocPlayer(Player *head)
{
    if (head->next)
        deallocPlayer(head->next);
    /// fac o parcurgere backtracking si dupa dau free de la coada la cap

    free(head->firstName);
    free(head->secondName);
    free(head);
}

void afisare2(FILE *output, echipa *head) /// afisare subpunct 2
{
    echipa *team = head;
    while (team)
    {
        fprintf(output, "%s\n", team->numeEchipa);
        team = team->urm;
    }
}

void queue(coada *headCoada, echipa *head) /// creez coada
{

    echipa *curent = head; /// parcurg coada
    while (curent != NULL)
    {
        echipa *left = curent;
        curent = curent->urm;

        echipa *right = curent;
        curent = curent->urm;

        /// noul nod in coada
        coadaMeci *nouMeci = malloc(sizeof(coadaMeci));

        /// copiez profund echipa
        nouMeci->stanga = copieProfunda(left);
        nouMeci->dreapta = copieProfunda(right);
        nouMeci->next = NULL; /// pregatesc urmatorul meci

        if (headCoada->primul == NULL) /// daca e primul nod
            headCoada->primul = headCoada->ultimul = nouMeci;
        else
        {
            headCoada->ultimul->next = nouMeci; /// adaug nodul in coada
            headCoada->ultimul = nouMeci;       /// il salvez ca ultimul nod din coada
        }
    }
}

void realizareMeci(coada *headCoada, stiva **castigator, stiva **pierzator)
{
    coadaMeci *meci = headCoada->primul; /// pornesc meciurile din runda
    while (meci != NULL)
    {
        if (meci->dreapta->medie >= meci->stanga->medie) /// daca a 2 echipa e mai tare decat prima
        {
            echipa *team = meci->dreapta;
            recalculare(team);            /// cresc cu 1 valoarea
            push_stiva(castigator, team); /// se adauga echipele conform performantelor
            push_stiva(pierzator, meci->stanga);
        }
        else /// aici se aplica cazul de <
        {
            echipa *team = meci->stanga;
            recalculare(team);
            push_stiva(castigator, team);
            push_stiva(pierzator, meci->dreapta);
        }
        meci = meci->next; /// trec la urmatorul meci
    }
}

void recalculare(echipa *head)
{
    Player *curent = head->jucator; /// cresc la fiecare jucator cu 1 pct
    while (curent != NULL)
    {
        curent->points++;
        curent = curent->next;
    }
    mediePunctaj(head); // se face media
}

void push_stiva(stiva **stack, echipa *head)
{
    stiva *nodStiva = malloc(sizeof(stiva)); /// aloc
    echipa *team = copieProfunda(head);
    nodStiva->echipa_stiva = team; /// adaug echipa ca nod

    if (*stack == NULL)
    {
        nodStiva->next = NULL;
    }
    else
    {
        nodStiva->next = *stack; /// fiind stiva, se adauga noduri doar la inceput
    }

    *stack = nodStiva;
}

echipa *copieProfunda(echipa *head)
{
    echipa *team = malloc(sizeof(echipa));
    team->medie = head->medie;
    team->nrJ = head->nrJ;

    team->numeEchipa = malloc((strlen(head->numeEchipa)) * sizeof(char));
    strcpy(team->numeEchipa, head->numeEchipa);

    char *ptr = strchr(team->numeEchipa, '\r');

    // Dacă am găsit \r, înlocuim-l cu \0
    if (ptr != NULL)
    {
        *ptr = '\0';
    }
    // team->numeEchipa[strlen(team->numeEchipa) - 1] = '\0';

    team->jucator = copieProfundaJucator(head->jucator);
    return team;
}

Player *copieProfundaJucator(Player *head)
{
    if (head == NULL)
        return NULL;
    Player *copy = malloc(sizeof(Player));
    copy->points = head->points;

    copy->firstName = malloc((strlen(head->firstName)) * sizeof(char));
    copy->secondName = malloc((strlen(head->secondName)) * sizeof(char));
    strcpy(copy->firstName, head->firstName);
    strcpy(copy->secondName, head->secondName);

    copy->next = copieProfundaJucator(head->next);
    return copy;
}

void pop_stiva(stiva **stack)
{

    while (*stack != NULL)
    {
        stiva *curent = *stack; /// parcurg stiva

        *stack = curent->next;
        deallocTeam(curent->echipa_stiva); /// dau free
        free(curent);
    }
}

/// practic e la fel ca fct "queue" dar cu stiva
void repopulareQueue(coada *headCoada, stiva *head) /// creez coada
{

    stiva *curent = head; /// parcurg coada
    while (curent != NULL)
    {

        echipa *left = curent->echipa_stiva;
        curent = curent->next;

        echipa *right = curent->echipa_stiva;
        curent = curent->next;

        /// noul nod in coada
        coadaMeci *nouMeci = malloc(sizeof(coadaMeci));

        /// copiez profund echipa
        nouMeci->stanga = copieProfunda(left);
        nouMeci->dreapta = copieProfunda(right);
        nouMeci->next = NULL; /// pregatesc urmatorul meci

        if (headCoada->primul == NULL) /// daca e primul nod
            headCoada->primul = headCoada->ultimul = nouMeci;
        else
        {
            headCoada->ultimul->next = nouMeci; /// adaug nodul in coada
            headCoada->ultimul = nouMeci;       /// il salvez ca ultimul nod din coada
        }
    }
}

void afisare3(coada *headCoada, int k, FILE *output, int copie)
{
    fprintf(output, "\n--- ROUND NO:%d\n", k);
    coadaMeci *curent = headCoada->primul;

    char nume[50];
    int ec;
    if (copie == 1)
        ec = 1;
    else
        ec = copie / 2;

    for (int i = 0; i < ec && curent != NULL; i++)
    {
        strcpy(nume, curent->stanga->numeEchipa);
        fprintf(output, "%s", nume);

        for (int j = strlen(nume); j < 33; j++)
        {
            fprintf(output, " ");
        }
        fprintf(output, "-");
        for (int j = strlen(curent->dreapta->numeEchipa); j < 33; j++)
        {
            fprintf(output, " ");
        }
        strcpy(nume, curent->dreapta->numeEchipa);
        fprintf(output, "%s", nume);
        fprintf(output, "\n");
        curent = curent->next;
    }
}

void deallocCoada(coada *headCoada) /// e important sa dau free la coada cand o reiau ca ramane ocupata memoria
{
    coadaMeci *curent = headCoada->primul;
    coadaMeci *next;
    while (curent != NULL)
    {
        next = curent->next;
        if (curent->dreapta != NULL)
            deallocTeam(curent->dreapta);
        if (curent->stanga != NULL)
            deallocTeam(curent->stanga);

        free(curent);
        curent = next;
    }

    free(headCoada);
    headCoada = NULL;
}

void afisareWinner(stiva *head, FILE *output, int k)
{
    stiva *curent = head;
    fprintf(output, "\nWINNERS OF ROUND NO:%d\n", k);
    while (curent != NULL)
    {

        fprintf(output, "%s", curent->echipa_stiva->numeEchipa);
        for (int i = strlen(curent->echipa_stiva->numeEchipa); i < 34; i++)
        {
            fprintf(output, " ");
        }
        fprintf(output, "-  %.2f\n", curent->echipa_stiva->medie);
        curent = curent->next;
    }
}

void top8(coada *head, echipa **team)
{
    coadaMeci *parcurgere = head->primul;
    echipa *curent;

    for (int i = 1; i <= 4 && parcurgere != NULL; i++)
    {
        if (i == 1)
        {
            *team = copieProfunda(parcurgere->stanga);
            curent = *team;
            curent->urm = copieProfunda(parcurgere->dreapta);
            curent = curent->urm;
        }
        else
        {
            curent->urm = copieProfunda(parcurgere->stanga);
            curent = curent->urm;
            curent->urm = copieProfunda(parcurgere->dreapta);
            curent = curent->urm;
        }

        parcurgere = parcurgere->next;
    }
    curent->urm = NULL;
}

void aranjareBST(echipa *top, arbore **root)
{
    echipa *curent = top;
    *root = adaugNodArbore(*root, curent);
    curent = curent->urm;
    while (curent != NULL)
    {
        adaugNodArbore(*root, curent);
        curent = curent->urm;
    }
}

arbore *creezNodArbore(echipa *head)
{
    arbore *newNod = malloc(sizeof(arbore));
    newNod->stanga = newNod->dreapta = NULL;
    newNod->team = head;
    return newNod;
}

arbore *adaugNodArbore(arbore *root, echipa *head)
{
    if (root == NULL)
    {
        root = creezNodArbore(head);
        return root;
    }
    if (head->medie < root->team->medie)
    {
        root->stanga = adaugNodArbore(root->stanga, head);
    }
    if (head->medie > root->team->medie)
    {
        root->dreapta = adaugNodArbore(root->dreapta, head);
    }
    if (head->medie == root->team->medie)
    {
        if (strcmp(head->numeEchipa, root->team->numeEchipa) < 0) /// primul e mai mic
            root->stanga = adaugNodArbore(root->stanga, head);
        else
            root->dreapta = adaugNodArbore(root->dreapta, head);
    }
    return root;
}

void afisareTOP(arbore *root, FILE *output)
{
    if (root == NULL)
        return;
    afisareTOP(root->dreapta, output);
    fprintf(output, "%s", root->team->numeEchipa);
    for (int i = strlen(root->team->numeEchipa); i < 34; i++)
        fprintf(output, " ");
    fprintf(output, "-  %.2f\n", root->team->medie);
    afisareTOP(root->stanga, output);
}

arboreAVL *nouNodAVL(echipa *head)
{
    arboreAVL *nouNod = malloc(sizeof(arboreAVL));
    nouNod->team = head;
    nouNod->dreapta = nouNod->stanga = NULL;
    nouNod->inaltime = 1;
    return nouNod;
}

int inaltimeAVL(arboreAVL *root)
{
    if (root == NULL)
        return 0;
    else
        return root->inaltime;
}

int maxAVL(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

arboreAVL *adaugNodAVL(arboreAVL *root, echipa *head)
{
    if (root == NULL)
    {
        root = nouNodAVL(head);
        return root;
    }

    // adaugarea nodului în arbore
    if (head->medie < root->team->medie)
        root->stanga = adaugNodAVL(root->stanga, head);
    else if (head->medie > root->team->medie)
        root->dreapta = adaugNodAVL(root->dreapta, head);
    else
    {
        if (strcmp(head->numeEchipa, root->team->numeEchipa) < 0) /// primul e mai mic
            root->stanga = adaugNodAVL(root->stanga, head);
        else
            root->dreapta = adaugNodAVL(root->dreapta, head);
    }

    // se actualoizeaza inaltimea subarborelui
    root->inaltime = 1 + maxAVL(inaltimeAVL(root->stanga), inaltimeAVL(root->dreapta));

    // factor de echilibru
    int factorE = 0;
    if (root != 0)
        factorE = inaltimeAVL(root->stanga) - inaltimeAVL(root->dreapta);

    if (factorE > 1 && head->medie < root->stanga->team->medie)
        return rotatieDreapta(root);

    if (factorE < -1 && head->medie > root->dreapta->team->medie)
        return rotatieStanga(root);

    if (factorE > 1 && head->medie > root->stanga->team->medie)
    {
        root->stanga = rotatieStanga(root->stanga);
        return rotatieDreapta(root);
    }

    if (factorE < -1 && head->medie < root->dreapta->team->medie)
    {
        root->dreapta = rotatieDreapta(root->dreapta);
        return rotatieStanga(root);
    }

    return root;
}

arboreAVL *rotatieDreapta(arboreAVL *y)
{
    arboreAVL *x = y->stanga;
    arboreAVL *T2 = x->dreapta;

    x->dreapta = y;
    y->stanga = T2;

    y->inaltime = maxAVL(inaltimeAVL(y->stanga), inaltimeAVL(y->dreapta)) + 1;
    x->inaltime = maxAVL(inaltimeAVL(x->stanga), inaltimeAVL(x->dreapta)) + 1;

    return x;
}

arboreAVL *rotatieStanga(arboreAVL *x)
{
    arboreAVL *y = x->dreapta;
    arboreAVL *T2 = y->stanga;

    y->stanga = x;
    x->dreapta = T2;

    x->inaltime = maxAVL(inaltimeAVL(x->stanga), inaltimeAVL(x->dreapta)) + 1;
    y->inaltime = maxAVL(inaltimeAVL(y->stanga), inaltimeAVL(y->dreapta)) + 1;

    return y;
}

void aranjareAVL(echipa *top, arboreAVL **root)
{
    echipa *curent = top;
    *root = adaugNodAVL(*root, curent);
    curent = curent->urm;
    while (curent != NULL)
    {
        *root = adaugNodAVL(*root, curent);
        curent = curent->urm;
    }
}

void afisareAVL(arboreAVL *root, FILE *output)
{
    if (root == NULL)
        return;

    //  fprintf(output, "%s %d\n", root->team->numeEchipa,root->inaltime);

    afisareAVL(root->dreapta, output);

    if (root->inaltime == 1)
    {
        fprintf(output, "%s\n", root->team->numeEchipa);
    }
    afisareAVL(root->stanga, output);
}

void freeAVL(arboreAVL *root)
{
    if (root == NULL)
        return;

    freeAVL(root->stanga);
    freeAVL(root->dreapta);
    free(root);
}

void freeBST(arbore *root)
{
    if (root == NULL)
        return;

    freeBST(root->stanga);
    freeBST(root->dreapta);
    free(root);
}

