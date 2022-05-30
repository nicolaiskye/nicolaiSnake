//
//  main.c
//  nicolaiSnake
//
//  Created by Nicolai Skye on 5/29/22.
//

#include <ncurses.h>
#include <string.h>

#define TIMEOUT 300

enum xDirection {
    Left,
    Right,
};

enum yDirection {
    Up,
    Down,
};

void drawBox(int height, int width, int startx, int starty);
void drawBorder(int winh, int winw, char *ch);

int main() {
    int ch;
    int window_height, window_width;
    enum xDirection xdir;
    enum yDirection ydir;
    
    
    
    initscr(); /* start curses mode */
    raw(); /* line buffering disabled */
    noecho(); /* disable echoing of char type by user */
    getmaxyx(stdscr, window_height, window_width);
    
    mvprintw(0, 0, "Terminal Width: %d, Terminal Height: %d", window_width, window_height);
    
    mvprintw(2, 0, "Press any key to start");
    move(4, 0);
    ch = getch();
    clear();
    
    timeout(TIMEOUT);
    int x = 10;
    int y = 10;
    xdir = Right;
    ydir = Down;
    
    const char* quitmsg = "Press 'q' to quit.";
    const int hlquitmsg = (int)strlen(quitmsg)/2;
    const int halfscreenx = (int)window_width/2;
    const int xquitmsg = halfscreenx-hlquitmsg;
    
    char* locmsg;
    int llocmsg;
    
    while(true) {
        clear();
        locmsg = "x: %d y: %d";
        llocmsg = (int)strlen(locmsg)/2;
        int xlocmsg = halfscreenx-llocmsg;
        mvprintw(0, xlocmsg, locmsg, x, y);
        mvprintw(2, xquitmsg, quitmsg);
        attron(A_BOLD);
        mvaddch(y, x, 'D');
        mvaddch(y, x+1, 'V');
        mvaddch(y, x+2, 'D');
        attroff(A_BOLD);
        
        drawBox(5, 10, 5, 5);
        drawBox(8, 15, 45, 15);
        
        if (x+4 >= window_width)
            xdir = Left;
        if (x-1 <= 0)
            xdir = Right;
        if (y+2 >= window_height)
            ydir = Up;
        if (y-1 <= 0)
            ydir = Down;
        
        if (xdir == Right)
            x += 1;
        if (xdir == Left)
            x -= 1;
        if (ydir == Down)
            y += 1;
        if (ydir == Up)
            y -= 1;

        drawBorder(window_height, window_width, '#');
        refresh();
//        move(window_height, window_width); // doesn't seem to work
        if (getch() == 'q')
            break;
    }
    
    endwin(); /* end curses session */
    
    return 0;
}


void drawBox(int height, int width, int startx, int starty /*, WINDOW *win*/) {
    
//    if (win == NULL)
//        win = stdscr;
    
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            mvaddch(starty+i, startx+j, '#');
            
        }
    }
    refresh();
}


void drawBorder(int winh, int winw, char *ch) {
    for (int wrow = 0; wrow < winh; ++wrow) {
        for (int wcol = 0; wcol < winw; ++wcol) {
            if (wcol == 0 || wcol == winw-1 || wrow == 0 || wrow == winh-1)
                mvaddch(wrow, wcol, '#');
        }
    }
    refresh();
}
