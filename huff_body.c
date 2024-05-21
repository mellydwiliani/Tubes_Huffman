#include "huff_head.h"
#include "queue_head.h"
#include "tree_head.h"

void proces_input(char kalimat[1000], ListQueue *L){
	Taddres P;
    int freq[128] = {0}; //  agar dapat menampung semua karakter ASCII
    int i;
    for (i = 0; kalimat[i] != '\0'; i++) {
        freq[(int)kalimat[i]]++; // Tambahkan karakter ke array frekuensi
    }
    
    for (i = 0; i < 128; i++) { // Melakukan iterasi dari nilai ASCII 0 hingga 127
        if (freq[i] > 0) {
            char info = i; // Simpan nilai ASCII ke dalam variabel info
            P = Create_TNode(freq[i],info);
            enque(&(*L),P);
        } 
    }
}


char* input_teks(){
    char* input = (char*) malloc(1001 * sizeof(char)); // tambahkan 1 byte untuk null terminator
    if (input == NULL) {
        printf("Gagal mengalokasikan memori.\n");
        exit(1);
    }
    printf("\t  Masukkan Text : ");
    fgets(input, 1001, stdin); // baca string input dengan fungsi fgets
    input[strcspn(input, "\n")] = '\0'; // hapus karakter newline pada akhir string
    return input;
}


char* read_file(char* input_file) {
    char* text = (char*) malloc(1000 * sizeof(char));
    FILE *file;
    file = fopen(input_file, "r");

    if (file == NULL) {
        printf("File tidak ditemukan\n");
        return;
    }
	fscanf(file, "%[^\n]s", text);
    fclose(file);
	return text;
}

Taddres Build_Huffman(ListQueue *L){
    Taddres root;
    Taddres n1;
    Taddres n2;

    while (L->size > 1)
    {
        // Get first smallest node  
        n1 = peek(*L);
        // Remove a front element
        Deque(&(*L));
        // Get second smallest node
        n2 = peek(*L);
        // Remove a front element
        Deque(&(*L));
        // Make new node using two smallest node
        root = Create_TNode(n1->freq + n2->freq, ' ');
        // Set left and right child
        root->LSon = n1;
        root->RSon = n2;
        // Add new node into priority queue 
        enque(&(*L),root); 
		//Insrt_Node(&(*L), root->freq, root->info);
    }
    return root;
}



// Display Huffman code

void PrintHuffman(Taddres node, char* result, int n,encoding tamp[])
{
	
	int x = 0;
    if (node == NULL)
    {
        return;
    }
    if (node->LSon == NULL && node->RSon == NULL)
    {
        result[n] = '\0';
        tamp[x].info = node->info;
        strcpy(tamp[x].value,result);
        printf("\n\t %c = %s", node->info, result);
        x += 1;
        return;
    }
    result[n] = '0';
    PrintHuffman(node->LSon, result, n + 1, tamp);
    result[n] = '1';
    PrintHuffman(node->RSon, result, n + 1, tamp);
}

// Fungsi untuk mengatur posisi kursor di layar
void gotoxy(int x, int y) {
    printf("\033[%d;%df", y, x);
}

void PrintTree(Taddres root, int level, int x, int y) {
    if (root == NULL) {
        return;
    }

    PrintTree(root->RSon, level + 1, x + 4, y + 2); // Posisi x digeser ke kanan sejauh 4 dan posisi y ditambah 2
    gotoxy(x, y); // Mengatur posisi kursor

    if (root->LSon != NULL && root->RSon != NULL) {
        printf("  %d", root->freq);
    } else {
        printf("%c:%d", root->info, root->freq);
        gotoxy(x, y);
    }

    PrintTree(root->LSon, level + 1, x - 2, y + 2); // Posisi x digeser ke kiri sejauh 2 dan posisi y ditambah 2
    gotoxy(x, y + 1); // Mengatur posisi kursor untuk anak pertama
    
	if (root->LSon != NULL){
		printf("/");
	}
    
	if (root->RSon != NULL) {
        gotoxy(x + 4, y + 1); // Mengatur posisi kursor untuk anak kedua
        printf("\\");
    }
}


void encode(Taddres node, char* result, int level , char* str, int i, char* temp, char** code){
	if (node == NULL) {
		return;
	}
	if (node->LSon == NULL && node->RSon == NULL) {
		if(str[i] == node->info) {
			result[level] = '\0';
			strcat(temp,result);
			*code = strdup(result); // duplikasi string agar hasil encode bisa digunakan kembali
			return ;
		}
	}
	result[level] = '0';
	encode(node->LSon, result, level + 1, str, i, temp, code);
	result[level] = '1';
	encode(node->RSon, result, level + 1, str, i, temp, code);
}



