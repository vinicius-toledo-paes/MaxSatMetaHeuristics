#include <stdlib.h>
#include <time.h>

typedef enum{
    FALSE, TRUE, UNSOLVED
} Value;

typedef struct lit{
    int id;
    Value valor;
} Literal;

typedef struct lits{
    Literal *literal;
    struct lits *next;
} Literals;

Literals* initLiterals(){
    return (Literals *) malloc(sizeof(Literals));
}

typedef struct{
    Literals *literais;
    int satClauses;
} Solution;

typedef enum{
    IDENTITY, COMPLEMENT
} Sign;

typedef struct clau{
    Literal *literal;
    Sign sinal;
    struct clau *next;
} Clause;

typedef struct form{
    Clause *clausula;
    struct form *next;
}Formula;

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