#ifndef BAR_H
#define BAR_H

#include "utils.h"
#include "editorTypes.h"
#include "insideFunc.h"
extern struct editorConfig E;

void    editorDrawStatusBar(struct abuf *b);
void    editorSetStatusMessage(const char *fmt, ...);
void    editorDrawMessageBar(struct abuf *ab);

#endif
