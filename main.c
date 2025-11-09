#include "utils.h"
#include "editorTypes.h"
#include "insideFunc.h"
#include "termIO.h"
#include "editRowOps.h"
/*** function prototypes ***/

void    initEditor(void);
int     editorReadKey(void);
void    editorProcessKeypress(void);
void    editorRefreshScreen(void);
void    editorMoveCursor(int key);
void    editorUpdateRow(erow *row);
void    editorOpen(char *filename);
void    editorScroll(void);

void    editorDrawStatusBar(struct abuf *b);
void    editorSetStatusMessage(const char *fmt, ...);
void    editorDrawMessageBar(struct abuf *ab);

void    editorInsertChar(int c);
void    editorSave(void);

void    editorInsertNewLine(void);

char    *editorPrompt(char *prompt);
/*** global variables ***/
struct editorConfig E;

void editorInsertChar(int c){
    if(E.cy == E.numrows){
        editorInsertRow(E.numrows,"", 0);
    }
    editorRowInsertChar(&E.row[E.cy], E.cx, c);
    E.cx++;
}
void editorInsertNewLine(void){
    erow *row;
    if(E.cx == 0){
        editorInsertRow(E.cy, "", 0);
    } else{
        row = &E.row[E.cy];
        editorInsertRow(E.cy + 1, &row->chars[E.cx], (size_t)row ->size - (size_t)E.cx);
        row = &E.row[E.cy];
        row->size = E.cx;
        row->chars[row->size] = '\0';
        editorUpdateRow(row);
    }
    E.cy++;
    E.cx = 0;
}
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

/**/
int editorReadKey(void){
    char c;
    ssize_t nread;
    char seq[3];
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("read");
    }
    if(c == '\x1b'){
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
        if(seq[0] == '['){
            if(seq[1] >= '0' && seq[1] <= '9'){
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if(seq[2] == '~' ){
                    switch(seq[1]){
                        case '1': return HOME_KEY;
                        case '3': return DEL_KEY;
                        case '4': return END_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        case '7': return HOME_KEY;
                        case '8': return END_KEY;
                    }
                }
            } else{
                switch(seq[1]){
                    case 'A': return ARROW_UP; 
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;
                }
            }
        } else if(seq[0] == 'O'){
            switch(seq[1]){
                case 'H': return HOME_KEY;
                case 'F': return END_KEY;
            }
        }
        return '\x1b';
    } else{
        return c;
    }
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
void editorMoveCursor(int key){
    int rowlen;
    erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
    switch(key){
        case ARROW_LEFT: 
            if(E.cx != 0){
                E.cx--;
            } else if(E.cy > 0){
                E.cy--;
                E.cx = E.row[E.cy].size;
            }
            break;
        case ARROW_RIGHT: 
            if(row && E.cx < row->size){
                E.cx++;
            } else if(row && E.cx ==row ->size){
                E.cy++;
                E.cx = 0;
            }
            break;
        case ARROW_UP: 
            if(E.cy != 0){
                E.cy--;
            } 
            break;
        case ARROW_DOWN: 
            if(E.cy != E.numrows){
                E.cy++;
            } 
            break;
    }
    row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
    rowlen = row ? row -> size : 0;
    if(E.cx > rowlen){
        E.cx = rowlen;
    }
}
void editorProcessKeypress(void){
    int times;
    static int quit_times = QUIT_TIMES;
    int c = editorReadKey();
    
    switch (c) {
        case '\r':
            editorInsertNewLine();
            break;
        case CTRL_KEY('q'):
            if(E.dirty && quit_times > 0){
                editorSetStatusMessage("YOU WANT TO QUIT: FILE IS UNSAVED, HIT CTRL-Q %d more times to quit.", quit_times);
                quit_times--;
                return;
            }
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
        case CTRL_KEY('s'):
            editorSave(); 
            break;
        case HOME_KEY:
            E.cx = 0; break;
        case END_KEY:
            if(E.cy < E.numrows){
                E.cx = E.row[E.cy].size;
            }
            break;
        case BACKSPACE:
        case CTRL_KEY('h'):
        case DEL_KEY:
            if(c == DEL_KEY) editorMoveCursor(ARROW_RIGHT);
            editorRowDelChar();
            /*TODO*/
            break;
        case PAGE_UP:
        case PAGE_DOWN:
            {
                if(c == PAGE_UP){
                    E.cy = E.rowoff;
                } else if(c == PAGE_DOWN){
                    E.cy = E.rowoff + E.screenrows - 1;
                    if(E.cy > E.numrows) E.cy = E.numrows;
                }
                times = E.screenrows;
                while (times--)
                    editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
            }
            break;
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            editorMoveCursor(c);
            break;
        case CTRL_KEY('l'): 
        case '\x1b':
            break;
        default: editorInsertChar(c); break;
    }
    quit_times = QUIT_TIMES;
}
void editorDrawStatusBar(struct abuf *ab){
    int len;
    char status[80];
    char rstatus[80];
    int rlen;
    abAppend(ab, "\x1b[7m", 4);
    len = snprintf(status, sizeof(status), "%.20s - %d lines %s", E.filename ? E.filename : "[No Name]", E.numrows, E.dirty ? "(modified)": "");
    rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d", E.cy + 1, E.numrows);
    if(len > E.screencols) len = E.screencols;
    abAppend(ab, status, len);
    while(len < E.screencols){
        if(E.screencols - len == rlen){
            abAppend(ab, rstatus, rlen);
            break;
        } else{
            abAppend(ab, " ", 1);
            len++;
        }
    }
    abAppend(ab, "\x1b[m", 3);
    abAppend(ab, "\r\n", 2);
}
void editorDrawMessageBar(struct abuf *ab){
    int msglen;
    abAppend(ab, "\x1b[K", 3);
    msglen = (int)strlen(E.statusmsg);
    if (msglen > E.screencols) msglen = E.screencols;
    if (msglen && time(NULL) - E.statusmsg_time < 5)
        abAppend(ab, E.statusmsg, msglen);
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
void editorSetStatusMessage(const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
    va_end(ap);
    E.statusmsg_time = time(NULL);
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
void editorDrawRows(struct abuf *ab) {
  int y;
  int welcomelen;
  int padding;
  int filerow;
  char welcome[80];
  for (y = 0; y < E.screenrows; y++) {
      filerow = y + E.rowoff;
      if (filerow >= E.numrows) {
          if (E.numrows == 0 && y == E.screenrows / 3) {
              welcomelen = snprintf(welcome, sizeof(welcome),
                      "ibredit -- version %s", VERSION);
              if (welcomelen > E.screencols) welcomelen = E.screencols;
              padding = (E.screencols - welcomelen) / 2;
              if (padding) {
                  abAppend(ab, "~", 1);
                  padding--;
              }
              while (padding--) abAppend(ab, " ", 1);
              abAppend(ab, welcome, welcomelen);
          } else {
              abAppend(ab, "~", 1);
          }
      } else {
          int len = E.row[filerow].rsize - E.coloff;
          if(len < 0) len = 0;
          if (len > E.screencols) len = E.screencols;
          abAppend(ab, &E.row[filerow].render[E.coloff], len);
      }
      abAppend(ab, "\x1b[K", 3);
      abAppend(ab, "\r\n", 2);
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


