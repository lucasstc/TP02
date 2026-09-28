#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_TEXTO 60
#define MAX_COMB 5
#define TAM_COMB 20
#define CAMINHO "/tmp/veiculos.csv"
#define MAX_VEICULOS 600
#define TAM_LINHA 512

typedef struct {
    int ano;
    int mes;
    int dia;
} Data;

typedef struct {
    int id;
    char marca[TAM_TEXTO];
    char modelo[TAM_TEXTO];
    int ano;
    char categoria[TAM_TEXTO];
    char combustivel[MAX_COMB][TAM_COMB];
    int numCombustiveis;
    int cilindros;
    double cilindrada;
    char transmissao[TAM_TEXTO];
    char tracao[TAM_TEXTO];
    double consumoCidade;
    double consumoEstrada;
    double co2;
    bool turbo;
    Data dataRegistro;
} Veiculo;

Data parseData(char* s) {
    Data d;

    char* token = strtok(s, "-");

    d.ano = atoi(token);

    token = strtok(NULL, "-");
    d.mes = atoi(token);

    token = strtok(NULL, "-");
    d.dia = atoi(token);

    return d;
}

void formatData(Data d, char* buffer) {
    sprintf(
        buffer,
        "%02d/%02d/%04d",
        d.dia,
        d.mes,
        d.ano
    );
}

Veiculo* parseVeiculo(char* s) {
    Veiculo* v = malloc(sizeof(Veiculo));

    if (v == NULL) {
        return NULL;
    }

    char* token = strtok(s, ",");

    if (token == NULL) {
        free(v);
        return NULL;
    }

    v->id = atoi(token);

    token = strtok(NULL, ",");
    sprintf(v->marca, "%s", token);

    token = strtok(NULL, ",");
    sprintf(v->modelo, "%s", token);

    token = strtok(NULL, ",");
    v->ano = atoi(token);

    token = strtok(NULL, ",");
    sprintf(v->categoria, "%s", token);

    char* combToken = strtok(NULL, ",");

    token = strtok(NULL, ",");
    v->cilindros = atoi(token);

    token = strtok(NULL, ",");
    v->cilindrada = atof(token);

    token = strtok(NULL, ",");
    sprintf(v->transmissao, "%s", token);

    token = strtok(NULL, ",");
    sprintf(v->tracao, "%s", token);

    token = strtok(NULL, ",");
    v->consumoCidade = atof(token);

    token = strtok(NULL, ",");
    v->consumoEstrada = atof(token);

    token = strtok(NULL, ",");
    v->co2 = atof(token);

    token = strtok(NULL, ",");
    v->turbo = (strcmp(token, "true") == 0);

    token = strtok(NULL, ",");
    v->dataRegistro = parseData(token);

    v->numCombustiveis = 0;

    char* comb = strtok(combToken, ";");

    while (
        comb != NULL &&
        v->numCombustiveis < MAX_COMB
    ) {
        sprintf(
            v->combustivel[v->numCombustiveis],
            "%s",
            comb
        );

        v->numCombustiveis++;

        comb = strtok(NULL, ";");
    }

    return v;
}

void formatVeiculo(Veiculo v, char* buffer) {
    char combs[MAX_COMB * (TAM_COMB + 1)];

    int pos = 0;

    for (int i = 0; i < v.numCombustiveis; i++) {
        if (i != 0) {
            pos += sprintf(
                combs + pos,
                ","
            );
        }

        pos += sprintf(
            combs + pos,
            "%s",
            v.combustivel[i]
        );
    }

    combs[pos] = '\0';

    char dataStr[11];

    formatData(
        v.dataRegistro,
        dataStr
    );

    sprintf(
        buffer,
        "[%d ## %s ## %s ## %d ## %s ## [%s] ## %d ## %.1f ## %s ## %s ## %.2f ## %.2f ## %.1f ## %s ## %s]",
        v.id,
        v.marca,
        v.modelo,
        v.ano,
        v.categoria,
        combs,
        v.cilindros,
        v.cilindrada,
        v.transmissao,
        v.tracao,
        v.consumoCidade,
        v.consumoEstrada,
        v.co2,
        v.turbo ? "true" : "false",
        dataStr
    );
}

