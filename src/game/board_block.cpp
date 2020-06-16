#include "board_block.h"

// ------------------------------------------------------------

BoardBlock::BoardBlock(unsigned int y, unsigned int x, BlockType type) :
type(type) {

  hitbox = Hitbox(y + 0.5, x + 0.5, 1, 1);

}

// ------------------------------------------------------------

const Hitbox * BoardBlock::getHitbox() {

  return &(this->hitbox);

}
