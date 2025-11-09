#ifndef TERMIO_H
#define TERMIO_H
#include "utils.h"
#include "editorTypes.h"
#include "insideFunc.h"

extern struct editorConfig E;

void    enableRawMode(void);
void    disableRawMode(void);
int     getCursorPosition(int *rows, int *cols);
int     getWindowSize(int *rows, int *column);
#endif
