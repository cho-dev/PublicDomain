/***********************************************************
 keyin-curses.c
     PDCurses test program.
     (c)2016 programming by Coffey.
     20160423
************************************************************/
// build command
// (Windows gcc) gcc keyin-curses.c pdcurses.dll -Wall -o keyin-curses.exe

#include <stdlib.h>
#include "curses.h"

int main(void)
{
    WINDOW *inputwin, *mainwin, *subwin, *countwin;
    int  key = 0, count = 0;
    char buf[256];
    
    initscr();      // initialize curses screen
    start_color();  // initialize color
    
    // set color-pair
    init_pair(1, COLOR_YELLOW, COLOR_BLUE);   // #1 = yellow, blue(back)
    init_pair(2, COLOR_RED,    COLOR_YELLOW); // #2 = red, yellow(back)
    init_pair(3, COLOR_BLACK,  COLOR_GREEN);  // #3 = black, green(back)
    init_pair(4, COLOR_BLACK,  COLOR_WHITE);  // #4 = black, white(back)
    
    // key input setting
    noecho();       // no echo
    nodelay(stdscr, TRUE);  // non-blocking (no waiting)
    
    // draw default screen (stdscr)
    erase();        // erase screen
    mvaddstr(1, 2, "PDCurses sample code  by Coffey");
    mvaddstr(2, 2, "Type any word and enter.");
    mvaddstr(LINES - 2, 2, "Press [0] or [Esc] to exit");
    refresh();      // draw default screen(stdscr)
    
    
    // create input window
    inputwin = newwin(1, 20, 4, 2); // create size(20x1) at (2,4)
    if (inputwin == NULL) {
        endwin();
        exit(-1);
    }
    wcolor_set(inputwin, 4, NULL);   // color set to color-pair #4
    wbkgd(inputwin, COLOR_PAIR(4));  // background color set to color-pair #4
    werase(inputwin);                // erase all
    wrefresh(inputwin);              // draw mainwin
    
    // create main window
    mainwin = newwin(13, 20, 6, 2); // create size(20x13) at (2,6)
    if (mainwin == NULL) {
        endwin();
        exit(-1);
    }
    wcolor_set(mainwin, 1, NULL);   // color set to color-pair #1
    scrollok(mainwin, TRUE);        // enable scroll window
    wbkgd(mainwin, COLOR_PAIR(1));  // background color set to color-pair 1
    werase(mainwin);                // erase all
    wrefresh(mainwin);              // draw mainwin
    
    // create sub window
    subwin = newwin(15, 40, 4, 26); // create size(40x15) at (26,4)
    if (subwin == NULL) {
        endwin();
        exit(-1);
    }
    wcolor_set(subwin, 2, NULL);    // color set to color-pair #2
    scrollok(subwin, TRUE);         // enable scroll window
    wbkgd(subwin, COLOR_PAIR(2));   // background color set to color-pair 2
    werase(subwin);                 // erase all
    wrefresh(subwin);               // draw subwin
    
    // create count window
    countwin = newwin(15, 8, 4, 66);  // create size(8x15) at (66,4)
    if (countwin == NULL) {
        endwin();
        exit(-1);
    }
    wcolor_set(countwin, 3, NULL);    // color set to color-pair #3
    scrollok(countwin, TRUE);         // enable scroll window
    wbkgd(countwin, COLOR_PAIR(3));   // background color set to color-pair 3
    werase(countwin);                 // erase all
    wrefresh(countwin);               // draw countwin
    
    
    curs_set(0);    // hide cursor
    
    // main loop
    while ((key != 0x1b) && (key != '0')) {  // [0] or [Esc] to Quit
        key = getch();  // get current key
        if (key > 0) {
            wprintw(subwin, "[%02x]", key); // print keycode to subwin
            
            if (key == 0x0a) {  // enter
                // move text in inputwin to mainwin
                wmove(inputwin, 0, 0);
                winnstr(inputwin, buf, sizeof(buf) - 1);
                waddstr(mainwin, buf);
                werase(inputwin);
            } else {
                if ((getcurx(inputwin) < getmaxx(inputwin) - 1) || (key == 0x08))
                    waddch(inputwin, key);  // echo back to inputwin
                if (key == 0x08)
                    wdelch(inputwin);  // del character when press backspace
            }
            wrefresh(inputwin);
            wrefresh(mainwin);
            wrefresh(subwin);
        }
        // show loop count
        wprintw(countwin, " %06d\n", ++count); // print count to countwin
        wrefresh(countwin);   // refresh countwin
        napms(20);            // wait 20 ms
    }
    
    delwin(inputwin);  // free inputwin
    delwin(mainwin);   // free mainwin
    delwin(subwin);    // free subwin
    delwin(countwin);  // free counwin
    curs_set(1);       // show cursor
    endwin();          // end of curses
    
    return 0;
}

