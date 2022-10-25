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
    bool aux;
} node;

node* alloc_node(){
    node* new_node = malloc(sizeof(node));
    new_node->filho = malloc(26* sizeof(char));

    for (size_t i = 0; i < 26; i++){
        new_node->filho[i] == NULL;
    };
    
    new_node->aux = false;

    return new_node;
};

void insere_node(node** root, char* palavra){
    if (root == NULL){
        *root = alloc_node();  
    };
    
    unsigned char* texto = (unsigned char*)palavra;
    node* temp = root;
    int len = strlen(palavra);
    
    for(size_t i = 0; i < len; i++){
      if (temp->filho[texto[i]] == NULL){
        temp->filho[texto[i]] = alloc_node();
      };  
      temp = temp->filho[texto[i]];
    };
    if (temp->aux){
        return false;
    } else {
        temp->aux = true;
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