char decode(FILE *f, Taddres root) {
    Taddres node = root;
    char bit, result_char;
    
    if (node->LSon == NULL && node->RSon == NULL) {
        if (isspace(node->info)) {
            result_char = ' ';
        } else {
            result_char = node->info;
        }
        return result_char;
    }
    
    bit = fgetc(f);
    if (bit == EOF) {
        return EOF;
    } 
    if (bit == '0') {
        return decode(f, node->LSon);
    } else if (bit == '1') {
        return decode(f, node->RSon);
    }
    
    return EOF;
}

void export_file( char* input_text) {
    
	FILE* fp = fopen("export_file.txt", "w");
    fprintf(fp, "%s", input_text);

	fclose(fp);
}

// Fungsi untuk membaca bitstream dari file
Bitstream read_bitstream(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Gagal membuka file\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    Bitstream bitstream;
    bitstream.buffer_size = file_size;
    bitstream.buffer = (unsigned char*)malloc(file_size * sizeof(unsigned char));
    fread(bitstream.buffer, sizeof(unsigned char), file_size, file);
    fclose(file);

    return bitstream;
}

void decompress(const char* filename, Taddres root) {
    // Baca bitstream dari file
    Bitstream bitstream = read_bitstream(filename);
    int byte_count = (bitstream.buffer_size + 7) / 8; // Jumlah byte dalam bitstream

    // Membaca karakter dari bitstream menggunakan Huffman Tree
    Taddres node = root;
    int i;
    for (i = 0; i < byte_count; i++) { // Mengiterasi melalui byte-bitstream
        unsigned char byte = bitstream.buffer[i];
        int j;
        for (j = 0; j < 8; j++) { // Mengiterasi melalui setiap bit dalam byte
            int bit = (byte >> j) & 1;

            if (bit == 0) {
                node = node->LSon;
            } else if (bit == 1) {
                node = node->RSon;
            }

            if (node->LSon == NULL && node->RSon == NULL) {
                putchar(node->info);
                node = root;
            }
        }
    }

    // Bebaskan memori bitstream
    free(bitstream.buffer);
}

void prosesKompres(const char* namafile) {
    FILE* file = fopen(namafile, "r");
    Bitstream bitstream;
    init_bitstream(&bitstream);
    
    if (file == NULL) {
        printf("Gagal membuka file\n");
        exit(EXIT_FAILURE);
    }

 	char* string = (char*) malloc(255 * sizeof(char));
	fscanf(file, "%s", string);
    int i = 0;
    while (string[i] != '\0') {
        // Dapatkan karakter dari string
        char current_char = string[i];
        // TODO: Lakukan operasi Huffman coding pada current_char menggunakan Huffman Tree

        // Misalnya, jika kita ingin menambahkan bit ke bitstream berdasarkan current_char
        int bit = current_char - '0';
        write_bit(&bitstream, bit);

        i++;
    }

    // Simpan bitstream ke file
    save_bitstream(&bitstream, "output.bin");

    // Bebaskan memori bitstream
    free(bitstream.buffer);

    // Bebaskan memori string
    free(string);

    fclose(file);
}

// Inisialisasi bitstream
void init_bitstream(Bitstream* bitstream) {
    bitstream->buffer = (unsigned char*)malloc(sizeof(unsigned char));
    bitstream->buffer_size = 1;
    bitstream->bit_index = 0;
    bitstream->buffer[0] = 0;
}

// Tambahkan bit ke bitstream
void write_bit(Bitstream* bitstream, int bit) {
    if (bitstream->bit_index >= (bitstream->buffer_size * 8)) {
        bitstream->buffer_size++;
        bitstream->buffer = (unsigned char*)realloc(bitstream->buffer, bitstream->buffer_size);
        bitstream->buffer[bitstream->buffer_size - 1] = 0;
    }
    if (bit)
        bitstream->buffer[bitstream->bit_index / 8] |= (1 << (bitstream->bit_index % 8));
    bitstream->bit_index++;
}

// Simpan bitstream ke file
void save_bitstream(Bitstream* bitstream, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Gagal membuka file\n");
        exit(EXIT_FAILURE);
    }

    // Hitung jumlah byte yang sebenarnya digunakan oleh bitstream
    size_t byte_count = (bitstream->bit_index + 7) / 8;

    // Tulis bitstream ke file
    fwrite(bitstream->buffer, sizeof(unsigned char), byte_count, file);

    fclose(file);
}














    
    


