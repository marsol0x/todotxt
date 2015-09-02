#ifndef TODOITEMS_H
#define TODOITEMS_H

#include <time.h>

typedef struct todoitem
{
    struct todoitem *next;
    struct todoitem *prev;

    time_t datetime;
    int priority;
    char item[MAX_ITEM_LEN];
} TodoItem;

typedef struct
{
    TodoItem root;
    int count;
} TodoList;

void todoitem_add(TodoList *list, TodoItem *ti)
{
    TodoItem *t = &list->root;
    while (t->next != NULL)
    {
        // Priority of 0 is the highest, going lower as the priority int increases
        if (ti->priority < t->next->priority)
        {
            break;
        } else {
            t = t->next;
            continue;
        }

        if (difftime(ti->datetime, t->next->datetime) < 0)
        {
            break;
        } else {
            t = t->next;
        }
    }

    ti->next = t->next;
    ti->prev = t;
    t->next = ti;

    if (ti->next)
    {
        ti->next->prev = ti;
    }

    list->count++;
}

void todoitem_remove(TodoList *list, int id)
{
    if (id >= list->count)
    {
        return;
    }

    TodoItem *ti = list->root.next;
    for (int i = 0; i < id; ++i)
    {
        ti = ti->next;
    }

    if (ti->prev)
    {
        ti->prev->next = ti->next;
    }
    if (ti->next)
    {
        ti->next->prev = ti->prev;
    }

    ti->next = 0;
    ti->prev = 0;

    list->count--;
}

TodoItem * todoitem_get_item(TodoList *list, int id)
{
    if (id >= list->count)
    {
        return NULL;
    }

    TodoItem *ti = list->root.next;
    for (int i = 0; i < id; ++i)
    {
        ti = ti->next;
    }

    return ti;
}

void todoitem_get_items(FILE *file, TodoList *list)
{
    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, file) != NULL)
    {
        if (line[0] == '-')
        {
            break;
        }

        char *pos;
        time_t epoch_time;
        char *priority;
        char *item;

        pos = strchr(line, ' ');
        *pos++ = 0;
        epoch_time = (time_t) atol(line);

        priority = pos;
        pos = strchr(pos, ' ');
        *pos++ = 0;

        item = pos;
        pos = strchr(pos, '\n');
        *pos = 0;

        TodoItem *ti = (TodoItem *) malloc(sizeof(TodoItem));
        memset(ti, 0, sizeof(TodoItem));
        ti->datetime = epoch_time;
        ti->priority = (int) (priority[0] - 'A');
        strncpy(ti->item, item, MAX_ITEM_LEN - 1);

        todoitem_add(list, ti);
    }
}

void todoitem_write_items(TodoList *list, FILE *out)
{
    int itemNum = 1;
    TodoItem *item = list->root.next;

    while (item != NULL)
    {
        if (out == stdout)
        {
            char countStr[5];
            char dateStr[19];

            struct tm *itemTm = localtime(&item->datetime);
            strftime(dateStr, 19, "%b %d, %Y %H:%M", itemTm);

            sprintf(countStr, "%d", list->count);
            fprintf(out, " %*d ", (int) strlen(countStr), itemNum++);
            fprintf(out, "%s %c %s\n", dateStr, item->priority + 'A', item->item);
        } else {
            fprintf(out, "%ld %c %s\n", (long) item->datetime, item->priority + 'A', item->item);
        }
        item = item->next;
    }
}

void todoitem_write_all(TodoList *todoItems, TodoList *doneItems, FILE *todoFile)
{
    rewind(todoFile);
    ftruncate(fileno(todoFile), 0);
    todoitem_write_items(todoItems, todoFile);
    fprintf(todoFile, "-\n");
    todoitem_write_items(doneItems, todoFile);
}

#endif