Veiculo* lerCsv(
    char* caminhoArquivo,
    int* n
) {
    FILE* f = fopen(
        caminhoArquivo,
        "r"
    );

    if (f == NULL) {
        *n = 0;
        return NULL;
    }

    Veiculo* vet = malloc(
        MAX_VEICULOS * sizeof(Veiculo)
    );

    if (vet == NULL) {
        fclose(f);
        *n = 0;
        return NULL;
    }

    char linha[TAM_LINHA];

    *n = 0;

    fgets(
        linha,
        TAM_LINHA,
        f
    );

    while (
        *n < MAX_VEICULOS &&
        fgets(linha, TAM_LINHA, f) != NULL
    ) {
        Veiculo* p = parseVeiculo(linha);

        if (p != NULL) {
            vet[*n] = *p;
            (*n)++;

            free(p);
        }
    }

    fclose(f);

    return vet;
}

Veiculo* pesquisar(
    Veiculo* vet,
    int n,
    int id
) {
    for (int i = 0; i < n; i++) {
        if (vet[i].id == id) {
            return &vet[i];
        }
    }

    return NULL;
}

int lerLinha(
    char* buf,
    int tam
) {
    if (fgets(buf, tam, stdin) == NULL) {
        return 0;
    }

    for (int i = 0; buf[i] != '\0'; i++) {
        if (
            buf[i] == '\n' ||
            buf[i] == '\r'
        ) {
            buf[i] = '\0';
            break;
        }
    }

    return 1;
}

typedef struct Celula {
    Veiculo elemento;
    struct Celula* prox;
} Celula;

typedef struct {
    Celula* primeiro;
    Celula* ultimo;
    int n;
} Lista;

void iniciarLista(Lista* l) {
    l->primeiro = malloc(sizeof(Celula));

    l->primeiro->prox = NULL;
    l->ultimo = l->primeiro;
    l->n = 0;
}

void inserirInicio(
    Lista* l,
    Veiculo v
) {
    Celula* tmp = malloc(sizeof(Celula));

    tmp->elemento = v;
    tmp->prox = l->primeiro->prox;

    l->primeiro->prox = tmp;

    if (l->primeiro == l->ultimo) {
        l->ultimo = tmp;
    }

    l->n++;
}

void inserirFim(
    Lista* l,
    Veiculo v
) {
    Celula* tmp = malloc(sizeof(Celula));

    tmp->elemento = v;
    tmp->prox = NULL;

    l->ultimo->prox = tmp;
    l->ultimo = tmp;

    l->n++;
}

int inserir(
    Lista* l,
    Veiculo v,
    int posicao
) {
    if (
        posicao < 0 ||
        posicao > l->n
    ) {
        return 0;
    }

    if (posicao == 0) {
        inserirInicio(
            l,
            v
        );
    } else if (posicao == l->n) {
        inserirFim(
            l,
            v
        );
    } else {
        Celula* i = l->primeiro;

        for (int j = 0; j < posicao; j++) {
            i = i->prox;
        }

        Celula* tmp = malloc(sizeof(Celula));

        tmp->elemento = v;
        tmp->prox = i->prox;

        i->prox = tmp;

        l->n++;
    }

    return 1;
}

int removerInicio(
    Lista* l,
    Veiculo* resp
) {
    if (l->primeiro == l->ultimo) {
        return 0;
    }

    Celula* tmp = l->primeiro->prox;

    *resp = tmp->elemento;

    l->primeiro->prox = tmp->prox;

    if (tmp == l->ultimo) {
        l->ultimo = l->primeiro;
    }

    free(tmp);

    l->n--;

    return 1;
}

