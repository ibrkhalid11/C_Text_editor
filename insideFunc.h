#ifndef INSIDEFUNC_H
#define INSIDEFUNC_H

#include "utils.h"
#include "editorTypes.h"
void    _die(const char *s); /* error recorder and exit for write() in die() */
void    die(const char *s);  /* error recorder and exit for all code*/
void    abAppend(struct abuf *ab, const char *s, int len);
void    abFree(struct abuf *ab);

#endif
