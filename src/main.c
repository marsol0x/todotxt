#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pwd.h>

#define MAX_LINE_LEN 1056
#define MAX_ITEM_LEN 1024
#define HASHTABLE_MAX_SIZE 16
#define ARRAY_LEN(x) (sizeof((x)) / sizeof((x)[1]))

#define COLOR_START "\x1B["
#define COLOR_RESET "\x1B[0m"
#define COLOR_BLACK   COLOR_START "30m"
#define COLOR_RED     COLOR_START "31m"
#define COLOR_GREEN   COLOR_START "32m"
#define COLOR_YELLOW  COLOR_START "33m"
#define COLOR_BLUE    COLOR_START "34m"
#define COLOR_MAGENTA COLOR_START "35m"
#define COLOR_CYAN    COLOR_START "36m"
#define COLOR_WHITE   COLOR_START "37m"

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

#define HASHTABLE_ADD(name, cmd) hashtable_add(hashTable, HASHTABLE_MAX_SIZE, (name), (cmd))
    HASHTABLE_ADD("help", cmd_help);
    HASHTABLE_ADD("add", cmd_add);
    HASHTABLE_ADD("a", cmd_add);
    HASHTABLE_ADD("delete", cmd_delete);
    HASHTABLE_ADD("del", cmd_delete);
    HASHTABLE_ADD("list", cmd_list);
    HASHTABLE_ADD("ls", cmd_list);
    HASHTABLE_ADD("done", cmd_done);
    HASHTABLE_ADD("d", cmd_done);
    HASHTABLE_ADD("priority", cmd_priority);
    HASHTABLE_ADD("pri", cmd_priority);
    HASHTABLE_ADD("depriority", cmd_depriority);
    HASHTABLE_ADD("depri", cmd_depriority);
    HASHTABLE_ADD("num", cmd_num);
    HASHTABLE_ADD("search", cmd_search);
    HASHTABLE_ADD("s", cmd_search);
#undef HASHTABLE_ADD

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
