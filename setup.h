#ifndef SETUP_H
#define SETUP_H

#include "utils.h"
#include "editorTypes.h"
#include "insideFunc.h"
#include "termIO.h"
extern struct editorConfig E;

void    initEditor(void);
void    editorSave(void);
char    *editorPrompt(char *prompt);


#endif



