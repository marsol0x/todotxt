#ifndef COMMANDS_H
#define COMMANDS_H

#define COMMAND(name) void name(TodoList *todoItems, TodoList *doneItems, FILE *todoFile, int argc, char **argv)
typedef COMMAND(Command);

typedef struct
{
    char *name;
    char *shortcuts;
    char *args;
    char *help;
} HelpCommand;
COMMAND(cmd_help)
{
    HelpCommand helpCommands[] = {
        {"add",        "a",     "string", "Add a todo item of <string>"},
        {"delete",     "del",   "id",     "Deletes <id> todo item"},
        {"depriority", "depri", "id",     "Depriorities <id> by one step"},
        {"done",       "d",     "id",     "Marks <id> todo item as complete"},
        {"list",       "ls",    0,        "Lists all uncomplete items and ids"},
        {"num",        0,       0,        "Returns the number of uncomplete items"},
        {"priority",   "pri",   "id",     "Prioritizes <id> by one step"},
        {"search",     "s",     "string", "Search for items containing the word <string>"},
    };

    char *format = "  %-12s %-6s %-10s %s\n";
    printf("List of Commands:\n");
    printf(format, "Name", "Args", "Shortcuts", "Help");
    printf(format, "----", "----", "---------", "----");
    for (int i = 0; i < ARRAY_LEN(helpCommands); ++i)
    {
        HelpCommand *cmd = helpCommands + i;
        printf(format, cmd->name,
                       cmd->args ? cmd->args : "",
                       cmd->shortcuts ? cmd->shortcuts : "",
                       cmd->help);
    }
}

COMMAND(cmd_add)
{
    if (argc > 1)
    {
        error_and_exit("Too many parameters. Please use quotes to add new todo items.");
    } else if (argc == 0) {
        error_and_exit("Too few parameters.");
    }

    TodoItem ti;
    ti.next = ti.prev = 0;
    ti.priority = 5;
    ti.datetime = time(0);
    strncpy(ti.item, argv[0], MAX_ITEM_LEN);

    todoitem_add(todoItems, &ti);
    printf("Added: %s\n", ti.item);
    todoitem_write_all(todoItems, doneItems, todoFile);
}

COMMAND(cmd_delete)
{
    int itemNum = atoi(argv[0]) - 1;
    TodoItem *ti = todoitem_get_item(todoItems, itemNum);
    todoitem_remove(todoItems, itemNum);

    printf("Deleted: %s\n", ti->item);
    todoitem_write_all(todoItems, doneItems, todoFile);
}

COMMAND(cmd_list)
{
    printf("%d Todo item%c\n---\n", todoItems->count, todoItems->count > 1 ? 's' : ' ');
    todoitem_write_items(todoItems, stdout);
}

COMMAND(cmd_done)
{
    int itemNum = atoi(argv[0]) - 1;
    TodoItem *ti = todoitem_get_item(todoItems, itemNum);
    todoitem_remove(todoItems, itemNum);
    todoitem_add(doneItems, ti);

    printf("Completed: %s\n", ti->item);
    todoitem_write_all(todoItems, doneItems, todoFile);
}

#define MAX_PRIORITY 5
int clamp(int num, int min, int max)
{
    if (num > max)
    {
        return max;
    }

    if (num < min)
    {
        return min;
    }

    return num;
}

COMMAND(cmd_priority)
{
    int itemNum = atoi(argv[0]) - 1;
    TodoItem *ti = todoitem_get_item(todoItems, itemNum);
    int newPri = clamp(ti->priority - 1, 0, MAX_PRIORITY);
    if (newPri != ti->priority)
    {
        ti->priority = newPri;
        todoitem_remove(todoItems, itemNum);
        todoitem_add(todoItems, ti);
        todoitem_write_all(todoItems, doneItems, todoFile);
    }

    printf("Increased priority of: %s\n", ti->item);
}

COMMAND(cmd_depriority)
{
    int itemNum = atoi(argv[0]) - 1;
    TodoItem *ti = todoitem_get_item(todoItems, itemNum);
    int newPri = clamp(ti->priority + 1, 0, MAX_PRIORITY);
    if (newPri != ti->priority)
    {
        ti->priority = newPri;
        todoitem_remove(todoItems, itemNum);
        todoitem_add(todoItems, ti);
        todoitem_write_all(todoItems, doneItems, todoFile);
    }

    printf("Decreased priority of: %s\n", ti->item);
}

COMMAND(cmd_num)
{
    printf("%d\n", todoItems->count);
}

COMMAND(cmd_search)
{
    if (argc < 1)
    {
        error_and_exit("Too few parameters. What are you searching for?");
    } else if (argc > 1) {
        error_and_exit("Too many parameters.");
    }

    char dateStr[19] = {0};
    char countStr[5] = {0};
    sprintf(countStr, "%d", todoItems->count);

    int count = 0;
    TodoItem *item = todoItems->root.next;
    while (item)
    {
        count++;
        if (strstr(item->item, argv[0]))
        {
            struct tm *itemTm = localtime(&item->datetime);
            strftime(dateStr, 19, "%b %d, %Y %H:%M", itemTm);
            printf(" %*d ", (int) strlen(countStr), count);
            printf("%s %s%c%s %s\n", dateStr, getColor(item->priority), item->priority + 'A', COLOR_RESET, item->item);
        }
        item = item->next;
    }
}

#endif
