// Board on which the game takes place

#ifndef BOARD_H
#define BOARD_H

#define BOARD_HEIGHT 10
#define BOARD_WIDTH 10

// ------------------------------------------------------------

enum Terrain {ground, breakable, unbreakable};

// ------------------------------------------------------------

class Board {

private:
  unsigned int height, width;
  Terrain **terrain;

public:
  Board(unsigned int, unsigned int);
  Board(const Board&);
  Board& operator=(const Board&);
  ~Board();
  unsigned int getHeight();
  unsigned int getWidth();
  Terrain getTerrain(unsigned int, unsigned int);
  void detonateBomb(unsigned int, unsigned int, unsigned int);

};

// ------------------------------------------------------------

#endif
