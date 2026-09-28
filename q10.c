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
#define TAM_FILA 5

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

typedef struct {
    Veiculo array[TAM_FILA];
    int primeiro;
    int n;
} Fila;

void iniciarFila(Fila* f) {
    f->primeiro = 0;
    f->n = 0;
}

int enfileirar(
    Fila* f,
    Veiculo v
) {
    if (f->n == TAM_FILA) {
        return 0;
    }

    int ultimo =
        (f->primeiro + f->n) % TAM_FILA;

    f->array[ultimo] = v;

    f->n++;

    return 1;
}

int desenfileirar(
    Fila* f,
    Veiculo* resp
) {
    if (f->n == 0) {
        return 0;
    }

    *resp = f->array[f->primeiro];

    f->primeiro =
        (f->primeiro + 1) % TAM_FILA;

    f->n--;

    return 1;
}

void mostrarFila(Fila* f) {
    char saida[600];

    for (int i = 0; i < f->n; i++) {
        formatVeiculo(
            f->array[
                (f->primeiro + i) % TAM_FILA
            ],
            saida
        );

        printf(
            "%s\n",
            saida
        );
    }
}

void inserirComRemocao(
    Fila* f,
    Veiculo v
) {
    if (f->n == TAM_FILA) {
        Veiculo removido;

        desenfileirar(
            f,
            &removido
        );

        printf(
            "(R)%s %s\n",
            removido.marca,
            removido.modelo
        );
    }

    enfileirar(
        f,
        v
    );
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

    Fila fila;

    iniciarFila(
        &fila
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
            inserirComRemocao(
                &fila,
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

        int id = 0;

        sscanf(
            linha,
            "%7s %d",
            cmd,
            &id
        );

        if (strcmp(cmd, "I") == 0) {
            Veiculo* v = pesquisar(
                vet,
                n,
                id
            );

            if (v != NULL) {
                inserirComRemocao(
                    &fila,
                    *v
                );
            }
        } else {
            Veiculo removido;

            if (
                desenfileirar(
                    &fila,
                    &removido
                )
            ) {
                printf(
                    "(R)%s %s\n",
                    removido.marca,
                    removido.modelo
                );
            }
        }
    }

    mostrarFila(
        &fila
    );

    free(vet);

    return 0;
}