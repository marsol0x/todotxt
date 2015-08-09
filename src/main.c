#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>

#define MAX_LINE_LEN 1056
#define MAX_ITEM_LEN 1024

#include "todoitems.h"

TodoItem todoItems;
TodoItem doneItems;
 
int main(int argc, char **argv)
{
    FILE *todoFile = fopen("/home/philosoraptor/todo.txt", "r");
    if (todoFile == NULL)
    {
        perror("fopen");
        exit(1);
    }

    todoitem_get_items(todoFile, &todoItems);
    todoitem_get_items(todoFile, &doneItems);

    todoitem_print_items(todoItems.next);

    return 0;
}
