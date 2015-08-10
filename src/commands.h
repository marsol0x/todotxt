#ifndef COMMANDS_H
#define COMMANDS_H

typedef void Command(TodoItem *todoItems, TodoItem *doneItems, int argc, char **argv);
#define COMMAND(name) void cmd_##name(TodoItem *todoItems, TodoItem *doneItems, int argc, char **argv)

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
    // NOTE: The first item is the sentinel
    TodoItem *ti = todoItems->next;
    todoitem_print_items(ti);
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
    printf("Num command\n");
}

#endif
