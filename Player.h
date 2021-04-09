#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Field.h"
#include <memory>
#include <vector> 

using namespace std;
 
enum class MovementDir
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Player
{
    explicit Player(Point pos = {.x = 10, .y = 10}) :
            coords(pos), old_coords(coords) {
                InitPlayer();
            };

    bool Moved() const;
    void ProcessInput(MovementDir dir, Field &field);
    void ProcessInput_step(MovementDir dir);
    void Draw(Image &field, Image &screen, int k, int i);
    void draw(Image &field, Image &screen, vector<shared_ptr<Image>>& animation_player, int i);
    void InitPlayer(); 
    Point coords {.x = 10, .y = 10};
    private:
      Point old_coords {.x = 10, .y = 10};
      Pixel color {.r = 145, .g = 255, .b = 0, .a = 255};
      int move_speed = 2;
      vector<shared_ptr<Image>> animation_player_front; 
      vector<shared_ptr<Image>> animation_player_back;
      vector<shared_ptr<Image>> animation_player_left; 
      vector<shared_ptr<Image>> animation_player_right;
      vector<shared_ptr<Image>> animation_player_died;
};

#endif //MAIN_PLAYER_H
