#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_usage(char *cmd)
{
    printf("Usage:\n");
    printf("\t%s <input_file>\n", cmd);
}

void error_and_exit(char *cmd, char *msg)
{
    printf("%s\n", msg);
    print_usage(cmd);
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        error_and_exit(argv[0], "Too few parameters");
    }

    if (argc > 2)
    {
        error_and_exit(argv[0], "Too many parameters");
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL)
    {
        perror("inputFile fopen()");
        char *msg = calloc(256, sizeof(*msg));
        strcat(msg, "Could not open: ");
        strcat(msg, argv[1]);
        error_and_exit(argv[0], msg);
    }

    char line[256];
    while (fgets(line, 256, inputFile))
    {
        if (line[0] == '-')
        {
            printf("%s", line);
            continue;
        }

        // NOTE: We only care about the first set of data, the rest can remain as-is
        struct tm datetime = {};
        int year, month, day, hour, minute;
        sscanf(line, "%4d%2d%2d%2d%2d", &year, &month, &day, &hour, &minute);

        datetime.tm_year = year - 1900;
        datetime.tm_mon = month - 1;
        datetime.tm_mday = day;
        datetime.tm_hour = hour;
        datetime.tm_min = minute;

        // NOTE: Skip the current datetime in line, and skip the space
        char *newLine = strstr(line, " ");
        printf("%ld %s", (long) mktime(&datetime), newLine + 1);

    }

    return 0;
}
