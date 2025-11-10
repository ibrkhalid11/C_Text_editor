#include "utils.h"
#include "editorTypes.h"
#include "insideFunc.h"
#include "termIO.h"
#include "editRowOps.h"
#include "keyOps.h"
#include "setup.h" 
#include "bar.h"
/*** function prototypes ***/

void    editorRefreshScreen(void);
void    editorOpen(char *filename);
void    editorScroll(void);


void    editorSave(void);


char    *editorPrompt(char *prompt);
/*** global variables ***/
struct editorConfig E;


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
void editorSave(void){
    int len;
    char *buf;
    int fd;
    if(E.filename == NULL){
        E.filename = editorPrompt("Save as %s(ESC TO CANCEL)");
        if(E.filename == NULL){
            editorSetStatusMessage("Save aborted");
            return;
        }
    }
    buf = editorRowsToString(&len);
    fd = open(E.filename, O_RDWR | O_CREAT, 0644);
    if(fd != -1){
        if(ftruncate(fd, len) != -1){
            if(write(fd, buf, (size_t)len)){
                close(fd);
                free(buf);
                E.dirty = 0;
                editorSetStatusMessage("%d bytes written to disk", len);
                return;
            }
        }
        close(fd);
    }
    free(buf);
    editorSetStatusMessage("Unable to save!!! I/O Error: %s", strerror(errno));
}

char *editorPrompt(char *prompt){
    int c;
    size_t bufsize = 128;
    char *buf = malloc(bufsize);
    size_t buflen = 0;
    buf[0] = '\0';

    while(1){
        editorSetStatusMessage(prompt, buf);
        editorRefreshScreen();
        c = editorReadKey();
        if(c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE){
            if(buflen != 0) buf[--buflen] = '\0';
        } else if(c == '\x1b'){
            editorSetStatusMessage("");
            free(buf);
            return NULL;
        } else if(c == '\r'){
            if(buflen != 0){
                editorSetStatusMessage("");
                return buf;
            }
        }
        else if(!iscntrl(c) && c < 128){
            if(buflen == bufsize - 1){
                bufsize *= 2;
                buf = realloc(buf, bufsize);
            }
            buf[buflen++] = c;
            buf[buflen] = '\0';
        }
    }
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
/*main logic*/
int main(int argc, char *argv[]){
    enableRawMode();
    initEditor();
    if (argc >= 2){
        editorOpen(argv[1]);
    }
    editorSetStatusMessage("HELP: CTRL-S = save | CTRL-Q = quit");
    while(1){
        editorRefreshScreen();
        editorProcessKeypress();
    }
    return 0;
}


