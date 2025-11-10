#include "editorOps.h"

void editorOpen(char *filename){
    free(E.filename);
    E.filename = strdup(filename);
    FILE *fp = fopen(filename, "r");
    if(!fp) die("fopen");

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while((linelen = getline(&line, &linecap, fp)) != -1){
        if(linelen != -1){
            while(linelen > 0 && (line[linelen -1] == '\n' || line[linelen - 1] == '\r')){
                linelen--;
            }
            editorInsertRow(E.numrows,line, (size_t)linelen);
        }
    }
    free(line);
    fclose(fp);
    E.dirty = 0;
}




void editorRefreshScreen(void){
    char buf[32];
    struct abuf ab = ABUF_INIT;
    editorScroll();
    abAppend(&ab, "\x1b[?25l", 6);
    abAppend(&ab, "\x1b[H", 3);
    abAppend(&ab, "\x1b[2J", 4);
    editorDrawRows(&ab);
    editorDrawStatusBar(&ab);
    editorDrawMessageBar(&ab);

    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1, (E.rx - E.coloff) + 1);
    abAppend(&ab, buf, (int)strlen(buf));

    abAppend(&ab, "\x1b[?25h", 6);

    if(write(STDOUT_FILENO, ab.b, (size_t)ab.len) == -1){die("write");}
    
    abFree(&ab);
}
void editorScroll(void){
    E.rx = 0;
    if(E.cy < E.numrows){
        E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
    }
    if(E.cy < E.rowoff){
        E.rowoff = E.cy;
    }
    if(E.cy >= E.rowoff + E.screenrows + 1){
        E.rowoff = E.cy - E.screenrows + 1;
    }
    if(E.rx < E.coloff){
        E.coloff = E.rx;
    }
    if(E.rx  >= E.coloff + E.screencols){
        E.coloff = E.rx - E.screencols + 1;
    }
}