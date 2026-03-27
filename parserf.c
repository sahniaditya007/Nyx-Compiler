#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexerf.h"

typedef struct Node {
    char *value;
    TokenType type;
    struct Node *right;
    struct Node *left;
} Node;


/* ================= TREE PRINT ================= */

void print_tree(Node *node, int indent, char *identifier)
{
    if (node == NULL)
        return;

    for (int i = 0; i < indent; i++)
        printf(" ");

    printf("%s -> %s\n", identifier, node->value);

    print_tree(node->left, indent + 1, "left");
    print_tree(node->right, indent + 1, "right");
}


/* ================= NODE INIT ================= */

Node *init_node(char *value, TokenType type)
{
    Node *node = malloc(sizeof(Node));

    if (!node)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    node->value = strdup(value);
    node->type = type;
    node->left = NULL;
    node->right = NULL;

    return node;
}


/* ================= ERROR HANDLER ================= */

void print_error(char *error_type)
{
    printf("ERROR: %s\n", error_type);
    exit(1);
}


/* ================= EXPRESSION PARSER ================= */

Token *generate_operation_nodes(Token *current_token, Node *current_node)
{
    Node *oper_node = init_node(current_token->value, OPERATOR);

    current_node->left->left = oper_node;
    current_node = oper_node;

    current_token--;

    Node *expr_node = init_node(current_token->value, INT);
    current_node->left = expr_node;

    current_token += 2;

    while (current_token != NULL &&
           (current_token->type == INT ||
            current_token->type == OPERATOR))
    {

        if (current_token->type == INT)
        {
            Node *right_expr =
                init_node(current_token->value, INT);

            current_node->right = right_expr;

            current_token++;

            if (current_token->type != OPERATOR)
                break;
        }

        if (current_token->type == OPERATOR)
        {
            Node *next_oper =
                init_node(current_token->value, OPERATOR);

            current_node->right = next_oper;
            current_node = next_oper;

            current_token--;

            Node *left_expr =
                init_node(current_token->value, INT);

            current_node->left = left_expr;

            current_token += 2;
        }
    }

    return current_token;
}


/* ================= MAIN PARSER ================= */

Node *parser(Token *tokens)
{
    Token *current_token = &tokens[0];

    Node *root = init_node("PROGRAM", BEGINNING);
    Node *current = root;

    while (current_token->type != END_OF_TOKENS)
    {

        switch (current_token->type)
        {

        case KEYWORD:

            if (strcmp(current_token->value, "EXIT") == 0)
            {
                Node *exit_node =
                    init_node(current_token->value, KEYWORD);

                root->right = exit_node;
                current = exit_node;

                current_token++;

                if (current_token->type != SEPARATOR ||
                    strcmp(current_token->value, "(") != 0)
                    print_error("Missing '(' after EXIT");

                Node *open_paren =
                    init_node(current_token->value, SEPARATOR);

                current->left = open_paren;

                current_token++;

                if (current_token->type != INT)
                    print_error("Expected integer expression");

                /* handle expression */

                if ((current_token + 1)->type == OPERATOR)
                {
                    current_token =
                        generate_operation_nodes(current_token + 1,
                                                 current);
                }
                else
                {
                    Node *expr =
                        init_node(current_token->value, INT);

                    current->left->left = expr;

                    current_token++;
                }

                if (current_token->type != SEPARATOR ||
                    strcmp(current_token->value, ")") != 0)
                    print_error("Missing ')'");

                Node *close_paren =
                    init_node(current_token->value, SEPARATOR);

                current->left->right = close_paren;

                current_token++;

                if (current_token->type != SEPARATOR ||
                    strcmp(current_token->value, ";") != 0)
                    print_error("Missing ';'");

                Node *semi =
                    init_node(current_token->value, SEPARATOR);

                current->right = semi;
            }

            break;

        case INT:
            printf("INTEGER: %s\n", current_token->value);
            break;

        case IDENTIFIER:
        printf("IDENTIFIER\n");
        break;

        case OPERATOR:
        case SEPARATOR:
        case BEGINNING:
        case END_OF_TOKENS:
            break;
        }

        current_token++;
    }

    print_tree(root, 0, "root");

    return root;
}