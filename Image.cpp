#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

using namespace std;

Pixel blend(Pixel oldPixel, Pixel newPixel)
{
    newPixel.r = newPixel.a / 255.0 * (newPixel.r - oldPixel.r) + oldPixel.r;
    newPixel.g = newPixel.a / 255.0 * (newPixel.g - oldPixel.g) + oldPixel.g;
    newPixel.b = newPixel.a / 255.0 * (newPixel.b - oldPixel.b) + oldPixel.b;
    newPixel.a = 255;
    return newPixel; 
}

Image::Image(const Image &other) {
    width = other.width;
    height = other.height;
    channels = other.channels;
    size = other.size;
    self_allocated = other.self_allocated;
    if (other.self_allocated) {
        data = new Pixel[width * height] {};
    } else {
        data = (Pixel *)malloc(size);
    }
        if (data != nullptr) {
        memcpy(data, other.data, size);
    }
}

Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, sizeof(Pixel))) != nullptr)
  {
    size = width * height * channels;
  }
}

Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * a_height ]{};

  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}


int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}
 
Image &Image::operator= (const Image &other) {
  if (this == &other) {
    return *this;
  }
  this->~Image();
  width = other.width;
  height = other.height;
  channels = other.channels;
  size = other.size;
  self_allocated = other.self_allocated;
  if (other.self_allocated) {
    data = new Pixel[width * height] {};
  } else {
    data = (Pixel *)malloc(size);
  }
  if (data != nullptr) {
    memcpy(data, other.data, size);
  }
  return *this;
}

/*Image &Image::operator= (Image &&other) {
  if (this == &other) {
    return *this;
  }
  this->~Image();
  width = other.width;
  height = other.height;
  channels = other.channels;
  size = other.size;
  self_allocated = other.self_allocated;
  data = other.data;
  /*other.data = nullptr;
  other.size = 0;
  other.width = -1;
  other.height = -1;
  other.channels = 3;
  other.self_allocated = false;
  return *this;
}*/

Image::~Image()
{
  if(self_allocated)
    delete [] data;
  else
  {
    stbi_image_free(data);
  }
}