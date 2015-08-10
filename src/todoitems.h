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

void todoitem_add(TodoItem *root, TodoItem *ti)
{
    TodoItem *t = root;
    while (t->next != NULL)
    {
        t = t->next;
    }
    t->next = ti;
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

void todoitem_get_items(FILE *file, TodoItem *list)
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

void todoitem_print_items(TodoItem *items)
{
    int itemNum = 1;
    char datetimeLine[MAX_LINE_LEN] = {};
    while (items != NULL)
    {
        strftime(datetimeLine, MAX_LINE_LEN, "%Y%m%d%H%M", &items->datetime);
        printf("%d %s %c %s\n", itemNum++, datetimeLine, items->priority + 'A', items->item);
        items = items->next;
    }
}

#endif
