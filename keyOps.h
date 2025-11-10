#ifndef KEYOPS_H
#define KEYOPS_H

#include "utils.h"
#include "editorTypes.h" /* not sure if we need this*/
#include "editRowOps.h"
#include "insideFunc.h"


/* prototypes NOT implemented in this file*/
void    editorSetStatusMessage(const char *fmt, ...);
void    editorSave(void);
void    editorMoveCursor(int key);




int     editorReadKey(void);
void    editorProcessKeypress(void);
void    editorInsertChar(int c);
void    editorInsertNewLine(void);

#endif
