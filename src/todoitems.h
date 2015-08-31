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

int compare_datetime(struct tm *a, struct tm *b)
{
    if (a->tm_year > b->tm_year &&
        a->tm_mon > b->tm_mon &&
        a->tm_mday > b->tm_mday &&
        a->tm_hour > b->tm_hour &&
        a->tm_min > b->tm_min)
    {
        return -1;
    } else if (a->tm_year < b->tm_year &&
               a->tm_mon < b->tm_mon &&
               a->tm_mday < b->tm_mday &&
               a->tm_hour < b->tm_hour &&
               a->tm_min < b->tm_min)
    {
        return 1;
    }

    return 0;
}

void todoitem_add(TodoList *list, TodoItem *ti)
{
    TodoItem *t = &list->root;
    while (t->next != NULL)
    {
        // Priority of 0 is the highest, going lower as the priority int increases
        if (ti->priority < t->next->priority || compare_datetime(&ti->datetime, &t->next->datetime) > 0)
        {
            break;
        }
        t = t->next;
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

void todoitem_write_all(TodoList *todoItems, TodoList *doneItems, FILE *todoFile)
{
    rewind(todoFile);
    ftruncate(fileno(todoFile), 0);
    todoitem_write_items(todoItems, todoFile);
    fprintf(todoFile, "-\n");
    todoitem_write_items(doneItems, todoFile);
}

#endif
