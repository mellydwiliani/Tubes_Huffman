#ifndef tree_head_H
#define tree_head_H

#include "huff_head.h"

/*List of Modul */

/**** Konstruktor/Kreator List Kosong ****/
void Create_Tree(Tree *T);

Taddres Create_TNode(frequnce freq, infotype item);

Taddres peek(ListQueue L);

#endif