#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef enum  {
    false = 0 , true = 1 
} bool;

char* entrada = "gabarito.input";
char* saida = "mysistemadeimpressao.output";

/*>>--------------------------------- Utils ---------------------------------*/
// retorna o index da primeira ocorrencia de um dado charactere em um string
int str_indexof(char* string, char token) {
    int index; char curr;
    
    index = 0;
    while ((curr = string[index]) != '\0') {
        if ( curr == token) {
            return index;
        }
        index += 1;
    }
    return -1;
}
/*<<--------------------------------- Utils ---------------------------------*/

/*>>--------------------------------- PILHA ---------------------------------*/


typedef struct pilha {
    int capacity;
    int count;
    void **data;
} pilha;

void* pilha_pop(pilha* p) {
    if(p->count == 0){
        return NULL;
    }
    p->count -= 1;
    void* result = p->data[p->count];
    
    return result;
}

void pilha_add(pilha* p, void* object) {
    *(p->data + p->count) = object;
    p->count += 1 ;
}

pilha pilha_criar(int capacity) {
    pilha nova_pilha;
    nova_pilha.capacity = capacity;
    nova_pilha.count = 0;
    nova_pilha.data = malloc(sizeof(void*)*capacity);   
    return nova_pilha;
}

/*<<--------------------------------- PILHA ---------------------------------*/

/*>>--------------------------------- FILA ---------------------------------*/

typedef struct doc_node doc_node;
typedef struct impressora_node impressora_node; 

struct impressora_node{
    char* nome;
    doc_node* primeiro_doc;
    struct impressora_node* proximo;
    struct impressora_node* anterior;
};

typedef struct doc_node{
    char* nome;
    int paginas_total, paginas_processadas;
    struct doc_node* proximo;
} doc_node;

typedef struct fila_impressora {
    impressora_node* inicio;
    impressora_node* atualiza_atual;
} fila;

doc_node* alloc_doc_node(){
    doc_node* doc = (doc_node*)malloc(sizeof(doc_node));
    
    doc->paginas_processadas = doc->paginas_total = -1;
    doc->proximo = NULL;

	doc->nome = (char*)malloc(sizeof(char)*50);
    return doc;
}

void free_doc_node(doc_node* doc){
	free(doc->nome);
    free(doc);
}

impressora_node* alloc_node() {
    impressora_node* novo_node = (impressora_node*)malloc(sizeof(impressora_node));
    
    novo_node->proximo  = NULL;
    novo_node->anterior = NULL;
    novo_node->primeiro_doc = NULL;

	novo_node->nome = (char*)malloc(sizeof(char)*50);
    return novo_node;
};

void free_node(impressora_node* new_node) {
	free(new_node->nome);
	free(new_node);
};


/*/////////////////////////////////*/
/*/////////// ESTRUTURA DE PILHA*/

typedef struct dados{
    
    char linha[210];
    int quantidadedoc;
    int quantidadeimp;

} dados;

/*/////////////////////////////////*/
/*/////////// ESTRUTURA */

fila fila_criar(void){

    fila nova_fila;
    nova_fila.inicio = NULL;
    nova_fila.atualiza_atual = NULL;

    return nova_fila;
};
void fila_adicionar(fila* fila_requerida, char* elemento){
    //Alocando espaço para o no
    impressora_node* n = alloc_node();

    strcpy(n->nome, elemento);

    if (fila_requerida->inicio == NULL && fila_requerida->atualiza_atual == NULL){
        fila_requerida->inicio = fila_requerida->atualiza_atual = n;
    } else{
        fila_requerida->atualiza_atual->proximo = n;
        n->anterior = fila_requerida->atualiza_atual;
        fila_requerida->atualiza_atual = n;
    };
    return;
};

