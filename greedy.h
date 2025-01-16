#include <stdlib.h>
#include <time.h>
#include "clause.h"


Literals *copyLiterals(Literals *liter);
Clause* shuffleClause(Clause *clause);
Clause** clauseSort(int n, int* lista, Clause** clause);
Formula *shuffleFormula(Formula *formula);
Formula **formulaSort(int n, int* lista, Formula **formula);
int tryGreedy(Literals *literals, Formula *formula, int numberOfIterations);
void greedy(Solution *soluction, Formula *formula);