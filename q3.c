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
#define MAX_PEDIDOS 1000

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

Veiculo* lerCsv(char* caminhoArquivo, int* n) {
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

char minuscula(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }

    return c;
}

int compararIgnorando(
    char* a,
    char* b
) {
    int i = 0;

    while (
        a[i] != '\0' &&
        b[i] != '\0' &&
        minuscula(a[i]) == minuscula(b[i])
    ) {
        i++;
    }

    return minuscula(a[i]) - minuscula(b[i]);
}

void selecao(
    Veiculo* a,
    int n
) {
    for (int i = 0; i < n - 1; i++) {
        int menor = i;

        for (int j = i + 1; j < n; j++) {
            if (
                compararIgnorando(
                    a[j].modelo,
                    a[menor].modelo
                ) < 0
            ) {
                menor = j;
            }
        }

        if (menor != i) {
            Veiculo tmp = a[i];

            a[i] = a[menor];
            a[menor] = tmp;
        }
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

    Veiculo* a = malloc(
        MAX_PEDIDOS * sizeof(Veiculo)
    );

    int qtd = 0;
    int id;

    char saida[600];

    while (
        scanf("%d", &id) == 1 &&
        id != -1
    ) {
        Veiculo* v = pesquisar(
            vet,
            n,
            id
        );

        if (
            v != NULL &&
            qtd < MAX_PEDIDOS
        ) {
            a[qtd] = *v;
            qtd++;
        }
    }

    selecao(
        a,
        qtd
    );

    for (int i = 0; i < qtd; i++) {
        formatVeiculo(
            a[i],
            saida
        );

        printf(
            "%s\n",
            saida
        );
    }

    free(a);
    free(vet);

    return 0;
}