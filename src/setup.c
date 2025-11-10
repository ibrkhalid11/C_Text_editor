
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