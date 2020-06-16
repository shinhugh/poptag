#include "hitbox.h"

// ------------------------------------------------------------

Hitbox::Hitbox(float center_y, float center_x, float height, float width) {

  this->center[0] = center_y;
  this->center[1] = center_x;
  this->dimensions[0] = height;
  this->dimensions[1] = width;

}

// ------------------------------------------------------------

float Hitbox::getCenterY() const {

  return this->center[0];

}

// ------------------------------------------------------------

void Hitbox::setCenterY(float center_y) {

  this->center[0] = center_y;

}

// ------------------------------------------------------------

float Hitbox::getCenterX() const {

  return this->center[1];

}

// ------------------------------------------------------------

void Hitbox::setCenterX(float center_x) {

  this->center[1] = center_x;

}

// ------------------------------------------------------------

float Hitbox::getHeight() const {

  return this->dimensions[0];

}

// ------------------------------------------------------------

float Hitbox::getWidth() const {

  return this->dimensions[1];

}


// ------------------------------------------------------------

bool Hitbox::moveUp(float distance,
const std::vector<const Hitbox *> *neighbors,
float area_height, float area_width) {

  // Two relevant corners
  float corner_ul[2];
  float corner_ur[2];

  // Up left corner, y-coordinate
  corner_ul[0] = this->center[0] > (dimensions[0] / 2)
  ? this->center[0] - (dimensions[0] / 2) : 0;
  // Up left corner, x-coordinate
  corner_ul[1] = this->center[1] > (dimensions[1] / 2)
  ? this->center[1] - (dimensions[1] / 2) : 0;
  // Up right corner, y-coordinate
  corner_ur[0] = corner_ul[0];
  // Up right corner, x-coordinate
  corner_ur[1] = this->center[1] + (dimensions[1] / 2) <= area_width
  ? this->center[1] + (dimensions[1] / 2) : area_width;

  // Farthest boundary the hitbox can touch
  float closest_threshold
  = corner_ul[0] > distance ? corner_ul[0] - distance : 0;

  // Check all neighbor hitboxes for potential collision
  if(neighbors) {
    for(unsigned int i = 0; i < neighbors->size(); i++) {
      // Current neighbor's boundaries along x-axis
      float lower_bound = neighbors->at(i)->center[1]
      > (neighbors->at(i)->dimensions[1] / 2)
      ? neighbors->at(i)->center[1] - (neighbors->at(i)->dimensions[1] / 2) : 0;
      float upper_bound = neighbors->at(i)->center[1]
      + (neighbors->at(i)->dimensions[1] / 2) <= area_width
      ? neighbors->at(i)->center[1] + (neighbors->at(i)->dimensions[1] / 2)
      : area_width;
      // Check for possibility of collision
      if((lower_bound >= corner_ul[1] && lower_bound < corner_ur[1])
      || (upper_bound <= corner_ur[1] && upper_bound > corner_ul[1])
      || (lower_bound <= corner_ul[1] && upper_bound >= corner_ur[1])) {
        // Collision may be possible, depending on distance traveled
        float threshold = neighbors->at(i)->center[0]
        + (neighbors->at(i)->dimensions[0] / 2) <= area_height
        ? neighbors->at(i)->center[0] + (neighbors->at(i)->dimensions[0] / 2)
        : area_height;
        if(threshold <= corner_ul[0] && closest_threshold < threshold) {
          // Collision confirmed
          closest_threshold = threshold;
        }
      }
    }
  }

  // Determine whether hitbox can move at all
  bool canMove
  = this->center[0] == closest_threshold + (this->dimensions[0] / 2)
  ? false : true;

  // Move to closest collision (if at all)
  this->center[0] = closest_threshold + (this->dimensions[0] / 2);

  // Return whether hitbox moved
  return canMove;

}

// ------------------------------------------------------------

