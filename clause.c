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

    while(clause){
        dyingClause = clause;
        clause = clause->next;
        free(dyingClause);
    }
}

void freeFormula(Formula *formula){
    Formula *dyingFormula;

    while(formula){
        dyingFormula = formula;
        formula = formula->next;
        freeClause(dyingFormula->clausula);
        free(dyingFormula);
    }
}

void freeSolution(Solution *soluction){
    free(soluction);
}

void freeLiterals(Literals *literals){
    Literals *dyingLiteral;

    while(literals){
        dyingLiteral = literals;
        literals = literals->next;
        free(dyingLiteral);
    }
}

void printFormula(Formula *formula){
    Formula *formu = formula;
    while (formu){
        Clause *claus = formu->clausula;
        while(claus){
            if (claus->literal){
                printf("%d ", claus->literal->id);
            }
            claus = claus->next;
        }
        printf("\n");
        formu = formu->next;
    }
}