#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "thread_display.h"
#include "game_common.h"
#include "data_packet.h"
#include "event_data.h"
#include "game_state.h"
#include "hitbox.h"
#include "character.h"

// Unix
// #define PATH_SHADER_DIR "../src/core/"
// #define PATH_TEXTURE_DIR "../assets/"
// MSVC
#define PATH_SHADER_DIR "../../src/core/"
#define PATH_TEXTURE_DIR "../../assets/"
// Portable
// #define PATH_SHADER_DIR "./"
// #define PATH_TEXTURE_DIR "./"

#define SLIT_HEIGHT 0.1f

// ------------------------------------------------------------

// Game instance being rendered; pointer is updated when thread is initialized
static Game* game_instance;

// ------------------------------------------------------------

static void errorCallback(int, const char*);

static void keyCallback(GLFWwindow *, int, int, int, int);

static std::string readFile(std::string);

static float translateLocation(float, unsigned int, bool);

static void generateVertices(float *, const Hitbox *, unsigned int,
unsigned int);

static void augmentYCoordinates(float *, float);

static void generateTexture(GLuint *id, const char *);

// ------------------------------------------------------------

void threadRoutine_Display(Game& game) {

  // Update game instance pointer
  game_instance = &game;

  // Initialize GLFW
  if (!glfwInit())
  {
    // Initialization failed
    std::cerr << std::string("Error: GLFW initialization failed.\n");
    return;
  }

  // Set error handler function
  glfwSetErrorCallback(errorCallback);

  // Minimum OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create OpenGl context and containing window
  GLFWwindow* window = glfwCreateWindow(720, 720, "Test", NULL, NULL);
  if (!window)
  {
    // Window or OpenGL context creation failed
    std::cerr << std::string("Error: Window creation failed.\n");
    glfwTerminate();
    return;
  }

  // Set key event handler function
  glfwSetKeyCallback(window, keyCallback);

  // Make the current context the newly created window
  glfwMakeContextCurrent(window);

  // Load OpenGL functions
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cerr << std::string("Error: OpenGL context initialization failed.\n");
    glfwTerminate();
    return;
  }

  // Minimum number of monitor refreshes between a glfwSwapBuffers() call and
  // the actual swap
  // Set to > 0 to avoid tearing
  glfwSwapInterval(1);

  // Vertex shader
  std::string vertex_shader_src = readFile(std::string(PATH_SHADER_DIR)
  + "shader.vert");
  const char * vertex_shader_src_c = vertex_shader_src.c_str();
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_src_c, 0);
  glCompileShader(vertex_shader);
  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    std::cerr << info_log << std::endl;
    return;
  }

  // Fragment shader
  std::string fragment_shader_src = readFile(std::string(PATH_SHADER_DIR)
  + "shader.frag");
  const char * fragment_shader_src_c = fragment_shader_src.c_str();
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_src_c, 0);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    std::cerr << info_log << std::endl;
    return;
  }

  // Shader program
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    std::cerr << info_log << std::endl;
    return;
  }
  glUseProgram(shader_program);

  // Delete shaders; already linked to shader program
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // Texture for background
  GLuint texture_bg;
  generateTexture(&texture_bg, (std::string(PATH_TEXTURE_DIR)
  + "background.png").c_str());
  // Texture for bomb
  GLuint texture_bomb;
  generateTexture(&texture_bomb, (std::string(PATH_TEXTURE_DIR)
  + "bomb.png").c_str());
  // Texture for board block, unbreakable
  GLuint texture_block_unbreakable;
  generateTexture(&texture_block_unbreakable, (std::string(PATH_TEXTURE_DIR)
  + "block_unbreakable.png").c_str());
  // Texture for board block, breakable
  GLuint texture_block_breakable;
  generateTexture(&texture_block_breakable, (std::string(PATH_TEXTURE_DIR)
  + "block_breakable.png").c_str());
  // Texture for character
  GLuint texture_character;
  generateTexture(&texture_character, (std::string(PATH_TEXTURE_DIR)
  + "character.png").c_str());
  // Texture for explosion, bomb
  GLuint texture_explosion_bomb;
  generateTexture(&texture_explosion_bomb, (std::string(PATH_TEXTURE_DIR)
  + "explosion_bomb.png").c_str());
  // Texture for explosion, up
  GLuint texture_explosion_up;
  generateTexture(&texture_explosion_up, (std::string(PATH_TEXTURE_DIR)
  + "explosion_up.png").c_str());
  // Texture for explosion, right
  GLuint texture_explosion_right;
  generateTexture(&texture_explosion_right, (std::string(PATH_TEXTURE_DIR)
  + "explosion_right.png").c_str());
  // Texture for explosion, down
  GLuint texture_explosion_down;
  generateTexture(&texture_explosion_down, (std::string(PATH_TEXTURE_DIR)
  + "explosion_down.png").c_str());
  // Texture for explosion, left
  GLuint texture_explosion_left;
  generateTexture(&texture_explosion_left, (std::string(PATH_TEXTURE_DIR)
  + "explosion_left.png").c_str());

  // Generate and bind vertex array object
  GLuint vert_array;
  glGenVertexArrays(1, &vert_array);
  glBindVertexArray(vert_array);

  // Memory for vertices' position coordinates
  float *vertices_pos = new float[12];
  // Generate vertex buffer object for vertex position
  GLuint buf_vert_pos;
  glGenBuffers(1, &buf_vert_pos);
  // Configure vertex array object
  glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
  (void *) 0);
  glEnableVertexAttribArray(0);

  // Memory for vertices' colors
  float *vertices_color = new float[16];
  // Generate vertex buffer object for vertex color
  GLuint buf_vert_color;
  glGenBuffers(1, &buf_vert_color);
  // Configure vertex array object
  glBindBuffer(GL_ARRAY_BUFFER, buf_vert_color);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
  (void *) 0);
  glEnableVertexAttribArray(1);

  // Texture coordinates (quadrant order)
  float *vertices_texture = new float[8];
  vertices_texture[0] = 1.0f;
  vertices_texture[1] = 1.0f;
  vertices_texture[2] = 0.0f;
  vertices_texture[3] = 1.0f;
  vertices_texture[4] = 0.0f;
  vertices_texture[5] = 0.0f;
  vertices_texture[6] = 1.0f;
  vertices_texture[7] = 0.0f;
  // Generate and bind vertex buffer object for texture coordinates
  GLuint buf_vert_texture;
  glGenBuffers(1, &buf_vert_texture);
  glBindBuffer(GL_ARRAY_BUFFER, buf_vert_texture);
  // Populate texture coordinate buffer
  glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices_texture,
  GL_STATIC_DRAW);
  // Free memory
  delete[] vertices_texture;
  // Configure vertex array object
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
  (void *) 0);
  glEnableVertexAttribArray(2);

  // Vertex index ordering to draw a square (quadrant order)
  unsigned int *indices_square = new unsigned int[6];
  indices_square[0] = 0;
  indices_square[1] = 1;
  indices_square[2] = 2;
  indices_square[3] = 0;
  indices_square[4] = 2;
  indices_square[5] = 3;
  // Generate and bind element buffer object
  GLuint buf_elem;
  glGenBuffers(1, &buf_elem);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_elem);
  // Populate element buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
  indices_square, GL_STATIC_DRAW);
  // Free memory
  delete[] indices_square;

  // Unbind vertex array object
  glBindVertexArray(0);

  // While game is ongoing
  while (!(game.isExit()) && !glfwWindowShouldClose(window))
  {

    // Read game state
    GameState game_state = game.stateSnapshot();

    // Get frame dimensions and specify viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Clear display
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw background
    Hitbox background_space(0.5, 0.5, 1, 1);
    for(unsigned int y = 0; y < game_state.getBoardHeight(); y++) {
      for(unsigned int x = 0; x < game_state.getBoardWidth(); x++) {

        background_space.setCenterY(y + 0.5);
        background_space.setCenterX(x + 0.5);
        generateVertices(vertices_pos, &background_space,
        game_state.getBoardHeight(), game_state.getBoardWidth());
        augmentYCoordinates(vertices_pos, SLIT_HEIGHT);

        // Texture
        glBindTexture(GL_TEXTURE_2D, texture_bg);

        // Bind vertex array object
        glBindVertexArray(vert_array);

        // Bind buffer for vertex position
        glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
        // Copy vertex position data into currently bound buffer
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices_pos,
        GL_STATIC_DRAW);

        // Draw
        glBindVertexArray(vert_array);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Unbind vertex array object
        glBindVertexArray(0);

      }
    }

    // Draw board blocks and bombs
    for(unsigned int y = 0; y < game_state.getBoardHeight(); y++) {

      for(unsigned int x = 0; x < game_state.getBoardWidth(); x++) {

        // Board blocks
        if(game_state.getBlockExist(y, x)) {

          // Vertex positions for hitbox (quadrant order)
          generateVertices(vertices_pos, game_state.getBlock(y, x)->getHitbox(),
          game_state.getBoardHeight(), game_state.getBoardWidth());
          augmentYCoordinates(vertices_pos, SLIT_HEIGHT);

          // Texture
          if(game_state.getBlock(y, x)->getType() == unbreakable) {
            glBindTexture(GL_TEXTURE_2D, texture_block_unbreakable);
          } else {
            glBindTexture(GL_TEXTURE_2D, texture_block_breakable);
          }

          // Bind vertex array object
          glBindVertexArray(vert_array);

          // Bind buffer for vertex position
          glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
          // Copy vertex position data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices_pos,
          GL_DYNAMIC_DRAW);

          // Draw
          glBindVertexArray(vert_array);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

          // Unbind vertex array object
          glBindVertexArray(0);

        }

        // Bombs
        if(game_state.getBombExist(y, x)) {

          // Vertex positions for hitbox (quadrant order)
          generateVertices(vertices_pos, game_state.getBomb(y, x)->getHitbox(),
          game_state.getBoardHeight(), game_state.getBoardWidth());
          augmentYCoordinates(vertices_pos, SLIT_HEIGHT);

          // Texture
          glBindTexture(GL_TEXTURE_2D, texture_bomb);

          // Bind vertex array object
          glBindVertexArray(vert_array);

          // Bind buffer for vertex position
          glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
          // Copy vertex position data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices_pos,
          GL_DYNAMIC_DRAW);

          // Draw
          glBindVertexArray(vert_array);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

          // Unbind vertex array object
          glBindVertexArray(0);

        }

      }

      // Draw characters
      for(unsigned int i = 0; i < game_state.getCharacterCount(); i++) {
        if(game_state.getCharacterAlive(i)
        && static_cast<unsigned int>(game_state.getCharacter(i)->getHitbox()
        ->getCenterY()) == y) {

          // Vertex positions for hitbox (quadrant order)
          generateVertices(vertices_pos, game_state.getCharacter(i)
          ->getHitbox(), game_state.getBoardHeight(),
          game_state.getBoardWidth());
          augmentYCoordinates(vertices_pos, SLIT_HEIGHT);

          // Texture
          glBindTexture(GL_TEXTURE_2D, texture_character);

          // Bind vertex array object
          glBindVertexArray(vert_array);

          // Bind buffer for vertex position
          glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
          // Copy vertex position data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices_pos,
          GL_DYNAMIC_DRAW);

          // Draw
          glBindVertexArray(vert_array);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

          // Unbind vertex array object
          glBindVertexArray(0);

        }
      }

      // Draw explosions
      for(unsigned int x = 0; x < game_state.getBoardWidth(); x++) {
        if(game_state.getExplosionExist(y, x)) {

          // Vertex positions for hitbox (quadrant order)
          generateVertices(vertices_pos,
          game_state.getExplosion(y, x)->getHitbox(),
          game_state.getBoardHeight(), game_state.getBoardWidth());
          augmentYCoordinates(vertices_pos, SLIT_HEIGHT);

          // Texture
          if(game_state.getExplosion(y, x)->getDirection() == up) {
            glBindTexture(GL_TEXTURE_2D, texture_explosion_up);
          } else if(game_state.getExplosion(y, x)->getDirection() == right) {
            glBindTexture(GL_TEXTURE_2D, texture_explosion_right);
          } else if(game_state.getExplosion(y, x)->getDirection() == down) {
            glBindTexture(GL_TEXTURE_2D, texture_explosion_down);
          } else if(game_state.getExplosion(y, x)->getDirection() == left) {
            glBindTexture(GL_TEXTURE_2D, texture_explosion_left);
          } else {
            glBindTexture(GL_TEXTURE_2D, texture_explosion_bomb);
          }

          // Bind vertex array object
          glBindVertexArray(vert_array);

          // Bind buffer for vertex position
          glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
          // Copy vertex position data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices_pos,
          GL_DYNAMIC_DRAW);

          // Draw
          glBindVertexArray(vert_array);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

          // Unbind vertex array object
          glBindVertexArray(0);

        }
      }

    }

    // Swap front/back render buffers (to display new render)
    glfwSwapBuffers(window);

    // Handle input events in queue
    glfwPollEvents();

  }

  // If window close wasn't triggered by in-game exit, trigger in-game exit too
  game.exit();

  // Close window
  glfwDestroyWindow(window);

  // Free resources used by OpenGL
  glDeleteProgram(shader_program);
  glDeleteTextures(1, &texture_bg);
  glDeleteTextures(1, &texture_bomb);
  glDeleteTextures(1, &texture_block_unbreakable);
  glDeleteTextures(1, &texture_block_breakable);
  glDeleteTextures(1, &texture_character);
  glDeleteTextures(1, &texture_explosion_bomb);
  glDeleteTextures(1, &texture_explosion_up);
  glDeleteTextures(1, &texture_explosion_right);
  glDeleteTextures(1, &texture_explosion_down);
  glDeleteTextures(1, &texture_explosion_left);
  glDeleteVertexArrays(1, &vert_array);
  glDeleteBuffers(1, &buf_vert_pos);
  glDeleteBuffers(1, &buf_vert_color);
  glDeleteBuffers(1, &buf_vert_texture);
  glDeleteBuffers(1, &buf_elem);

  // Free previously allocated memory
  delete[] vertices_pos;
  delete[] vertices_color;

  // Free resources used by GLFW
  glfwTerminate();

  // Exit program
  return;

}

