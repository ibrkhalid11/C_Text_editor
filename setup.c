
#include "setup.h"
void initEditor(void){
    E.cx = 0;
    E.cy = 0;
    E.rx = 0;
    E.row = NULL;
    E.rowoff = 0;
    E.coloff = 0;
    E.dirty = 0;
    E.filename = NULL;
    E.numrows = 0;
    E.statusmsg[0] = '\0';
    E.statusmsg_time = 0;

    if(getWindowSize(&E.screenrows, &E.screencols) == -1){die("getWindowSize");}
    E.screenrows -= 2;
}