bool Hitbox::moveRight(float distance,
const std::vector<const Hitbox *> *neighbors,
float area_height, float area_width) {

  // Two relevant corners
  float corner_ur[2];
  float corner_dr[2];

  // Up right corner, y-coordinate
  corner_ur[0] = this->center[0] > (dimensions[0] / 2)
  ? this->center[0] - (dimensions[0] / 2) : 0;
  // Up right corner, x-coordinate
  corner_ur[1] = this->center[1] + (dimensions[1] / 2) <= area_width
  ? this->center[1] + (dimensions[1] / 2) : area_width;
  // Down right corner, y-coordinate
  corner_dr[0] = this->center[0] + (dimensions[0] / 2) <= area_height
  ? this->center[0] + (dimensions[0] / 2) : area_height;
  // Down right corner, x-coordinate
  corner_dr[1] = corner_ur[1];

  // Farthest boundary the hitbox can touch
  float closest_threshold
  = corner_ur[1] + distance <= area_width ? corner_ur[1] + distance
  : area_width;

  // Check all neighbor hitboxes for potential collision
  if(neighbors) {
    for(unsigned int i = 0; i < neighbors->size(); i++) {
      // Current neighbor's boundaries along y-axis
      float lower_bound = neighbors->at(i)->center[0]
      > (neighbors->at(i)->dimensions[0] / 2)
      ? neighbors->at(i)->center[0] - (neighbors->at(i)->dimensions[0] / 2) : 0;
      float upper_bound = neighbors->at(i)->center[0]
      + (neighbors->at(i)->dimensions[0] / 2) <= area_height
      ? neighbors->at(i)->center[0] + (neighbors->at(i)->dimensions[0] / 2)
      : area_height;
      // Check for possibility of collision
      if((lower_bound >= corner_ur[0] && lower_bound < corner_dr[0])
      || (upper_bound <= corner_dr[0] && upper_bound > corner_ur[0])
      || (lower_bound <= corner_ur[0] && upper_bound >= corner_dr[0])) {
        // Collision may be possible, depending on distance traveled
        float threshold = neighbors->at(i)->center[1]
        > (neighbors->at(i)->dimensions[1] / 2)
        ? neighbors->at(i)->center[1] - (neighbors->at(i)->dimensions[1] / 2)
        : 0;
        if(threshold >= corner_ur[1] && closest_threshold > threshold) {
          // Collision confirmed
          closest_threshold = threshold;
        }
      }
    }
  }

  // Determine whether hitbox can move at all
  bool canMove
  = this->center[1] == closest_threshold - (this->dimensions[1] / 2)
  ? false : true;

  // Move to closest collision (if at all)
  this->center[1] = closest_threshold - (this->dimensions[1] / 2);

  // Return whether hitbox moved
  return canMove;

}

// ------------------------------------------------------------

bool Hitbox::moveDown(float distance,
const std::vector<const Hitbox *> *neighbors,
float area_height, float area_width) {

  // Two relevant corners
  float corner_dl[2];
  float corner_dr[2];

  // Down left corner, y-coordinate
  corner_dl[0] = this->center[0] + (dimensions[0] / 2) <= area_height
  ? this->center[0] + (dimensions[0] / 2) : area_height;
  // Down left corner, x-coordinate
  corner_dl[1] = this->center[1] > (dimensions[1] / 2)
  ? this->center[1] - (dimensions[1] / 2) : 0;
  // Down right corner, y-coordinate
  corner_dr[0] = corner_dl[0];
  // Down right corner, x-coordinate
  corner_dr[1] = this->center[1] + (dimensions[1] / 2) <= area_width
  ? this->center[1] + (dimensions[1] / 2) : area_width;

  // Farthest boundary the hitbox can touch
  float closest_threshold
  = corner_dl[0] + distance <= area_height ? corner_dl[0] + distance
  : area_height;

  // Check all neighbor hitboxes for potential collision
  if(neighbors) {
    for(unsigned int i = 0; i < neighbors->size(); i++) {
      // Current neighbor's boundaries along x-axis
      float lower_bound = neighbors->at(i)->center[1]
      > (neighbors->at(i)->dimensions[1] / 2)
      ? neighbors->at(i)->center[1] - (neighbors->at(i)->dimensions[1] / 2) : 0;
      float upper_bound = neighbors->at(i)->center[1]
      + (neighbors->at(i)->dimensions[1] / 2) <= area_width
      ? neighbors->at(i)->center[1] + (neighbors->at(i)->dimensions[1] / 2)
      : area_width;
      // Check for possibility of collision
      if((lower_bound >= corner_dl[1] && lower_bound < corner_dr[1])
      || (upper_bound <= corner_dr[1] && upper_bound > corner_dl[1])
      || (lower_bound <= corner_dl[1] && upper_bound >= corner_dr[1])) {
        // Collision may be possible, depending on distance traveled
        float threshold = neighbors->at(i)->center[0]
        > (neighbors->at(i)->dimensions[0] / 2)
        ? neighbors->at(i)->center[0] - (neighbors->at(i)->dimensions[0] / 2)
        : 0;
        if(threshold >= corner_dl[0] && closest_threshold > threshold) {
          // Collision confirmed
          closest_threshold = threshold;
        }
      }
    }
  }

  // Determine whether hitbox can move at all
  bool canMove
  = this->center[0] == closest_threshold - (this->dimensions[0] / 2)
  ? false : true;

  // Move to closest collision (if at all)
  this->center[0] = closest_threshold - (this->dimensions[0] / 2);

  // Return whether hitbox moved
  return canMove;

}

