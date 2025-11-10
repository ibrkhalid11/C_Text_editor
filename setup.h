#ifndef SETUP_H
#define SETUP_H

#include "utils.h"
#include "editRowOps.h"
#include "keyOps.h"
#include "editorTypes.h"
#include "insideFunc.h"
#include "termIO.h"
#include "editorOps.h"
extern struct editorConfig E;

void    initEditor(void);
void    editorSave(void);
char    *editorPrompt(char *prompt);


#endif



