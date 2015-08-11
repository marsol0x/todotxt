#ifndef COMMANDS_H
#define COMMANDS_H

typedef void Command(TodoList *todoItems, TodoList *doneItems, int argc, char **argv);
#define COMMAND(name) void cmd_##name(TodoList *todoItems, TodoList *doneItems, int argc, char **argv)

COMMAND(add)
{
    printf("Add command\n");
}

COMMAND(delete)
{
    printf("Delete command\n");
}

COMMAND(list)
{
    todoitem_print_items(todoItems);
}

COMMAND(done)
{
    printf("Done command\n");
}

COMMAND(priority)
{
    printf("Priority command\n");
}

COMMAND(depriority)
{
    printf("Depriority command\n");
}

COMMAND(num)
{
    printf("%d\n", todoItems->count);
}

#endif
