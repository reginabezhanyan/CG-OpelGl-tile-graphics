#include "Player.h"
#include "Field.h"
#include <GLFW/glfw3.h>
#include <sstream> 

using namespace std;

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir, Field &field)
{
  int move_dist = move_speed * 2;  
  int k = 0;
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y; 
      k = field.state[(coords.y + move_dist + 20) / tileSize][(coords.x + 12) / tileSize];
      if((field.state[(coords.y + 20) / tileSize][(coords.x + 12)/ tileSize] == 3 && k != 3) ||
                (field.state[(coords.y + 20) / tileSize][(coords.x + 12)/ tileSize] == 4 && k != 4)) {
          field.state[(coords.y + 20) / tileSize][(coords.x + 12)/ tileSize] = 2;
          field.floor = true; 
          field.floors_w = 15;
          field.floors.x = (coords.x + 12) / tileSize * tileSize;
          field.floors.y = (coords.y + 20) / tileSize * tileSize; 
      } else if((field.state[(coords.y + 20) / tileSize][(coords.x + 12)/ tileSize] == 5 && k != 5) ||
                (field.state[(coords.y + 20) / tileSize][(coords.x + 12)/ tileSize] == 6 && k != 6)) {
          field.state[(coords.y + 20) / tileSize][(coords.x + 12)/ tileSize] -= 2;
          field.floor = true; 
          field.floors_w = 16;
          field.floors.x = (coords.x + 12) / tileSize * tileSize;
          field.floors.y = (coords.y + 20) / tileSize * tileSize; 
      } 
      if(k == 0 || k == 3 || k == 4 || k == 5 || k == 6) {
          coords.y += move_dist;
      } else if(k == 10) { 
          field.level = true;
          coords.y += tileSize/8;
      } else if(k == 2) {
          field.die = true;
          coords.y += tileSize/4;
      } else if(k == 7) {
          field.win = true;
          coords.y += tileSize/4;
      }
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      k = field.state[(coords.y - move_dist + 7) / tileSize][(coords.x + 12) / tileSize];
      if((field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] == 3 && k != 3) ||
                (field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] == 4 && k != 4)) {
          field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] = 2;
          field.floor = true; 
          field.floors_w = 15;
          field.floors.x = (coords.x + 12) / tileSize * tileSize;
          field.floors.y = (coords.y + 7) / tileSize * tileSize; 
      } else if((field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] == 5 && k != 5) ||
                (field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] == 6 && k != 6)) {
          field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] -= 2;
          field.floor = true; 
          field.floors_w = 16;
          field.floors.x = (coords.x + 12) / tileSize * tileSize;
          field.floors.y = (coords.y + 7) / tileSize * tileSize; 
      }
      if(k == 0 || k == 3 || k == 4 || k == 5 || k == 6) { 
          coords.y -= move_dist;
      } else if(k == 10) { 
          field.level = true;
          coords.y -= tileSize/4;
      } else if(k == 2) {
          field.die = true;
          coords.y -= tileSize/4;
      } else if(k == 7) {
          field.win = true;
          coords.y -= tileSize/4;
      }
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      k = field.state[(coords.y + 7)/ tileSize][(coords.x - move_dist + 12) / tileSize];
      if((field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] == 3 && k != 3) ||
                (field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] == 4 && k != 4)) {
          field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] = 2;
          field.floor = true;
          field.floors_w = 15;
          field.floors.x = (coords.x + 12) / tileSize * tileSize;
          field.floors.y = (coords.y + 7) / tileSize * tileSize; 
      } else if((field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] == 5 && k != 5) ||
                (field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] == 6 && k != 6)) {
          field.state[(coords.y + 7) / tileSize][(coords.x + 12)/ tileSize] -= 2;
          field.floor = true;
          field.floors_w = 16;
          field.floors.x = (coords.x + 12) / tileSize * tileSize;
          field.floors.y = (coords.y + 7) / tileSize * tileSize; 
      }
      if(k == 0 || k == 3 || k == 4 || k == 5 || k == 6) { 
          coords.x -= move_dist;
      } else if(k == 10) { 
          field.level = true; 
          coords.x -= tileSize/4;
      } else if(k == 2) {
          field.die = true;
          coords.x -= tileSize/4;
      } 
      if(k == 7) {
          field.win = true;
          coords.x -= tileSize/4;
      }   
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      k = field.state[(coords.y + 7) / tileSize][(coords.x + move_dist - 11) / tileSize + 1];
      if((field.state[(coords.y + 7) / tileSize][(coords.x - 11)/ tileSize + 1] == 3 && k != 3) ||
                (field.state[(coords.y + 7) / tileSize][(coords.x - 11)/ tileSize + 1] == 4 && k != 4)) {
          field.state[(coords.y + 7) / tileSize][(coords.x - 11)/ tileSize + 1] = 2;
          field.floor = true; 
          field.floors_w = 15;
          field.floors.x = ((coords.x - 11) / tileSize + 1) * tileSize;
          field.floors.y = (coords.y + 7) / tileSize * tileSize; 
      } else if((field.state[(coords.y + 7) / tileSize][(coords.x - 11)/ tileSize + 1] == 5 && k != 5) ||
                (field.state[(coords.y + 7) / tileSize][(coords.x - 11)/ tileSize + 1] == 6 && k != 6)) {
          field.state[(coords.y + 7) / tileSize][(coords.x - 11)/ tileSize + 1] -= 2;
          field.floor = true; 
          field.floors_w = 16;
          field.floors.x = ((coords.x - 11) / tileSize + 1) * tileSize;
          field.floors.y = (coords.y + 7) / tileSize * tileSize; 
      }
      if(k == 0 || k == 3 || k == 4 || k == 5 || k == 6) { 
          coords.x += move_dist;
      } else if(k == 10) { 
          field.level = true;
      } else if(k == 2) {
          field.die = true;
          coords.x += tileSize/4;
          coords.x += tileSize/4;
      } else if(k == 7) {
          field.win = true;
          coords.x += tileSize/4;
      }  
      break;
    default:
      break;
  }
}

