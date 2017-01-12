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

char validatePriorityLetter(char priorityLetter)
{
    char result = toupper(priorityLetter);
    if (result > 'F')
    {
        result = 'F';
    } else if (result < 'A') {
        result = 'A';
    }

    return result;
}

int getPriorityNumber(char priorityLetter)
{
    int result;
    char priority = validatePriorityLetter(priorityLetter);

    result = priority - 'A';

    return result;
}

char getPriorityLetter(int priority)
{
    switch (priority)
    {
        case 0: return 'A';
        case 1: return 'B';
        case 2: return 'C';
        case 3: return 'D';
        case 4: return 'E';
        case 5:
        default:
         return 'F';
    }
}

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

char * getColor(int priority)
{
    int p = priority + 'A';
    switch (p)
    {
        case 'A': { return COLOR_RED; } break;
        case 'B': { return COLOR_BLUE; } break;
        case 'C': { return COLOR_YELLOW; } break;
        case 'D': { return COLOR_MAGENTA; } break;
        case 'E': { return COLOR_GREEN; } break;

        case 'F': 
        default:
        {
            return "";
        } break;
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
            fprintf(out, "%s %s%c%s %s\n", dateStr, getColor(item->priority), getPriorityLetter(item->priority), COLOR_RESET, item->item);
        } else {
            fprintf(out, "%ld %c %s\n", (long) item->datetime, getPriorityLetter(item->priority), item->item);
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
