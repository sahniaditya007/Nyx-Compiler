#ifndef PARSER_H_
#define PARSER_H_

#include "lexerf.h"

typedef struct Node {
  char *value;
  int type;
  struct Node *right;
  struct Node *left;
} Node;

typedef enum {
  INT_LIT,
  STATEMENT,
  EXTRA,
  BEGINNING,
} NodeTypes;

Node *parser(Token *tokens);

#endif