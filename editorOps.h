#ifndef EDITOROOPS_H
#define EDITOROOPS_H
#include "utils.h"
#include "editorTypes.h"
#include "insideFunc.h"
#include "editRowOps.h"
#include "bar.h"
extern struct editorConfig E;

void    editorRefreshScreen(void);
void    editorOpen(char *filename);
void    editorScroll(void);

#endif