#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* entrada = "redesocial.input";
char* saida = "redesocial.output";

typedef int bool;
#define false 0
#define true 1

//struct para a lista encadeada
typedef struct node {
	char *nome;	
	struct node* anterior;
	struct node* proximo;	
}	node;

typedef struct lista_amigos {
	node* elemento;
	int len;
} lista_amigos;


lista_amigos global_lista = {0};


node* alloc_node() {
	node* novo_node = malloc(sizeof(node));
	novo_node->anterior = NULL;
	novo_node->proximo = NULL;
	novo_node->nome = malloc(sizeof(char)*200);

	return novo_node;
};

void free_node(node* new_node) {
	free(new_node->nome);
	free(new_node);
};

lista_amigos cria_lista(void){
	lista_amigos novalista;
	novalista.elemento = NULL;
	novalista.len = 0;
	return novalista;
};

//struct para guardar os outros dados necessários e os deixar organizados
typedef struct reserve_dados {

	char cmd_amigo[200];
	int len;
	// comando = &cmd_amigo[0];  i = ? ' ' cmd_amigo[i] = '\0' 
	char* amigo;
	char* comando;
	char saida[200];

} reserve_dados;

bool lista_vazia(lista_amigos* lista){
	node* n;
	n = lista->elemento;
	if(n == NULL){
		return true;
	} else {
		return false;
	};
};

int lista_len(lista_amigos* lista){
	return lista->len;
};

// retorna zero caso
node* lista_busca(lista_amigos* lista, char* procurado){
    node* elemento_lista;
	node* comparador;

	elemento_lista = lista->elemento;
	comparador = lista->elemento;
	if(lista_vazia(lista)){
		return NULL;
	}
	else {
		while (strcmp(elemento_lista->nome, procurado) != 0){
			elemento_lista = elemento_lista->proximo;
			if (comparador == elemento_lista){
				return NULL;
			};
		};
		return elemento_lista;
	};
};

void lista_add(lista_amigos* lista, char* nome, char* comando, FILE* output) {
		//alocando memória para um no
	node* n = alloc_node();

	//copiando dado para um no
	strcpy(n->nome, nome);

	
	//lista esta vazia
	if (lista_vazia(lista)){
			
		lista->elemento = n;
		n->proximo = n;
		n->anterior = n;
		lista->len += 1;
		fprintf(output, "%s%s %s\n", "[ OK  ] ", comando, nome);	

	}
	//lista tem elemento
	else {
		// se for diferente de nulo quer dizer que existe um nome igual a esse
		if (lista_busca(lista, nome) != NULL){
			fprintf(output, "%s%s %s\n", "[ERROR] ", comando, nome);
		// Podemos adicionar
		} else{
			node* primeiro;
			//achando o primeiro elemento, assumindo que o elemento da lista é sempre o útimo
			primeiro = lista->elemento->proximo; // lista->anterior->proximo
			//1. mudar o ponteiros do primeiro elemento da lista
			primeiro->anterior = n; //primeiro->anterior = n;
			//2. mudar o ponteiro do ultimo atual
			node* antepenultimo;
			antepenultimo = lista->elemento;
			antepenultimo->proximo = n;
			//3. colocar os ponteiros do ultimo elemento
			node* ultimo;
			ultimo = n;
			ultimo->proximo  = primeiro;
			ultimo->anterior = antepenultimo;
			lista->elemento  = n; 
			lista->len += 1;
			fprintf(output, "%s%s %s\n", "[ OK  ] ", comando, nome);
		};
	};
}

void lista_mostre(lista_amigos* lista, char* procurado, FILE* output){
    node* pegue_node;
	pegue_node = lista_busca(lista, procurado);

	
	if (pegue_node != NULL){
		//!= NULL significa que o node foi encontrado
		   
		char* nome_pexibir = pegue_node->nome;
		char* nome_antes = pegue_node->anterior->nome;
		char* nome_depois = pegue_node->proximo->nome;

		fprintf(output, "%s %s%s%s%s%s\n", "[ OK  ]", nome_antes, "<-", nome_pexibir, "->", nome_depois);
	} else{
		fprintf(output, "%s%s%s%s\n", "[ERROR]", " ?<-", procurado, "->?");
	}
    
}; //mostrar o nome SE ele estiver na lista

void lista_remove(lista_amigos* lista, char* procurado, char* comando, FILE* output){
    node* pegue_principal;
    node* anterior;
    node* proximo;
	pegue_principal = NULL;
	pegue_principal = lista_busca(lista, procurado);

	if(lista_vazia(lista)) {
		fprintf(output, "%s %s %s\n", "[ERROR]", comando, procurado);
		return;
	
	}
	// Não existe node 
	else if ( pegue_principal == NULL){
		fprintf(output, "%s %s %s\n", "[ERROR]", comando, procurado);
	}
	else if (lista->len == 1) {
		free_node(lista->elemento);
		lista->elemento = NULL;
	}
	// Existe node 
	else{
		anterior = pegue_principal->anterior;
		proximo = pegue_principal->proximo;

		anterior->proximo = proximo;
		proximo->anterior = anterior;

		free_node(pegue_principal);
		lista->len -= 1;
		fprintf(output, "%s %s %s\n", "[ OK  ]", comando, procurado);
	};

}; //retirar um nome da lista 

int main(int argc, char* argv[]) {

	reserve_dados d;
	global_lista = cria_lista();

	entrada = argv[1];
	saida = argv[2];


	// Abrindo arquivos
    FILE* input = fopen(entrada, "r");
	FILE* output = fopen(saida, "w");

	//extraindo as frases do arquivo
	if (input == NULL){
		printf("%s", "peidou");};

	if (output == NULL){
		printf("%s", "peidou");
	};
	

	//fgets é a função que retira linha por linha de um arquivo
	while(fgets(d.cmd_amigo, sizeof(d.cmd_amigo), input) != NULL){
		d.len = strlen(d.cmd_amigo);
		d.cmd_amigo[d.len-1] = '\0';
		d.comando = &d.cmd_amigo[0];

		//printf("\n\nd.len = %d\n",d.len );
		for (size_t i = 0; i < d.len; i++){
			if (d.comando[i] == ' '){
				d.comando[i] = '\0';
				d.amigo = &d.cmd_amigo[i+1];
				break;
			};   
		};

		if (strcmp("ADD\0", d.comando) == 0) {
			
			lista_add(&global_lista, d.amigo, d.comando, output);
	
		} else if (strcmp("SHOW\0", d.comando) == 0) {

			lista_mostre(&global_lista, d.amigo, output);

		} else if (strcmp("REMOVE\0", d.comando) == 0) {

			lista_remove(&global_lista, d.amigo, d.comando, output);
		
		};
	};

	//Escrevendo saída:



	//fechando arquivos
	fclose(input);
	fclose(output);
	return 0;
}