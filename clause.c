#include "clause.h"

Literals* initLiterals(){
    Literals *literal = (Literals *) malloc(sizeof(Literals));
    return literal;
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
    if (!clause){
        return;
    }
    freeClause(clause->next);
    free(clause);
}

void freeFormula(Formula *formula){
    if(!formula){
        return;
    }
    freeFormula(formula->next);
    free(formula);
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
    freeLiterals(literals->next);
    free(literals);
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