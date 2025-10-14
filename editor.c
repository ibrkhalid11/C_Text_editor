#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE



/*** includes ***/
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
/*** hash defines ***/
#define VERSION "0.0.1" 
#define CTRL_KEY(k) ((k) & 0x1F)
#define ABUF_INIT {NULL, 0}
#define IBREDIT_TAB_STOP 8

enum editorKey{
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};
struct abuf{
    char *b;
    int len;
};
typedef struct erow{
    int size;
    int rsize;
    char *chars;
    char *render;
}erow;

struct editorConfig{
    int cx;
    int cy;
    int rx;
    int rowoff;
    int coloff;
    int screenrows;
    int screencols;
    int numrows;
    erow *row;
    char *filename;
    char statusmsg[80];
    time_t statusmsg_time;
    struct termios orig_termios;
};

/*** function prototypes ***/
void    enableRawMode(void);
void    disableRawMode(void);

void    _die(const char *s); /* error recorder and exit for write() in die() */
void    die(const char *s);  /* error recorder and exit for all code*/

void    initEditor(void);
int     editorReadKey(void);
void    editorProcessKeypress(void);
void    editorRefreshScreen(void);
void    editorDrawRows(struct abuf *ab);
void    editorMoveCursor(int key);
int     editorRowCxToRx(erow *row, int cx);
void    editorUpdateRow(erow *row);
void    editorOpen(char *filename);
void    editorAppendRow(char *s, size_t len);
void    editorScroll(void);
void    abAppend(struct abuf *ab, const char *s, int len);
void    abFree(struct abuf *ab);
int     getCursorPosition(int *rows, int *cols);
int     getWindowSize(int *rows, int *column);

void    editorDrawStatusBar(struct abuf *b);
void    editorSetStatusMessage(const char *fmt, ...);
void    editorDrawMessageBar(struct abuf *ab);

void    editorRowInsertChar(erow *row, int at, int c);
void    editorInsertChar(int c);
void    editorSave(void);
char    *editorRowsToString(int *buflen);
/*** global variables ***/
struct editorConfig E;

int editorRowCxToRx(erow *row, int cx){
    int rx;
    int j;
    rx = 0;
    for(j = 0; j < cx; j++){
        if(row->chars[j] == '\t'){
            rx +=  (IBREDIT_TAB_STOP - 1) - (rx % IBREDIT_TAB_STOP);
        }
        rx++;
    }
    return rx;
}
void editorUpdateRow(erow *row){
    int j;
    int idx = 0;
    int tabs = 0;
    for(j = 0; j < row->size; j++){
        if(row ->chars[j] == '\t'){
            tabs++;
        }
    }

    free(row->render);
    row->render = malloc((size_t)row->size + (size_t) (tabs *(IBREDIT_TAB_STOP - 1)) + 1);
    
    for(j = 0; j < row->size; j++){
        if(row->chars[j] == '\t'){
            row -> render[idx++] = ' ';
            while (idx % (IBREDIT_TAB_STOP) != 0){row -> render[idx++] = ' ';}
        } else{
            row ->render[idx++] = row->chars[j];
        }
    }
    row -> render[idx] = '\0';
    row -> rsize = idx;
}
void editorAppendRow(char *s, size_t len){
    E.row = realloc(E.row, sizeof(erow) * (size_t)(E.numrows + 1));
    int at = E.numrows;

    E.row[at].size = (int)len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len);
    E.row[at].chars[len] = '\0';
    E.row[at].rsize = 0;
    E.row[at].render = NULL;
    editorUpdateRow(&E.row[at]);
    E.numrows++;
}
void editorRowInsertChar(erow *row, int at, int c){
    if(at < 0 || at > row->size) at = row-> size;
    row->chars = realloc(row->chars, (size_t)row->size + 2);
    memmove(&row->chars[at + 1], &row->chars[at], (size_t)(row->size - at + 1));
    row->size++;
    row->chars[at] = (char)c;
    editorUpdateRow(row);
}
void editorInsertChar(int c){
    if(E.cy == E.numrows){
        editorAppendRow("", 0);
    }
    editorRowInsertChar(&E.row[E.cy], E.cx, c);
    E.cx++;
}
char *editorRowsToString(int *buflen){
    int totlen = 0;
    int j;
    char *buf;
    char *p;
    for(j = 0; j < E.numrows; j++){
        totlen += E.row[j].size + 1;
    }
    *buflen = totlen;
    buf = malloc(totlen * sizeof(char));
    p = buf;
    for(j = 0; j < E.numrows; j++){
        memcpy(p, E.row[j].chars, E.row[j].size);
        p += E.row[j].size;
        *p = '\n';
        p++;
    }
    return buf;
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
            editorAppendRow(line, (size_t)linelen);
        }
    }
    free(line);
    fclose(fp);
}
void editorSave(void){
    int len;
    char *buf;
    int fd;
    if(E.filename == NULL) return;
    buf = editorRowsToString(&len);
    fd = open(E.filename, O_RDWR | O_CREAT, 0644);
    ftruncate(fd, len);
    write(fd, buf, (size_t)len);
    close(fd);
    free(buf);
}
void abAppend(struct abuf *ab, const char *s, int len){
    char *new = realloc(ab->b, (size_t)(ab->len + len));
    if (new == NULL) return;
    memcpy(&new[ab->len], s, (size_t)len);
    ab->b = new;
    ab->len += len;
}
void abFree(struct abuf *ab){
    free(ab->b);
}

