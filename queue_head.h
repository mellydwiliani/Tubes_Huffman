#ifndef queue_head_H
#define queue_head_H

#include "huff_head.h"

/*List of Modul */

/**** Konstruktor/Kreator List Kosong ****/
void Create_List(ListQueue *L);

Qaddres Create_Node(Taddres dummy);


void Print_Queue(ListQueue L);

void Deque(ListQueue *L);

void enque(ListQueue *L,Taddres T);

#endif