// Board on which the game takes place

#ifndef BOARD_H
#define BOARD_H

// ------------------------------------------------------------

enum Terrain {ground, breakable, unbreakable};

// ------------------------------------------------------------

class Board {

private:
  unsigned int width;
  unsigned int height;
  Terrain **terrain;

public:
  Board(unsigned int, unsigned int);
  Board(const Board&);
  operator=(const Board&);
  ~Board();
  Terrain getTerrain(unsigned int, unsigned int);
  void detonateBomb(unsigned int, unsigned int, unsigned int);

};

// ------------------------------------------------------------

#endif
