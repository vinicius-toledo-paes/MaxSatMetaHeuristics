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



    ///* Começo da abordagem gulosa:

    printf("Heuristica utilizada: greedy\n");
    printf("Resultados: \n\n");
    
        
    for (j = 0; j < literais; j ++){
        liter[j].valor = UNSOLVED;
    }

    int numberOfLiterals = literais;
    int repetitons = numberOfLiterals + 1;

    tempoInicial = clock();

    maxClauses = tryGreedy(&liter, formula, repetitons, numberOfLiterals);

    tempoFinal = clock();

    printf("O maior numero de clausulas satisfeitas pelo metodo foi: %d", maxClauses);
    printf("Tempo de execução: %f segundos\n\n", (tempoFinal - tempoInicial)/CLOCKS_PER_SEC);

    //  Fim da abordagem gulosa */



    ///* começo da abordagem genética
    
    printf("Heuristica utilizada: genetic\n");
    printf("Resultado: \n\n");
    
    int populationSize = clausulas + literais;
    int maxIterations = (int) (populationSize / 2) + 1;
    int mrsDeath = (int) (5625 * populationSize / 10000) + 1;
    int mrLife = (int) (625 * populationSize / 1000) + 1;
    int geneSize = literais;
    int mutationProbability = (int) (populationSize / (geneSize + 1));

    tempoInicial = clock();

    maxClauses = bestGenes(populationSize, maxIterations, mrsDeath, mrLife, mutationProbability, geneSize, liter, formula);

    tempoFinal = clock();
    
    printf("Maior numero de clausulas satisfeitas durante a execucao: %d\n", maxClauses);
    printf("Tempo de execução: %f segundos\n\n", (tempoFinal - tempoInicial)/CLOCKS_PER_SEC);

    // Fim da abordagem genética   */

    fclose(fp);

    return 0;
}