/// 2 stelute inseamna ca schimb informatii in lista (adaug/sterg)
void readTeams(FILE *fisier, int nr, echipa **head);
    citesc si aloc echipele, la fel si jucatorii

void eliminaSpatiileGoale(char *s);
    pt ca la finalul unui string se salveaza cu '\r' sau exista spatii le elimin

void mediePunctaj(echipa *team);
    fac un punctaj per echipa cum o citesc

void afisare1(FILE *output, echipa *head);
    doar afisez

void putere2(int *nr);
    vad care e puterea lui 2 mai mica

void eliminare(echipa **head);
    elimin din lista echipele in plus

void deallocTeam(echipa *head);
void deallocPlayer(Player *head);
    normal ca trebuie si sa dau free la memoria alocata

void afisare2(FILE *output, echipa *head);
    afisez cerinta 2

void queue(coada *headCoada, echipa *head); 
    creez o coada sub forma unui fel de arbore astfel incat in dreapta si stanga sunt echipele per meci

void realizareMeci(coada *headCoada, stiva **castigator, stiva **pierzator);
    meciul se realizeaza si intra in stiva echipele conform

void recalculare(echipa *head);
    la castigare se creste cu un pct, deci treb recalculat

void push_stiva(stiva **stack, echipa *head);
    adaug cate o echipa in stiva

void pop_stiva(stiva **stack);
    iar la final treb eliberata toata stiva

void repopulareQueue(coada *headCoada, stiva *head);
    aici iau ce se afla in stiva de invingatori si trec in coada noua

void deallocCoada(coada *headCoada);
    in urma unei runde exista echipe alocate in coada care dispar deci free

void afisare3(coada *headCoada, int k, FILE *output, int copie);
    afisarea rundelor

echipa *copieProfunda(echipa *head);
Player *copieProfundaJucator(Player *head);
    pt ca sterg din memorie echipele din coada ele treb copiate profund

void afisareWinner(stiva *head, FILE *output, int k);
    e nevoie de afisarea castigatorilor mereu

void top8(coada *head, echipa **team);
    aici salvez in lista top 8

void aranjareBST(echipa *top, arbore **root);
    creez un arbore BST si il populez

void afisareTOP(arbore *root, FILE *output);
    il afisez

arbore *creezNodArbore(echipa *head);
arbore *adaugNodArbore(arbore *root, echipa *head);
    practic aici il aranjez unde sa stea in BST

arboreAVL *nouNodAVL(echipa *head);
    arbore AVL

int inaltimeAVL(arboreAVL *root);
    aici intorc inaltimea arborelui

int maxAVL(int a, int b);
    calculez max ca sa adaug la inaltimea recalculata

arboreAVL *adaugNodAVL(arboreAVL *root, echipa *head);
    aranjarea in cadrul AVL

arboreAVL *rotatieDreapta(arboreAVL *y);
arboreAVL *rotatieStanga(arboreAVL *x);
    rotirile necesare astfel incat sa fie mereu AVL

void aranjareAVL(echipa *top, arboreAVL **root);
    adaug fiecare echipa pe rand in AVL

void afisareAVL(arboreAVL *root, FILE *output);

void freeAVL(arboreAVL *root);
void freeBST(arbore *root);
    e important sa nu ramana alocata memoria folosita