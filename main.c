#include "huff_head.h"
#include "queue_head.h"
#include "tree_head.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(){

	int pil;
	
	
	do{		
		int max_tree,max_char;
		int n = 0;
		ListQueue list,listTemp;
		char temp[255];
		char result[255]; // inisialisasi array result
		
		Create_List(&list);
		Create_List(&listTemp);
		Taddres root;
		char* input_text;
		char* hasil_encode = NULL;
		
		system("cls");
		printf("\n");
		printf("\t		Huffman Code 		\n");
		printf("\t\n");
		printf("\t 1. Input\n");
		printf("\t 2. Show Queue\n");
		printf("\t 3. Show Tree & Huffman\n");
		printf("\t 3. Encode & Export\n");
		printf("\t 4. Decode from File\n");
		printf("\t 5. Exit Program\n");
		printf("\t\n");
		
		printf("\t  Masukkan Pilihan	: ");		
		scanf("%d",&pil);fflush(stdin);
		
		switch(pil){
			case 1:{
				int pil_menu;
				do{
					system("cls");
					printf("\n");
					printf("\t 1. Input Text\n");
					printf("\t 2. Input File\n");
					printf("\t 3. Kembali\n");
					printf("\t  Masukkan Pilihan	: ");
					scanf("%d",&pil_menu);fflush(stdin);	
					switch(pil_menu){
						case 1:{
							input_text = input_teks() ;
							max_char = strlen(input_text);

							printf("Press any key to continue...");
							getchar();
							proces_input(input_text,&list);
							
							pil_menu = 3;
							break;
						}
						case 2:{
							char input_file[100];
							
							printf("\t  Masukkan Nama File	: ");		
							scanf("%s",&input_file);fflush(stdin);
							
							input_text = read_file(input_file);

							printf("Press any key to continue...");
							getchar();
							max_char = strlen(input_text);
							proces_input(input_text,&list);
							
							pil_menu = 3;
							break;
						}case 3:{
							break;
						}default :{
							printf("\tPilihan Salah");
							break;
						}
					}			
				}while(pil_menu != 3);
				
				root = Build_Huffman(&list);
				max_tree = ceil(log2(max_char));
				break;
			}
			case 2:{
				proces_input(input_text,&listTemp);
				Print_Queue(listTemp);
				Create_List(&listTemp);
				break;
			}
			case 3:{
	
				printf("\n\n\t Tree Huffman Code\n");

			    if(root == NULL){
			    	printf("root kosong");
				}else{
				    gotoxy(17, 18);
					PrintTree(root, 0, 17, 18);
				}
				
				printf("\n\n\t Huffman Code\n");
				char result_code[max_tree]; // inisialisasi array result
			    encoding code[max_tree]; // inisialisasi array tamp
			    
			
			    PrintHuffman(root, result_code,n ,code); // panggil fungsi rekursif helper
				printf("\nPress any key to continue...");
				getchar();
				break;
			}
			case 4:{
			    FILE* fp = fopen("compressed.txt", "w");	
				int i;
				encoding code[max_tree]; // inisialisasi array tamp
				Bitstream bitstream;
    			init_bitstream(&bitstream);
    			
				printf("\n\n\t Huffman Code\n");
			
			    PrintHuffman(root, result,n ,code); // panggil fungsi rekursif helper
				printf("\n\n\t Encode String \n\t ");
				char* code_binner = malloc(max_char * sizeof(char));
				for( i=0;i<max_char;i++){
				
					encode(root,result,0,input_text, i, temp,&code_binner);
					write_bit(&bitstream, code_binner);
					printf("%s" ,code_binner);
					
					fprintf(fp, "%s", code_binner);
		
					strcpy(code_binner, "");
				}
				

				export_file(input_text);
				
				 // Simpan bitstream ke file
			    save_bitstream(&bitstream, "output.bin");
			
			    // Dealokasi memori
			    free(bitstream.buffer);
				
				printf("\n\nSuccses to Export \nPress any key to continue...");
				fclose(fp);
				getchar();
				
				break;
			}
			case 5:{
				FILE* fp = fopen("compressed.txt", "r");
				int i;		
			    char bit;
				
				printf("\n\n\t Encode String \n\t ");
				
				for( i=0;i<max_char;i++){
					encode(root,result,0,input_text, i, temp,&hasil_encode);
					printf("%s" ,hasil_encode);
				}
			   
				if (fp == NULL) {
				    printf("File tidak dapat dibuka.\n");
				}
				printf("\n\t Decode to String \n\t ");
						 
				while ((bit = decode(fp, root)) != EOF) {
				    putchar(bit);
				}

				printf("\n\n\t Decode ke String dari file compress \n\t " );
//				// Melakukan dekompresi dan menampilkan karakter hasil dekompresi
			   decompress("output.bin", root);

				
			    // Menutup file dan membersihkan memori
			    fclose(fp);
				
				printf("\n\nPress any key to continue...");
				getchar();


				break;
			}
			case 6:{
				break;
			}
			default:{
				printf("\tPilihan Salah");
				break;
			}
			
		}
	}while(pil != 6);
	return 0;
}
