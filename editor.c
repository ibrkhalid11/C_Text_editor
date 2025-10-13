

/*** includes ***/
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

/*** hash defines ***/
#define VERSION "0.0.1" 
#define CTRL_KEY(k) ((k) & 0x1F)
#define ABUF_INIT {NULL, 0}

struct abuf{
    char *b;
    int len;
};

struct editorConfig{
    int cx;
    int cy;
    int screenrows;
    int screencols;
    struct termios orig_termios;
};

/*** function prototypes ***/
void    enableRawMode(void);
void    disableRawMode(void);
void    _die(const char *s); /* error recorder and exit for write() in die() */
void    die(const char *s);  /* error recorder and exit for all code*/
void    initEditor(void);
char    editorReadKey(void);
void    editorProcessKeypress(void);
void    editorRefreshScreen(void);
void    editorDrawRows(struct abuf *ab);
void    editorMoveCursor(char key);
void    abAppend(struct abuf *ab, const char *s, int len);
void    abFree(struct abuf *ab);
int     getCursorPosition(int *rows, int *cols);
int     getWindowSize(int *rows, int *column);

/*** global variables ***/
struct editorConfig E;



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
    if(getWindowSize(&E.screenrows, &E.screencols) == -1){die("getWindowSize");}
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
char editorReadKey(void){
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
            switch(seq[1]){
                case 'A': return 'w'; 
                case 'B': return 's';
                case 'C': return 'd';
                case 'D': return 'a';
            }
        }
        return '\x1b';
    } else{
        return c;
    }
}
void editorMoveCursor(char key){
    switch(key){
        case 'a': E.cx--; break;
        case 'd': E.cx++; break;
        case 'w': E.cy--; break;
        case 's': E.cy++; break;
    }
}
void editorProcessKeypress(void){
    char c = editorReadKey();

    switch(c){
        case CTRL_KEY('q'): exit(0); break;
        case 'a': case 'w': case 's': case 'd':
                editorMoveCursor(c); break;
    }
}

void editorRefreshScreen(void){
    char buf[32];
    struct abuf ab = ABUF_INIT;
    abAppend(&ab, "\x1b[?25l", 6);
    abAppend(&ab, "\x1b[H", 3);
    
    editorDrawRows(&ab);

    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
    abAppend(&ab, buf, (int)strlen(buf));

    abAppend(&ab, "\x1b[?25h", 6);

    if(write(STDIN_FILENO, ab.b, (size_t)ab.len) == -1){die("write");}
    
    abFree(&ab);
}
void editorDrawRows(struct abuf *ab){
    int y;
    char welcome[88];
    int welcomelen;
    int padding;
    for(y = 0; y < E.screenrows; y++){
        if(y == E.screenrows / 3){
            welcomelen = snprintf(welcome, sizeof(welcome), "ibredit --version %s", VERSION);
            if(welcomelen > E.screencols){
                welcomelen = E.screencols;
            }
            padding = (E.screencols - welcomelen) / 2;
            if(padding){
                abAppend(ab, "~", 1);
                padding--;
            }
            while(padding--) {abAppend(ab, " ", 1);}
            abAppend(ab, welcome, (size_t)welcomelen);
        } else{
            abAppend(ab, "~", 1);
        }
        abAppend(ab, "\x1b[K", 3);
        if(y < E.screenrows -1){
            abAppend(ab, "\r\n", 2);
        }
    }
}
/*main logic*/
int main(void){

    enableRawMode();
    initEditor();
    while(1){
        editorRefreshScreen();
        editorProcessKeypress();
    }
    return 0;
}


