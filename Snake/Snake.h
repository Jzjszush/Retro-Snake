/**************************************************
 * Snake.h                                        *
 * Jake Kellogg                                   *
 * 4/5/2024                                       *
 * Create a working retro snake game using raylib *
**************************************************/
#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <fstream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
using namespace std;


//color structs that takes in value for colors
Color green = {173,204,96,255};
Color darkGreen = {43,51,24,255};


int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

//This function checks if the head is hiting a certain spot
bool ElementInDeque(Vector2 element, deque<Vector2> deque){
    for(unsigned int i = 0; i < deque.size(); i++){
        if(Vector2Equals(deque[i],element)){
            return true;
        }
    }
    return false;

}
//this function is a checker to see if something happened, is used to update the game as it goes
bool eventTrigger(double interval){
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    else return false;
}

class Snake{

    public:
        deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        Vector2 direction = {1,0};
        bool addSegment = false;
    //draws the snake on the screen
    void Draw(){
        //unsigned int is because we cannot compare two unlike things
        for(unsigned int i = 0; i < body.size(); i++){
            float x  = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x*cellSize, offset + y*cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment,0.5,6,darkGreen);
        }
    }
    void Update(){
        
        //goes outside to avoid duplicate code
        body.push_front(Vector2Add(body[0], direction));
        if(addSegment == true){
            addSegment = false;
        }
        else{
            body.pop_back();
        }
    }
    void Reset(){
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0};
    }
};

class Food{

    public:
        Vector2 pos;
        Texture2D texture;

        //constructor
        Food(deque<Vector2> snakeBody){
            //create a new image using the load image function
            Image image = LoadImage("Graphics/food.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            pos = GenerateRandomPos(snakeBody);
        }
        //destructor
        ~Food(){
            UnloadTexture(texture);
        }

        void Draw(){
            DrawTexture(texture, offset + pos.x*cellSize, offset + pos.y*cellSize, WHITE);

        }

        Vector2 GenerateRandomCell(){
            float x = GetRandomValue(0,cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);    
            return Vector2{x,y};
        }
        

        Vector2 GenerateRandomPos(deque<Vector2> snakeBody){
            //getting random x and  y values withing the screen so the food is randomly placed each time
            
            Vector2 pos = GenerateRandomCell();
            while(ElementInDeque(pos, snakeBody)){
                pos = GenerateRandomCell();
            }
            return pos;
         }

};

class Game{
    public:
        Snake snake = Snake();
        Food food = Food(snake.body);
        bool running = false;
        int score = 0;
        int highScore;
        ifstream infile;
        ofstream outfile;
       
        
        

        void Draw(){
            food.Draw();
            snake.Draw();
        }
        void Update(){
            if(running == true){
            snake.Update();
            CheckCollisionFood();
            CheckCollisionEdges();
            CheckCollisionTail();
            }
        }
        //this will see if the food is in the same spot as the head
        void CheckCollisionFood(){
            //Vector2Equals returns 1 if two vector2 structs are equal
            if(Vector2Equals(snake.body[0],food.pos)){
                food.pos = food.GenerateRandomPos(snake.body);
                snake.addSegment = true;
                score ++;
            //Here is where the high score will be stored so it can be tracked over multiple games
                infile.open("Snake.txt");
                infile >> highScore;
                if(score > highScore){
                    highScore ++;
                    infile.close();
                    outfile.open("Snake.txt");
                    outfile << highScore;
                    outfile.close();
                }   
                

            }
        }
        void CheckCollisionEdges(){
            //if it has hit the right side of the grid or behind the grid (-1)
            if(snake.body[0].x == cellCount || snake.body[0].x == -1){
                GameOver();
            }
            if(snake.body[0].y == cellCount || snake.body[0].y == -1){
                GameOver();
            }
        }
        void CheckCollisionTail(){
            //noHead requresents all the other elements in the snake not inlcuding its body
            deque<Vector2> noHead = snake.body;
            noHead.pop_front();
            if(ElementInDeque(snake.body[0], noHead)){
                GameOver();
            }
        }

        void GameOver(){
            snake.Reset();
            food.pos = food.GenerateRandomPos(snake.body);
            running = false;
            score = 0;
        }

};

#endif