#include "board.h"

// ------------------------------------------------------------

Board::Board(unsigned int width, unsigned int height) :
width(width), height(height) {

  // Allocate memory for terrain
  this->terrain = new Terrain *[this->height];
  for(unsigned int y = 0; y < this->height; y++) {
    this->terrain[y] = new Terrain[this->width];
  }

  // Initialize terrain to all ground
  for(unsigned int y = 0; y < this->height; y++) {
    for(unsigned int x = 0; x < this->width; x++) {
      this->terrain[y][x] = ground;
    }
  }

}

// ------------------------------------------------------------

Board::Board(const Board& src) :
width(src.width), height(src.height) {

  // Allocate memory for terrain
  this->terrain = new Terrain *[this->height];
  for(unsigned int y = 0; y < this->height; y++) {
    this->terrain[y] = new Terrain[this->width];
  }

  // Copy terrain from source
  for(unsigned int y = 0; y < this->height; y++) {
    for(unsigned int x = 0; x < this->width; x++) {
      this->terrain[y][x] = src.terrain[y][x];
    }
  }

}

// ------------------------------------------------------------

Board& Board::operator=(const Board& src) {

  // Copy width and height from source
  this->width = src.width;
  this->height = src.height;

  // Copy terrain from source
  for(unsigned int y = 0; y < this->height; y++) {
    for(unsigned int x = 0; x < this->width; x++) {
      this->terrain[y][x] = src.terrain[y][x];
    }
  }

}

// ------------------------------------------------------------

Board::~Board() {

  // Free memory allocated for terrain
  for(unsigned int y = 0; y < this->height; y++) {
    delete[] this->terrain[y];
  }
  delete[] this->terrain;

}

// ------------------------------------------------------------

Terrain Board::getTerrain(unsigned int y, unsigned int x) {

  return this->terrain[y][x];

}

// ------------------------------------------------------------

void Board::detonateBomb(unsigned int y, unsigned int x, unsigned int range) {

  // Coordinates to check for terrain modification
  unsigned int curr_y = y;
  unsigned int curr_x = x;

  // Limits
  unsigned int y_l = range > y ? 0 : y - range;
  unsigned int y_h = y + range >= this->height ? this->height - 1 : y + range;
  unsigned int x_l = range > x ? 0 : x - range;
  unsigned int x_h = x + range >= this->width ? this->width - 1 : x + range;

  // Upward
  while(curr_y >= y_l) {
    if(this->terrain[curr_y][curr_x] == breakable) {
      this->terrain[curr_y][curr_x] = ground;
    }
    else if(this->terrain[curr_y][curr_x] == unbreakable) {
      break;
    }
    if(curr_y == 0) {
      break;
    }
    curr_y--;
  }
  curr_y = y;

  // Rightward
  while(curr_x <= x_h) {
    if(this->terrain[curr_y][curr_x] == breakable) {
      this->terrain[curr_y][curr_x] = ground;
    }
    else if(this->terrain[curr_y][curr_x] == unbreakable) {
      break;
    }
    curr_x++;
  }
  curr_x = x;

  // Downward
  while(curr_y <= y_h) {
    if(this->terrain[curr_y][curr_x] == breakable) {
      this->terrain[curr_y][curr_x] = ground;
    }
    else if(this->terrain[curr_y][curr_x] == unbreakable) {
      break;
    }
    curr_y++;
  }
  curr_y = y;

  // Leftward
  while(curr_x >= x_l) {
    if(this->terrain[curr_y][curr_x] == breakable) {
      this->terrain[curr_y][curr_x] = ground;
    }
    else if(this->terrain[curr_y][curr_x] == unbreakable) {
      break;
    }
    if(curr_x == 0) {
      break;
    }
    curr_x--;
  }
  curr_x = x;

}
