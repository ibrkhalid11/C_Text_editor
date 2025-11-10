#include "keyOps.h"

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

