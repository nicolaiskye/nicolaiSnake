//
//  main.c
//  nicolaiSnake
//
//  Created by Nicolai Skye on 5/29/22.
//

#include <ncurses.h>
#include <string.h>

#define TIMEOUT 300

typedef struct GameContext {
    int window_height;
    int window_width;
    int game_area_height;
    int game_area_width;
} gameContext;

typedef struct Position {
    int x;
    int y;
} position;

typedef enum Direction {
    Up,
    Down,
    Left,
    Right,
    None,
} direction;

struct SnakePiece {
    position pos;
    struct SnakePiece *next_piece;
};

void drawBorder(gameContext *context);
void drawPlayer(void);

int main() {
    
    int ch;
    struct GameContext context;
    direction dir;
    
    initscr(); /* start curses mode */
    raw(); /* line buffering disabled */
    noecho(); /* disable echoing of char type by user */
    getmaxyx(stdscr, context.window_height, context.window_width);
    context.game_area_height = context.window_height;
    context.game_area_width = context.window_width;
    
    // Game area is just the window area for now.
    // Will update later when I add an info panel.
    const int game_area_height = context.window_height;
    const int game_area_width = context.window_width;
    
    // Game start screen and debug info
    mvprintw(0, 0, "Terminal Width: %d, Terminal Height: %d", context.window_width, context.window_height);
    mvprintw(2, 0, "Game Area Width: %d, Game Area Height: %d", game_area_width, game_area_height);
    mvprintw(4, 0, "Press any key to start");
    move(4, 0);
    ch = getch();
    clear();
    
    // Timeout acts as timer to run the games FPS
    timeout(TIMEOUT);
    
    // Player starting position and direction
    const char player_snake_head = '#';
    position player_pos;
    position next_block;
    player_pos.x = (int)game_area_width/2;
    player_pos.y = (int)game_area_height/2;
    char player_input;
    dir = None; // Player starts standing still
    
    // Create the 'quit' message and determine its placement on screen
    const char* quitmsg = "Press 'q' to quit.";
    const int quitmsg_halfpoint = (int)strlen(quitmsg)/2;
    const int window_halfpoint = (int)context.game_area_width/2;
    const int quitmsg_x_pos = window_halfpoint-quitmsg_halfpoint;
    
    // Game loop
    while(true) {
        clear();
        mvprintw(2, quitmsg_x_pos, quitmsg);
                
        // Move the player head in the desired direction
        switch(dir) {
            case Up:
                player_pos.y -= 1;
                break;
            case Down:
                player_pos.y += 1;
                break;
            case Left:
                player_pos.x -= 1;
                break;
            case Right:
                player_pos.x += 1;
                break;
            case None: // Do nothing
                break;
        }
        
        // Wrap player if past game bounds
        if (player_pos.x <= 0)
            player_pos.x = game_area_width-2;
        if (player_pos.x >= game_area_width-1)
            player_pos.x = 1;
        
        if (player_pos.y <= 0)
            player_pos.y = game_area_height-2;
        if (player_pos.y >= game_area_height-1)
            player_pos.y = 1;
        
        attron(A_BOLD);
        mvaddch(player_pos.y, player_pos.x, player_snake_head);
        attroff(A_BOLD);

    
        drawBorder(&context);
        refresh();
        
        player_input = getch();
        if (player_input == 'w')
            dir = Up;
        if (player_input == 'a')
            dir = Left;
        if (player_input == 's')
            dir = Down;
        if (player_input == 'd')
            dir = Right;
        
        // Quit the game
        if (player_input == 'q')
            break;
    }
    
    endwin(); /* end curses session */
    return 0;
}

void drawBorder(gameContext *context) {
    
    // DEBUG
    mvprintw(2, 2, "ga height: %d", context->game_area_height);
    mvprintw(4, 2, "ga width: %d", context->game_area_width);
    // END DEBUG
    
    for (int wrow = 0; wrow < context->game_area_height; ++wrow) {
        for (int wcol = 0; wcol < context->game_area_width; ++wcol) {
            // Left and right sides of border
            if (wcol == 0 || wcol == context->game_area_width-1)
                mvaddch(wrow, wcol, '|');
            
            // Top and bottom of border
            if (wrow == 0 || wrow == context->game_area_height-1)
                mvaddch(wrow, wcol, '-');
        }
    }
}

void drawPlayer(void) {
    
}