void fila_adicionar_node(fila* fila_requerida, impressora_node* n) {
    n->proximo = NULL;
    n->anterior = NULL;
    impressora_node* ultimo = fila_requerida->atualiza_atual;
    // fila vazia
    if (fila_requerida->inicio == NULL && fila_requerida->atualiza_atual == NULL){
        fila_requerida->inicio = fila_requerida->atualiza_atual = n;
    } else {
        ultimo->proximo = n;
        n->anterior = ultimo;
        fila_requerida->atualiza_atual = n;
    };
    return;
};

void fila_tirar(fila* requerida) {
    impressora_node *inicio, *fim;

    inicio = requerida->inicio;
    fim = requerida->atualiza_atual;

    if(inicio == fim & inicio == NULL){
        return;
    }
    if (inicio == fim){
        free_node(inicio);
        requerida->inicio = requerida ->atualiza_atual = NULL;
    } else{
        free(inicio);
        requerida->inicio = requerida->inicio->proximo;
        requerida->inicio->anterior = NULL;
    };
};

void fila_tirar_no_free(fila* requerida) {
    impressora_node *inicio, *fim;

    inicio = requerida->inicio;
    fim = requerida->atualiza_atual;

    if(inicio == fim & inicio == NULL){
        return;
    }
    if (inicio == fim){
        requerida->inicio = requerida ->atualiza_atual = NULL;
    }
     else{
        requerida->inicio = requerida->inicio->proximo;
        requerida->inicio->anterior = NULL;

    };
};

void fila_tirar_node_no_free(fila* requerida, impressora_node* impressora) {
    impressora_node *antes, *depois;
    //impressora vazia
    if (impressora == NULL)
        return;
    antes = impressora->anterior;
    depois = impressora->proximo;

    // impressor é o nó inicial e é nó final  é e a fila só te 1 elemnto
    if(antes == depois & depois == NULL){
        assert(impressora == requerida->inicio);
       fila_tirar_no_free(requerida);
    }
    // impressora é o nó incial da fila e a fila tem mais de 1 elemento;
    else if (antes == NULL & depois != NULL) {
        fila_tirar_no_free(requerida);
    }
    // impressora é o nó final da fila e a fila tem mais de 1 elemento;
    else if (antes != NULL & depois == NULL){
        antes->proximo = NULL;
        requerida->atualiza_atual = antes;
    }
    // impressora não é final nem inicial, e tem mais de um elemento
    else {
        antes->proximo  = depois;
		depois->anterior = antes;
    }
    return;
}

impressora_node* fila_peek(fila* requerida){
    return requerida->inicio;
}

bool fila_vazia(fila* requerida) {
    return (requerida->inicio == requerida->atualiza_atual && requerida->atualiza_atual == NULL);
}

void impressora_fprint_node(impressora_node* impressora, FILE* output) {
    fprintf(output,"[%s]",impressora->nome);    
    doc_node* doc = impressora->primeiro_doc;
    while (doc) {
        if (doc->proximo == NULL)
            fprintf(output," %s-%dp\n",doc->nome,doc->paginas_total);
        else
            fprintf(output," %s-%dp,",doc->nome,doc->paginas_total);
        doc = doc->proximo;    
    }
}

void adicionar_doc(impressora_node* impressora, char* nome, int n_paginas) {
    doc_node* doc = alloc_doc_node();
    strcpy(doc->nome, nome);
    doc->paginas_processadas = 0;
    doc->paginas_total = n_paginas;
    doc->proximo = impressora->primeiro_doc;

    impressora->primeiro_doc = doc; 
}

