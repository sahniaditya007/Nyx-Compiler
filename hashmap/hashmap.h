#ifndef HASHMAP_H_
#define HASHMAP_H_

#define SIZE 20

/* Data structure */
struct DataItem {
    char key;     // operator key like '+', '-', '*', '/'
    char *data;   // mapped instruction like "add", "sub"
};

/* Function declarations */
int hashCode(char key);

struct DataItem *search(char key);

void insert(char key, char *data);

struct DataItem* deleteItem(struct DataItem* item);

void display(void);

#endif