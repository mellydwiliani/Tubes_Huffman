#ifndef huff_head_H
#define huff_head_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*Struktur Data */
typedef char infotype;
typedef int frequnce;
typedef struct TbTree *Taddres;
typedef struct QueueNode *Qaddres;
typedef struct TbTree{
	frequnce freq;
	infotype info;
	Taddres LSon;
	Taddres RSon;
	Taddres parent;
}TNode;

typedef struct{
	Taddres root;
}Tree;

typedef struct QueueNode {
	Taddres treenode;
	Qaddres next;
	Qaddres prev;
}QNode;

typedef struct 
{
	Qaddres front;
	Qaddres rear;
	int size;
}ListQueue;

typedef struct{
	infotype info;
	infotype value[255];
}encoding;

typedef struct {
    unsigned char* buffer;  // Buffer untuk menyimpan bitstream
    int buffer_size;        // Ukuran buffer dalam byte
    int bit_index;          // Indeks bit saat ini dalam buffer
} Bitstream;

/*List of Modul */

void Proces_Input(char* kalimat, ListQueue *L);

char* input_tekss();

char* read_file(char *input_file);

Taddres Build_Huffman(ListQueue *L);

void PrintHuffman(Taddres node, char* result,int n ,encoding tamp[]);

void gotoxy(int x, int y);

void PrintTree(Taddres root, int level, int x, int y);

void encode(Taddres node, char* result, int level , char* str, int i, char* temp, char** code);

char decode(FILE *f, Taddres root);

void export_file( char* input_text);

void init_bitstream(Bitstream* bitstream);

void write_bit(Bitstream* bitstream, int bit);

void save_bitstream(Bitstream* bitstream, const char* filename);

void decompress(const char* filename, Taddres root);

Bitstream read_bitstream(const char* filename);

#endif

