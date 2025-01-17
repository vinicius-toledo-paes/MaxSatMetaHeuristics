#include "greedy.h"

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

Literal **copyLiterals(Literal *liter, int numberOfLiterals){

    Literal **literal = (Literal **) malloc(numberOfLiterals * sizeof(Literal *));

    for (int i = 0; i < numberOfLiterals; i++){
        literal[i] = (Literal *) malloc(sizeof(Literal));
        literal[i]->id = liter[i].id;
        literal[i]->valor = UNSOLVED;
    }

    return literal;



    /*
    Literals *newLiterals = initLiterals();
    Literals *initPointer = NULL;
    Literals *oldLiterals = liter;

    while(oldLiterals){
        newLiterals->literal = (Literal *) malloc(sizeof(Literal));

        newLiterals->literal->id = oldLiterals->literal->id;
        newLiterals->literal->valor = UNSOLVED;

        newLiterals->next = initPointer;
        initPointer = newLiterals;
        
        oldLiterals = oldLiterals->next;
    }
    return initPointer;
    */
}

int tryGreedy(Literal **literals, Formula *formula, int numberOfRepetitions, int numberOfLiterals){
    
    Solution solucao;
    solucao.literais = literals;
    solucao.numberOfLiterals = numberOfLiterals;
    solucao.satClauses = 0;

    greedy(&solucao, formula);

    int i, j;
    
    Solution *newSolution;

    for (i = 0; i < numberOfRepetitions; i++){
        newSolution = (Solution *) malloc(sizeof(Solution));
        newSolution->literais = copyLiterals(*literals, numberOfLiterals);
        newSolution->numberOfLiterals = numberOfLiterals;
        newSolution->satClauses = 0;

        greedy(newSolution, formula);

        if ((newSolution->satClauses) > (solucao.satClauses)){
            for (j = 0; j < numberOfLiterals; j++){
                solucao.literais[j]->valor = newSolution->literais[j]->valor;
                solucao.satClauses = newSolution->satClauses;
            }
        }

        for(j = 0; j < numberOfLiterals; j++){
            free(newSolution->literais[j]);
        }
        free(newSolution->literais);
        free(newSolution);
    }
    
    return solucao.satClauses;
    
    /*
    Literals *newLiteral;

    Solution oldSolution;
    Solution newSolution;

    oldSolution.satClauses = -1;
    for (int i = 0; i < numberOfIterations; i++){
        newLiteral = copyLiterals(*literals);
        newSolution.literais = newLiteral;
        greedy(&newSolution, formula);

        if (0 && newSolution.satClauses > oldSolution.satClauses){
            oldSolution.satClauses = newSolution.satClauses;
            freeLiterals(oldSolution.literais);
            oldSolution.literais = NULL;
            oldSolution.literais = copyLiterals(newSolution.literais);
            freeLiterals(newSolution.literais);
            newSolution.literais = NULL;
        }

        freeLiterals(newLiteral);
        newLiteral = NULL;
    }
    freeLiterals(*literals);
    *literals = NULL;
    *literals = copyLiterals(oldSolution.literais);
    freeLiterals(oldSolution.literais);
    
    return oldSolution.satClauses;
    */
}