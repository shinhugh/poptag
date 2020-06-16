// Board on which the game takes place

#ifndef BOARD_H
#define BOARD_H

#include "board_block.h"

#define BOARD_HEIGHT 10
#define BOARD_WIDTH 10

// ------------------------------------------------------------

class Board {

private:
  unsigned int height, width;
  BoardBlock **blocks;
  bool **blocks_exist;

public:
  Board(unsigned int, unsigned int);
  Board(const Board&);
  Board& operator=(const Board&);
  ~Board();
  unsigned int getHeight() const;
  unsigned int getWidth() const;
  bool getBlockExist(unsigned int, unsigned int) const;
  const BoardBlock * getBlock(unsigned int, unsigned int) const;
  void createBlock(unsigned int, unsigned int);

  // void detonateBomb(unsigned int, unsigned int, unsigned int);

};

// ------------------------------------------------------------

#endif