// ------------------------------------------------------------

static void errorCallback(int error, const char* description)
{

  // Log error
  std::cerr << std::string("Error: ") + std::string(description)
  + std::string("\n");

}

// ------------------------------------------------------------

static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
int mods) {

  static unsigned int current_direction = 0;

  // Escape, pressed
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    game_instance->exit();
  }

  // I, pressed
  else if(key == GLFW_KEY_I && action == GLFW_PRESS) {
    DataPacket packet;
    EventData_Initialize event_data;
    event_data.initialize = true;
    packet.setType(initialize);
    packet.setData(&event_data, sizeof(EventData_Initialize));
    game_instance->queueEvent(packet);
  }

  // W, pressed
  else if(key == GLFW_KEY_W && action == GLFW_PRESS) {
    current_direction = 1;
    DataPacket packet;
    EventData_MoveUp event_data;
    event_data.character_id = 0;
    packet.setType(moveUp);
    packet.setData(&event_data, sizeof(EventData_MoveUp));
    game_instance->queueEvent(packet);
  }

  // W, released
  else if(key == GLFW_KEY_W && action == GLFW_RELEASE) {
    if(current_direction == 1) {
      current_direction = 0;
      DataPacket packet;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      packet.setType(moveStop);
      packet.setData(&event_data, sizeof(EventData_MoveStop));
      game_instance->queueEvent(packet);
    }
  }

  // D, pressed
  else if(key == GLFW_KEY_D && action == GLFW_PRESS) {
    current_direction = 2;
    DataPacket packet;
    EventData_MoveRight event_data;
    event_data.character_id = 0;
    packet.setType(moveRight);
    packet.setData(&event_data, sizeof(EventData_MoveRight));
    game_instance->queueEvent(packet);
  }

  // D, released
  else if(key == GLFW_KEY_D && action == GLFW_RELEASE) {
    if(current_direction == 2) {
      current_direction = 0;
      DataPacket packet;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      packet.setType(moveStop);
      packet.setData(&event_data, sizeof(EventData_MoveStop));
      game_instance->queueEvent(packet);
    }
  }

  // S, pressed
  else if(key == GLFW_KEY_S && action == GLFW_PRESS) {
    current_direction = 3;
    DataPacket packet;
    EventData_MoveDown event_data;
    event_data.character_id = 0;
    packet.setType(moveDown);
    packet.setData(&event_data, sizeof(EventData_MoveDown));
    game_instance->queueEvent(packet);
  }

  // S, released
  else if(key == GLFW_KEY_S && action == GLFW_RELEASE) {
    if(current_direction == 3) {
      current_direction = 0;
      DataPacket packet;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      packet.setType(moveStop);
      packet.setData(&event_data, sizeof(EventData_MoveStop));
      game_instance->queueEvent(packet);
    }
  }

  // A, pressed
  else if(key == GLFW_KEY_A && action == GLFW_PRESS) {
    current_direction = 4;
    DataPacket packet;
    EventData_MoveLeft event_data;
    event_data.character_id = 0;
    packet.setType(moveLeft);
    packet.setData(&event_data, sizeof(EventData_MoveLeft));
    game_instance->queueEvent(packet);
  }

  // A, released
  else if(key == GLFW_KEY_A && action == GLFW_RELEASE) {
    if(current_direction == 4) {
      current_direction = 0;
      DataPacket packet;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      packet.setType(moveStop);
      packet.setData(&event_data, sizeof(EventData_MoveStop));
      game_instance->queueEvent(packet);
    }
  }

  // Up, pressed
  else if(key == GLFW_KEY_UP && action == GLFW_PRESS) {
    current_direction = 1;
    DataPacket packet;
    EventData_MoveUp event_data;
    event_data.character_id = 0;
    packet.setType(moveUp);
    packet.setData(&event_data, sizeof(EventData_MoveUp));
    game_instance->queueEvent(packet);
  }

  // Up, released
  else if(key == GLFW_KEY_UP && action == GLFW_RELEASE) {
    if(current_direction == 1) {
      current_direction = 0;
      DataPacket packet;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      packet.setType(moveStop);
      packet.setData(&event_data, sizeof(EventData_MoveStop));
      game_instance->queueEvent(packet);
    }
  }

  // Right, pressed
  else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
    current_direction = 2;
    DataPacket packet;
    EventData_MoveRight event_data;
    event_data.character_id = 0;
    packet.setType(moveRight);
    packet.setData(&event_data, sizeof(EventData_MoveRight));
    game_instance->queueEvent(packet);
  }

  // Right, released
  else if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
    if(current_direction == 2) {
      current_direction = 0;
      DataPacket packet;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      packet.setType(moveStop);
      packet.setData(&event_data, sizeof(EventData_MoveStop));
      game_instance->queueEvent(packet);
    }
  }

  // Down, pressed
  else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    current_direction = 3;
    DataPacket packet;
    EventData_MoveDown event_data;
    event_data.character_id = 0;
    packet.setType(moveDown);
    packet.setData(&event_data, sizeof(EventData_MoveDown));
    game_instance->queueEvent(packet);
  }

  // Down, released
  else if(key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
    if(current_direction == 3) {
      current_direction = 0;
      DataPacket packet;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      packet.setType(moveStop);
      packet.setData(&event_data, sizeof(EventData_MoveStop));
      game_instance->queueEvent(packet);
    }
  }

  // Left, pressed
  else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
    current_direction = 4;
    DataPacket packet;
    EventData_MoveLeft event_data;
    event_data.character_id = 0;
    packet.setType(moveLeft);
    packet.setData(&event_data, sizeof(EventData_MoveLeft));
    game_instance->queueEvent(packet);
  }

  // Left, released
  else if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
    if(current_direction == 4) {
      current_direction = 0;
      DataPacket packet;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      packet.setType(moveStop);
      packet.setData(&event_data, sizeof(EventData_MoveStop));
      game_instance->queueEvent(packet);
    }
  }

  // Space, pressed
  else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    DataPacket packet;
    EventData_PlaceBomb event_data;
    event_data.character_id = 0;
    packet.setType(placeBomb);
    packet.setData(&event_data, sizeof(EventData_PlaceBomb));
    game_instance->queueEvent(packet);
  }

}

