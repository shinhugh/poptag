// A block on the board

#ifndef BOARD_BLOCK_H
#define BOARD_BLOCK_H

#include "hitbox.h"

// ------------------------------------------------------------

enum BlockType {breakable, unbreakable};

// ------------------------------------------------------------

class BoardBlock {

private:
  Hitbox hitbox;
  BlockType type;

public:
  BoardBlock(unsigned int, unsigned int, BlockType);
  const Hitbox * getHitbox();

};

// ------------------------------------------------------------

#endif
