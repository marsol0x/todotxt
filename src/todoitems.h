#ifndef TODOITEMS_H
#define TODOITEMS_H

#include <time.h>

typedef struct todoitem
{
    struct todoitem *next;
    struct todoitem *prev;

    struct tm datetime;
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
        t = t->next;
    }
    t->next = ti;
    ti->prev = t;
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

    ti->prev->next = ti->next;
    ti->next->prev = ti->prev;

    ti->next = 0;
    ti->prev = 0;
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

void todoitem_set_datetime(TodoItem *item, char *datestr)
{
    struct tm todoTime = {};

    int year, month, day, hour, minute;
    sscanf(datestr, "%4d%2d%2d%2d%2d", &year, &month, &day, &hour, &minute);

    todoTime.tm_year = year - 1900;
    todoTime.tm_mon = month;
    todoTime.tm_mday = day;
    todoTime.tm_hour = hour;
    todoTime.tm_min = minute;

    item->datetime = todoTime;
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
        char *datestr;
        char *priority;
        char *item;

        datestr = line;
        pos = strchr(line, ' ');
        *pos++ = 0;

        priority = pos;
        pos = strchr(pos, ' ');
        *pos++ = 0;

        item = pos;
        pos = strchr(pos, '\n');
        *pos = 0;

        TodoItem *ti = (TodoItem *) malloc(sizeof(TodoItem));
        memset(ti, 0, sizeof(TodoItem));
        todoitem_set_datetime(ti, datestr);
        ti->priority = (int) (priority[0] - 'A');
        strncpy(ti->item, item, MAX_ITEM_LEN - 1);

        todoitem_add(list, ti);
    }
}

void todoitem_write_items(TodoList *list, FILE *out)
{
    int itemNum = 1;
    char datetimeLine[MAX_LINE_LEN] = {};
    TodoItem *item = list->root.next;

    while (item != NULL)
    {
        strftime(datetimeLine, MAX_LINE_LEN, "%Y%m%d%H%M", &item->datetime);
        if (out == stdout)
        {
            char countStr[5];
            sprintf(countStr, "%d", list->count);
            fprintf(out, " %*d ", (int) strlen(countStr), itemNum++);
        }
        fprintf(out, "%s %c %s\n", datetimeLine, item->priority + 'A', item->item);
        item = item->next;
    }
}

#endif