// ------------------------------------------------------------

bool Hitbox::moveLeft(float distance,
const std::vector<const Hitbox *> *neighbors,
float area_height, float area_width) {

  // Two relevant corners
  float corner_ul[2];
  float corner_dl[2];

  // Up left corner, y-coordinate
  corner_ul[0] = this->center[0] > (dimensions[0] / 2)
  ? this->center[0] - (dimensions[0] / 2) : 0;
  // Up left corner, x-coordinate
  corner_ul[1] = this->center[1] > (dimensions[1] / 2)
  ? this->center[1] - (dimensions[1] / 2) : 0;
  // Down left corner, y-coordinate
  corner_dl[0] = this->center[0] + (dimensions[0] / 2) <= area_height
  ? this->center[0] + (dimensions[0] / 2) : area_height;
  // Down left corner, x-coordinate
  corner_dl[1] = corner_ul[1];

  // Farthest boundary the hitbox can touch
  float closest_threshold
  = corner_ul[1] > distance ? corner_ul[1] - distance : 0;

  // Check all neighbor hitboxes for potential collision
  if(neighbors) {
    for(unsigned int i = 0; i < neighbors->size(); i++) {
      // Current neighbor's boundaries along y-axis
      float lower_bound = neighbors->at(i)->center[0]
      > (neighbors->at(i)->dimensions[0] / 2)
      ? neighbors->at(i)->center[0] - (neighbors->at(i)->dimensions[0] / 2) : 0;
      float upper_bound = neighbors->at(i)->center[0]
      + (neighbors->at(i)->dimensions[0] / 2) <= area_height
      ? neighbors->at(i)->center[0] + (neighbors->at(i)->dimensions[0] / 2)
      : area_height;
      // Check for possibility of collision
      if((lower_bound >= corner_ul[0] && lower_bound < corner_dl[0])
      || (upper_bound <= corner_dl[0] && upper_bound > corner_ul[0])
      || (lower_bound <= corner_ul[0] && upper_bound >= corner_dl[0])) {
        // Collision may be possible, depending on distance traveled
        float threshold = neighbors->at(i)->center[1]
        + (neighbors->at(i)->dimensions[1] / 2) <= area_width
        ? neighbors->at(i)->center[1] + (neighbors->at(i)->dimensions[1] / 2)
        : area_width;
        if(threshold <= corner_ul[1] && closest_threshold < threshold) {
          // Collision confirmed
          closest_threshold = threshold;
        }
      }
    }
  }

  // Determine whether hitbox can move at all
  bool canMove
  = this->center[1] == closest_threshold + (this->dimensions[1] / 2)
  ? false : true;

  // Move to closest collision (if at all)
  this->center[1] = closest_threshold + (this->dimensions[1] / 2);

  // Return whether hitbox moved
  return canMove;

}
