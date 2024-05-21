#include <stdio.h>
#include <stdlib.h>
#include "huff_head.h"

void input_teks(){
	char input[100];
	printf("\t  Masukkan Text	: ");		
	scanf("%s",&input);fflush(stdin);
}

void read_file(char *input_file) {
    char teks[100];
    FILE *file;
    file = fopen(input_file, "r");

    if (file == NULL) {
        printf("File tidak ditemukan\n");
        return;
    }

    printf("\n\tIsi file %s:\n", input_file);
    printf("\t--------------------\n");

    while (fscanf(file, "%s", teks) == 1) {
        printf("\t%s ", teks);
    }

    printf("\n\t---------------------\n");
    fclose(file);
}

