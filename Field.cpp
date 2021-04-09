#include "Field.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

void Field::InitTileset() {
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/bg.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/wall_1.png")); 
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/wall_2.png")); 
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/wall_3.png")); 
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/wall_4.png")); 
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/road_5.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/lake_1_6.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/lake_2_7.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/lake_3_8.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/lake_4_9.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/palm_10.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/lake_2_11.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/lake_4_12.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/lake_1_13.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/lake_3_14.png")); 
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/dirt_1_15.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/dirt_2_16.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/dirt_3_17.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/fire_1_18.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/fire_2_19.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/fire_3_20.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/fire_2_21.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/land_22.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/land_23.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/land_24.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/land_25.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/land_26.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/land_27.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/land_28.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/land_29.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/land_30.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/land_31.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/stone_32.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/stone_33.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/stone_34.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/stone_35.png")); 
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/stone_36.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/stone_37.png"));
    resources.push_back(make_shared<Image>("./resources/32x32/Sands/stone_38.png")); 
}

void Field::InitField(string &path1, string &path2)
{
    fstream file; 
    int tmp;
    
    file.open(path1);
    for(int j = TILE_HEIGHT - 1; j >= 0; j--) {
        for(int i = 0; i < TILE_WIDTH; i++) { 
            file >> field[j][i];
        }
    }
    file.close(); 
    
    file.open(path2); 
    for(int j = TILE_HEIGHT - 1; j >= 0; j--) {
        for(int i = 0; i < TILE_WIDTH; i++) { 
            file >> state[j][i];
            state_begin[j][i] = state[j][i];
        }
    }
    file.close(); 
}

void Field::DrawBackground(Image &screen) {
    int w = 0;
    if(level) w = 5;
    for(int j = 0; j < TILE_HEIGHT; j++) {
        for(int i = 0; i < TILE_WIDTH; i++) {
            for(int y = coords.y; y < coords.y + tileSize; ++y) {
                for(int x = coords.x; x < coords.x + tileSize; ++x) {
                    Pixel pix = resources[w]->GetPixel(x - coords.x, y - coords.y);
                    screen.PutPixel(x, y, pix);  
                }
            }
            coords.x += tileSize;
        }
        coords.y += tileSize;
        coords.x = 0;
    } 
}

Point Field::DrawResources(Image &screen) {
    coords.x = coords.y = 0;
    Point pos {.x = 0, .y = 0};
    for(int j = 0; j < TILE_HEIGHT; j++) {
        for(int i = 0; i < TILE_WIDTH; i++) {
            if(field[j][i] < 0) {
                pos.x = coords.x;
                pos.y = coords.y;
                field[j][i] *= -1;
            }
            
            if(field[j][i] ==  18 || (fg && (field[j][i] ==  32) && (field[(0 < j ? j - 1 : j)][i] != 18) && (field[(j < 31 ? j + 1 : j)][i] != 18)
                    && (field[j][(i < 49 ? i + 1 : i)] != 18) && (field[j][(0 < i ? i - 1 : i)] != 18) && (field[j + 1][i + 1] != 18)
                            && (field[j - 1][i - 1] != 18) && (field[j + 1][i - 1] != 18) && (field[j - 1][i + 1] != 18))) { //animatons
                Point pos{.x = coords.x, .y = coords.y};
                if(field[j][i] ==  18) {
                    static_animations.push_back(make_pair(pos, make_pair(field[j][i], 4)));
                    pos.x += tileSize/2;
                    pos.y += tileSize/2;
                    light.push_back(pos);
                } else {
                    static_animations.push_back(make_pair(pos, make_pair(field[j][i], 7)));
                    fg = false;
                }
                field[j][i] = 0;
            }
            if(!fg && field[j][i] ==  32) fg = true;
            if(field[j][i] != 0) {
                for(int y = coords.y; y < coords.y + tileSize; ++y) {
                    for(int x = coords.x; x < coords.x + tileSize; ++x) {
                        Pixel pix = resources[field[j][i]]->GetPixel(x - coords.x, tileSize - y - 1 + coords.y); 
                        screen.PutPixel(x, y, blend(screen.GetPixel(x, y), pix));  
                    }
                }
            }
            coords.x += tileSize;
        }
        coords.y += tileSize;
        coords.x = 0; 
    } 
    return pos;
}

void Field::DrawGO(Image &screen, string path) {
    shared_ptr<Image> img = make_shared<Image>(path); 
    for(int j = 0; j < TILE_HEIGHT * tileSize; j++) {
        for(int i = 0; i < TILE_WIDTH * tileSize; i++) { 
            Pixel pix = img->GetPixel(i, 1024 - j);
            screen.PutPixel(i, j, blend(screen.GetPixel(i, j), pix));  
        }
    } 
}

void Field::DrawStaticAnimations(Image &screen, Image &screenBuffer, Point coords, int w) {
    for(int y = coords.y; y < coords.y + tileSize; ++y) {
        for(int x = coords.x; x < coords.x + tileSize; ++x) {
            Pixel pix = resources[w]->GetPixel(x - coords.x, tileSize - y - 1 + coords.y);
            screenBuffer.PutPixel(x, y, blend(screen.GetPixel(x, y), pix));
        } 
    }
}

void Field::cpy() {
    for(int j = 0; j < TILE_HEIGHT; j++) {
        for(int i = 0; i < TILE_WIDTH; i++) { 
            state[j][i] = state_begin[j][i]; 
        }
    } 
}

void Field::Light(Image &screen, int k, int x, int y) {
    Pixel pix = screen.GetPixel(x, y);
    if(pix.r + k > 255) {
        pix.r = (uint8_t) 255;
    } else {
        pix.r += k; 
    }
    if(pix.g + k > 255) {
        pix.g = (uint8_t) 255;
    } else {
        pix.g += k; 
    }
    if(pix.b + k > 255) {
        pix.b = (uint8_t) 255;
    } else {
        pix.b += k; 
    }
    if(pix.a + k > 255) {
        pix.a = (uint8_t) 255;
    } else {
        pix.a += k; 
    }
    screen.PutPixel(x, y, pix);
}

void Field::DrawLight(Image &screen, Point coords){
    for(int y = coords.y - 2*R1; y < coords.y + 2*R1; ++y) {
        for(int x = coords.x - 2*R1; x < coords.x + 2*R1; ++x) {
            double a = coords.y - y, b = coords.x - x;
            if(sqrt(a*a + b*b) <= R2) {
                Light(screen, k2, x, y);     
            } else if (sqrt(a*a + b*b) <= R1) {
                Light(screen, k1, x, y);    
            }
        } 
    }
}