#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "thread_display.h"
#include "data_packet.h"
#include "event_data.h"
#include "game_state.h"
#include "hitbox.h"
#include "character.h"

// Linux
// #define SHADER_DIR_PATH "../src/core/"
// MSVC
#define SHADER_DIR_PATH "../../src/core/"
// Portable
// #define SHADER_DIR_PATH "./"

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
  std::string vertex_shader_src = readFile(std::string(SHADER_DIR_PATH)
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
  std::string fragment_shader_src = readFile(std::string(SHADER_DIR_PATH)
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

  // Generate and bind vertex array object
  GLuint array_vert;
  glGenVertexArrays(1, &array_vert);
  glBindVertexArray(array_vert);

  // Memory for vertices' coordinates
  float *vertices_pos = new float[12];
  // Memory for vertices' colors
  float *vertices_color = new float[16];

  // Generate vertex buffer object for vertex position
  GLuint buf_vert_pos;
  glGenBuffers(1, &buf_vert_pos);

  // Generate vertex buffer object for vertex color
  GLuint buf_vert_color;
  glGenBuffers(1, &buf_vert_color);

  // Configure vertex array object
  glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
  (void *) 0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buf_vert_color);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
  (void *) 0);
  glEnableVertexAttribArray(1);

  // Generate and bind element buffer object
  GLuint buf_elem;
  glGenBuffers(1, &buf_elem);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_elem);

  // Vertex index ordering to draw a square (quadrant order)
  unsigned int *indices_square = new unsigned int[6];
  indices_square[0] = 0;
  indices_square[1] = 1;
  indices_square[2] = 2;
  indices_square[3] = 0;
  indices_square[4] = 2;
  indices_square[5] = 3;

  // Populate element buffer object
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
  indices_square, GL_DYNAMIC_DRAW);

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

    // Draw characters
    for(unsigned int i = 0; i < game_state.getCharacterCount(); i++) {
      if(game_state.getCharacterAlive(i)) {

        // Vertex positions for hitbox (quadrant order)
        generateVertices(vertices_pos, game_state.getCharacter(i)->getHitbox(),
        game_state.getBoardHeight(), game_state.getBoardWidth());

        // Vertex colors
        // Red
        for(unsigned int i = 0; i < 16; i += 4) {
          vertices_color[i] = 0;
        }
        // Green
        for(unsigned int i = 1; i < 16; i += 4) {
          vertices_color[i] = 1;
        }
        // Blue
        for(unsigned int i = 2; i < 16; i += 4) {
          vertices_color[i] = 0;
        }
        // Alpha
        for(unsigned int i = 3; i < 16; i += 4) {
          vertices_color[i] = 1;
        }

        // Bind vertex array object
        glBindVertexArray(array_vert);

        // Bind buffer for vertex position
        glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
        // Copy vertex position data into currently bound buffer
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices_pos,
        GL_DYNAMIC_DRAW);

        // Bind buffer for vertex color
        glBindBuffer(GL_ARRAY_BUFFER, buf_vert_color);
        // Copy vertex position data into currently bound buffer
        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices_color,
        GL_DYNAMIC_DRAW);

        // Draw
        glBindVertexArray(array_vert);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Unbind vertex array object
        glBindVertexArray(0);

      }
    }

    // Draw board blocks
    for(unsigned int y = 0; y < game_state.getBoardHeight(); y++) {
      for(unsigned int x = 0; x < game_state.getBoardWidth(); x++) {
        if(game_state.getBlockExist(y, x)) {

          // Vertex positions for hitbox (quadrant order)
          generateVertices(vertices_pos, game_state.getBlock(y, x)->getHitbox(),
          game_state.getBoardHeight(), game_state.getBoardWidth());

          // Vertex colors
          if(game_state.getBlock(y, x)->getType() == unbreakable) {
            // Red
            for(unsigned int i = 0; i < 16; i += 4) {
              vertices_color[i] = 1;
            }
            // Green
            for(unsigned int i = 1; i < 16; i += 4) {
              vertices_color[i] = 1;
            }
            // Blue
            for(unsigned int i = 2; i < 16; i += 4) {
              vertices_color[i] = 1;
            }
            // Alpha
            for(unsigned int i = 3; i < 16; i += 4) {
              vertices_color[i] = 1;
            }
          } else {
            // Red
            for(unsigned int i = 0; i < 16; i += 4) {
              vertices_color[i] = 0.8;
            }
            // Green
            for(unsigned int i = 1; i < 16; i += 4) {
              vertices_color[i] = 0.8;
            }
            // Blue
            for(unsigned int i = 2; i < 16; i += 4) {
              vertices_color[i] = 0.8;
            }
            // Alpha
            for(unsigned int i = 3; i < 16; i += 4) {
              vertices_color[i] = 1;
            }
          }

          // Bind vertex array object
          glBindVertexArray(array_vert);

          // Bind buffer for vertex position
          glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
          // Copy vertex position data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices_pos,
          GL_DYNAMIC_DRAW);

          // Bind buffer for vertex color
          glBindBuffer(GL_ARRAY_BUFFER, buf_vert_color);
          // Copy vertex position data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices_color,
          GL_DYNAMIC_DRAW);

          // Draw
          glBindVertexArray(array_vert);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

          // Unbind vertex array object
          glBindVertexArray(0);

        }
      }
    }

    // Draw bombs
    for(unsigned int y = 0; y < game_state.getBoardHeight(); y++) {
      for(unsigned int x = 0; x < game_state.getBoardWidth(); x++) {
        if(game_state.getBombExist(y, x)) {

          // Vertex positions for hitbox (quadrant order)
          generateVertices(vertices_pos, game_state.getBomb(y, x)->getHitbox(),
          game_state.getBoardHeight(), game_state.getBoardWidth());

          // Vertex colors
          // Red
          for(unsigned int i = 0; i < 16; i += 4) {
            vertices_color[i] = 1;
          }
          // Green
          for(unsigned int i = 1; i < 16; i += 4) {
            vertices_color[i] = 0;
          }
          // Blue
          for(unsigned int i = 2; i < 16; i += 4) {
            vertices_color[i] = 1;
          }
          // Alpha
          for(unsigned int i = 3; i < 16; i += 4) {
            vertices_color[i] = 1;
          }

          // Bind vertex array object
          glBindVertexArray(array_vert);

          // Bind buffer for vertex position
          glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
          // Copy vertex position data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices_pos,
          GL_DYNAMIC_DRAW);

          // Bind buffer for vertex color
          glBindBuffer(GL_ARRAY_BUFFER, buf_vert_color);
          // Copy vertex position data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices_color,
          GL_DYNAMIC_DRAW);

          // Draw
          glBindVertexArray(array_vert);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

          // Unbind vertex array object
          glBindVertexArray(0);

        }
      }
    }

    // Draw explosions
    for(unsigned int y = 0; y < game_state.getBoardHeight(); y++) {
      for(unsigned int x = 0; x < game_state.getBoardWidth(); x++) {
        if(game_state.getExplosionExist(y, x)) {

          // Vertex positions for hitbox (quadrant order)
          generateVertices(vertices_pos,
          game_state.getExplosion(y, x)->getHitbox(),
          game_state.getBoardHeight(), game_state.getBoardWidth());

          // Vertex colors
          // Red
          for(unsigned int i = 0; i < 16; i += 4) {
            vertices_color[i] = 1;
          }
          // Green
          for(unsigned int i = 1; i < 16; i += 4) {
            vertices_color[i] = 0;
          }
          // Blue
          for(unsigned int i = 2; i < 16; i += 4) {
            vertices_color[i] = 0;
          }
          // Alpha
          for(unsigned int i = 3; i < 16; i += 4) {
            vertices_color[i] = 1;
          }

          // Bind vertex array object
          glBindVertexArray(array_vert);

          // Bind buffer for vertex position
          glBindBuffer(GL_ARRAY_BUFFER, buf_vert_pos);
          // Copy vertex position data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices_pos,
          GL_DYNAMIC_DRAW);

          // Bind buffer for vertex color
          glBindBuffer(GL_ARRAY_BUFFER, buf_vert_color);
          // Copy vertex position data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices_color,
          GL_DYNAMIC_DRAW);

          // Draw
          glBindVertexArray(array_vert);
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
  glDeleteVertexArrays(1, &array_vert);
  glDeleteBuffers(1, &buf_vert_pos);
  glDeleteBuffers(1, &buf_vert_color);
  glDeleteBuffers(1, &buf_elem);

  // Free previously allocated memory
  delete[] vertices_pos;
  delete[] vertices_color;
  delete[] indices_square;

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

  buffer[0] = translateLocation(center[1] + radius, board_width, false);
  buffer[1] = translateLocation(center[0] + radius, board_width, true);
  buffer[3] = translateLocation(center[1] - radius, board_width, false);
  buffer[4] = translateLocation(center[0] + radius, board_width, true);
  buffer[6] = translateLocation(center[1] - radius, board_width, false);
  buffer[7] = translateLocation(center[0] - radius, board_width, true);
  buffer[9] = translateLocation(center[1] + radius, board_width, false);
  buffer[10] = translateLocation(center[0] - radius, board_width, true);

}
