/**************************************************
 * main.cpp                                       *
 * Jake Kellogg                                   *
 * 4/5/2024                                       *
 * Create a working retro snake game using raylib *
**************************************************/
#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include "Snake.h"


using namespace std;


int main (){
    cout << "Starting the Game..." << endl;
    //Here is where we create the window for the game. It is the blank canvas for the snake game
    InitWindow(2*offset + cellSize*cellCount,2*offset + cellSize*cellCount,"RetroSnake");
    SetTargetFPS(60);
    Game game = Game();
    //Game Loop
    //if the escape key or exit is pressed, it tunrs true
    while(WindowShouldClose() == false){
        
        BeginDrawing();

        if(eventTrigger(0.2)){
            game.Update();
        }
        //here is where we are going to allow for the arrows to control the snake
        //will not allow snake to do a 180
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0,-1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0,1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1,0};
            game.running = true;
        }
        if(IsKeyPressed(KEY_RIGHT)&& game.snake.direction.x != -1){
            game.snake.direction = {1,0};
            game.running = true;
        }
        

        //Drawing the background green
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize*cellCount + 10, (float)cellSize*cellCount + 10},5,darkGreen);
        DrawText("RETRO SNAKE", offset - 5, 20,40, darkGreen);
        DrawText(TextFormat("%i",game.score), offset-5,offset + 5 +cellSize*cellCount,40,darkGreen );
        DrawText("HIGH SCORE: ",offset + 400, 20,40, darkGreen);
        DrawText(TextFormat("%i", game.highScore), offset +700,20,40,darkGreen);

        game.Draw();

        EndDrawing();
    }
    

    //We need to close the window at the end, this is what this does
    CloseWindow();
    return 0;
}