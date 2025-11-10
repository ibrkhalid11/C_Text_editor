#include "termIO.h"

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




