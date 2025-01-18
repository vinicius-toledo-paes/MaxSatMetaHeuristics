#include "genetic.h"

Individual* generateIndividual(Literal *blueprint, int geneSize){
    Individual *individual = (Individual *) malloc(sizeof(Individual));
    individual->geneSize = geneSize;
    individual->literais = (Literal **) malloc(geneSize * sizeof(Literal*));

    for (int i = 0; i < geneSize; i++){
        individual->literais[i] = (Literal *) malloc(sizeof(Literal));
        individual->literais[i]->id = blueprint[i].id;
        if (rand()%2){
            individual->literais[i]->valor = TRUE;
        }else {
            individual->literais[i]->valor = FALSE;
        }
    }
    

    /*
    Literals *gene;
    individual->literais = gene;

    
    while(blueprint){
        gene = (Literals *) malloc(sizeof(Literals));
        gene->literal = (Literal *) malloc(sizeof(Literal));
        gene->literal->id = blueprint->literal->id;
        gene->next = NULL;
        
        if(rand()%2){
            gene->literal->valor = TRUE;
        }else {
            gene->literal->valor = FALSE;
        }

        gene = gene->next;
        blueprint = blueprint->next;

    }
    */
    individual->satClauses = 0;

    return individual;
}

Individual* cloneIndividual(Individual *individual, int geneSize){
    Individual *newIndividual = (Individual *) malloc(sizeof(Individual));

    newIndividual->geneSize = geneSize;
    newIndividual->literais = (Literal **) malloc(individual->geneSize * sizeof(Literal*));

    for(int i = 0; i < (individual->geneSize); i++){
        newIndividual->literais[i] = (Literal *) malloc(sizeof(Literal));
        newIndividual->literais[i]->id = individual->literais[i]->id;
        newIndividual->literais[i]->valor = individual->literais[i]->valor;
    }

    /*
    Literals *newLiteral = newIndividual->literais;
    for(Literals *l = individual->literais; l; l = l->next){
        newLiteral = (Literals *) malloc(sizeof(Literals));
        newLiteral->literal = (Literal *) malloc(sizeof(Literal));
        newLiteral->literal->id = l->literal->id;
        newLiteral->literal->valor = l->literal->valor;
        newLiteral = newLiteral->next;
    }
    newLiteral = NULL;
    */
    newIndividual->satClauses = individual->satClauses;

    return newIndividual;
}

/*
Individual** cloneIndividuals(int numberOfNewIndividuals, Individual *individual){
    Individual **solucoes = (Individual **) malloc(numberOfNewIndividuals * sizeof(Individual *));
    
    for (int i = 0; i < numberOfNewIndividuals; i++){
        solucoes[i] = cloneIndividual(individual);
    }

    return solucoes;
}
*/

void freePopulation(int sizeOfPopulation, Individual **population, int geneSize){
    if (!population){
        return;
    }
    for(int i = 0; i < sizeOfPopulation; i++){
        for(int j = 0; j < geneSize; j++){
            free(population[i]->literais[j]);
        }
        free(population[i]->literais);
        free(population[i]);
    }
    if (population){
        free(population);
    }
}

Individual** completePopulation(int originalPopulationSize, int oldPopulationSize, int numberOfNewIndividuals, int geneSize, Individual **population, Literal *originalIndividual){
    int i;

    Individual **newPopulation = (Individual **) malloc((oldPopulationSize + numberOfNewIndividuals) * sizeof(Individual *));

    for(i = 0; i < oldPopulationSize; i++){
        newPopulation[i] = cloneIndividual(population[i], geneSize);
    }

    freePopulation(originalPopulationSize, population, geneSize);

    for(i = oldPopulationSize; i < oldPopulationSize + numberOfNewIndividuals; i++){
        newPopulation[i] = generateIndividual(originalIndividual, geneSize);
    }

    return newPopulation;
}

Individual* breedIndividuals(Individual *individual1, Individual *individual2, int geneSize){
    Individual *offspring = (Individual *) malloc(sizeof(Individual));
    offspring->satClauses = 0;
    offspring->geneSize = geneSize;
    offspring->literais = (Literal **) malloc(individual1->geneSize * sizeof(Literal*));

    for(int i = 0; i < geneSize; i++){
        offspring->literais[i] = (Literal *) malloc(sizeof(Literal));
        offspring->literais[i]->id = individual1->literais[i]->id;
        if (individual1->literais[i]->valor == individual2->literais[i]->valor){
            offspring->literais[i]->valor = individual1->literais[i]->valor;
        } else {
            if (rand()%2){
                offspring->literais[i]->valor = individual1->literais[i]->valor;
            } else {
                offspring->literais[i]->valor = individual2->literais[i]->valor;
            }
        }
    }

    /*
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

    */

    return offspring;
}

