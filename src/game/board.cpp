#include "board.h"

// ------------------------------------------------------------

Board::Board(unsigned int height, unsigned int width) :
height(height), width(width) {

  // Allocate memory for blocks and their existential flags
  this->blocks = new BoardBlock *[this->height];
  this->blocks_exist = new bool *[this->height];
  for(unsigned int y = 0; y < this->height; y++) {
    this->blocks[y] = new BoardBlock[this->width];
    this->blocks_exist[y] = new BoardBlock[this->width];
    // Unset all flags
    for(unsigned int x = 0; x < this->width; x++) {
      this->blocks_exist[y][x] = false;
    }
  }

}

// ------------------------------------------------------------

Board::Board(const Board& src) :
height(src.height), width(src.width) {

  // Allocate memory for blocks and their existential flags
  this->blocks = new BoardBlock *[this->height];
  this->blocks_exist = new bool *[this->height];
  for(unsigned int y = 0; y < this->height; y++) {
    this->blocks[y] = new BoardBlock[this->width];
    this->blocks_exist[y] = new BoardBlock[this->width];
  }

  // Copy from source
  for(unsigned int y = 0; y < this->height; y++) {
    for(unsigned int x = 0; x < this->width; x++) {
      this->blocks[y][x] = src.blocks[y][x];
      this->blocks_exist[y][x] = src.blocks_exist[y][x];
    }
  }

}

// ------------------------------------------------------------

Board& Board::operator=(const Board& src) {

  // Free memory previously allocated for blocks
  for(unsigned int y = 0; y < this->height; y++) {
    delete[] this->blocks[y];
    delete[] this->blocks_exist[y];
  }
  delete[] this->blocks;
  delete[] this->blocks_exist;

  // Copy width and height from source
  this->height = src.height;
  this->width = src.width;

  // Allocate memory for blocks and their existential flags
  this->blocks = new BoardBlock *[this->height];
  this->blocks_exist = new bool *[this->height];
  for(unsigned int y = 0; y < this->height; y++) {
    this->blocks[y] = new BoardBlock[this->width];
    this->blocks_exist[y] = new BoardBlock[this->width];
  }

  // Copy from source
  for(unsigned int y = 0; y < this->height; y++) {
    for(unsigned int x = 0; x < this->width; x++) {
      this->blocks[y][x] = src.blocks[y][x];
      this->blocks_exist[y][x] = src.blocks_exist[y][x];
    }
  }

  return *this;

}

// ------------------------------------------------------------

Board::~Board() {

  // Free memory allocated for blocks
  for(unsigned int y = 0; y < this->height; y++) {
    delete[] this->blocks[y];
    delete[] this->blocks_exist[y];
  }
  delete[] this->blocks;
  delete[] this->blocks_exist;

}

// ------------------------------------------------------------

unsigned int Board::getHeight() const {

  return this->height;

}

// ------------------------------------------------------------

unsigned int Board::getWidth() const {

  return this->width;

}

// ------------------------------------------------------------

bool Board::getBlockExist(unsigned int y, unsigned int x) const {

  return this->blocks_exist[y][x];

}

// ------------------------------------------------------------

const BoardBlock * Board::getBlock(unsigned int y, unsigned int x) const {

  return &(this->blocks[y][x]);

}

// ------------------------------------------------------------

/*
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
*/
