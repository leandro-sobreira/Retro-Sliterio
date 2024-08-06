#include <SDL2/SDL.h>
#include <time.h>

//     By Leandro Sobreira
//Commented in English to pratice

#define windowW 640
#define windowH 480
#define true 1
#define false 0

#define cellsNum 20              // Number of cells in the grid (cellNum*cellNum)
#define gridSize 400             //  Size of grid (in pixels)
#define centerGridH ((windowH - gridSize)/2)-(gridSize/cellsNum)
#define centerGridW ((windowW - gridSize)/2)-(gridSize/cellsNum)

int bot2player = false;

typedef struct
{
    int size;
    int x[cellsNum*cellsNum];
    int y[cellsNum*cellsNum];
    int moveDir;    //1 = UP, 2 = RIGHT, 3 = DOWN, 4 = LEFT
    int moved;
    int score;
    SDL_Color hRGB;
    SDL_Color tRGB;
    SDL_Rect scoreRect;
} Snake;

struct apple
{
    int x;
    int y;
} apple;

int growestPlayerSize;

Snake player1 = 
{
    1,                          // start size
    {2},                        // start X
    {2},                        // start Y
    2,                          // start movedir
    false,              
    0, 
    {0,180,0},                  // head Color   
    {0,255,0},                  // tail Color
    {0,0,0,0}                   // Score Rect values
};

Snake player2 = 
{
    1,                          // start size
    {cellsNum-1},               // start X
    {cellsNum-1},               // start Y
    4,                          // start move dir
    false, 
    0, 
    {50,50,255},                // head Color   
    {100,100,255},              // tail Color
    {windowW-windowH/10,0,0,0}  // Score Rect values
};

void createApple(SDL_Renderer *renderer)
{
    apple.x = (rand()%cellsNum)+1;
    apple.y = (rand()%cellsNum)+1;

    if(player1.size >= player2.size)
    {
        growestPlayerSize = player1.size;
    }
    else
    {
        growestPlayerSize = player2.size;
    }

    for (int i = 0; i < growestPlayerSize; i++) // Count wanna be the most long snake
    {
        while((apple.x == player1.x[i] && apple.y == player1.y[i]) || (apple.x == player2.x[i] && apple.y == player2.y[i]))
        {
            if(apple.x != cellsNum)
            {
                apple.x++;
            }
            else
            {
                apple.x = 1;

                if(apple.y != cellsNum)
                {
                    apple.y++;
                }
                else
                {
                    apple.y = 1;
                }
            }
        }
    }
}

void eatApple(SDL_Renderer *renderer, Snake *player)
{
    if(apple.x == player->x[0] && apple.y == player->y[0]) // Player eat the apple
    {
        player->size++;
        createApple(renderer);
    }
}

void movePlayer(Snake *player)
{
    switch(player->moveDir)
        {
            case  1: // UP
                player->y[0]--;
            break;
            case  2: // RIGHT
                player->x[0]++;
            break;
            case  3: // DOWN
                player->y[0]++;
            break;
            case  4: // LEFT
                player->x[0]--;
            break;
        }

        if(player->x[0] == cellsNum+1)
        {
            player->x[0] = 1;
        }
        if(player->x[0] == 0)
        {
            player->x[0] = cellsNum;
        }

        if(player->y[0] == cellsNum+1)
        {
            player->y[0] = 1;
        }
        if(player->y[0] == 0)
        {
            player->y[0] = cellsNum;
        }

        player->moved = true;
}

void keysPlayer(SDL_Event *input, Snake *player, int up, int down, int left, int right)
{
    if(input->type == SDL_KEYDOWN && player->moved)
    {
        if(input->key.keysym.sym == up && player->moveDir != 3 && player->moveDir !=1)
        {
            player->moveDir = 1;
            player->moved = false;
        }
        if(input->key.keysym.sym == down && player->moveDir != 1 && player->moveDir !=3)
        {
            player->moveDir = 3;
            player->moved = false;
        }
        if(input->key.keysym.sym == left && player->moveDir != 2 && player->moveDir !=4)
        {
            player->moveDir = 4;
            player->moved = false;
        }
        if(input->key.keysym.sym == right && player->moveDir != 4 && player->moveDir !=2)
        {
            player->moveDir = 2;
            player->moved = false;
        }
    }
}

void resetGame(SDL_Renderer *renderer)
{
    player1.size = 1;
    player1.x[0] = 2; 
    player1.y[0] = 2; 
    player1.moveDir = 2; 
    player1.moved = false;
    
    player2.size = 1;
    player2.x[0] = cellsNum-1;
    player2.y[0] = cellsNum-1;
    player2.moveDir = 4;
    player2.moved = false;

    createApple(renderer);
}

void playerScore(SDL_Renderer *renderer)
{
    int player1Bateu = false;
    int player2Bateu = false;

    for (int i = 0; i < player1.size; i++)
    {
        if (player2.x[0] == player1.x[i] && player2.y[0] == player1.y[i]) // Player 2 head touch the player 1
        {
            player2Bateu = true;
        }
    }

    for (int i = 0; i < player2.size; i++)
    {
        if (player1.x[0] == player2.x[i] && player1.y[0] == player2.y[i]) // Player 1 head touch the player 2
        { 
            player1Bateu = true;
        }
    }

    
    if (player1Bateu == true && player2Bateu == false)
    {
        player2.score++;
    }

    if (player2Bateu == true && player1Bateu == false)
    {
        player1.score++;
    }

        if (player1Bateu || player2Bateu)
    {
        SDL_Delay(1500);
        resetGame(renderer);
    }

}

