#ifndef MAIN_FIELD_H
#define MAIN_FIELD_H
 
#include "Image.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;

static int TILE_WIDTH = 50, TILE_HEIGHT = 32;
static int R1 = 64, R2 = 55, k1 = 35, k2 = 50;
 
struct Point
{
  int x;
  int y;
};


struct Field {    
    explicit Field(Point pos = {.x = 0, .y = 0}) : coords(pos) { 
        InitTileset();
    };
    
    void InitField(string &path1, string &path2);
    void InitTileset();
    void DrawBackground(Image &screen);
    Point DrawResources(Image &screen);
    void DrawGO(Image &screen, string path);
    void DrawStaticAnimations(Image &screen, Image &screenBuffer, Point coords, int w);
    void DrawLight(Image &screen, Point coords);
    void Light(Image &screen, int k, int x, int y);
    void cpy();
    int state[32][50];
    int state_begin[32][50];
    int field[32][50]; 
    vector<shared_ptr<Image>> resources;
    bool level = false;
    bool floor = false;
    bool die = false;
    bool win = false;
    Point floors {.x = 0, .y = 0}; 
    int floors_w = 0;
    int fg = true;
    Point coords {.x = 0, .y = 0};   
    vector<pair<Point, pair<int, unsigned>>> static_animations;
    vector<Point> light;
};



#endif //MAIN_FIELD_H