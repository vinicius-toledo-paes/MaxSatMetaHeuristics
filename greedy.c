#include <stdlib.h>
#include <time.h>
#include "clause.h"

Clause** clauseSort(int n, int* lista, Clause** clause){
    int i, j, k, menor;

    for(i = 0; i < n; i++){
        menor = lista[i];
        k = i;
        for(j = i; j < n; j++){
            if(lista[j] < menor){
                menor = lista[j];
                k = j;
            }
        }
        int tempi = lista[i];
        Clause *tempc = clause[i];

        lista[i] = lista[k];
        clause[i] = clause[k];

        lista[k] = tempi;
        clause[k] = tempc;
    }

    return clause;
}

Clause* shuffleClause(Clause *clause){
    Clause* clausePointer = clause;
    int n = 0;
    while(clausePointer){
        n++;
        clausePointer = clausePointer->next;
    }

    clausePointer = clause;
    
    Clause **newClause = (Clause **) malloc(n*sizeof(Clause*));

    int *lista = (int*) malloc(n*sizeof(int));

    for(int i = 0; i < n; i++){
        lista[i] = rand()%(4*n);
        newClause[i] = (Clause *) malloc(sizeof(Clause));
        newClause[i]->literal = clausePointer->literal;
        newClause[i]->sinal = clausePointer->sinal;
        newClause[i]->next = NULL;
        clausePointer = clausePointer->next;
    }

    newClause = clauseSort(n, lista, newClause);

    for(int i = 0; i < n-1; i++){
        newClause[i]->next = newClause[i+1];
    }

    Clause *returningClause = newClause[0];

    free(newClause);
    free(lista);

    return returningClause;
}



Formula **formulaSort(int n, int* lista, Formula **formula){
    int i, j, k, menor;

    for(i = 0; i < n; i++){
        menor = lista[i];
        k = i;
        for(j = i; j < n; j++){
            if(lista[j] < menor){
                menor = lista[j];
                k = j;
            }
        }
        int tempi = lista[i];
        Formula *tempf = formula[i];

        lista[i] = lista[k];
        formula[i] = formula[k];

        lista[k] = tempi;
        formula[k] = tempf;
    }

    return formula;
}

Formula *shuffleFormula(Formula *formula){
    Formula *formulaPointer = formula;
    int n = 0;

    while(formulaPointer){
        n++;
        formulaPointer = formulaPointer->next;
    }

    formulaPointer = formula;

    Formula **newFormula = (Formula **) malloc(n*sizeof(Formula*));

    int *lista = (int *)  malloc(n*sizeof(int));

    for(int i = 0; i < n; i++){
        newFormula[i] = (Formula *) malloc(sizeof(Formula));
        lista[i] = rand()%(3*n);
        newFormula[i]->clausula = shuffleClause(formulaPointer->clausula);
        newFormula[i]->next = NULL;
        formulaPointer = formulaPointer->next;
    }

    newFormula = formulaSort(n, lista, newFormula);

    for(int i = 0; i < n-1; i++){
        newFormula[i]->next = newFormula[i+1];
    }

    Formula *returningFormula = newFormula[0];

    free(newFormula);
    free(lista);

    return returningFormula;
}


void greedy(Solution *soluction, Formula *formula){
    Formula *newFormula = shuffleFormula(formula);
    soluction->satClauses = 0;

    for(Formula *f = newFormula; f; f = f->next){
        Clause *c;
        int necessary = 1;

        for(c = f->clausula; c && necessary; c = c->next){
            if((c->literal->valor == TRUE && c->sinal == IDENTITY) || (c->literal->valor == FALSE && c->sinal == COMPLEMENT)){
                necessary = 0;
                (soluction->satClauses)++;
            }
        }

        int unaltered = 1;
        for(c = f->clausula; c && unaltered && necessary; c = c->next){
            if (c->literal->valor == UNSOLVED){
                if(c->sinal == IDENTITY){
                    c->literal->valor = TRUE;
                }
                if(c->sinal == COMPLEMENT){
                    c->literal->valor = FALSE;
                }
                unaltered = 0;
                soluction->satClauses++;
            }
        }
    }

    freeFormula(newFormula);
}

Literals *copyLiterals(Literals *liter){
    Literals *newLiterals = initLiterals();
    Literals *initPointer = NULL;
    Literals *oldLiterals = liter;

    while(oldLiterals){
        newLiterals->literal = (Literal *) malloc(sizeof(Literal));
        newLiterals->next = NULL;

        newLiterals->literal->id = oldLiterals->literal->id;
        newLiterals->literal->valor = UNSOLVED;

        if (initPointer){
            newLiterals->next = initPointer;
        }
        initPointer = newLiterals;
        
        oldLiterals = oldLiterals->next;
    }

    return initPointer;

}

int tryGreedy(Literals **literals, Formula *formula, int numberOfIterations){
    Literals *newLiteral;

    Solution oldSolution;
    Solution newSolution;

    oldSolution.satClauses = 0;
    for (int i = 0; i < numberOfIterations; i++){
        newLiteral = copyLiterals(*literals);
        newSolution.literais = newLiteral;
        greedy(&newSolution, formula);

        if (newSolution.satClauses > oldSolution.satClauses){
            oldSolution.satClauses = newSolution.satClauses;
            Literals *temp = newSolution.literais;
            newSolution.literais = oldSolution.literais;
            oldSolution.literais = temp;
            newLiteral = newSolution.literais;
        }

        freeLiterals(newLiteral);
        newLiteral = NULL;
    }
    freeLiterals(*literals);
    *literals = oldSolution.literais;

    return oldSolution.satClauses;
}