// ------------------------------------------------------------

static std::string readFile(std::string path) {

  std::string output;

  std::ifstream stream(path.c_str(), std::ios::in);
  if(!stream.is_open()) {
    std::cerr << "Could not read file \"" << path << "\"." << std::endl;
    return "";
  }

  std::string line;
  while(!stream.eof()) {
    std::getline(stream, line);
    output.append(line + "\n");
  }
  stream.close();

  return output;

}

// ------------------------------------------------------------

static float translateLocation(float location, unsigned int axis_max,
bool invert) {

  return invert ? 1 - ((2.0f / axis_max) * location)
  : -1 + ((2.0f / axis_max) * location);

}

// ------------------------------------------------------------

static void generateVertices(float * buffer, const Hitbox *hitbox,
unsigned int board_height, unsigned int board_width) {

  float center[2] = {hitbox->getCenterY(), hitbox->getCenterX()};
  float radius = hitbox->getHeight() / 2;

  buffer[2] = 0;
  buffer[5] = 0;
  buffer[8] = 0;
  buffer[11] = 0;

  // Quadrant order
  buffer[0] = translateLocation(center[1] + radius, board_width, false);
  buffer[1] = translateLocation(center[0] + radius, board_width, true);
  buffer[3] = translateLocation(center[1] - radius, board_width, false);
  buffer[4] = translateLocation(center[0] + radius, board_width, true);
  buffer[6] = translateLocation(center[1] - radius, board_width, false);
  buffer[7] = translateLocation(center[0] - radius, board_width, true);
  buffer[9] = translateLocation(center[1] + radius, board_width, false);
  buffer[10] = translateLocation(center[0] - radius, board_width, true);

}

// ------------------------------------------------------------

static void augmentYCoordinates(float * buffer, float slit_height) {

  buffer[1] = (2.0f / (2.0f + slit_height)) * (buffer[1] + 1) - 1;
  buffer[4] = (2.0f / (2.0f + slit_height)) * (buffer[4] + 1) - 1;
  buffer[7] = (2.0f / (2.0f + slit_height)) * (buffer[7] + 1 + slit_height) - 1;
  buffer[10] = (2.0f / (2.0f + slit_height)) * (buffer[10] + 1 + slit_height)
  - 1;

}

// ------------------------------------------------------------

static void generateTexture(GLuint *id, const char *file_name) {

  int texture_width, texture_height, texture_channel_count;
  unsigned char *texture_data = stbi_load(file_name, &texture_width,
  &texture_height, &texture_channel_count, STBI_rgb_alpha);
  glGenTextures(1, id);
  glBindTexture(GL_TEXTURE_2D, *id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0,
  GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
  stbi_image_free(texture_data);
  glGenerateMipmap(GL_TEXTURE_2D);

}
