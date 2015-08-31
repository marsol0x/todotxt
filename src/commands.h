#ifndef COMMANDS_H
#define COMMANDS_H

#define COMMAND(name) void name(TodoList *todoItems, TodoList *doneItems, FILE *todoFile, int argc, char **argv)
typedef COMMAND(Command);

COMMAND(cmd_add)
{
    if (argc > 1)
    {
        error_and_exit("Too many parameters. Please use quotes to add new todo items.");
    } else if (argc == 0) {
        error_and_exit("Too few parameters.");
    }

    time_t t = time(0);
    TodoItem ti;
    ti.next = ti.prev = 0;
    ti.priority = 5;
    localtime_r(&t, &ti.datetime);
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

#endif