void Player::ProcessInput_step(MovementDir dir)
{
  int move_dist = move_speed * 2;   
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y; 
      coords.y += move_dist; 
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      coords.y -= move_dist; 
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      coords.x -= move_dist; 
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      coords.x += move_dist; 
      break;
    default:
      break;
  }
}

void Player::InitPlayer() {
     //front
     for(auto i = 0; i < 10; i++) {
        ostringstream oss;
        oss<< "./resources/32x32/Warrior/Run/Front/0_Warrior_Run_00" << i << ".png";
        animation_player_front.push_back(make_shared<Image>(oss.str()));
     } 
     for(auto i = 10; i < 15; i++) {
        ostringstream oss;
        oss<< "./resources/32x32/Warrior/Run/Front/0_Warrior_Run_0" << i << ".png";
        animation_player_front.push_back(make_shared<Image>(oss.str()));
     } 
     //back
     for(auto i = 0; i < 10; i++) {
        ostringstream oss;
        oss<< "./resources/32x32/Warrior/Run/Back/0_Warrior_Run_00" << i << ".png";
        animation_player_back.push_back(make_shared<Image>(oss.str()));
     } 
     for(auto i = 10; i < 15; i++) {
        ostringstream oss;
        oss<< "./resources/32x32/Warrior/Run/Back/0_Warrior_Run_0" << i << ".png";
        animation_player_back.push_back(make_shared<Image>(oss.str()));
     }  
     //left
     for(auto i = 0; i < 10; i++) {
        ostringstream oss;
        oss<< "./resources/32x32/Warrior/Run/Left/0_Warrior_Run_00" << i << ".png";
        animation_player_left.push_back(make_shared<Image>(oss.str()));
     } 
     for(auto i = 10; i < 15; i++) {
        ostringstream oss;
        oss<< "./resources/32x32/Warrior/Run/Left/0_Warrior_Run_0" << i << ".png";
        animation_player_left.push_back(make_shared<Image>(oss.str()));
     }  
     //right
     for(auto i = 0; i < 10; i++) {
        ostringstream oss;
        oss<< "./resources/32x32/Warrior/Run/Right/0_Warrior_Run_00" << i << ".png";
        animation_player_right.push_back(make_shared<Image>(oss.str()));
     } 
     for(auto i = 10; i < 15; i++) {
        ostringstream oss;
        oss<< "./resources/32x32/Warrior/Run/Right/0_Warrior_Run_0" << i << ".png";
        animation_player_right.push_back(make_shared<Image>(oss.str()));
     }
     //died
     for(auto i = 0; i < 10; i++) {
        ostringstream oss;
        oss<< "./resources/32x32/Warrior/Died/0_Warrior_Died_00" << i << ".png";
        animation_player_died.push_back(make_shared<Image>(oss.str()));
     } 
     for(auto i = 10; i < 30; i++) {
        ostringstream oss;
        oss<< "./resources/32x32/Warrior/Died/0_Warrior_Died_0" << i << ".png";
        animation_player_died.push_back(make_shared<Image>(oss.str()));
     }
}

void Player::draw(Image &field, Image &screen, vector<shared_ptr<Image>>& animation_player, int i) { 
     for(int y = old_coords.y; y < old_coords.y + tileSize; ++y) {
        for(int x = old_coords.x; x < old_coords.x + tileSize; ++x) {
            screen.PutPixel(x, y, field.GetPixel(x, y));
        }
     }
     
    for(int y = coords.y; y < coords.y + tileSize; ++y) {
        for(int x = coords.x; x < coords.x + tileSize; ++x) {
            Pixel pix = animation_player[i]->GetPixel(x - coords.x, tileSize - y - 1 + coords.y);
            screen.PutPixel(x, y, blend(field.GetPixel(x, y), pix));
        } 
    }
    old_coords = coords;
}



void Player::Draw(Image &field, Image &screen, int k, int i)
{   
    if(k == 1) {
        draw(field, screen, animation_player_back, i);
    } else if(k == 2) {
        draw(field, screen, animation_player_front, i);
    } else if(k == 3) {
        draw(field, screen, animation_player_left, i);
    } else if(k == 4) {
        draw(field, screen, animation_player_right, i);
    } else if(k == 5) {
        draw(field, screen, animation_player_died, i);
    }
}