int removerFim(
    Lista* l,
    Veiculo* resp
) {
    if (l->primeiro == l->ultimo) {
        return 0;
    }

    Celula* i = l->primeiro;

    while (i->prox != l->ultimo) {
        i = i->prox;
    }

    *resp = l->ultimo->elemento;

    free(l->ultimo);

    l->ultimo = i;
    i->prox = NULL;

    l->n--;

    return 1;
}

int remover(
    Lista* l,
    int posicao,
    Veiculo* resp
) {
    if (
        l->primeiro == l->ultimo ||
        posicao < 0 ||
        posicao >= l->n
    ) {
        return 0;
    }

    if (posicao == 0) {
        return removerInicio(
            l,
            resp
        );
    }

    if (posicao == l->n - 1) {
        return removerFim(
            l,
            resp
        );
    }

    Celula* i = l->primeiro;

    for (int j = 0; j < posicao; j++) {
        i = i->prox;
    }

    Celula* tmp = i->prox;

    *resp = tmp->elemento;

    i->prox = tmp->prox;

    free(tmp);

    l->n--;

    return 1;
}

void mostrar(Lista* l) {
    char saida[600];

    for (
        Celula* i = l->primeiro->prox;
        i != NULL;
        i = i->prox
    ) {
        formatVeiculo(
            i->elemento,
            saida
        );

        printf(
            "%s\n",
            saida
        );
    }
}

void liberarLista(Lista* l) {
    Celula* i = l->primeiro;

    while (i != NULL) {
        Celula* prox = i->prox;

        free(i);

        i = prox;
    }
}

int main() {
    int n;

    Veiculo* vet = lerCsv(
        CAMINHO,
        &n
    );

    if (vet == NULL) {
        return 1;
    }

    Lista lista;

    iniciarLista(
        &lista
    );

    char linha[TAM_LINHA];

    while (
        lerLinha(
            linha,
            TAM_LINHA
        )
    ) {
        int id = atoi(linha);

        if (id == -1) {
            break;
        }

        Veiculo* v = pesquisar(
            vet,
            n,
            id
        );

        if (v != NULL) {
            inserirFim(
                &lista,
                *v
            );
        }
    }

    lerLinha(
        linha,
        TAM_LINHA
    );

    int qtd = atoi(linha);

    for (int i = 0; i < qtd; i++) {
        lerLinha(
            linha,
            TAM_LINHA
        );

        char cmd[8];

        int a = 0;
        int b = 0;

        sscanf(
            linha,
            "%7s %d %d",
            cmd,
            &a,
            &b
        );

        if (strcmp(cmd, "II") == 0) {
            Veiculo* v = pesquisar(
                vet,
                n,
                a
            );

            if (v != NULL) {
                inserirInicio(
                    &lista,
                    *v
                );
            }
        } else if (strcmp(cmd, "IF") == 0) {
            Veiculo* v = pesquisar(
                vet,
                n,
                a
            );

            if (v != NULL) {
                inserirFim(
                    &lista,
                    *v
                );
            }
        } else if (strcmp(cmd, "I*") == 0) {
            Veiculo* v = pesquisar(
                vet,
                n,
                b
            );

            if (v != NULL) {
                inserir(
                    &lista,
                    *v,
                    a
                );
            }
        } else {
            Veiculo removido;

            int ok = 0;

            if (strcmp(cmd, "RI") == 0) {
                ok = removerInicio(
                    &lista,
                    &removido
                );
            } else if (strcmp(cmd, "RF") == 0) {
                ok = removerFim(
                    &lista,
                    &removido
                );
            } else {
                ok = remover(
                    &lista,
                    a,
                    &removido
                );
            }

            if (ok) {
                printf(
                    "(R)%s %s\n",
                    removido.marca,
                    removido.modelo
                );
            }
        }
    }

    mostrar(
        &lista
    );

    liberarLista(
        &lista
    );

    free(vet);

    return 0;
}