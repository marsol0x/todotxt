#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pwd.h>

#define MAX_LINE_LEN 1056
#define MAX_ITEM_LEN 1024

#define HASHTABLE_MAX_SIZE 13

void error_and_exit(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

#include "todoitems.h"
#include "commands.h"
#include "hashtable.h"

TodoList todoItems;
TodoList doneItems;

HashItem *hashTable[HASHTABLE_MAX_SIZE];
 
int main(int argc, char **argv)
{
    FILE *todoFile;
    char *todoFileTxt = "/todo.txt";
    char todoFilePath[256] = {};

    // NOTE: First we'll check the current working directory
    strcat(todoFilePath, getcwd(0, 0));
    strcat(todoFilePath, todoFileTxt);
    todoFile = fopen(todoFilePath, "r+");
    if (todoFile == NULL)
    {
        // NOTE: Next, try the user's home directory
        uid_t currentUid = getuid();
        struct passwd *userPasswd = getpwuid(currentUid);
        memset(todoFilePath, 0, 256);
        strcat(todoFilePath, userPasswd->pw_dir);
        strcat(todoFilePath, todoFileTxt);

        todoFile = fopen(todoFilePath, "r+");
        if (todoFile == NULL)
        {
            error_and_exit("Could not find todo.txt");
        }
    }

    memset(&todoItems, 0, sizeof(TodoList));
    memset(&doneItems, 0, sizeof(TodoList));
    todoitem_get_items(todoFile, &todoItems);
    todoitem_get_items(todoFile, &doneItems);

    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "add", cmd_add);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "a", cmd_add);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "delete", cmd_delete);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "del", cmd_delete);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "list", cmd_list);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "ls", cmd_list);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "done", cmd_done);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "d", cmd_done);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "priority", cmd_priority);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "pri", cmd_priority);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "depriority", cmd_depriority);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "depri", cmd_depriority);
    hashtable_add(hashTable, HASHTABLE_MAX_SIZE, "num", cmd_num);

    HashItem *item;
    char cmd[20] = {};
    if (argc == 1)
    {
        strncat(cmd, "list", 19);
        item = hashtable_get(hashTable, HASHTABLE_MAX_SIZE, "list");
    } else {
        strncat(cmd, argv[1], 19);
        item = hashtable_get(hashTable, HASHTABLE_MAX_SIZE, cmd);
    }

    if (!item)
    {
        char msg[MAX_ITEM_LEN];
        strncat(msg, "No such command: ", MAX_ITEM_LEN);
        strncat(msg, cmd, MAX_ITEM_LEN);
        error_and_exit(msg);
    }

    item->cmd(&todoItems, &doneItems, todoFile, argc - 2, argv + 2);

    return 0;
}