void initEditor(void){
    E.cx = 0;
    E.cy = 0;
    E.rx = 0;
    E.row = NULL;
    E.rowoff = 0;
    E.coloff = 0;
    E.filename = NULL;
    E.numrows = 0;
    E.statusmsg[0] = '\0';
    E.statusmsg_time = 0;

    if(getWindowSize(&E.screenrows, &E.screencols) == -1){die("getWindowSize");}
    E.screenrows -= 2;
}

int getCursorPosition(int *rows, int *cols){
    char buf[32];
    unsigned int i = 0;

    if(write(STDOUT_FILENO, "\x1b[6n", 4) != 4 ){return -1;}
    
    while (i < sizeof(buf) - 1){
        if(read(STDIN_FILENO, &buf[i], 1) != 1) break; 
        if(buf[i] == 'R') {break;}
        i++;
    }
    buf[i] = '\0';
    if (buf[0] != '\x1b' || buf[1] != '[') return -1;
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
    return 0;
}

/*purpose of getwindowsize*/
int getWindowSize(int *rows, int *cols){
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) 
            return -1;
    return getCursorPosition(rows, cols);
    } else{
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        return 0;
    }
}

/*purpose of die...*/
void _die(const char *s){
    perror(s);
    exit(1);
}
void die(const char *s){
    if(write(STDOUT_FILENO, "\x1b[2J", 4) == -1 ){_die("write");}
    if(write(STDOUT_FILENO, "\x1b[H", 3) == -1){_die("write");}
    _die(s);
}

/*purpose of rawmode*/
void disableRawMode(void){
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1){
        die("tcsetattr");
    }
}

/*purpose of rawmode...*/
void enableRawMode(void){
    struct termios raw;
    tcgetattr(STDIN_FILENO, &E.orig_termios);
    atexit(disableRawMode);

    raw = E.orig_termios;
    raw.c_iflag &= (tcflag_t)~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= (tcflag_t)~(OPOST);
    raw.c_cflag |= (tcflag_t)(CS8);
    raw.c_lflag &= (tcflag_t)~(ECHO | IEXTEN | ICANON | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1){die("tcsetattr");}
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
    int c = editorReadKey();
    
    switch (c) {
        case '\r':
            /*TODO*/
            break;
        case CTRL_KEY('q'):
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
}
void editorDrawStatusBar(struct abuf *ab){
    int len;
    char status[80];
    char rstatus[80];
    int rlen;
    abAppend(ab, "\x1b[7m", 4);
    len = snprintf(status, sizeof(status), "%.20s - %d lines", E.filename ? E.filename : "[No Name]", E.numrows);
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
    editorSetStatusMessage("HELP: CTRL-Q = quit");
    while(1){
        editorRefreshScreen();
        editorProcessKeypress();
    }
    return 0;
}


