#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Player
{
    char *firstName;
    char *secondName;
    int points;
    struct Player *next;
} Player;

typedef struct echipa
{
    float medie;

    int nrJ;
    char *numeEchipa;
    Player *jucator;
    struct echipa *urm;
} echipa;

typedef struct coadaMeci
{
    echipa *stanga;
    echipa *dreapta;
    struct coadaMeci *next;
} coadaMeci;

typedef struct coada /// queue
{
    struct coadaMeci *primul, *ultimul;
} coada;

typedef struct stiva
{
    echipa *echipa_stiva;
    struct stiva *next;
} stiva;

typedef struct arbore
{
    struct arbore *stanga;
    struct arbore *dreapta;
    echipa *team;
} arbore;

// Structura unui nod AVL
typedef struct arboreAVL
{
    echipa *team;
    struct arboreAVL *stanga;
    struct arboreAVL *dreapta;
    int inaltime;
} arboreAVL;

/// 2 stelute inseamna ca schimb informatii in lista (adaug/sterg)
void readTeams(FILE *fisier, int nr, echipa **head);
void eliminaSpatiileGoale(char *s);
void mediePunctaj(echipa *team);
void afisare1(FILE *output, echipa *head);

void putere2(int *nr);
void eliminare(echipa **head);
void deallocTeam(echipa *head);
void deallocPlayer(Player *head);
void afisare2(FILE *output, echipa *head);

void queue(coada *headCoada, echipa *head);
void realizareMeci(coada *headCoada, stiva **castigator, stiva **pierzator);
void recalculare(echipa *head);
void push_stiva(stiva **stack, echipa *head);
void pop_stiva(stiva **stack);
void repopulareQueue(coada *headCoada, stiva *head);
void deallocCoada(coada *headCoada);
void afisare3(coada *headCoada, int k, FILE *output, int copie);
echipa *copieProfunda(echipa *head);
Player *copieProfundaJucator(Player *head);
void afisareWinner(stiva *head, FILE *output, int k);

void top8(coada *head, echipa **team);
void aranjareBST(echipa *top, arbore **root);
void afisareTOP(arbore *root, FILE *output);
arbore *creezNodArbore(echipa *head);
arbore *adaugNodArbore(arbore *root, echipa *head);

arboreAVL *nouNodAVL(echipa *head);
int inaltimeAVL(arboreAVL *root);
int maxAVL(int a, int b);
arboreAVL *adaugNodAVL(arboreAVL *root, echipa *head);
arboreAVL *rotatieDreapta(arboreAVL *y);
arboreAVL *rotatieStanga(arboreAVL *x);
void aranjareAVL(echipa *top, arboreAVL **root);
void afisareAVL(arboreAVL *root, FILE *output);

void freeAVL(arboreAVL *root);
void freeBST(arbore *root);