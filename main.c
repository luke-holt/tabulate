#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "util.h"
#include "dynamic_array.h"

#define MAX_LINE_LEN (256u)

#define HELP "usage: table <csv-file>"

typedef struct {
    char *str;
    size_t len;
    char **cells;
    size_t ncells;
} Line;


static size_t sanitize_str(char *str);

static Line *line_from_csv(char *str, size_t len);
static void line_free(Line *line);

static FILE *fin;
bool fin_is_stdin;

static char inbuf[MAX_LINE_LEN];
static size_t inbuf_len;

static Line **lines;

int
main(int argc, char *argv[])
{
    // args

    if (argc > 2) {
        die(HELP);
    }
    else if (argc == 2) {
        // read file
        char *name = argv[1];
        if (access(name, F_OK))
            die("Could not find %s", name);
        fin = fopen(name, "r");
        fin_is_stdin = true;
    }
    else {
        // read stdin
        fin = stdin;
        fin_is_stdin = false;
    }

    // process input

    lines = da_create(sizeof(Line *), 32);

    Line *l;
    while (fgets(inbuf, MAX_LINE_LEN, fin)) {
        inbuf_len = sanitize_str(inbuf);
        l = line_from_csv(inbuf, inbuf_len);
        if (l)
            da_append(lines, &l);
    }
    if (fin_is_stdin)
        fclose(fin);

    // format

    size_t cols, *widths, max;
    cols = lines[0]->ncells;
    widths = ecalloc(cols, sizeof(*widths));
    for (size_t c = 0; c < cols; c++) {
        max = 0;
        for (size_t r = 0; r < da_len(lines); r++)
            max = MAX(max, strlen(lines[r]->cells[c]));
        widths[c] = max;
    }

    // print

    for (size_t r = 0; r < da_len(lines); r++) {
        char rowstr[256];
        size_t offset;
        memset(rowstr, ' ', sizeof(rowstr));
        offset = 0;
        for (size_t c = 0; c < cols; c++) {
            char *s;
            s = lines[r]->cells[c];
            strncpy(&rowstr[offset], s, strlen(s));
            offset += widths[c] + 2;
        }
        rowstr[offset] = '\0';
        printf("%s\n", rowstr);
    }

    // cleanup

    for (size_t i = 0; i < da_len(lines); i++)
        line_free(lines[i]);
    da_destroy(lines);

    return 0;
}

Line *
line_from_csv(char *str, size_t len)
{
    Line *l;
    l = emalloc(sizeof(Line));

    l->str = emalloc(len);
    memcpy(l->str, str, len);
    l->len = len;

    l->ncells = 0;
    for (size_t i = 0; i < l->len; i++) {
        if (l->str[i] == '\0')
            l->ncells++;
    }

    l->cells = ecalloc(l->ncells, sizeof(char *));
    size_t i = 0, pos = 0;
    while (i < l->ncells) {
        while (' ' == l->str[pos])
            pos++;
        l->cells[i++] = &l->str[pos];
        pos += strlen(&l->str[pos]) + 1;
    }
    return l;
}

size_t
sanitize_str(char *str)
{
    char *write;
    bool escape;
    size_t i, len;

    if (MAX_LINE_LEN < strlen(str))
        die("MAX_LINE_LEN:(%lu) < strlen(str):(%lu)", MAX_LINE_LEN, strlen(str));

    write = str;
    escape = false;
    i = len = 0;
    while (str[i] != '\0') {
        // escaped double quote
        if (str[i] == '"' && str[i+1] == '"') {
            i++;
        }
        // open double quote
        else if (!escape && str[i] == '"') {
            escape = true;
            i++;
            continue;
        }
        // close double quote
        else if (escape && str[i] == '"') {
            escape = false;
            i++;
            continue;
        }

        // replace comma and newline w terminator
        if (!escape && ((str[i] == ',') || (str[i] == '\n')))
            str[i] = '\0';

        *write++ = str[i++];
        len++;
    }
    *write = '\0';
    len++;
    return len;
}

void
line_free(Line *line)
{
    free(line->str);
    free(line->cells);
    free(line);
}

