#ifndef COMMANDS_H
#define COMMANDS_H

#define COMMAND(name) void name(TodoList *todoItems, TodoList *doneItems, FILE *todoFile, int argc, char **argv)
typedef COMMAND(Command);

COMMAND(cmd_add)
{
    printf("Add command\n");
}

COMMAND(cmd_delete)
{
    int itemNum = atoi(argv[0]) - 1;
    TodoItem *ti = todoitem_get_item(todoItems, itemNum);
    todoitem_remove(todoItems, itemNum);

    printf("Deleted: %s\n", ti->item);

    rewind(todoFile);
    ftruncate(fileno(todoFile), 0);
    todoitem_write_items(todoItems, todoFile);
    fprintf(todoFile, "-\n");
    todoitem_write_items(doneItems, todoFile);
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

    rewind(todoFile);
    todoitem_write_items(todoItems, todoFile);
    fprintf(todoFile, "-\n");
    todoitem_write_items(doneItems, todoFile);
}

COMMAND(cmd_priority)
{
    printf("Priority command\n");
}

COMMAND(cmd_depriority)
{
    printf("Depriority command\n");
}

COMMAND(cmd_num)
{
    printf("%d\n", todoItems->count);
}

#endif
