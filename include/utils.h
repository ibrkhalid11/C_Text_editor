#ifndef UTILS_H
#define UTILS_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE



/*** includes ***/
#include <errno.h>      /*editorSave, editorReadKey*/
#include <ctype.h>      /*editorPrompt*/
#include <fcntl.h>      /*editorSave*/
#include <stdarg.h>     /*editorSetStatusMessage*/
#include <stdlib.h>     /* editorUpdateRow, editorInsertRow, editorFreeRow, 
                         * editorRowInsertChar, editorRowAppendString
                         * editorRowsToString, editorSave, abAppend
                         * abFree, _die, enableRawMode, editorPrompt
                         * editorProcessKeypress                    */
#include <stdio.h>
#include <string.h>
#include <termios.h>    /* disableRawMode, enableRawMode*/
#include <time.h>       /* editorDrawMessageBar*/
#include <unistd.h>     /* editorSave, getCursorPosition, getWindowSize
                         * die, disableRawMode, enableRawMode, 
                         * editorReadKey, editorProcessKeypress
                         * editorRefreshScreen*/
#include <sys/ioctl.h>  /* getWindowSize */
#include <sys/types.h>
/*** hash defines ***/
#define VERSION "0.0.1" 
#define CTRL_KEY(k) ((k) & 0x1F)
#define ABUF_INIT {NULL, 0}
#define IBREDIT_TAB_STOP 8
#define QUIT_TIMES 3



#endif
