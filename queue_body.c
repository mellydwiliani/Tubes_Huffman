#include "queue_head.h"

void Create_List(ListQueue *L){
	L->front = NULL;
	L->rear = NULL;
	L->size = 0;
}

Qaddres Create_Node(Taddres dummy){
	Qaddres newNode = (Qaddres) malloc(sizeof(QNode));
    if (newNode == NULL) {
        printf("Error: Alokasi memori gagal");
        return NULL;
    }
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->treenode = dummy;
    return newNode;
}


void enque(ListQueue *L, Taddres T) {
    Qaddres node = NULL;
    node = Create_Node(T);
    if (L->front == NULL) {
        L->front = node;
        L->rear = node;
    } else if (L->front->treenode->freq > T->freq) {
        // Add node at beginning position
        node->next = L->front;
        L->front->prev = node;
        L->front = node;
    } else if (L->front->treenode->freq < T->freq) {
        // Add node at last position
        node->prev = L->rear;
        L->rear->next = node;
        L->rear = node;
    } else {
        Qaddres temp = L->front;
        // Find the location of inserting priority node
        while (temp != NULL && temp->treenode->freq <= T->freq) {
            temp = temp->next;
        }
        // Add node
        node->next = temp;
        if (temp != NULL) {
            node->prev = temp->prev;
            temp->prev = node;
            if (node->prev != NULL) {
                node->prev->next = node;
            }
        } else {
            node->prev = L->rear;
            L->rear->next = node;
            L->rear = node;
        }
    }
    L->size += 1;
}


void Print_Queue(ListQueue L){
	Qaddres P;
	P = L.front;
	printf("-------------------------------------------\n");
    while (P != NULL) {
        printf("[%c][%d] ", P->treenode->info, P->treenode->freq);
        P = P->next;
    }
    printf("\n-------------------------------------------\n");
    printf("Press any key to continue...");
	getchar();
}

void Deque(ListQueue *L){
	if (L->size != 0)
	{	
		Qaddres temp = L->front;
		L->front->treenode = NULL;
		if (L->front == L->rear)
		{
			// When queue contains only one node
			L->rear = NULL;
			L->front = NULL;
		}
		else
		{
			L->front = L->front->next;
			L->front->prev = NULL;
		}
		// Change queue size
		L->size--;
		free(temp);
	}
	else
	{
		printf("\n Empty Queue \n");
	}
}

