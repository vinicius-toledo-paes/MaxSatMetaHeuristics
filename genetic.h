#include <stdlib.h>
#include <time.h>
#include "clause.h"


typedef struct {
    Literal **literais;
    int geneSize;
    int satClauses;
} Individual;

Individual* generateIndividual(Literal *blueprint, int geneSize);
Individual* cloneIndividual(Individual *individual, int geneSize);
Individual* breedIndividuals(Individual *individual1, Individual *individual2, int geneSize);
//Individual** cloneIndividuals(int numberOfNewIndividuals, Individual *individual);
Individual** completePopulation(int originalPopulationSize, int oldPopulationSize, int numberOfNewIndividuals, int geneSize, Individual **population, Literal *originalIndividual);
Individual** genetic(int sizeOfPopulation, int maxIterations, int deathsPerIteration, int offspringsPerIteration, int mutationProbability, int geneSize, Literal *literais, Formula *formula);
int bestGenes(int sizeOfPopulation, int maxIterations, int deathsPerIteration, int offspringsPerIteration, int mutationProbability, int geneSize, Literal *literais, Formula *formula);
void freePopulation(int sizeOfPopulation, Individual **population, int geneSize);
void mutateIndividual(Individual *individual, int mutationProbability, int geneSize);
void countSatClauses(int sizeOfPoputation, Individual **population, Formula *formula);
void qualifyIndividuals(int numberOfIndividuals, Individual **individuals);