void mutateIndividual(Individual *individual, int mutationProbability, int geneSize){

    for(int i = 0; i < geneSize; i++){
        if (!rand()%mutationProbability){
            if ((individual->literais[i]->valor) == TRUE){
                (individual->literais[i]->valor) = FALSE;
            } else{
                (individual->literais[i]->valor) = TRUE;
            }
        }
    }

    /*
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
    */
}

void qualifyIndividuals(int numberOfIndividuals, Individual **individuals){
    for(int i = 0; i < numberOfIndividuals; i++){
        int k = i;
        for(int j = i + 1; j < numberOfIndividuals; j++){
            if ((individuals[j]->satClauses) > (individuals[k]->satClauses)){
                k = j;
            }
        }

        if (k != i){
            Individual *temp = individuals[i];
            individuals[i] = individuals[k];
            individuals[k] = temp;
        }
    }
}

void countSatClauses(int sizeOfPoputation, Individual **population, Formula *formula){
    for(int i = 0; i < sizeOfPoputation; i++){
        population[i]->satClauses = 0;
        Formula *temporaryFormula = formula;
        while(temporaryFormula){
            int sat = 0;
            Clause *temporaryClause = temporaryFormula->clausula;
            while(temporaryClause && !sat){
                if(((population[i]->literais[(temporaryClause->literal->id) - 1]->valor) == TRUE && (temporaryClause->sinal) == IDENTITY) || ((population[i]->literais[(temporaryClause->literal->id) - 1]->valor) == FALSE && (temporaryClause->sinal) == COMPLEMENT)){
                    sat = 1;
                    (population[i]->satClauses)++;
                }
                temporaryClause = (temporaryClause->next);
            }
            temporaryFormula = (temporaryFormula->next);
        }
    }
}

Individual** genetic(int sizeOfPopulation, int maxIterations, int deathsPerIteration, int offspringsPerIteration, int mutationProbability, int geneSize, Literal *literais, Formula *formula){
    int i, j;

    Individual **population = completePopulation(0, 0, sizeOfPopulation, geneSize, NULL, literais);

    for(j = 0; j < maxIterations; j++){
        countSatClauses(sizeOfPopulation, population, formula);

        Individual **newPopulation = (Individual **) malloc(sizeof(Individual));

        qualifyIndividuals(sizeOfPopulation, population);

        for(i = 0; i < (sizeOfPopulation - offspringsPerIteration); i++){
            newPopulation[i] = cloneIndividual(population[i], geneSize);
        }

        for(i = sizeOfPopulation - offspringsPerIteration; i < sizeOfPopulation; i++){
            newPopulation[i] = breedIndividuals(population[rand()%sizeOfPopulation], population[rand()%sizeOfPopulation], geneSize);
        }

        freePopulation(sizeOfPopulation, population, geneSize);

        for(i = 0; i < sizeOfPopulation; i++){
            mutateIndividual(newPopulation[i], mutationProbability, geneSize);
        }

        countSatClauses(sizeOfPopulation, newPopulation, formula);
        qualifyIndividuals(sizeOfPopulation, newPopulation);

        population = completePopulation(sizeOfPopulation, sizeOfPopulation - deathsPerIteration, deathsPerIteration, geneSize, newPopulation, literais);
    }

    qualifyIndividuals(sizeOfPopulation, population);
    
    return population;
}

int bestGenes(int sizeOfPopulation, int maxIterations, int deathsPerIteration, int offspringsPerIteration, int mutationProbability, int geneSize, Literal *literais, Formula *formula){

    Individual **people = genetic(sizeOfPopulation, maxIterations, deathsPerIteration, offspringsPerIteration, mutationProbability, geneSize, literais, formula);

    qualifyIndividuals(sizeOfPopulation, people);

    int valor = people[0]->satClauses;

    freePopulation(sizeOfPopulation, people, geneSize);

    return valor;
}
