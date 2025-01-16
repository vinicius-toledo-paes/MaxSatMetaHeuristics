#include "clause.h"

Literals* initLiterals(){
    return (Literals *) malloc(sizeof(Literals));
}

Formula* initFormula(){
    Formula *formula = (Formula *) malloc(sizeof(Formula));
    formula->clausula = NULL;
    formula->next = NULL;

    return formula;
}

Formula* addClause(Formula* formula, Clause* clause){
    if (!(formula->clausula)) {
        formula->clausula = clause;
        return formula;
    }
    Formula *novaFormula = (Formula *) malloc(sizeof(Formula));
    novaFormula->clausula = clause;
    novaFormula->next = formula;

    return formula;
}

Clause* initClause(Literal *literal, Sign sinal){
    Clause *clause = (Clause *) malloc(sizeof(Clause));
    clause->literal = literal;
    clause->sinal = sinal;
    clause->next = NULL;
}

Clause* addLiteral(Clause *clause, Literal *literal, Sign sinal){
    Clause *clausula = (Clause *) malloc(sizeof(Clause));
    clausula->literal = literal;
    clausula->sinal = sinal;
    clausula->next = clause;

    return clausula;
}

void freeClause(Clause *clause){
    Clause *dyingClause;

    dyingClause = clause;
    clause = clause->next;
    if (dyingClause){
        free(dyingClause);
        dyingClause = NULL;
    }
    freeClause(clause);
    clause = NULL;
}

void freeFormula(Formula *formula){
    if(!formula){
        return;
    }
    Formula *dyingFormula;
    dyingFormula = formula;
    formula = formula->next;
    freeClause(dyingFormula->clausula);
    if (dyingFormula){
        free(dyingFormula);
        dyingFormula = NULL;
    }
    freeFormula(formula);
    formula = NULL;
}

void freeSolution(Solution *soluction){
    if(soluction){
        free(soluction);
        soluction = NULL;
    }
}

void freeLiterals(Literals *literals){
    if (!literals){
        return;
    }
    Literals *dyingLiteral;
    dyingLiteral = literals;
    literals = literals->next;
    if (dyingLiteral){
        free(dyingLiteral);
        dyingLiteral = NULL;
    }
    freeLiterals(literals);
    literals = NULL;
}

void printFormula(Formula *formula){
    Formula *formu = formula;
    while (formu){
        Clause *claus = formu->clausula;
        while(claus){
            if (claus->literal){
                if (claus->sinal == IDENTITY){
                    printf("%d ", claus->literal->id);
                }
                if (claus->sinal == COMPLEMENT){
                    printf("%d ", -claus->literal->id);
                }
            }
            claus = claus->next;
        }
        printf("\n");
        formu = formu->next;
    }
}