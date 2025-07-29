#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define N_CIDADES 10
#define MAX_CAMINHOS 30
#define MAX_PASSOS 20

const char* nomeCidades[N_CIDADES] = {
    "Corrente", "Gilbues", "Bom Jesus", "Floriano", "Oeiras",
    "Teresina", "Campo Maior", "Piripiri", "Luís Correia", "Parnaíba"
};

typedef struct {
    int adj[N_CIDADES][N_CIDADES];
    int nVertices;
} Grafo;

Grafo* grafo_criar(int n) {
    Grafo* g = malloc(sizeof(Grafo));
    g->nVertices = n;
    memset(g->adj, 0, sizeof(g->adj));
    return g;
}

void grafo_adicionarAresta(Grafo* g, int origem, int destino) {
    g->adj[origem][destino] = 1;
}

void grafo_mostrar(Grafo* g) {
    printf("Conexoes entre cidades:\n");
    for (int i = 0; i < g->nVertices; i++) {
        printf("%s:", nomeCidades[i]);
        int temLigacao = 0;
        for (int j = 0; j < g->nVertices; j++) {
            if (g->adj[i][j]) {
                printf(" -> %s", nomeCidades[j]);
                temLigacao = 1;
            }
        }
        if (!temLigacao) printf(" (sem ligacoes)");
        printf("\n");
    }
}

void listarCaminhos(Grafo* g, int atual, int destino, bool visitado[], int caminho[], int len,
                    int caminhos[MAX_CAMINHOS][MAX_PASSOS], int* totalCaminhos, int tamanhos[MAX_CAMINHOS]) {
    visitado[atual] = true;
    caminho[len] = atual;
    len++;

    if (atual == destino) {
        memcpy(caminhos[*totalCaminhos], caminho, len * sizeof(int));
        tamanhos[*totalCaminhos] = len;
        (*totalCaminhos)++;
    } else {
        for (int v = 0; v < g->nVertices; v++) {
            if (g->adj[atual][v] && !visitado[v]) {
                listarCaminhos(g, v, destino, visitado, caminho, len, caminhos, totalCaminhos, tamanhos);
            }
        }
    }
    visitado[atual] = false;
}

void mostrarCaminhos(int caminhos[MAX_CAMINHOS][MAX_PASSOS], int total, int tamanhos[MAX_CAMINHOS]) {
    printf("\nTodos os caminhos possiveis de Corrente a Parnaíba:\n");
    for (int i = 0; i < total; i++) {
        for (int j = 0; j < tamanhos[i]; j++) {
            printf("%s", nomeCidades[caminhos[i][j]]);
            if (j < tamanhos[i] - 1) printf(" -> ");
        }
        printf("\n");
    }
}
void menorCaminho(Grafo* g, int origem, int destino) {
    int fila[N_CIDADES*N_CIDADES], anterior[N_CIDADES], visitado[N_CIDADES] = {0};
    int frente = 0, tras = 0;

    for (int i=0; i<N_CIDADES; i++) anterior[i] = -1;
    fila[tras++] = origem;
    visitado[origem] = 1;

    while (frente < tras) {
        int u = fila[frente++];
        if (u == destino) break;
        for (int v = 0; v < g->nVertices; v++) {
            if (g->adj[u][v] && !visitado[v]) {
                fila[tras++] = v;
                anterior[v] = u;
                visitado[v] = 1;
            }
        }
    }


    int caminho[N_CIDADES], tam = 0, atual = destino;
    while (atual != -1) {
        caminho[tam++] = atual;
        atual = anterior[atual];
    }
    if (tam == 1 && caminho[0] != origem) {
        printf("\nNao existe caminho de Corrente a Parnaiba\n");
        return;
    }
    printf("\nMenor caminho: ");
    for (int i = tam-1; i >= 0; i--) {
        printf("%s", nomeCidades[caminho[i]]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

void maiorCaminhoDFS(Grafo* g, int atual, int destino, bool visitado[], int caminho[], int len, int* maxTam, int melhor[]) {
    visitado[atual] = true;
    caminho[len++] = atual;
    if (atual == destino) {
        if (len > *maxTam) {
            *maxTam = len;
            memcpy(melhor, caminho, len * sizeof(int));
        }
    } else {
        for (int v = 0; v < g->nVertices; v++) {
            if (g->adj[atual][v] && !visitado[v]) {
                maiorCaminhoDFS(g, v, destino, visitado, caminho, len, maxTam, melhor);
            }
        }
    }
    visitado[atual] = false;
}

void mostrarMaior(Grafo* g, int origem, int destino) {
    int maxTam = 0, caminho[N_CIDADES], melhor[N_CIDADES];
    bool visitado[N_CIDADES] = {0};
    maiorCaminhoDFS(g, origem, destino, visitado, caminho, 0, &maxTam, melhor);
    if (maxTam == 0) {
        printf("\nNao existe caminho de Corrente a Parnaiba\n");
        return;
    }
    printf("\nMaior caminho (mais cidades): ");
    for (int i = 0; i < maxTam; i++) {
        printf("%s", nomeCidades[melhor[i]]);
        if (i < maxTam-1) printf(" -> ");
    }
    printf("\n");
}


int main() {
    Grafo* G = grafo_criar(N_CIDADES);


    grafo_adicionarAresta(G, 0, 1); // Corrente → Gilbués
    grafo_adicionarAresta(G, 1, 2); // Gilbués → Bom Jesus
    grafo_adicionarAresta(G, 2, 3); // Bom Jesus → Floriano
    grafo_adicionarAresta(G, 3, 4); // Floriano → Oeiras
    grafo_adicionarAresta(G, 4, 5); // Oeiras → Teresina
    grafo_adicionarAresta(G, 5, 6); // Teresina → Campo Maior
    grafo_adicionarAresta(G, 6, 7); // Campo Maior → Piripiri
    grafo_adicionarAresta(G, 7, 8); // Piripiri → Luís Correia
    grafo_adicionarAresta(G, 8, 9); // Luís Correia → Parnaíba
    grafo_adicionarAresta(G, 5, 9); // Teresina → Parnaíba
    grafo_adicionarAresta(G, 2, 5); // Bom Jesus → Teresina

    grafo_mostrar(G);

    int caminhos[MAX_CAMINHOS][MAX_PASSOS], tamanhos[MAX_CAMINHOS], total = 0;
    bool visitado[N_CIDADES] = {0};
    int caminhoTemp[MAX_PASSOS];
    listarCaminhos(G, 0, 9, visitado, caminhoTemp, 0, caminhos, &total, tamanhos);
    mostrarCaminhos(caminhos, total, tamanhos);

    menorCaminho(G, 0, 9);

    mostrarMaior(G, 0, 9);

    free(G);
    return 0;
}