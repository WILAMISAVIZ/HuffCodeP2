#include "HuffmanTADS/Huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void conting_freq(FILE *file_input, hash *Hash)
{
	long long int bytes = 0;
	unsigned char c;
	while(fscanf(file_input,"%c", &c) != EOF)
	{
		Hash->array[c]->frequencia++;
	}
}

Nodes *construct_tree(char nome_arquivo[], FILE *file_input, hash *Hash) {
	FILE *file_output; //variavel que guardará o arquivo de entrada e saida
	unsigned char caracter;
	//char nome_arquivo[30];
	Nodes *root, *left, *right, *top;
	int byte = 0;

	heap *Heap = CreatTable(256);
	Heap->size = 0;

	unsigned int *string = (unsigned int*) calloc(256, sizeof(unsigned int));
	int cont, i;
	i = cont = 0;
	file_input = fopen(nome_arquivo, "rb");
	if (file_input == NULL) {
		printf("Unable to open file: %s\n", nome_arquivo);
		exit(1);
	} else {
		conting_freq(file_input, Hash);
	}
	/*printf("****************************************\n");
	printf("SIZE FILE: %lld\n", FileSize(file_input));
	printf("****************************************\n");*/
	for (i = 0; i < 256; i++) {
		if (Hash->array[i]->frequencia >= 1) {
			Insert(Hash->array[i]->frequencia, i, Heap, NULL, NULL);
			printf("Characters: %c Frequency: %d\n", i, Hash->array[i]->frequencia);
		}
	}

	while (!(Heap->size == 1)) {
		left = Pop(Heap);
		right = Pop(Heap);
		Insert(left->frequency + right->frequency, '*', Heap, left, right);
	}
	//Nodes *root = Heap[1];
	return root = Heap->table[Heap->size];
	//View(Heap);
}

void Encode(Nodes *root, hash *HASH, char *new_path_bits)
{
	if(root != NULL)
	{
		if(eh_folha(root))
		{
			printf("%c : %s\n", root->character, new_path_bits);
			strcat(HASH->array[root->character]->bits, new_path_bits);
			HASH->array[root->character]->frequencia = root->frequency;
			HASH->array[root->character]->c = root->character;
		}
		else
		{
			Encode(root->left, HASH, strcat(new_path_bits, "0"));
			new_path_bits[strlen(new_path_bits) - 1] = '\0';
			Encode(root->right, HASH, strcat(new_path_bits, "1"));
			new_path_bits[strlen(new_path_bits) - 1] = '\0';
		}
	}
}

void get_header_compactacao(FILE* fileout, hash *HASH, Nodes *root, int tam_arvore) {
	unsigned char* bytes = (unsigned char*)malloc(3 * sizeof(unsigned char));
	int lixo = Cont_lixo_file(HASH);

	bytes[0] = lixo << 5 | tam_arvore >> 8;
	bytes[1] = tam_arvore;

	fputc(bytes[0], fileout);
	fputc(bytes[1], fileout);

	print_tree_huffman_file(fileout, root);
}

void compress() {
	FILE *file_input;
	char nome_arquivo[30];
	printf("Informe o nome do arquivo.\n");
	scanf("%s", nome_arquivo);
	hash *HASH = create_hash();
	Nodes *root = construct_tree(nome_arquivo, file_input, HASH);
	FILE *file_output;
	int bin_tam[8];
	int i = 0;
	unsigned long long int size_tree;
	unsigned char *vetor = (unsigned char *)malloc(10 * sizeof(unsigned char));
	i = 0;
	Encode(root, HASH, vetor);
	int tam_str = strlen(vetor);
	size_tree = lenght_tree(root);
	printf("TAM DA ARVORE EM DECIMAL: %lld\n", size_tree);

	/*for (i = 0; i < 256; i++) {
		if (HASH->array[i]->frequencia > 1) {
			printf("CHAR: %c BITS: %s FREQ: %d\n", HASH->array[i]->c, HASH->array[i]->bits, HASH->array[i]->frequencia);
		}
	}*/
	int tam_lixo = Cont_lixo_file(HASH);
	printf("LIXO: %d\n", tam_lixo);

	char nome_file_output[30];
    strcpy(nome_file_output, nome_arquivo);
    strcat(nome_file_output, ".huff");

	file_output = fopen(nome_file_output, "wb");
	get_header_compactacao(file_output, HASH, root, size_tree);
	//Decode(root, vetor, );
	printf("PRE ORDEM: ");
	print_tree_huffman(root);
	printf("\n");
	convert_size_tree_to_bin(size_tree, bin_tam);
	for (i = 0; i < 14; i++) {
		printf("%d", bin_tam[i]);
	}
	printf("\n");
}
