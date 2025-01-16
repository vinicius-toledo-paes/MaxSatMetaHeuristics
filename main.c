#include <stdio.h>
#include <time.h>

#include "genetic.h"
#include "greedy.h"


int main(int argc, char *argv[]){

    char heuristica[8];
    
    if (argc < 2){
        printf("ERRO: nome de arquivo não encontrado \n\n");
        exit(1);
    } else if (argc < 3){
        if (rand()%2){
            heuristica[0] = 'g';
            heuristica[1] = 'r';
            heuristica[2] = 'e';
            heuristica[3] = 'e';
            heuristica[4] = 'd';
            heuristica[5] = 'y';
            heuristica[6] = '\0';
        } else {
            heuristica[0] = 'g';
            heuristica[1] = 'e';
            heuristica[2] = 'n';
            heuristica[3] = 'e';
            heuristica[4] = 't';
            heuristica[5] = 'i';
            heuristica[6] = 'c';
            heuristica[7] = '\0';
        }
    } else {
        for(char i = 0, c = argv[2][0]; c != '\0' && i < 7; i++, c = argv[2][i]){
            heuristica[i] = c;
            heuristica[i+1] = '\0';
        }
        if (!(heuristica[0] == 'g' && (((heuristica[1] == 'r') && (heuristica[2] == 'e') && (heuristica[3] == 'e') && (heuristica[4] == 'd') && (heuristica[5] == 'y')) || ((heuristica[1] == 'e') && (heuristica[2] == 'n') && (heuristica[3] == 'e') && (heuristica[4] == 't') && (heuristica[5] == 'i') && (heuristica[6] == 'c'))))){
            printf("ERRO: heuristica desconhecida");
            exit(1);
        }
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp){
        printf("ERRO: Não foi possível encontrar arquivo indicado");
        fclose(fp);
        exit(1);
    }

    double tempoInicial;
    double tempoFinal;

    int literais;
    fscanf(fp, "%d*c", &literais);
    int clausulas;
    fscanf(fp, "%d*c", &clausulas);

    int j;

    Literal *liter = (Literal *) malloc(literais * sizeof(Literal));
    for(j = 0; j < literais; j ++){
        liter[j].id = j + 1;
        liter[j].valor = UNSOLVED;
    }

    Literals *lite;
    Literals *literals = lite;
    for(j = 0; j < literais; j++){
        lite = (Literals *) malloc(sizeof(Literals));
        lite->literal = &liter[j];
        lite->next = NULL;
        lite = lite->next;
    }

    int valor = 1;
    int eof = 1;

    Formula *nescau;
    Formula *formula = nescau;
    while(eof != EOF){
        Clause *santa;
        Clause *clausula = santa;
        while(valor){
            valor = 0;
            eof = fscanf(fp, "%d*c", &valor);
            if ((eof != EOF) && (valor > 0)){
                santa = (Clause *) malloc(sizeof(Clause));
                santa->sinal = IDENTITY;
                santa->literal = &liter[valor - 1];
                santa->next = NULL;
                santa = santa->next;
            } else if ((eof != EOF) && (valor < 0)){
                santa = (Clause *) malloc(sizeof(Clause));
                santa->sinal = COMPLEMENT;
                santa->literal = &liter[-(valor + 1)];
                santa->next = NULL;
                santa = santa->next;
            }
        }
        nescau = (Formula *) malloc(sizeof(Formula));
        nescau->clausula = clausula;
        nescau->next = NULL;
        nescau = nescau->next;
    }

    int maxClauses;

    if ((heuristica[0] == 'g') && (heuristica[1] == 'r') && (heuristica[2] == 'e') && (heuristica[3] == 'e') && (heuristica[4] == 'd') && (heuristica[5] == 'y')) {
        printf("Heuristica utilizada: greedy");
        int repetitons = literais;

        tempoInicial = clock();

        maxClauses = tryGreedy(literals, formula, repetitons);

        tempoFinal = clock();
    }

    if ((heuristica[0] == 'g') && (heuristica[1] == 'e') && (heuristica[2] == 'n') && (heuristica[3] == 'e') && (heuristica[4] == 't') && (heuristica[5] == 'i') && (heuristica[6] == 'c')) {
        printf("Heuristica utilizada: genetic");
        int populationSize = clausulas + literais;
        int maxIterations = literais;
        int mrsDeath = literais;
        int mrLife = clausulas;
        int mutationProbability = literais;
        
        tempoInicial = clock();

        maxClauses = bestGenes(populationSize, maxIterations, mrsDeath, mrLife, mutationProbability, literals, formula);

        tempoFinal = clock();
    }

    printf("Numero de literais: %d", literais);
    printf("Numero de clausulas: %d", clausulas);
    printf("Maior numero de clausulas satisfeitas durante a execucao: %d", maxClauses);
    printf("Tempo de execução: %f", tempoFinal - tempoInicial);

    fclose(fp);

    return 0;
}