void renderPlayer(SDL_Renderer *renderer, int i, int j, SDL_Rect cell, Snake *player)
{
    for (int k = 0; k < player->size; k++)
    {
        if(player->x[0] == i && player->y[0] == j)
        {
            SDL_SetRenderDrawColor(renderer, player->hRGB.r, player->hRGB.g, player->hRGB.b, 255);
            SDL_RenderFillRect(renderer, &cell);
        }
        else if(player->x[k] == i && player->y[k] == j)
        {
            SDL_SetRenderDrawColor(renderer, player->tRGB.r, player->tRGB.g, player->tRGB.b, 255);
            SDL_RenderFillRect(renderer, &cell);
        }
    }
}

void growPlayer(Snake *player)
{
    for (int i = player->size; i >= 1; i--)
        {
            player->x[i] = player->x[i-1];
            player->y[i] = player->y[i-1];
        }
}

void aiPlayer2()
{
    if(apple.x < player2.x[0] && player2.moveDir != 2 && player2.moveDir != 4)
    {
        player2.moveDir = 4;
    }
    else if(apple.x > player2.x[0] && player2.moveDir != 2 && player2.moveDir != 4)
    {
        player2.moveDir = 2;
    }
    else if(apple.y < player2.y[0] && player2.moveDir != 1 && player2.moveDir != 3)
    {
        player2.moveDir = 1;
    }
    else if(apple.y > player2.y[0] && player2.moveDir != 1 && player2.moveDir != 3)
    {
        player2.moveDir = 3;
    }
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING); // Initialize all resources of SDL
    SDL_Window *window = NULL;
    window = SDL_CreateWindow(                          // Create the window
        "Retro Sliter.io",                              // Name of the window
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // Centralize the window
        windowW, windowH,                               // Size of the window
        SDL_WINDOW_SHOWN);                              // Show the window
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Rect cell;
        cell.h = gridSize/cellsNum;
        cell.w = gridSize/cellsNum;
        cell.x = 10;
        cell.y = 10;
    
    createApple(renderer);
    SDL_Event input;

    while (true)
    {
        while(SDL_PollEvent(&input))
        {
            if(input.type == SDL_QUIT || input.key.keysym.sym == SDLK_ESCAPE)
            {
                exit(0);
            }
            if(input.type == SDL_KEYDOWN && input.key.keysym.sym == SDLK_r)
            {
                resetGame(renderer);
            }

            /*==== PLAYER MOVE KEYS ====*/
            keysPlayer(&input, &player1, SDLK_w, SDLK_s, SDLK_a, SDLK_d);
            //if (!bot2player)
            {
                keysPlayer(&input, &player2, SDLK_KP_8, SDLK_KP_5, SDLK_KP_4, SDLK_KP_6);
                keysPlayer(&input, &player2, SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT);
            }
            /*==X= PLAYER MOVE KEYS =X==*/
        }

        // ACTIONS

        /*==== EAT APPLE ====*/
        eatApple(renderer, &player1);
        eatApple(renderer, &player2);
        /*==X= EAT APPLE =X==*/

        /*==== MOVE PLAYER ====*/
        if(bot2player)
        {
            aiPlayer2();
        }
        movePlayer(&player1);
        movePlayer(&player2);
        /*==X= MOVE PLAYER =X==*/

        /*==== UPDATE SCORE ====*/
        player1.scoreRect.h = (windowH/10)*player1.score;
        player1.scoreRect.w = windowH/10;
        player2.scoreRect.h = (windowH/10)*player2.score;
        player2.scoreRect.w = windowH/10;
        /*==X= UPDATE SCORE =X==*/

        /*==== EAT APPLE ====*/
        eatApple(renderer, &player1);
        eatApple(renderer, &player2);
        /*==X= EAT APPLE =X==*/

        // START OF RENDERER
        SDL_RenderClear(renderer); //  Clear screen

        // Player Score
        playerScore(renderer);

        /*================= RENDER OBJECTS =================*/
        for (int i = 1; i <= cellsNum; i++)
        {
            for (int j = 1; j <= cellsNum; j++)
            {
                /*==== RENDER GRID ====*/
                cell.x = (i*cell.h) + centerGridW;
                cell.y = (j*cell.w) + centerGridH;
                if((i+j)%2 == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 30,30,30,255);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 25,25,25,255);
                }
                
                SDL_RenderFillRect(renderer, &cell);
                /*==X= RENDER GRID =X==*/

                /*==== RENDER APPLE ====*/
                if(apple.x == i && apple.y == j)
                {
                    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
                    SDL_RenderFillRect(renderer, &cell);
                }
                /*==X= RENDER APPLE =X==*/

                /*==== RENDER PLAYER ====*/
                renderPlayer(renderer, i, j, cell, &player1);
                renderPlayer(renderer, i, j, cell, &player2);
                /*==X= RENDER PLAYER =X==*/
                

                /*==== RENDER SCORE ====*/
                SDL_SetRenderDrawColor(renderer, player1.tRGB.r,player1.tRGB.g,player1.tRGB.b,255);
                SDL_RenderFillRect(renderer, &player1.scoreRect); // Player 1
                
                SDL_SetRenderDrawColor(renderer, player2.tRGB.r,player2.tRGB.g,player2.tRGB.b,255);
                SDL_RenderFillRect(renderer, &player2.scoreRect); // Player 2
                /*==X= RENDER SCORE =X==*/
            }
        }
        /*===============X= RENDER OBJECTS =X===============*/

        //Grow Player
        growPlayer(&player1);
        growPlayer(&player2);

        // BACKGROUND
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255); // Background Collor
        SDL_RenderPresent(renderer);                       // Fill the Screen with background collor
        
        SDL_Delay(100);
    }

    // END OF PROGRAM
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}