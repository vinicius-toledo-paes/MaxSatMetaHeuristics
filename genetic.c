#include <stdlib.h>
#include <time.h>
#include "clause.h"

Solution* generateIndividual(Literals *blueprint){
    Solution *individual = (Solution *) malloc(sizeof(Solution));
    Literals *gene = individual->literais;

    while(blueprint){
        gene = (Literals *) malloc(sizeof(Literals));
        gene->literal = (Literal *) malloc(sizeof(Literal));
        gene->literal->id = blueprint->literal->id;
        
        if(rand()%2){
            gene->literal->valor = TRUE;
        }else {
            gene->literal->valor = FALSE;
        }

        gene = gene->next;
        blueprint = blueprint->next;
    }

    gene->next = NULL;

    individual->satClauses = 0;

    return individual;
}

Solution* cloneIndividual(Solution *individual){
    Solution *newIndividual = (Solution *) malloc(sizeof(Solution));

    Literals *newLiteral = newIndividual->literais;
    for(Literals *l = individual->literais; l; l = l->next){
        newLiteral = (Literals *) malloc(sizeof(Literals));
        newLiteral->literal = (Literal *) malloc(sizeof(Literal));
        newLiteral->literal->id = l->literal->id;
        newLiteral->literal->valor = l->literal->valor;
        newLiteral = newLiteral->next;
    }
    newLiteral = NULL;

    newIndividual->satClauses = individual->satClauses;

    return newIndividual;
}

Solution** cloneIndividuals(int numberOfNewIndividuals, Solution *individual){
    Solution **solucoes = (Solution **) malloc(numberOfNewIndividuals * sizeof(Solution *));
    
    for (int i = 0; i < numberOfNewIndividuals; i++){
        solucoes[i] = cloneIndividual(individual);
    }

    return solucoes;
}

void freePopulation(int sizeOfPopulation, Solution **population){
    for(int i = 0; i < sizeOfPopulation; i++){
        freeLiterals(population[i]->literais);
        freeSolution(population[i]);
    }
    if (population){
        free(population);
    }
}

Solution** completePopulation(int originalPopulationSize, int oldPopulationSize, int numberOfNewIndividuals, Solution **population, Literals *originalIndividual){
    int i;

    Solution **newPopulation = (Solution **) malloc((oldPopulationSize + numberOfNewIndividuals) * sizeof(Solution *));

    for(i = 0; i < oldPopulationSize; i++){
        newPopulation[i] = cloneIndividual(population[i]);
    }

    freePopulation(originalPopulationSize, population);

    for(i = oldPopulationSize; i < oldPopulationSize + numberOfNewIndividuals; i++){
        newPopulation[i] = generateIndividual(originalIndividual);
    }

    return newPopulation;
}

Solution* breedIndividuals(Solution *individual1, Solution *individual2){
    Solution *offspring = (Solution *) malloc(sizeof(Solution));
    Literals *offspringGene = offspring->literais;

    Literals *gene1 = individual1->literais;
    Literals *gene2 = individual2->literais;
    while(gene1 && gene2){
        offspringGene = (Literals *) malloc(sizeof(Literals));
        offspringGene->literal = (Literal *) malloc(sizeof(Literal));
        offspringGene->literal->id = gene1->literal->id;

        if(gene1->literal->valor == gene2->literal->valor){
            offspringGene->literal->valor = gene2->literal->valor;
        }else if(rand()%2){
            offspringGene->literal->valor = gene1->literal->valor;
        }else {
            offspringGene->literal->valor = gene2->literal->valor;
        }

        offspringGene = offspringGene->next;
        gene1 = gene1->next;
        gene2 = gene2->next;
    }

    offspringGene = NULL;

    offspring->satClauses = 0;

    return offspring;
}

void mutateIndividual(Solution *individual, int mutationProbability){
    Literals *gene = individual->literais;

    while(gene){
        Value valor = gene->literal->valor;
        if(!rand()%mutationProbability){
            if(gene->literal->valor == TRUE){
                valor = FALSE;
            }
            if(gene->literal->valor == FALSE){
                valor = TRUE;
            }
            if(gene->literal->valor == UNSOLVED){
                valor = FALSE;
                if(rand()%2){
                    valor = TRUE;
                }
            }
        }
        gene->literal->valor = valor;
    }
}

void qualifyIndividuals(int numberOfIndividuals, Solution **individuals){
    for(int i = 0; i < numberOfIndividuals; i++){
        int k = i;
        for(int j = i + 1; j < numberOfIndividuals; j++){
            if (individuals[j] > individuals[ k]){
                k = j;
            }
        }

        if (k != i){
            Solution *temp = individuals[i];
            individuals[i] = individuals[k];
            individuals[k] = temp;
        }
    }
}

void countSatClauses(int sizeOfPoputation, Solution **population, Formula *formula){
    for(int i = 0; i < sizeOfPoputation; i++){
        population[i]->satClauses = 0;
        Formula *temporaryFormula = formula;
        while(temporaryFormula){
            int sat = 0;
            Clause *temporaryClause = temporaryFormula->clausula;
            while(temporaryClause && !sat){
                if((temporaryClause->literal->valor == TRUE && temporaryClause->sinal == IDENTITY) || (temporaryClause->literal->valor == FALSE && temporaryClause->sinal == COMPLEMENT)){
                    sat = 1;
                    (population[i]->satClauses)++;
                }
                temporaryClause = temporaryClause->next;
            }
            temporaryFormula = temporaryFormula->next;
        }
    }
}

Solution** genetic(int sizeOfPopulation, int maxIterations, int deathsPerIteration, int offspringsPerIteration, int mutationProbability, Literals *literais, Formula *formula){
    int i, j;

    Solution **population = completePopulation(0, 0, sizeOfPopulation, NULL, literais);

    for(j = 0; j < maxIterations; j++){
        countSatClauses(sizeOfPopulation, population, formula);

        Solution **newPopulation = (Solution **) malloc(sizeof(Solution));

        qualifyIndividuals(sizeOfPopulation, population);

        for(i = 0; i < sizeOfPopulation - offspringsPerIteration; i++){
            newPopulation[i] = cloneIndividual(population[i]);
        }

        for(i = sizeOfPopulation - offspringsPerIteration; i < sizeOfPopulation; i++){
            newPopulation[i] = breedIndividuals(population[rand()%sizeOfPopulation], population[rand()%sizeOfPopulation]);
        }

        freePopulation(sizeOfPopulation, population);

        for(i = 0; i < sizeOfPopulation; i++){
            mutateIndividual(newPopulation[i], mutationProbability);
        }

        countSatClauses(sizeOfPopulation, newPopulation, formula);
        qualifyIndividuals(sizeOfPopulation, newPopulation);

        population = completePopulation(sizeOfPopulation, sizeOfPopulation - deathsPerIteration, deathsPerIteration, newPopulation, literais);
    }

    qualifyIndividuals(sizeOfPopulation, population);
    
    return population;
}

int bestGenes(int sizeOfPopulation, int maxIterations, int deathsPerIteration, int offspringsPerIteration, int mutationProbability, Literals *literais, Formula *formula){

    Solution **people = genetic(sizeOfPopulation, maxIterations, deathsPerIteration, offspringsPerIteration, mutationProbability, literais, formula);

    qualifyIndividuals(sizeOfPopulation, people);

    int valor = people[0]->satClauses;

    freePopulation(sizeOfPopulation, people);

    return valor;
}
