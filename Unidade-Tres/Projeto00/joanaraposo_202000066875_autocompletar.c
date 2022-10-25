#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* entrada = "autocompletar.input";
char* saida = "autocompletar.output";

FILE* input;
FILE* output;

typedef enum  {
    false = 0 , true = 1 
} bool;

typedef struct node {
    //char letra;
    struct node** filho; 
    bool palavra_ornot;
} node;

node* alloc_node(){
    node* new_node = malloc(sizeof(node)); //aloca espaço do tam do node
    new_node->filho = malloc(26* sizeof(char));

    for (size_t i = 0; i < 26; i++){
        new_node->filho[i] == NULL;
    };
    
    new_node->palavra_ornot = false;

    return new_node;
};

typedef struct Tree_trie{
    node* root;
} Tree_trie;

Tree_trie* criar_tree(){
    Tree_trie* nova_tree = malloc(sizeof(Tree_trie));
    nova_tree->root = NULL;
    return nova_tree;
};

void insere_node(Tree_trie* Tree, char* palavra){
    node* new_node = alloc_node();


    if (Tree->root == NULL){
        Tree->root = new_node;
    };
    
    unsigned char* texto = (unsigned char*)palavra;
    node* temp = Tree->root;
    int len = strlen(palavra);
    
    for(size_t i = 0; i < len; i++){
      if (temp->filho[texto[i]] == NULL){
        temp->filho[texto[i]] = alloc_node();
      };  
      temp = temp->filho[texto[i]];
    };
    if (temp->palavra_ornot){
        return false;
    } else {
        temp->palavra_ornot = true;
        return true;
    };
    
};

node* busca (node* an_no, char * p , int d) {
    node* no = NULL;
    if(no != NULL) {
    if( d == strlen ( p ))
        no = an_no;
    } else{
        no = busca (an_no->filho [ indice (p , d ) ] , p , d + 1) ;
    };
    return no;
};

typedef struct dados{
    int quant_termos;
    char termo[20];
    int num_requisicoes;
    char requisicao[20];
}dados;

int main(int argc, char* argv[]) {  
    dados d;
    Tree_trie* global_tree = criar_tree();

    entrada = argv[1];
	saida = argv[2];

	// Abrindo arquivos
    FILE* input = fopen(entrada, "r");
	FILE* output = fopen(saida, "w");

    fscanf(input, "%i", &d.quant_termos);
    printf("termos: %i", d.quant_termos);
    for (size_t i = 0; i < d.quant_termos; i++){
        fscanf(input, "%s", d.termo);
        printf("termo: %s", d.termo);
    };

    fscanf(input, "%i", &d.num_requisicoes);
    printf("requisicoes: %i", d.num_requisicoes);
    for (size_t i = 0; i < d.num_requisicoes; i++){
        fscanf(input, "%s", d.requisicao);
        printf("requisicao: %s", d.requisicao);
    };
    
    fclose(input);
    fclose(output);
    return 0;
};