//>>---------- LOGICA ----------//
bool processe_impressoras_ocupadas(fila* ocupadas, fila* livres, pilha* pilha_docs) {
    impressora_node* curr_impressora = ocupadas->inicio;
    bool still_docs_to_process = false;
    while(curr_impressora) {
        doc_node* doc = curr_impressora->primeiro_doc;

        doc->paginas_processadas += 1;
        // SE verdade, então terminamos processamento
        if(doc->paginas_processadas == doc->paginas_total) {
            impressora_node* terminou_impressora = curr_impressora; 
            curr_impressora = curr_impressora->proximo;
            fila_tirar_node_no_free(ocupadas, terminou_impressora);
            fila_adicionar_node(livres, terminou_impressora);
            pilha_add(pilha_docs, doc);
        }
        // ainda falta processar
        else {
            still_docs_to_process = true;
            curr_impressora = curr_impressora->proximo;
        }
    }
    if( fila_vazia(livres)) {
        return processe_impressoras_ocupadas(ocupadas,livres,pilha_docs);
    }
    return still_docs_to_process;
}

//<<---------- LOGICA ----------//

/*/////////////////////////////////*/


//>>---------- Main ----------//
int main(int argc, char const *argv[]){
    dados d;
    fila fila_impressoras_livres , fila_impressoras_ocupadas;
    pilha pilha_docs_finalizados;
    
    if(argc > 2) {
        entrada = (char*)argv[1];
        saida   = (char*)argv[2];
    }
    //criando as referentes filas;
    fila_impressoras_livres = fila_criar();
    fila_impressoras_ocupadas = fila_criar();

    //Abrindo arquivos
    FILE* input = fopen(entrada, "r");
	FILE* output = fopen(saida, "w");

    // pegamos a qntd de impressoras na primeira linha
    fgets(d.linha, sizeof(d.linha), input);
    int n_impressoras = atoi(d.linha);
    
    for (size_t i = 0; i < n_impressoras; i++) {
        fgets(d.linha, sizeof(d.linha), input);
        int len = strlen(d.linha);
        d.linha[len-1] = '\0';
        fila_adicionar(&fila_impressoras_livres, d.linha);
    }
    
    int total_pages = 0;
    fgets(d.linha, sizeof(d.linha), input);
    int n_docs = atoi(d.linha);
    pilha_docs_finalizados = pilha_criar(n_docs);
    while (n_docs--) {
        fgets(d.linha, sizeof(d.linha), input);
        int index_do_split = str_indexof(d.linha,' ');
        d.linha[index_do_split] = '\0';


        char* doc_name = d.linha; 
        int doc_pages_count = atoi(&d.linha[index_do_split+1]);
        total_pages += doc_pages_count;
        impressora_node* impressora_disponivel;
        // Se não há impressoras disponiveis atualiza todos as paginas e vê qual o proximo viadinho (doc) a pegar;
        if(fila_vazia(&fila_impressoras_livres)){
            // atualiza a quantidade de paginas processas de todas as
            // impressoras ocupadas de maneira a fazer pelo menos uma impressora está livre.
            processe_impressoras_ocupadas(&fila_impressoras_ocupadas, &fila_impressoras_livres, &pilha_docs_finalizados); 
        }
        // Sempre deveriamos ter algo aqui pois processamos as
        // filas ocupadas anteriormente de maneira a manter sempre pelo menos UMA impressora livre
        if (impressora_disponivel = fila_peek(&fila_impressoras_livres)) {
            fila_tirar_no_free(&fila_impressoras_livres);
            adicionar_doc(impressora_disponivel, doc_name,doc_pages_count);
            impressora_fprint_node(impressora_disponivel,output);
            fila_adicionar_node(&fila_impressoras_ocupadas, impressora_disponivel);
        }
    };

    while (!fila_vazia(&fila_impressoras_ocupadas)) { 
        processe_impressoras_ocupadas(&fila_impressoras_ocupadas, &fila_impressoras_livres, &pilha_docs_finalizados);
    }
    
    fprintf(output,"%dp\n", total_pages);
    doc_node* docs;
    while(docs = pilha_pop(&pilha_docs_finalizados)){
        fprintf(output,"%s-%dp\n", docs->nome, docs->paginas_total);
    }
    //fechando arquivos
    fclose(input);
	fclose(output);

    return 0;
}
//<<---------- Main ----------//

