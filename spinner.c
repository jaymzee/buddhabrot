#include <stdio.h>
#include <string.h>

void init_spinner(const char *msg)
{
    fprintf(stderr, "%s       ", msg);
}

void update_spinner(double completed)
{
    fprintf(stderr, "\b\b\b\b\b\b%5.1f%%", 100.0 * completed);
}

void finish_spinner(const char *msg)
{
    const size_t n  = strlen(msg) + 7;
    update_spinner(1.0);
    for (int i = 0; i < n; i++) {
        fputc('\b', stderr);
    }
}
