#ifndef EDITROWOPS_H
#define EDITROWOPS_H

#include "utils.h"
#include "editorTypes.h"
#include "insideFunc.h"
/*extern E variable is used*/
extern struct editorConfig E;

/*function prototypes*/
int     editorRowCxToRx(erow *row, int cx);
void    editorUpdateRow(erow *row);
void    editorInsertRow(int at, char *s, size_t len);
void    editorRowInsertChar(erow *row, int at, int c);
char    *editorRowsToString(int *buflen);
void    editorRowDeleteChar(erow *row, int at);
void    editorRowDelChar(void);
void    editorFreeRow(erow *row);
void    editorDelRow(int at);
void    editorRowAppendString(erow *row, char *s, size_t len);
void    editorDrawRows(struct abuf *ab);
#endif
