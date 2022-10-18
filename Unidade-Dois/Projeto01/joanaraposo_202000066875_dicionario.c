#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//char* entrada = "teste.input";
//char* entrada = "teste2.input";
//char* entrada = "dicionario.input";
//char* saida = "dicionario.output";

FILE* input;
FILE* output;

//////////////////////////// Tree

typedef struct arquivo {
	char chave[51];
	char**  sinonimos;
	int quant_sinonimos;
} arquivo;

typedef struct node {
	char* key;
	arquivo* arquivo;
	struct node* direita;
	struct node* esquerda;
    int altura;
}node;

typedef struct Tree {
	node* root;
} Tree;

node* tree_criar_node() {
	node* new_node = malloc(sizeof(node));
    new_node->esquerda = new_node->direita = NULL;
	new_node->key = malloc(sizeof(char)*50);
	new_node->altura = 1;
	return new_node;
};

Tree* tree_criada(){
	Tree* nova_tree = malloc(sizeof(Tree));
	nova_tree->root = NULL;
	return nova_tree;
};

int altura_nonNull(node* node){
	if(node==NULL)
		return 0;

 	return node->altura;
};

int fator_balanceamento(node* node){
	if(node==NULL)
		return 0;

 	return altura_nonNull(node->esquerda) - altura_nonNull(node->direita);
};

int max(int a, int b){
	if(a > b){
		return a;
	} else {
		return b;
	};
};

node* rotacao_esquerda(node* elemento){
	node* eixo = elemento->direita;
	node* move_eixo = eixo->esquerda;

	eixo->esquerda = elemento;
	elemento->direita = move_eixo;

	elemento->altura = max(altura_nonNull(elemento->esquerda), altura_nonNull(elemento->direita))+1;
	eixo->altura = max(altura_nonNull(eixo->esquerda), altura_nonNull(eixo->direita))+1;

	return eixo;
};

node* rotacao_direita(node* elemento){
	node* eixo = elemento->esquerda;
	node* move_eixo  = eixo->direita;
	
	eixo->direita = elemento;
	elemento->esquerda = move_eixo;

	elemento->altura = max(altura_nonNull(elemento->esquerda), altura_nonNull(elemento->direita))+1;
	eixo->altura = max(altura_nonNull(eixo->esquerda), altura_nonNull(eixo->direita))+1;

	return eixo;
};

node* adicione_node_interno(node* root, arquivo* elemento){
	
	if(root==NULL){
		node* new_node = tree_criar_node();
		new_node->arquivo = elemento;
		new_node->key = elemento->chave;
		return new_node;
	};
	if(strcmp(elemento->chave, root->key) < 0){
		root->esquerda = adicione_node_interno(root->esquerda, elemento);
	} else if(strcmp(elemento->chave, root->key) > 0){
		root->direita = adicione_node_interno(root->direita, elemento);
	} else{
		return root;
	};

	root->altura = max(altura_nonNull(root->esquerda), altura_nonNull(root->direita))+1;

	int balance = fator_balanceamento(root);

	// Left Left Case
	if(balance > 1 && strcmp(elemento->chave, root->esquerda->key) < 0){
		return rotacao_direita(root);
	};
	// Right Right Case
	if(balance < -1 && strcmp(elemento->chave, root->direita->key) > 0){
		return rotacao_esquerda(root);
	};
	//Left Right Case
	if(balance > 1 && strcmp(elemento->chave, root->esquerda->key) > 0){
		root->esquerda = rotacao_esquerda(root->esquerda);
		return rotacao_direita(root);
	};
	// Right Left Case
	if(balance < -1 && strcmp(elemento->chave, root->direita->key) < 0){
		root->direita = rotacao_direita(root->direita);
		return rotacao_esquerda(root);
	};
	return root;
};

void adicione_node(Tree* arvore, arquivo* elemento){
	node* current = arvore->root;
	arvore->root = adicione_node_interno(current, elemento);
};

void print_sinonimos(node* elemento){
	int quant;

	quant = elemento->arquivo->quant_sinonimos;
	for (size_t i = 0; i < quant-1; i++){
		fprintf(output, "%s,", elemento->arquivo->sinonimos[i]);
	};
	fprintf(output, "%s \n", elemento->arquivo->sinonimos[quant-1]);
};	

void busca(Tree* tree, char* procurado, void (*fn)(node*)){
    node* procurando;

	procurando = tree->root;

	if(tree->root == NULL){
		return;
	}
	else {
		fprintf(output, "[");
		while (1){
			if (procurando == NULL){
				fprintf(output, "?");
				fprintf(output, "]\n");
				fprintf(output, "-\n");
				break;
			}
			else if (strcmp(procurado, procurando->key) == 0){
				fprintf(output, "%s", procurando->key);
				fprintf(output, "]\n");
				fn(procurando);
				break;
			} else if (strcmp(procurado, procurando->key) < 0){
 				fprintf(output, "%s->", procurando->key);
				procurando = procurando->esquerda;
			} else if (strcmp(procurado, procurando->key) > 0){
				fprintf(output, "%s->", procurando->key);
				procurando = procurando->direita;
			};
		};
	};
};

/////////////////////////////// 

typedef struct dados{
	int quant;
    int i;
    char* palavras;
	char* procurado;
} dados;

/////////////////////////////// 

int main(int argc, char* argv[]){

	dados d;
	d.procurado = malloc(sizeof(char)*50);
	d.palavras = malloc(sizeof(char)*50);
	Tree* tree = tree_criada();

	char* entrada = argv[1];
	char* saida = argv[2];
    
   // Abrindo arquivos
    input = fopen(entrada, "r");
	output = fopen(saida, "w");

	fscanf(input, "%i", &d.quant);
	//printf("quantidade: %i \n", d.quant);
	for (size_t i = 0; i < d.quant; i++) {
		arquivo* novo_arquivo = malloc(sizeof(arquivo));
		fscanf(input, "%s", novo_arquivo->chave);
        //printf("chave: %s \n", novo_arquivo->chave);
		fscanf(input, "%i", &d.i);
        //printf("quantidade de sinonimos: %i \n", d.i);
		novo_arquivo->quant_sinonimos = d.i;
		novo_arquivo->sinonimos = malloc(d.i * sizeof(char*));
		for (size_t i = 0; i < d.i; i++){
			fscanf(input, "%s", d.palavras);
			novo_arquivo->sinonimos[i] = malloc(64 * sizeof(char));
			strcpy(novo_arquivo->sinonimos[i], d.palavras);
		};
		adicione_node(tree, novo_arquivo);
	};
	fscanf(input, "%i", &d.i);
	d.procurado = malloc(sizeof(char)*50);
	for (size_t i = 0; i < d.i; i++){
		fscanf(input, "%s", d.procurado);
		busca(tree, d.procurado, print_sinonimos);
	};

	fclose(input); 
	fclose(output);
	return 0;
};