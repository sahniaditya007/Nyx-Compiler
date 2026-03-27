#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "lexerf.h"
#include "parserf.h"
#include "./hashmap/hashmap.h"

void traverse_tree(Node *node, int is_left, FILE *file)
{
    if (node == NULL)
        return;

    /* EXIT syscall setup */
    if (strcmp(node->value, "EXIT") == 0)
    {
        fprintf(file, "  mov rax, 60\n");
    }

    /* Handle operators */
    if (node->type == OPERATOR)
    {
        Node *tmp = node;

        /* -------- DIVISION -------- */
        if (strcmp(node->value, "/") == 0)
        {
            if (node->left && node->right)
            {
                fprintf(file, "  mov rax, %s\n", node->left->value);
                fprintf(file, "  cqo\n");  // sign extend rax → rdx:rax
                fprintf(file, "  mov rbx, %s\n", node->right->value);
                fprintf(file, "  idiv rbx\n");
                fprintf(file, "  mov rdi, rax\n");
            }
        }
        else
        {
            if (tmp->left != NULL)
                fprintf(file, "  mov rdi, %s\n", tmp->left->value);

            while (tmp->right != NULL && tmp->right->type == OPERATOR)
            {
                if (search(tmp->value[0]) == NULL)
                    break;

                char *oper = search(tmp->value[0])->data;

                tmp = tmp->right;

                if (tmp->left != NULL)
                    fprintf(file, "  %s rdi, %s\n", oper, tmp->left->value);
            }

            if (tmp->right != NULL && search(tmp->value[0]) != NULL)
            {
                char *oper = search(tmp->value[0])->data;
                fprintf(file, "  %s rdi, %s\n", oper, tmp->right->value);
            }
        }
    }

    /* Integer */
    if (node->type == INT)
    {
        fprintf(file, "  mov rdi, %s\n", node->value);
    }

    /* syscall trigger */
    if (strcmp(node->value, ";") == 0)
    {
        fprintf(file, "  syscall\n");
    }

    /* Debug print */
    printf("%s\n", node->value);

    /* Traverse */
    traverse_tree(node->left, 1, file);
    traverse_tree(node->right, 0, file);
}

/* Entry point */
int generate_code(Node *root)
{
    insert('-', "sub");
    insert('+', "add");
    insert('*', "imul");
    insert('/', "idiv");

    FILE *file = fopen("generated.asm", "w");
    assert(file != NULL && "FILE COULD NOT BE OPENED\n");

    fprintf(file, "section .text\n");
    fprintf(file, "  global _start\n\n");
    fprintf(file, "_start:\n");

    traverse_tree(root, 0, file);

    fclose(file);

    return 0;
}