
#include "editRowOps.h"
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
void editorInsertRow(int at ,char *s, size_t len){
    if(at < 0 || at > E.numrows) return;
    E.row = realloc(E.row, sizeof(erow) * (size_t)(E.numrows + 1));
    memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (size_t)(E.numrows - at));
    E.row[at].size = (int)len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len);
    E.row[at].chars[len] = '\0';
    E.row[at].rsize = 0;
    E.row[at].render = NULL;
    editorUpdateRow(&E.row[at]);
    E.numrows++;
    E.dirty++;
}

void editorFreeRow(erow *row){
    free(row->render);
    free(row->chars);
}
void editorDelRow(int at){
    if(at < 0 || at >= E.numrows) return;
    editorFreeRow(&E.row[at]);
    memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (size_t)(E.numrows - at - 1));
    E.numrows--;
    E.dirty++;
}
void editorRowInsertChar(erow *row, int at, int c){
    if(at < 0 || at > row->size) at = row-> size;
    row->chars = realloc(row->chars, (size_t)row->size + 2);
    memmove(&row->chars[at + 1], &row->chars[at], (size_t)(row->size - at + 1));
    row->size++;
    row->chars[at] = (char)c;
    editorUpdateRow(row);
    E.dirty++;
}
void editorRowAppendString(erow *row, char *s, size_t len){
    row->chars = realloc(row->chars, (size_t) row->size + len + 1);
    memcpy(&row->chars[row->size], s, len);
    row->size += (int)len;
    row->chars[row->size] = '\0';
    editorUpdateRow(row);
    E.dirty++;
}
void editorRowDelChar(void){
    erow *row;
    if(E.cy == E.numrows)return;
    if(E.cx  == 0 && E.cy == 0) return;
    row = &E.row[E.cy];
    if(E.cx > 0){
        editorRowDeleteChar(row, E.cx - 1);
        E.cx--;
    } else{
        E.cx = E.row[E.cy - 1].size;
        editorRowAppendString(&E.row[E.cy - 1], row->chars, (size_t)row->size);
        editorDelRow(E.cy);
        E.cx--;
    }
}
void editorRowDeleteChar(erow *row, int at){
    if(at < 0 || at >= row->size)return;
    memmove(&row->chars[at], &row->chars[at + 1], (size_t) row->size - (size_t)at);
    row->size--;
    editorUpdateRow(row);
    E.dirty++;
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