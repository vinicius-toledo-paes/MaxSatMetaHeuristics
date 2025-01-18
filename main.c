#include <stdio.h>
#include <time.h>

#include "genetic.h"
#include "greedy.h"

int main(int argc, char *argv[])
{

    srand(time(NULL));

    if (argc < 2)
    {
        printf("ERRO: nome de arquivo não encontrado \n\n");
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("ERRO: Não foi possível encontrar arquivo indicado");
        fclose(fp);
        exit(1);
    }

    double tempoInicial;
    double tempoFinal;

    int literais;
    int k = 0;
    char c = '\0';
    char num[10];
    while (c != ' ' && c != '\n')
    {
        c = fgetc(fp);
        if (c == ' ' && !k)
        {
            while (c == ' ')
            {
                c = fgetc(fp);
            }
        }
        if (c != EOF && c != ' ' && c != '\n')
        {
            num[k] = c;
            k++;
        }
    }
    num[k] = '\0';
    sscanf(num, "%d", &literais);
    int clausulas;
    c = '\0';
    k = 0;
    while (c != ' ' && c != '\n')
    {
        c = fgetc(fp);
        if (c == ' ' && !k)
        {
            while (c == ' ')
            {
                c = fgetc(fp);
            }
        }
        if (c != EOF && c != ' ' && c != '\n')
        {
            num[k] = c;
            k++;
        }
    }
    num[k] = '\0';
    sscanf(num, "%d", &clausulas);
    c = '\0';

    int j;

    Literal *liter = (Literal *)malloc(literais * sizeof(Literal));
    for (j = 0; j < literais; j++)
    {
        liter[j].id = j + 1;
        liter[j].valor = UNSOLVED;
    }

    Literals *lite = NULL;;
    Literals *literals = NULL;
    for (j = literais-1; j > -1; j--)
    {
        lite = (Literals *)malloc(sizeof(Literals));
        lite->literal = &liter[j];
        lite->next = literals;
        
        literals = NULL;
        literals = lite;
        lite = NULL;
    }
    
    int valor = 1;
    int eof = 1;

    Formula *nescau = NULL;
    Formula *formula = NULL;

    int falta = clausulas;
    while (falta)
    {
        Clause *santa = NULL;
        Clause *clausula = NULL;
        while (valor)
        {
            valor = 0;
            k = 0;
            while (c != ' ' && c != '\n')
            {
                c = fgetc(fp);
                if (c == ' ' && !k)
                {
                    while (c == ' ')
                    {
                        c = fgetc(fp);
                    }
                }
                if (c != EOF && c != ' ' && c != '\n')
                {
                    num[k] = c;
                    k++;
                }
            }
            num[k] = '\0';
            sscanf(num, "%d", &valor);
            c = '\0';
            if (valor > 0)
            {
                santa = (Clause *)malloc(sizeof(Clause));
                santa->sinal = IDENTITY;
                santa->literal = &(liter[valor - 1]);
                santa->next = clausula;
                
                clausula = santa;
            }
            if (valor < 0)
            {
                santa = (Clause *)malloc(sizeof(Clause));
                santa->sinal = COMPLEMENT;
                santa->literal = &(liter[-(valor + 1)]);
                santa->next = clausula;
                
                clausula = santa;
            }
        }
        valor = 1;
        nescau = (Formula *)malloc(sizeof(Formula));
        nescau->clausula = clausula;
        nescau->next = NULL;
        if (formula){
            nescau->next = formula;
        }
        formula = nescau;
        falta--;
    }

    printFormula(formula);
    
    int maxClauses;
    
    printf("\n\nNumero de literais: %d\n", literais);
    printf("Numero de clausulas: %d\n\n", clausulas);
    printf("Heuristica utilizada: greedy\n");
    printf("Resultados: \n\n");

    int i;
    for(i = 0; i < 10; i++){
        int repetitons = literais;

        tempoInicial = clock();

        maxClauses = tryGreedy(&literals, formula, repetitons);
        
        for (j = 0; j < literais; j ++){
            liter[j].valor = UNSOLVED;
        }

        tempoFinal = clock();
        
        printf("O numero de clausulas satisfeitas durante a execucao %d: %d\n", i, maxClauses);
        printf("Tempo de execução: %f\n\n", tempoFinal - tempoInicial);
    }


    printf("\n\nHeuristica utilizada: genetic\n");
    printf("Resultados: \n\n");
    for(i = 0; i < 10; i++){
        int populationSize = 100;
        int maxIterations = 50;
        int mrsDeath = 50;
        int mrLife = 60;
        int mutationProbability = 1000;
        int geneSize = literais;

        tempoInicial = clock();

        maxClauses = bestGenes(populationSize, maxIterations, mrsDeath, mrLife, mutationProbability, geneSize, liter, formula);

        tempoFinal = clock();
        
        printf("Maior numero de clausulas satisfeitas durante a execucao %d: %d\n", i, maxClauses);
        printf("Tempo de execução: %f segundos\n\n", (tempoFinal - tempoInicial)/CLOCKS_PER_SEC);
    }


    fclose(fp);

    return 0;
}