#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

typedef struct process
{
    int DA;
    int TE;
    int treponse;
    int temfin;
    int tattant;
    int pol;
    int pr;
    int reste;
    int completed;
    int NP;

} process;

typedef struct Node {
    process data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

// Function to initialize a queue
void initializeQueue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
}

// Function to check if the queue is empty
int isEmpty(Queue* q) {
    return q->front == NULL;
}

// Function to enqueue an item at the end of the queue
void enqueue(Queue* q, process item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->data = item;
    newNode->next = NULL;
    newNode->prev = q->rear;
    
    if (isEmpty(q)) {
        q->front = newNode;
    } else {
        q->rear->next = newNode;
    }
    q->rear = newNode;
}

// Function to dequeue an item from the front of the queue
process dequeue(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty.\n");
        exit(1);
    }
    Node* temp = q->front;
    process item = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    } else {
        q->front->prev = NULL;
    }
    free(temp);
    return item;
}

void Creer_tableau(int nb_processus, process **tableau)
{
    *tableau = (process *)malloc(sizeof(process) * nb_processus);
    for (int i = 0; i < nb_processus; i++)
    {

        do
        {
            printf("Entrez la date d'arrivee du processus %d : ", i + 1);
            scanf("%d", &((*tableau)[i].DA));
        } while ((*tableau)[i].DA < 0);

        do
        {
            printf("Entrez le temps d'execution du processus %d : ", i + 1);
            scanf("%d", &((*tableau)[i].TE));

        } while ((*tableau)[i].TE < 1);

        do
        {
            printf("Entrez la priorite du processus %d : ", i + 1);
            scanf("%d", &((*tableau)[i].pr));

        } while ((*tableau)[i].pr < 0);

        (*tableau)[i].NP = i + 1;
        (*tableau)[i].reste = (*tableau)[i].TE;
    }
}

process *Trier_tableau_DA(int nb_processus, process *tableau)
{
    for (int i = 0; i < nb_processus - 1; i++)
    {
        int min_index = i;

        for (int j = i + 1; j < nb_processus; j++)
        {
            if (tableau[j].DA < tableau[min_index].DA)
            {
                min_index = j;
            }
        }
        process temp = tableau[i];
        tableau[i] = tableau[min_index];
        tableau[min_index] = temp;
    }
    return tableau;
}

void Execution(int nb_processus, process *tableau)
{
    int current_time = 0;
    process *Sorted_DA = Trier_tableau_DA(nb_processus, tableau);

    printf("Execution:\n");
    printf("Time\t| Process\t| Remaining\t| Completed\n");
    printf("------------------------------------------------\n");

    int completed_processes = 0;

    while (completed_processes < nb_processus)
    {
        int current_process_index = Check_tableau(current_time, nb_processus, Sorted_DA);
        if (Sorted_DA[current_process_index].reste == 0)
        {
            printf("%d\t| NONE\t\t| NONE\t\t| NONE\n", current_time);
            current_time++;
            sleep(1);
            continue;
        }

        Sorted_DA[current_process_index].reste--;
        Sorted_DA[current_process_index].completed = Sorted_DA[current_process_index].TE - Sorted_DA[current_process_index].reste;
        printf("%d\t| %d\t\t| %d\t\t| %d\n", current_time, Sorted_DA[current_process_index].NP, Sorted_DA[current_process_index].reste, Sorted_DA[current_process_index].completed);

        if (Sorted_DA[current_process_index].reste == 0)
            completed_processes++;
        Sorted_DA[current_process_index].temfin = current_time + 1;
        Sorted_DA[current_process_index].treponse = Sorted_DA[current_process_index].temfin - Sorted_DA[current_process_index].DA;
        Sorted_DA[current_process_index].tattant = Sorted_DA[current_process_index].treponse - Sorted_DA[current_process_index].TE;

        current_time++;
        sleep(1);
    }
}

int Check_tableau(int current_time, int nb_processus, process *tableau)
{
    int min_index = -1;
    int min_pr = INT_MAX;

    for (int i = 0; i < nb_processus; i++)
    {
        if (tableau[i].DA <= current_time && tableau[i].reste > 0)
        {
            if (tableau[i].pr < min_pr)
            {
                min_index = i;
                min_pr = tableau[i].pr;
            }
        }
    }

    return min_index;
}

void Afficher_ordonnancement(int nb_processus, process *tableau)
{
    printf("Ordonnancement des processus :\n");
    printf("|Processus\t|Date d'arrivee\t|Temps d'execution\t|Priority\t|Temps d'attente\t|Temps de reponse \n");
    printf("--------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < nb_processus; i++)
    {
        printf("|%d\t\t|%d\t\t|%d\t\t\t|%d\t\t|%d\t\t\t|%d\n", tableau[i].NP, tableau[i].DA, tableau[i].TE, tableau[i].pr, tableau[i].tattant, tableau[i].treponse);
    }
}

void temps_moyen(int nb_processus, process *tableau)
{
    int somme_temps_reponse = 0;
    int somme_temps_attant = 0;
    for (int i = 0; i < nb_processus; i++)
    {
        somme_temps_reponse += tableau[i].treponse;
        somme_temps_attant += tableau[i].tattant;
    }
    float temps_moyen_reponse = (float)somme_temps_reponse / nb_processus;
    float temps_moyen_attant = (float)somme_temps_attant / nb_processus;

    printf("Temps moyen de reponse pour tous les processus : %.2f\n", temps_moyen_reponse);
    printf("Temps moyen d'attante pour tous les processus : %.2f\n", temps_moyen_attant);
}

int main()
{
    int nb_processus;
    process *tableau;
    int i = 0;

    do
    {
        if (i == 0)
        {
            printf("entrez le nombre de processus: ");
            scanf("%d", &nb_processus);
        }
        
        else
        {
            printf("Error nb processus sera etre > 0 \n");
            printf("entrez le nombre de processus:");
            scanf("%d", &nb_processus);
        }
        i++ ;
    } while (nb_processus <= 0);

    Creer_tableau(nb_processus, &tableau);
    Execution(nb_processus, tableau);
    sleep(1);
    Afficher_ordonnancement(nb_processus, tableau);
    sleep(1);
    temps_moyen(nb_processus, tableau);
    return 0;
}