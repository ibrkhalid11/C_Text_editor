#include "insideFunc.h"

void _die(const char *s){
    perror(s);
    exit(1);
}
void die(const char *s){
    if(write(STDOUT_FILENO, "\x1b[2J", 4) == -1 ){_die("write");}
    if(write(STDOUT_FILENO, "\x1b[H", 3) == -1){_die("write");}
    _die(s);
}


void abAppend(struct abuf *ab, const char *s, int len){
    char *new = realloc(ab->b, (size_t)(ab->len + len));
    if (new == NULL) return;
    memcpy(&new[ab->len], s, (size_t)len);
    ab->b = new;
    ab->len += len;
}
void abFree(struct abuf *ab){
    free(ab->b);
}

