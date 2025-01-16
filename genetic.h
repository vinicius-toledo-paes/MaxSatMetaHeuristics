#include <stdlib.h>
#include <time.h>
#include "clause.h"

Solution* generateIndividual(Literals *blueprint);
Solution* cloneIndividual(Solution *individual);
Solution* breedIndividuals(Solution *individual1, Solution *individual2);
Solution** cloneIndividuals(int numberOfNewIndividuals, Solution *individual);
Solution** completePopulation(int originalPopulationSize, int oldPopulationSize, int numberOfNewIndividuals, Solution **population, Literals *originalIndividual);
Solution** genetic(int sizeOfPopulation, int maxIterations, int deathsPerIteration, int offspringsPerIteration, int mutationProbability, Literals *literais, Formula *formula);
int bestGenes(int sizeOfPopulation, int maxIterations, int deathsPerIteration, int offspringsPerIteration, int mutationProbability, Literals *literais, Formula *formula);
void freePopulation(int sizeOfPopulation, Solution **population);
void mutateIndividual(Solution *individual, int mutationProbability);
void countSatClauses(int sizeOfPoputation, Solution **population, Formula *formula);
void qualifyIndividuals(int numberOfIndividuals, Solution **individuals);
