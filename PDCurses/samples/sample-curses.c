/***********************************************************
 sample-curses.c
     PDCurses version
     (c)2016 programming by Coffey.
     20160423
************************************************************/
// build command
// (Windows gcc) gcc sample-curses.c pdcurses.dll -Wall -o sample-curses.exe

#include "curses.h"

int main(void)
{
    WINDOW *mainwin = 0, *sprite = 0;
    int  x, y, xd, yd, key;
    char *helptext =  "Press [q] or [Esc] to exit";
    
    initscr();    // initialize curses screen
    
    // key input setting
    noecho();     // no echo
    nodelay(stdscr, TRUE);  // non-blocking (no waiting)
    
    curs_set(0);  // hide cursor
    erase();      // erase screen
    
    // create window
    // LINES = screen row size (curses global value)
    // COLS  = screen column size (curses global value)
    mainwin = newwin(LINES, COLS, 0, 0);  // create main window
                                          // size(COLS, LINES). put (0, 0) of default screen.
                                          // This line is equivalent to "mainwin = newwin(0, 0, 0, 0);"
    sprite  = newwin(9, 17, 0, 0);        // create sprite window
                                          // size(17, 9). put (0, 0) of default screen.
    if (!mainwin || !sprite) {
        goto end_of_program;
    }
    
    // draw mainwin
    for (y = 0; y < getmaxy(mainwin) - 1; y++)
        for (x = 0; x < getmaxx(mainwin); x++)
            mvwaddch(mainwin, y, x, ((x + y) % 2) ? '.': ':'); // mvwaddch() = move and add character
    mvwaddstr(mainwin, getmaxy(mainwin) -1, 0, helptext);  // add help text at bottom of mainwin
    
    // draw sprite
    mvwaddstr(sprite, 0, 0, "      $$$$$      ");  // set pattern
    mvwaddstr(sprite, 1, 0, "   $$$$$$$$$$$   ");  // mvwaddstr() = move and add string
    mvwaddstr(sprite, 2, 0, " $$$$$$$$$$$$$$$ ");
    mvwaddstr(sprite, 3, 0, "$$$$$$$$$$$$$$$$$");
    mvwaddstr(sprite, 4, 0, "$$$$$$$$$$$$$$$$$");
    mvwaddstr(sprite, 5, 0, "$$$$$$$$$$$$$$$$$");
    mvwaddstr(sprite, 6, 0, " $$$$$$$$$$$$$$$ ");
    mvwaddstr(sprite, 7, 0, "   $$$$$$$$$$$   ");
    mvwaddstr(sprite, 8, 0, "      $$$$$      ");
    
    x   = 0;    // initial position x
    y   = 0;    // initial position y
    xd  = 2;    // stride x
    yd  = 1;    // stride y
    key = 0;
    while ( key != 'q' && key != 0x1b ) {  // q or Esc to Quit
        mvwin(sprite, y, x);         // move sprite
        overwrite(mainwin, stdscr);  // overwrite mainwin to stdscr
        overlay(sprite, stdscr);     // overlay sprite to stdscr
        refresh();                   // refresh stdscr
        napms(100);                  // wait 100 ms
        x += xd;
        y += yd;
        // bounding check
        if ((x > getmaxx(mainwin) - getmaxx(sprite)) || (x < 0)) {
            xd = -xd;
            x += xd + xd;
        }
        if ((y > getmaxy(mainwin) - 1 - getmaxy(sprite)) || (y < 0)) {
            yd = -yd;
            y += yd + yd;
        }
        key = getch();    // get current key
    }
    
    end_of_program:
    
    if (sprite)
        delwin(sprite);   // delete sprite
    if (mainwin)
        delwin(mainwin);  // delete mainwin
    curs_set(1);          // show cursor
    endwin();             // end of curses
    
    return 0;
}

