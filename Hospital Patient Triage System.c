#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Patient {
    int queueNumber;
    char name[50];
    struct Patient *next;
};

struct Queue {
    struct Patient *front;
    struct Patient *rear;
};

static int nextNumber = 101;

void initQueue(struct Queue *q) { q->front = q->rear = NULL; }

int isEmpty(const struct Queue *q) { return q->front == NULL; }

struct Patient *createPatient(const char *name) {
    struct Patient *p = malloc(sizeof(struct Patient));
    if (p == NULL) {
        printf("Error: out of memory.\n");
        return NULL;
    }
    p->queueNumber = nextNumber++;
    strncpy(p->name, name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';
    p->next = NULL;
    return p;
}

/* Normal arrival: insert at the rear */
void enqueue(struct Queue *q, const char *name) {
    struct Patient *p = createPatient(name);
    if (p == NULL) return;
    if (isEmpty(q)) {
        q->front = q->rear = p;
    } else {
        q->rear->next = p;
        q->rear = p;
    }
    printf("Success: Patient %s registered. Queue Number: %d\n",
           p->name, p->queueNumber);
}

/* Extension: VIP arrival, insert at the front */
void enqueueVIP(struct Queue *q, const char *name) {
    struct Patient *p = createPatient(name);
    if (p == NULL) return;
    if (isEmpty(q)) {
        q->front = q->rear = p;
    } else {
        p->next = q->front;
        q->front = p;
    }
    printf("Success: VIP Patient %s registered at the front. Queue Number: %d\n",
           p->name, p->queueNumber);
}

void dequeue(struct Queue *q) {
    if (isEmpty(q)) {
        printf("No patients are waiting.\n");
        return;
    }
    struct Patient *temp = q->front;
    printf("Calling: Queue Number %d, Patient %s. "
           "Please enter the consultation room.\n",
           temp->queueNumber, temp->name);
    q->front = temp->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
}

void peek(const struct Queue *q) {
    if (isEmpty(q)) {
        printf("No patients are waiting.\n");
        return;
    }
    printf("Next in line: Queue Number %d, Patient %s\n",
           q->front->queueNumber, q->front->name);
}

void display(const struct Queue *q) {
    printf("\n--- Current Waiting Room ---\n");
    if (isEmpty(q)) {
        printf("(empty)\n");
    } else {
        int i = 1;
        for (struct Patient *p = q->front; p != NULL; p = p->next)
            printf("%d. [No.%d] %s\n", i++, p->queueNumber, p->name);
    }
    printf("----------------------------\n");
}

/* Called on exit: serves remaining patients and frees all memory */
void clearQueue(struct Queue *q) {
    while (!isEmpty(q)) dequeue(q);
}

void readName(char *buf, size_t size) {
    printf("Enter patient name: ");
    if (fgets(buf, (int)size, stdin) == NULL) buf[0] = '\0';
    buf[strcspn(buf, "\n")] = '\0';
    if (buf[0] == '\0') strcpy(buf, "Unknown");
}

int main(void) {
    struct Queue q;
    char line[100], name[50];
    int choice = 0;

    initQueue(&q);

    do {
        printf("\n===== Hospital Registration System =====\n");
        printf("1. Patient Arrives (Register)\n");
        printf("2. Doctor Calls Next Patient\n");
        printf("3. Check Who is Next\n");
        printf("4. View Waiting Room\n");
        printf("5. Exit System\n");
        printf("6. VIP Patient Arrives\n");
        printf("Enter your choice: ");

        if (fgets(line, sizeof(line), stdin) == NULL) break;
        if (sscanf(line, "%d", &choice) != 1) choice = 0;

        switch (choice) {
            case 1: readName(name, sizeof(name)); enqueue(&q, name);    break;
            case 2: dequeue(&q);                                         break;
            case 3: peek(&q);                                            break;
            case 4: display(&q);                                         break;
            case 5: printf("Shutting down system. Have a healthy day!\n"); break;
            case 6: readName(name, sizeof(name)); enqueueVIP(&q, name); break;
            default: printf("Invalid choice. Please enter 1-6.\n");
        }
    } while (choice != 5);

    clearQueue(&q);
    return 0;
}
