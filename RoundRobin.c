#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

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
    int index;
} process;

typedef struct Node
{
    process data;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct
{
    Node *front;
    Node *rear;
} Queue;

void initializeQueue(Queue *q)
{
    q->front = NULL;
    q->rear = NULL;
}

int isEmpty(Queue *q)
{
    if (q->front == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void enqueue(Queue *q, process item)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    newNode->data = item;
    newNode->next = NULL;
    newNode->prev = q->rear;

    if (isEmpty(q) == 1)
    {
        q->front = newNode;
    }
    else
    {
        q->rear->next = newNode;
    }
    q->rear = newNode;
}

void enqueueFront(Queue *q, process item)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    newNode->data = item;
    newNode->next = q->front;
    newNode->prev = NULL;

    if (isEmpty(q) == 1)
    {
        q->rear = newNode;
    }
    else
    {
        q->front->prev = newNode;
    }

    q->front = newNode;
}

process dequeue(Queue *q)
{
    if (isEmpty(q) == 1)
    {
        printf("Queue is empty.\n");
        process dummy;

        dummy.DA = -1;
        return dummy;
    }
    Node *temp = q->front;
    process item = temp->data;
    q->front = q->front->next;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    else
    {
        q->front->prev = NULL;
    }
    free(temp);

    return item;
}

void createProcessArray(int nb_processus, process **tableau)
{
    *tableau = (process *)malloc(sizeof(process) * nb_processus);
    if (*tableau == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < nb_processus; i++)
    {
        do
        {
            printf("Entrez la date d'arrivee du processus %d : ", i + 1);
            if (scanf("%d", &((*tableau)[i].DA)) != 1 || (*tableau)[i].DA < 0)
            {
                printf("Veuillez entrer une valeur entière positive pour la date d'arrivee.\n");
                while (getchar() != '\n')
                    ; // Clear input buffer
            }
        } while ((*tableau)[i].DA < 0);

        do
        {
            printf("Entrez le temps d'execution du processus %d (entre 1 et 10 inclusivement) : ", i + 1);
            if (scanf("%d", &((*tableau)[i].TE)) != 1 || (*tableau)[i].TE < 1 || (*tableau)[i].TE > 10)
            {
                printf("Veuillez entrer une valeur entière entre 1 et 10 pour le temps d'execution.\n");
                while (getchar() != '\n')
                    ; // Clear input buffer
            }
        } while ((*tableau)[i].TE < 1 || (*tableau)[i].TE > 10);

        do
        {
            printf("Entrez la priorite du processus %d (entre 0 et 10 inclusivement) : ", i + 1);
            if (scanf("%d", &((*tableau)[i].pr)) != 1 || (*tableau)[i].pr < 0 || (*tableau)[i].pr > 10)
            {
                printf("Veuillez entrer une valeur entière entre 0 et 10 pour la priorite.\n");
                while (getchar() != '\n')
                    ; // Clear input buffer
            }
        } while ((*tableau)[i].pr < 0 || (*tableau)[i].pr > 10);

        (*tableau)[i].NP = i + 1;
        (*tableau)[i].reste = (*tableau)[i].TE;
    }
}

// Function to sort the processes by arrival time
process *sortProcessesByArrival(int nb_processus, process *tableau)
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

// Function to execute processes with round-robin scheduling

// Function to sort a queue by priority
void sortQueueByPriority(Queue *q)
{
    if (isEmpty(q) == 1)
    {
        return;
    }
    Node *current = q->front;
    while (current->next != NULL)
    {
        Node *nextNode = current->next;
        while (nextNode != NULL)
        {
            if (current->data.pr > nextNode->data.pr)
            {
                process temp = current->data;
                current->data = nextNode->data;
                nextNode->data = temp;
            }
            nextNode = nextNode->next;
        }
        current = current->next;
    }
}

void executeProcesses(int nb_processus, process *tableau)
{
    int current_time = 0;
    int completed_processes = 0;
    int n = 0;
    Queue queue1, queue2, queue3;
    initializeQueue(&queue1);
    initializeQueue(&queue2);
    initializeQueue(&queue3);

    process *sortedProcesses = sortProcessesByArrival(nb_processus, tableau);
    for (int i = 0; i < nb_processus; i++)
    {
        sortedProcesses[i].index = i;
    }

    process *initialProcesses = (process *)malloc(sizeof(process) * nb_processus);
    memcpy(initialProcesses, tableau, sizeof(process) * nb_processus);

    printf("Execution:\n");
    printf("Time\t| Process\t| Remaining\t| Completed\n");
    printf("------------------------------------------------\n");

    for (int k = 0; k < nb_processus; k++)
    {

        if (initialProcesses[k].TE >= 0 && initialProcesses[k].TE <= 3 && sortedProcesses[k].reste > 0)
        {
            enqueue(&queue1, sortedProcesses[k]);
            sortQueueByPriority(&queue1);
        }
        else if (initialProcesses[k].TE >= 4 && initialProcesses[k].TE <= 6 && sortedProcesses[k].reste > 0)
        {
            enqueue(&queue2, sortedProcesses[k]);
            sortQueueByPriority(&queue2);
        }
        else if (initialProcesses[k].TE >= 7 && initialProcesses[k].TE <= 10 && sortedProcesses[k].reste > 0)
        {
            enqueue(&queue3, sortedProcesses[k]);
            sortQueueByPriority(&queue3);
        }
    }

    int f = 0;
    int s = 0;
    int h = 0;
    while (completed_processes < nb_processus && f < 30)
    {

        if (isEmpty(&queue1) == 0)
        {
            process currentProcess = dequeue(&queue1);

            if (sortedProcesses[currentProcess.index].DA <= current_time)
            {

                int k = 0;
                int quantum = 1;
                while (k < quantum && sortedProcesses[currentProcess.index].reste > 0)
                {
                    currentProcess.reste--;
                    sortedProcesses[currentProcess.index].completed++;
                    sortedProcesses[currentProcess.index].reste--;
                    printf("%d\t| %d\t\t| %d\t\t| %d\n", current_time, sortedProcesses[currentProcess.index].NP, sortedProcesses[currentProcess.index].reste, sortedProcesses[currentProcess.index].completed);
                    k++;
                    current_time++;
                }
                if (sortedProcesses[currentProcess.index].reste == 0)
                {
                    sortedProcesses[currentProcess.index].temfin = current_time;
                    sortedProcesses[currentProcess.index].treponse = sortedProcesses[currentProcess.index].temfin - sortedProcesses[currentProcess.index].DA;
                    sortedProcesses[currentProcess.index].tattant = sortedProcesses[currentProcess.index].treponse - sortedProcesses[currentProcess.index].TE;
                    completed_processes++;
                }
                else
                {
                    process nextProcess = dequeue(&queue1);
                    if (nextProcess.DA != -1)
                    {
                        if (nextProcess.DA > currentProcess.DA && nextProcess.DA > current_time)
                        {
                            enqueueFront(&queue1, nextProcess);
                            enqueueFront(&queue1, currentProcess);
                        }
                        else
                        {
                            enqueueFront(&queue1, nextProcess);
                            enqueue(&queue1, currentProcess);
                        }
                    }

                    s++;
                }
            }
            else
            {
                enqueueFront(&queue1, currentProcess);
                n++;
            }

            h++;
        }

        if (isEmpty(&queue2) == 0)
        {

            process currentProcess = dequeue(&queue2);
            if (sortedProcesses[currentProcess.index].DA <= current_time)
            {

                int k = 0;
                int quantum = 2;
                while (k < quantum && sortedProcesses[currentProcess.index].reste > 0)
                {
                    currentProcess.reste--;
                    sortedProcesses[currentProcess.index].completed++;
                    sortedProcesses[currentProcess.index].reste--;
                    printf("%d\t| %d\t\t| %d\t\t| %d\n", current_time, sortedProcesses[currentProcess.index].NP, sortedProcesses[currentProcess.index].reste, sortedProcesses[currentProcess.index].completed);
                    k++;
                    current_time++;
                }
                if (sortedProcesses[currentProcess.index].reste == 0)
                {
                    sortedProcesses[currentProcess.index].temfin = current_time;
                    sortedProcesses[currentProcess.index].treponse = sortedProcesses[currentProcess.index].temfin - sortedProcesses[currentProcess.index].DA;
                    sortedProcesses[currentProcess.index].tattant = sortedProcesses[currentProcess.index].treponse - sortedProcesses[currentProcess.index].TE;
                    completed_processes++;
                }
                else
                {
                    process nextProcess = dequeue(&queue2);
                    if (nextProcess.DA != -1)
                    {
                        if (nextProcess.DA > currentProcess.DA && nextProcess.DA > current_time)
                        {
                            enqueueFront(&queue2, nextProcess);
                            enqueueFront(&queue2, currentProcess);
                        }
                        else
                        {
                            enqueueFront(&queue2, nextProcess);
                            enqueue(&queue2, currentProcess);
                        }
                    }
                    s++;
                }
            }
            else
            {
                enqueueFront(&queue2, currentProcess);
                n++;
            }
            h++;
        }

        if (isEmpty(&queue3) == 0)
        {

            process currentProcess = dequeue(&queue3);
            if (sortedProcesses[currentProcess.index].DA <= current_time)
            {

                int quantum = 3;
                int k = 0;
                while (k < quantum && sortedProcesses[currentProcess.index].reste > 0)
                {
                    currentProcess.reste--;
                    sortedProcesses[currentProcess.index].completed++;
                    sortedProcesses[currentProcess.index].reste--;
                    printf("%d\t| %d\t\t| %d\t\t| %d\n", current_time, sortedProcesses[currentProcess.index].NP, sortedProcesses[currentProcess.index].reste, sortedProcesses[currentProcess.index].completed);
                    k++;
                    current_time++;
                }
                if (sortedProcesses[currentProcess.index].reste == 0)
                {
                    sortedProcesses[currentProcess.index].temfin = current_time;
                    sortedProcesses[currentProcess.index].treponse = sortedProcesses[currentProcess.index].temfin - sortedProcesses[currentProcess.index].DA;
                    sortedProcesses[currentProcess.index].tattant = sortedProcesses[currentProcess.index].treponse - sortedProcesses[currentProcess.index].TE;
                    completed_processes++;
                }
                else
                {
                    s++;
                    process nextProcess = dequeue(&queue3);
                    if (nextProcess.DA != -1)
                    {
                        if (nextProcess.DA > currentProcess.DA && nextProcess.DA > current_time)
                        {
                            enqueueFront(&queue3, nextProcess);
                            enqueueFront(&queue3, currentProcess);
                        }
                        else
                        {
                            enqueueFront(&queue3, nextProcess);
                            enqueue(&queue3, currentProcess);
                        }
                    }
                }
            }
            else
            {
                enqueueFront(&queue3, currentProcess);
                n++;
            }

            h++;
        }

        if (n == 5)
        {
            current_time++;
            n = 0;
        }

        if (isEmpty(&queue1) == 1 && isEmpty(&queue2) == 1 && isEmpty(&queue3) == 1)
        {
            printf("%d\t| NONE\t\t| NONE\t\t| NONE\n", current_time);
            current_time++;
        }
        f++;
    }
}

// Function to display the scheduling of processes
void displayScheduling(int nb_processus, process *tableau)
{
    printf("Ordonnancement des processus :\n");
    printf("|Processus\t|Date d'arrivee\t|Temps d'execution\t|Priority\t|Temps d'attente\t|Temps de reponse \n");
    printf("--------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < nb_processus; i++)
    {
        printf("|%d\t\t|%d\t\t|%d\t\t\t|%d\t\t|%d\t\t\t|%d\n", tableau[i].NP, tableau[i].DA, tableau[i].TE, tableau[i].pr, tableau[i].tattant, tableau[i].treponse);
    }
}

// Function to calculate average response and waiting times
void averageTimes(int nb_processus, process *tableau)
{
    int sum_response_time = 0;
    int sum_waiting_time = 0;
    for (int i = 0; i < nb_processus; i++)
    {
        sum_response_time += tableau[i].treponse;
        sum_waiting_time += tableau[i].tattant;
    }
    float average_response_time = (float)sum_response_time / nb_processus;
    float average_waiting_time = (float)sum_waiting_time / nb_processus;

    printf("Temps moyen de reponse pour tous les processus : %.2f\n", average_response_time);
    printf("Temps moyen d'attente pour tous les processus : %.2f\n", average_waiting_time);
}

int main()
{
    int nb_processus;
    process *tableau;

    do
    {
        printf("Entrez le nombre de processus: ");
        scanf("%d", &nb_processus);
        if (nb_processus <= 0)
        {
            printf("Le nombre de processus doit être supérieur à zéro.\n");
        }
    } while (nb_processus <= 0);

    createProcessArray(nb_processus, &tableau);
    executeProcesses(nb_processus, tableau);
    displayScheduling(nb_processus, tableau);
    averageTimes(nb_processus, tableau);

    return 0;
}