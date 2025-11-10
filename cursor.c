#include "cursor.h"

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