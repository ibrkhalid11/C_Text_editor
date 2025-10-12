

/*** includes ***/
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** function definitions ***/
void enableRawMode(void);
void disableRawMode(void);
void die(const char *s);

/*** global variables ***/
struct termios orig_termios;

/*purpose of die...*/
void die(const char *s){
    perror(s);
    exit(1);
}

/*purpose of rawmode*/
void disableRawMode(void){
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1){
        die("tcsetattr");
    }
}

/*purpose of rawmode...*/
void enableRawMode(void){
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    raw = orig_termios;
    raw.c_iflag &= (tcflag_t)~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= (tcflag_t)~(OPOST);
    raw.c_cflag |= (tcflag_t)(CS8);
    raw.c_lflag &= (tcflag_t)~(ECHO | IEXTEN | ICANON | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1){die("tcsetattr");}
}


/*main logic*/
int main(void){

    enableRawMode();

    while(1){
        char c = '\0';
        ssize_t nread = read(STDIN_FILENO, &c, 1);
        if (nread == -1 && errno != EAGAIN) {
            die("read");
        } else if (nread == 0) {
            continue;
        }
        if(iscntrl(c)){
            printf("%d\r\n", c);
        } else{
            printf("%d ('%c')\r\n", c, c);
        }
        if(c == 'q'){break;}
    }
    return 0;
}


