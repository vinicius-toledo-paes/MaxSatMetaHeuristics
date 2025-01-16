#include <stdlib.h>
#include <time.h>

#ifndef CLAUSE_H
#define CLAUSE_H

typedef enum val{
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


Literals* initLiterals();
Clause* initClause(Literal *literal, Sign sinal);
Clause* addLiteral(Clause *clause, Literal *literal, Sign sinal);
Formula* initFormula();
Formula* addClause(Formula* formula, Clause* clause);

void freeClause(Clause *clause);
void freeFormula(Formula *formula);
void freeSolution(Solution *soluction);
void freeLiterals(Literals *literals);

#endif