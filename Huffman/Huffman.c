#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define MAX 10000

typedef struct Tree
{
	unsigned int frequency;
	char character;
	struct Tree *left;
	struct Tree *right;
} Nodes;

typedef struct Array
{
	int size;
	Nodes *table[MAX];
} heap;

Nodes *CreatNode(int data, char character, Nodes * left, Nodes *right)
{
	Nodes* node = (Nodes*) malloc(sizeof(Nodes));
	node->frequency = data;
	node->character = character;
	node->left = left;
	node->right = right;
	return node;
}
/*It's in this function where we creat a array of pointers*/

heap *CreatTable(int size_table)
{
	int i;
	heap *Table = (heap*) malloc(sizeof(heap));
	for (i = 0; i < size_table; i++)
	{
		Table->table[i] = NULL;
	}
	return Table;
}

// swap two min heap nodes//
void Swap(Nodes **a, Nodes **b)
{
	Nodes *t = *a;
	*a = *b;
	*b = t;
}

int GetParentIndex(int index)
{
	return index / 2;
}

int GetChildrenLeftIndex(int index)
{
	return index * 2;
}

int GetChildrenRightIndex(int index)
{
	return index * 2 + 1;
}

int eh_folha(Nodes *huffman_node)
{
	if((huffman_node->left == NULL) && (huffman_node->right == NULL))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void View(heap *Heap)
{
	int i;
	for (i = 1; i <= Heap->size; i++)
	{
		printf(" %d %c ", Heap->table[i]->frequency, Heap->table[i]->character);
	}
	//printf("\n");
}

void DownHeapMin(int index, heap *Heap)
{
	if (index * 2 > Heap->size)
	{
		return;
	}
	else
	{
		int left = GetChildrenLeftIndex(index);
		int Right = GetChildrenRightIndex(index);
		int smallest = index;
		if (left <= Heap->size
				&& Heap->table[left]->frequency
						< Heap->table[smallest]->frequency)
		{
			smallest = left;
		}
		if (Right <= Heap->size
				&& Heap->table[Right]->frequency
						< Heap->table[smallest]->frequency)
		{
			smallest = Right;
		}
		if (smallest == index)
		{
			return;
		}
		else
		{
			Swap(&Heap->table[index], &Heap->table[smallest]);
			DownHeapMin(smallest, Heap);
		}
	}
}

Nodes *Pop(heap *Heap) {
	if (Heap->size == 0) {
		return 0;
	}
	else
	{
		Nodes *aux = Heap->table[1];
		Heap->table[1] = Heap->table[Heap->size];
		Heap->size--;
		DownHeapMin(1, Heap);
		return aux;
	}
}

void UpHeapMin(int index, heap *Heap) {
	if (index == 1)
	{
		return;
	}
	else
	{
		int Parentidx = GetParentIndex(index);
		if (Heap->table[index]->frequency < Heap->table[Parentidx]->frequency)
		{
			Swap(&Heap->table[index], &Heap->table[Parentidx]);
			UpHeapMin(Parentidx, Heap);
		}
	}
}

void Insert(int Value, char character, heap *Heap, Nodes *left, Nodes *right)
{
	Heap->size++;
	Heap->table[Heap->size] = CreatNode(Value, character, left, right);
	UpHeapMin(Heap->size, Heap);
}

void print_tree_huffman(Nodes *huffman_node) {
	if (huffman_node != NULL) {
		if (((huffman_node->character == '*') || (huffman_node->character == '\\')) && eh_folha(huffman_node)){
			printf("%c", '\\');
		}

		printf("%c", huffman_node->character);
		print_tree_huffman(huffman_node->left);
		print_tree_huffman(huffman_node->right);
	}
}

void print_tree_huffman_file(FILE *output_file, Nodes *huffman_node) {
	if (huffman_node != NULL) {
		if (((huffman_node->character == '*') || (huffman_node->character == '\\')) && eh_folha(huffman_node)){
			fprintf(output_file,"%c", '\\');
		}

		fprintf(output_file, "%c", huffman_node->character);
		print_tree_huffman_file(output_file,huffman_node->left);
		print_tree_huffman_file(output_file,huffman_node->right);
	}
}

Nodes *construct_tree()
{
	FILE *file_input; //variavel que guardará o arquivo de entrada
	unsigned char caracter;
	char nome_arquivo[30];
	Nodes *root;
	Nodes *left;
	Nodes *right;
	Nodes *top;
//every time we call the function we're going to have to pass character and value that we want to add in Heap //
	heap *Heap = CreatTable(256);
	Heap->size = 0;

	unsigned int *string = (unsigned int*) calloc(256, sizeof(unsigned int));
	int cont, i;
	i = cont = 0;
	scanf("%[^\n]", nome_arquivo);
	file_input = fopen(nome_arquivo, "rb");
	if (file_input == NULL) {
		printf("Unable to open file: %s\n", nome_arquivo);
		return NULL;
	} else {
		while (fscanf(file_input, "%c", &caracter) != EOF) {
			string[caracter]++;
			//printf("%c", caracter);
		}
	}
	for (i = 0; i < 256; i++) {
		if (string[i] >= 1) {
			Insert(string[i], i, Heap, NULL, NULL);
			printf("Characters: %c Frequency: %d\n", i, string[i]);
		}
	}

	while(!(Heap->size == 1))
	{
		left = Pop(Heap);
		right = Pop(Heap);

		Insert(left->frequency + right->frequency, '*', Heap, left, right);
	}
	//Nodes *root = Heap[1];
	return root = Heap->table[Heap->size];
	//View(Heap);
}

void printVetor(char v[], int tam)
{
    int i;
    for (i = 0; i < tam; i+=1)
    {
    	printf("%c", v[i]);
    }
    printf("\n");
}

void Encode(Nodes *root, char arr[], int top)

{
    // Assign 0 to left edge and recur
    if (root->left)
    {

        arr[top] = '0';
        Encode(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right)
    {

        arr[top] = '1';
        Encode(root->right, arr, top + 1);
    }

    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (eh_folha(root))
    {
        printf("%c: ", root->character);
        printVetor(arr, top);
    }
}

void Decode(Nodes* root, char array[], int index)
{
	if (root == NULL) {
		return;
	}

	// found a leaf node
	if (eh_folha(root))
	{
		printf("%c ", root->character);
		return;
	}

	if (array[index] == '0')
	{
		Decode(root->left, array, index + 1);
	}
	else if(array[index] ==  '1')
	{
		Decode(root->right, array, index + 1);
	}
}

void tam_tree(Nodes *node_huffman, int *size) {
    if (node_huffman == NULL)
    {
        return;
    }
    if((eh_folha(node_huffman) && node_huffman->character == '*') || (eh_folha(node_huffman) && node_huffman->character == '\\'))
    {
        *size += 1;
    }
    *size += 1;
    tam_tree(node_huffman->left, size);
    tam_tree(node_huffman->right, size);
}

int main()
{
	Nodes *root = construct_tree();
	int size_tree = 0;
	char vetor[MAX];
	int i = 0;
	//vetor = (char *)malloc(sizeof(char) * MAX);
	Encode(root, vetor, i);
	int tam_str = strlen(vetor);
	tam_tree(root, &size_tree);
	printf("TAM DA ARVORE EM DECIMAL: %d\n", size_tree);
	//printf("%d", tam_str);
	//Decode(root, vetor, );
	print_tree_huffman(root);
	printf("\n");
	return 0;
}
