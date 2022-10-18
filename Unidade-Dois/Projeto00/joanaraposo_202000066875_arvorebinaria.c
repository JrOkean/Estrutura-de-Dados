#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*char* entrada = "arvorebinaria.input";
char* saida = "arvorebinaria.output";
*/
FILE* input;
FILE* output;

/////////////////////////////// ESTRUTURA

typedef struct arquivo {
	char nome[50];
	char escreve_ler[3];
	char bytes[50];
	int i;
} arquivo;

typedef struct node {
	char* key;
	arquivo* arquivo;
	struct node* direita;
	struct node* esquerda;
}node;

typedef struct Tree {
	node* root;
} Tree;

node* tree_criar_node() {
	node* new_node = malloc(sizeof(node));
    new_node->esquerda = new_node->direita = NULL;
	new_node->key = malloc(sizeof(char)*50);
	return new_node;
};

Tree* tree_criada(){
	Tree* nova_tree = malloc(sizeof(Tree));
	nova_tree->root = NULL;
	return nova_tree;
};

void adicione_node(Tree* arvore, arquivo* elemento){
	node* new_node = tree_criar_node();
	

	new_node->arquivo = elemento;
	new_node->key = elemento->nome;
	
	if (arvore->root == NULL) {
		arvore->root = new_node;
		return;
	}; 
	node* current = arvore->root;
	while(1) {
	 	if (strcmp(elemento->nome, current->key) < 0) {
			if(current->esquerda == NULL){
				current->esquerda = new_node;
				break;
			};
			current = current->esquerda;	
		}	
		else if(strcmp(elemento->nome, current->key) > 0){
			if(current->direita == NULL){
				current->direita = new_node;
				break;
			};
			current = current->direita;
		}
		else if(strcmp(elemento->nome, current->key) == 0){
			if (strcmp(current->arquivo->escreve_ler, "rw") == 0){
				current->arquivo = elemento;
			};
			return;
		};
	};
};

void print_linha(node* node){
	if (strcmp(node->arquivo->bytes, "1") == 0){
		fprintf(output, "%i %s %s %s byte\n", node->arquivo->i, node->arquivo->nome, node->arquivo->escreve_ler, node->arquivo->bytes);
	} else {
		fprintf(output, "%i %s %s %s bytes\n", node->arquivo->i, node->arquivo->nome, node->arquivo->escreve_ler, node->arquivo->bytes);
	};
};

void tree_epd(node* root, void (*fn)(node*)) {
	if(root == NULL)
		return;
	tree_epd(root->esquerda,fn);
	fn(root);
	tree_epd(root->direita,fn); 
};

void tree_ped(node* root, void (*fn)(node*)){
	if(root == NULL)
		return;
	fn(root);
	tree_ped(root->esquerda,fn);
	tree_ped(root->direita,fn);
};

void tree_edp(node* root, void (*fn)(node*)){
	if(root == NULL)
		return;
	tree_edp(root->esquerda,fn);
	tree_edp(root->direita,fn);
	fn(root);
};


/////////////////////////////// 

typedef struct dados{

	int quant;
	char elemento[510];

} dados;

/////////////////////////////// BUSCAS

int main(int argc, char* argv[]) {

	dados d;
	Tree* tree = tree_criada();

	char* entrada = argv[1];
	char* saida = argv[2];
    
   // Abrindo arquivos
    input = fopen(entrada, "r");
	output = fopen(saida, "w");

	fscanf(input, "%i", &d.quant);
	//printf("%i \n", d.quant);
	for (size_t i = 0; i < d.quant; i++) {
		arquivo* novo_arquivo = malloc(sizeof(arquivo));
		fscanf(input, "%s", novo_arquivo->nome);
		fscanf(input, "%s", novo_arquivo->escreve_ler);
		fscanf(input, "%s", novo_arquivo->bytes);
		novo_arquivo->i = i;
		adicione_node(tree, novo_arquivo);
		//printf("arquivo: %s %s %s %i \n", novo_arquivo->nome, novo_arquivo->escreve_ler, novo_arquivo->bytes, novo_arquivo->i);
	};

	fprintf(output, "EPD:\n");
	tree_epd(tree->root, print_linha);
	fprintf(output, "PED:\n");
	tree_ped(tree->root, print_linha);
	fprintf(output, "EDP:\n");
	tree_edp(tree->root, print_linha);


	fclose(input);
	fclose(output);
	return 0;
}