/*for struct termios, tcgetattr, tcsetattr*/
#include <termios.h>


/*unistd for read function, STDIN_FILENO*/
#include <unistd.h>
void enableRawMode(){
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void){
    char c;
    enableRawMode();
    while(read(STDIN_FILENO, &c, 1) == 1 && (c != 'q'));
    return 0;
}


