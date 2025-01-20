#include <stdlib.h>
#include <time.h>
#include "clause.h"


Literal **copyLiterals(Literal *liter, int numberOfLiterals);
Clause* shuffleClause(Clause *clause);
Clause** clauseSort(int n, int* lista, Clause** clause);
Formula *shuffleFormula(Formula *formula);
Formula **formulaSort(int n, int* lista, Formula **formula);
int tryGreedy(Literal **literals, Formula *formula, int numberOfRepetitions, int numberOfLiterals);
void greedy(Solution *soluction, Formula *formula);