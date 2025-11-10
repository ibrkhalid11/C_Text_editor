#include "utils.h"
#include "editorTypes.h"
#include "insideFunc.h"
#include "termIO.h"
#include "editRowOps.h"
#include "keyOps.h"
#include "setup.h" 
#include "bar.h"
#include "editorOps.h"

/*** global variables ***/
struct editorConfig E;
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


