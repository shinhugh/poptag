#include "board_block.h"

// ------------------------------------------------------------

BoardBlock::BoardBlock() :
hitbox(0.5, 0.5, 1, 1), type(breakable) {}

// ------------------------------------------------------------

BoardBlock::BoardBlock(unsigned int y, unsigned int x, BlockType type) :
hitbox(y + 0.5, x + 0.5, 1, 1), type(type) {}

// ------------------------------------------------------------

const Hitbox * BoardBlock::getHitbox() const {

  return &(this->hitbox);

}
