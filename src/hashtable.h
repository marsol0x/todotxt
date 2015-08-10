#ifndef HASHTABLE_H
#define HASHTABLE_H

#define MAX_HASH_KEY_SIZE 20

typedef struct hashitem
{
    struct hashitem *next;
    struct hashitem *prev;
    char key[MAX_HASH_KEY_SIZE + 1];

    Command *cmd;
} HashItem;

unsigned int hashtable_hash(char *str, int len)
{
    unsigned int result = 0;
    while (*str)
    {
        result += *str++ + 64;
    }

    result %= len;

    return result;
}

void hashtable_add(HashItem **table, int len, char *key, Command *cmd)
{
    if (!table || !key || !cmd)
    {
        return;
    }

    HashItem *hi = (HashItem *) malloc(sizeof(HashItem));
    memset(hi, 0, sizeof(HashItem));
    strncpy(hi->key, key, MAX_HASH_KEY_SIZE);
    hi->cmd = cmd;

    unsigned int hash = hashtable_hash(key, len);
    if (table[hash] == NULL)
    {
        table[hash] = hi;
    } else {
        HashItem *item = table[hash];
        while (item->next != NULL)
        {
            item = item->next;
        }
        item->next = hi;
    }
}

HashItem * hashtable_get(HashItem **table, int len, char *key)
{
    unsigned int hash = hashtable_hash(key, len);
    HashItem *item = table[hash];

    while (item)
    {
        if (strncmp(item->key, key, MAX_HASH_KEY_SIZE) == 0)
        {
            return item;
        } else {
            item = item->next;
        }
    }

    return NULL;
}

#endif
