//
//  main.c
//  nicolaiSnake
//
//  Created by Nicolai Skye on 5/29/22.
//

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIMEOUT 300

typedef struct {
    int window_height;
    int window_width;
    int game_area_height;
    int game_area_width;
} gameContext_t;

typedef struct {
    int x;
    int y;
} position_t;

typedef enum {
    Up,
    Down,
    Left,
    Right,
    None,
} direction_t;

struct SnakePiece {
    position_t position;
    struct SnakePiece *next_piece;
};

typedef struct {
    char graphic;
    direction_t direction;
    struct SnakePiece *snakeHead;
} player_t;

void drawBorder(gameContext_t *context);
void drawPlayer(struct SnakePiece *piece);

void addNewPiece(player_t *player);
void movePiece(struct SnakePiece *piece, position_t position);
void movePlayer(player_t *player, position_t position);
position_t getNextPosition(position_t *currentPosition, position_t difference);
bool playerWillCollideWithSelf(player_t *player, position_t nextPosition);

int lengthOfTail(struct SnakePiece *piece);
struct SnakePiece* getTail(struct SnakePiece *piece);


int main() {
    
    int ch;
    gameContext_t context;
    
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
    
    // Player graphic, starting position, and direction
    player_t player;
    player.graphic = '#';
    player.snakeHead = (struct SnakePiece*) malloc(sizeof(struct SnakePiece));
    player.snakeHead->position.x = (int)game_area_width/2;
    player.snakeHead->position.y = (int)game_area_height/2;
    player.snakeHead->next_piece = NULL;
    player.direction = None; // Player starts standing still
    
    char playerInput;
    
    // Create the 'quit' message and determine its placement on screen
    const char *quitMessage = "Press 'q' to quit.";
    const int quitMessageHalfpoint = (int)strlen(quitMessage)/2;
    const int windowHalfpoint = (int)context.game_area_width/2;
    const int quitMessage_x_position = windowHalfpoint-quitMessageHalfpoint;
    
    int sizeToFive = 0; // debug only
    bool playerAteFood = true; // debug only
    position_t nextPosition;
    
    // Game loop
    while(true) {
        clear();
        mvprintw(2, quitMessage_x_position, quitMessage);
        
        
        if (sizeToFive >= 10) { playerAteFood = false; } // debug only
        
        // Move the player head in the desired direction
        switch(player.direction) {
            // TODO: Add checks if next block causes death
            case Up:
                nextPosition.x = 0;
                nextPosition.y = -1;
                nextPosition = getNextPosition(&player.snakeHead->position, nextPosition);
                
                // check if nextPosition = death
                if (playerWillCollideWithSelf(&player, nextPosition)) {
                    mvprintw(10, quitMessage_x_position, "GAME OVER");
                    timeout(99999);
                    getch();
                }
                
                // check if nextPosition = food
                if (playerAteFood) {
                    sizeToFive += 1; // debug only
                    addNewPiece(&player);
                }
                
                movePlayer(&player, nextPosition);
                break;
            case Down:
                nextPosition.x = 0;
                nextPosition.y = 1;
                nextPosition = getNextPosition(&player.snakeHead->position, nextPosition);
                
                // check if nextPosition = death
                if (playerWillCollideWithSelf(&player, nextPosition)) {
                    break;
                }
                
                // check if nextPosition = food
                if (playerAteFood) {
                    sizeToFive += 1; // debug only
                    addNewPiece(&player);
                }
                
                movePlayer(&player, nextPosition);
                break;
            case Left:
                nextPosition.x = -1;
                nextPosition.y = 0;
                nextPosition = getNextPosition(&player.snakeHead->position, nextPosition);
                
                // check if nextPosition = death
                if (playerWillCollideWithSelf(&player, nextPosition)) {
                    break;
                }
                
                // check if nextPosition = food
                if (playerAteFood) {
                    sizeToFive += 1; // debug only
                    addNewPiece(&player);
                }
                
                movePlayer(&player, nextPosition);
                break;
            case Right:
                nextPosition.x = 1;
                nextPosition.y = 0;
                nextPosition = getNextPosition(&player.snakeHead->position, nextPosition);
                
                // check if nextPosition = death
                if (playerWillCollideWithSelf(&player, nextPosition)) {
                    break;
                }
                
                // check if nextPosition = food
                if (playerAteFood) {
                    sizeToFive += 1; // debug only
                    addNewPiece(&player);
                }
                
                movePlayer(&player, nextPosition);
                break;
            case None: // Do nothing
                break;
        }

        // Warp player if past game bounds
        if (player.snakeHead->position.x <= 0)
            player.snakeHead->position.x = game_area_width-2;
        if (player.snakeHead->position.x >= game_area_width-1)
            player.snakeHead->position.x = 1;

        if (player.snakeHead->position.y <= 0)
            player.snakeHead->position.y = game_area_height-2;
        if (player.snakeHead->position.y >= game_area_height-1)
            player.snakeHead->position.y = 1;
        
        attron(A_BOLD);
        drawPlayer(player.snakeHead);
        attroff(A_BOLD);

        drawBorder(&context);
        refresh();
        
        // Get player input for next frame
        playerInput = getch();
        if (playerInput == 'w')
            player.direction = Up;
        if (playerInput == 'a')
            player.direction = Left;
        if (playerInput == 's')
            player.direction = Down;
        if (playerInput == 'd')
            player.direction = Right;
        
        // Quit the game
        if (playerInput == 'q')
            break;
    }
    
    
    mvprintw(2, quitMessage_x_position, quitMessage);
    // TODO: Clean up memory before exit
    
    endwin(); /* end curses session */
    return 0;
}

