#include <stdio.h>
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

process dequeue(Queue *q)
{
    if (isEmpty(q) == 1)
    {
        printf("Queue is empty.\n");
        exit(1);
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

// Function to create an array of processes
void createProcessArray(int nb_processus, process **tableau)
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
    Queue queue1, queue2, queue3;
    initializeQueue(&queue1);
    initializeQueue(&queue2);
    initializeQueue(&queue3);

    process *sortedProcesses = sortProcessesByArrival(nb_processus, tableau);

    printf("Execution:\n");
    printf("Time\t| Process\t| Remaining\t| Completed\n");
    printf("------------------------------------------------\n");

    int completed_processes = 0;
    Queue *currentQueue = NULL; // Initialize currentQueue pointer

    while (completed_processes < nb_processus)
    {
        printf("while 1");
        // Enqueue arriving processes into appropriate queues based on execution time
        for (int i = 0; i < nb_processus; i++)
        {
            printf("for 1");
            if (sortedProcesses[i].DA == current_time)
            {
                if (sortedProcesses[i].TE >= 0 && sortedProcesses[i].TE <= 3)
                {
                    enqueue(&queue1, sortedProcesses[i]);
                }
                else if (sortedProcesses[i].TE >= 4 && sortedProcesses[i].TE <= 6)
                {
                    enqueue(&queue2, sortedProcesses[i]);
                }
                else
                {
                    enqueue(&queue3, sortedProcesses[i]);
                }
            }
        }

        // Sort processes within each queue by priority
        sortQueueByPriority(&queue1);
        sortQueueByPriority(&queue2);
        sortQueueByPriority(&queue3);

        // Execute processes in round-robin manner
        Queue *queues[3] = {&queue1, &queue2, &queue3}; // Array of pointers to queues
        for (int i = 0; i < 3; i++)
        {
            printf("for 2");
            currentQueue = queues[i];
            int quantum = i + 1; // Quantum varies based on the queue number

            while (isEmpty(currentQueue) == 0)
            {
                printf("while 2");
                process currentProcess = dequeue(currentQueue);
                printf("%d\t| %d\t\t| %d\t\t| %d\n", current_time, currentProcess.NP, currentProcess.reste, currentProcess.completed);

                // Execute process for quantum time or remaining time if less than quantum
                int execution_time = (currentProcess.reste < quantum) ? currentProcess.reste : quantum;
                current_time += execution_time;
                currentProcess.completed += execution_time;
                currentProcess.reste -= execution_time;

                // Re-enqueue the process if it has remaining execution time
                if (currentProcess.reste > 0)
                {
                    enqueue(currentQueue, currentProcess);
                }
                else
                {
                    completed_processes++;
                }

                // Enqueue processes arriving at the current time
                while (completed_processes < nb_processus && current_time == sortedProcesses[completed_processes].DA)
                {
                    printf("while 3");
                    if (sortedProcesses[completed_processes].TE >= 0 && sortedProcesses[completed_processes].TE <= 3)
                    {
                        enqueue(&queue1, sortedProcesses[completed_processes]);
                    }
                    else if (sortedProcesses[completed_processes].TE >= 4 && sortedProcesses[completed_processes].TE <= 6)
                    {
                        enqueue(&queue2, sortedProcesses[completed_processes]);
                    }
                    else
                    {
                        enqueue(&queue3, sortedProcesses[completed_processes]);
                    }
                    completed_processes++;
                }
            }
        }
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