void drawBorder(gameContext_t *context) {
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

struct SnakePiece* getTail(struct SnakePiece *piece) {
    if (piece->next_piece == NULL) {
        return piece;
    } else {
        return getTail(piece->next_piece);
    }
}

void addNewPiece(player_t *player) {
    struct SnakePiece *newPiece = (struct SnakePiece*) malloc(sizeof(struct SnakePiece));
    newPiece->position.x = player->snakeHead->position.x;
    newPiece->position.y = player->snakeHead->position.y;
    newPiece->next_piece = player->snakeHead->next_piece;
    // Add new piece to end of tail
    player->snakeHead->next_piece = newPiece;
}

int lengthOfTail(struct SnakePiece *piece) {
    if (piece->next_piece == NULL) {
        return 1;
    } else {
        return lengthOfTail(piece->next_piece) + 1;
    }
}

void drawPlayer(struct SnakePiece *piece) {
    mvaddch(piece->position.y, piece->position.x, '#');
    if (piece->next_piece != NULL) {
        drawPlayer(piece->next_piece);
    }
}

void movePlayer(player_t *player, position_t position) {
//    position_t nextPosition;
//    position_t nextPosition = player->snakeHead->position.x += position.x;
//    position_t nextPosition = player->snakeHead->position.y += position.y;
    movePiece(player->snakeHead, position);
}

void movePiece(struct SnakePiece *piece, position_t position) {
    if (piece->next_piece != NULL) {
        movePiece(piece->next_piece, piece->position);
    }
    piece->position = position;
}

position_t getNextPosition(position_t *currentPosition, position_t difference) {
    position_t position;
    position.x = currentPosition->x += difference.x;
    position.y = currentPosition->y += difference.y;
    return position;
}

bool playerWillCollideWithSelf(player_t *player, position_t nextPosition) {
    char spot = (char) ((A_CHARTEXT) & mvinch(nextPosition.y, nextPosition.x));
    
    fprintf(stderr, "playerWillCollideWithSelf: spot: %c", spot);
    fprintf(stderr, "playerWillCollideWithSelf: player->graphic: %c", player->graphic);
    
    if (spot == player->graphic) {
        fprintf(stderr, "Collision with graphic detected... checking if == tail vector");
        position_t *tailPosition = &getTail(player->snakeHead)->position;
        if (tailPosition->x == nextPosition.x
            && tailPosition->y == nextPosition.y) {
            fprintf(stderr, "Yes, was tail vector. Ignoring...");
            return false;
        }
        return true;
    }
    return false;
}
