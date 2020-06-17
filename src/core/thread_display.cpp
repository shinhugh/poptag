#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "thread_display.h"
#include "data_packet.h"
#include "event_data.h"
#include "game_state.h"
#include "hitbox.h"
#include "character.h"

// ------------------------------------------------------------

// Game instance being rendered; pointer is updated when thread is initialized
static Game* game_instance;

// Vertex shader
const char *vertex_shader_src = "#version 330 core\n"
"layout (location = 0) in vec3 a_pos;\n"
"void main() {\n"
"  gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
"}\0";

// Fragment shader
const char *fragment_shader_src = "#version 330 core\n"
"out vec4 frag_color\n"
"void main() {\n"
"  frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

// ------------------------------------------------------------

static void error_callback(int, const char*);

static void key_callback(GLFWwindow *, int, int, int, int);

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
  glfwSetErrorCallback(error_callback);

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
  glfwSetKeyCallback(window, key_callback);

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
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_src, 0);
  glCompileShader(vertex_shader);

  // Fragment shader
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_src, 0);
  glCompileShader(fragment_shader);

  // Shader program
  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glUseProgram(shader_program);

  // Delete shaders; already linked to shader program
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // Vertex index ordering to draw a square (quadrant order)
  unsigned int square_indices[] = {0, 1, 2, 0, 2, 3};

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

    // Draw character
    for(unsigned int i = 0; i < game_state.getCharacterCount(); i++) {
      if(game_state.getCharacterAlive(i)) {

        // Vertices for hitbox (quadrant order)
        float *vertices = new float[12];
        generateVertices(vertices, game_state.getCharacter(i)->getHitbox(),
        game_state.getBoardHeight(), game_state.getBoardWidth());

        // Generate vertex array object
        unsigned int vertex_array_obj;
        glGenVertexArrays(1, &vertex_array_obj);
        // Bind newly generated vertex array object
        glBindVertexArray(vertex_array_obj);

        // Generate vertex buffer object
        unsigned int vertex_buffer_obj;
        glGenBuffers(1, &vertex_buffer_obj);
        // Bind newly generated buffer object
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
        // Copy vertex data into currently bound buffer
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices,
        GL_DYNAMIC_DRAW);

        // Generate element buffer object
        unsigned int element_buffer_obj;
        glGenBuffers(1, &element_buffer_obj);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
        square_indices, GL_DYNAMIC_DRAW);

        // Free memory
        delete[] vertices;

        // Specify structure for currently bound buffer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
        (void *) 0);
        glEnableVertexAttribArray(0);

        // Draw
        glBindVertexArray(vertex_array_obj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Delete vertex array and buffers
        glDeleteVertexArrays(1, &vertex_array_obj);
        glDeleteBuffers(1, &vertex_buffer_obj);
        glDeleteBuffers(1, &element_buffer_obj);

      }
    }

    // Draw board blocks
    for(unsigned int y = 0; y < game_state.getBoardHeight(); y++) {
      for(unsigned int x = 0; x < game_state.getBoardWidth(); x++) {
        if(game_state.getBlockExist(y, x)) {

          // Vertices for hitbox (quadrant order)
          float *vertices = new float[12];
          generateVertices(vertices, game_state.getBlock(y, x)->getHitbox(),
          game_state.getBoardHeight(), game_state.getBoardWidth());

          // Generate vertex array object
          unsigned int vertex_array_obj;
          glGenVertexArrays(1, &vertex_array_obj);
          // Bind newly generated vertex array object
          glBindVertexArray(vertex_array_obj);

          // Generate vertex buffer object
          unsigned int vertex_buffer_obj;
          glGenBuffers(1, &vertex_buffer_obj);
          // Bind newly generated buffer object
          glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
          // Copy vertex data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices,
          GL_DYNAMIC_DRAW);

          // Generate element buffer object
          unsigned int element_buffer_obj;
          glGenBuffers(1, &element_buffer_obj);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
          square_indices, GL_DYNAMIC_DRAW);

          // Free memory
          delete[] vertices;

          // Specify structure for currently bound buffer
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
          (void *) 0);
          glEnableVertexAttribArray(0);

          // Draw
          glBindVertexArray(vertex_array_obj);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

          // Delete vertex array and buffers
          glDeleteVertexArrays(1, &vertex_array_obj);
          glDeleteBuffers(1, &vertex_buffer_obj);
          glDeleteBuffers(1, &element_buffer_obj);

        }
      }
    }

    // Draw bombs
    for(unsigned int y = 0; y < game_state.getBoardHeight(); y++) {
      for(unsigned int x = 0; x < game_state.getBoardWidth(); x++) {
        if(game_state.getBombExist(y, x)) {

          // Vertices for hitbox (quadrant order)
          float *vertices = new float[12];
          generateVertices(vertices, game_state.getBomb(y, x)->getHitbox(),
          game_state.getBoardHeight(), game_state.getBoardWidth());

          // Generate vertex array object
          unsigned int vertex_array_obj;
          glGenVertexArrays(1, &vertex_array_obj);
          // Bind newly generated vertex array object
          glBindVertexArray(vertex_array_obj);

          // Generate vertex buffer object
          unsigned int vertex_buffer_obj;
          glGenBuffers(1, &vertex_buffer_obj);
          // Bind newly generated buffer object
          glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
          // Copy vertex data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices,
          GL_DYNAMIC_DRAW);

          // Generate element buffer object
          unsigned int element_buffer_obj;
          glGenBuffers(1, &element_buffer_obj);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
          square_indices, GL_DYNAMIC_DRAW);

          // Free memory
          delete[] vertices;

          // Specify structure for currently bound buffer
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
          (void *) 0);
          glEnableVertexAttribArray(0);

          // Draw
          glBindVertexArray(vertex_array_obj);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

          // Delete vertex array and buffers
          glDeleteVertexArrays(1, &vertex_array_obj);
          glDeleteBuffers(1, &vertex_buffer_obj);
          glDeleteBuffers(1, &element_buffer_obj);

        }
      }
    }

    // Draw explosions
    for(unsigned int y = 0; y < game_state.getBoardHeight(); y++) {
      for(unsigned int x = 0; x < game_state.getBoardWidth(); x++) {
        if(game_state.getExplosionExist(y, x)) {

          // Vertices for hitbox (quadrant order)
          float *vertices = new float[12];
          generateVertices(vertices, game_state.getExplosion(y, x)->getHitbox(),
          game_state.getBoardHeight(), game_state.getBoardWidth());

          // Generate vertex array object
          unsigned int vertex_array_obj;
          glGenVertexArrays(1, &vertex_array_obj);
          // Bind newly generated vertex array object
          glBindVertexArray(vertex_array_obj);

          // Generate vertex buffer object
          unsigned int vertex_buffer_obj;
          glGenBuffers(1, &vertex_buffer_obj);
          // Bind newly generated buffer object
          glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
          // Copy vertex data into currently bound buffer
          glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices,
          GL_DYNAMIC_DRAW);

          // Generate element buffer object
          unsigned int element_buffer_obj;
          glGenBuffers(1, &element_buffer_obj);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
          square_indices, GL_DYNAMIC_DRAW);

          // Free memory
          delete[] vertices;

          // Specify structure for currently bound buffer
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
          (void *) 0);
          glEnableVertexAttribArray(0);

          // Draw
          glBindVertexArray(vertex_array_obj);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

          // Delete vertex array and buffers
          glDeleteVertexArrays(1, &vertex_array_obj);
          glDeleteBuffers(1, &vertex_buffer_obj);
          glDeleteBuffers(1, &element_buffer_obj);

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

  // Free resources used by GLFW
  glfwTerminate();

  // Exit program
  return;

}

// ------------------------------------------------------------

static void error_callback(int error, const char* description)
{

  // Log error
  std::cerr << std::string("Error: ") + std::string(description)
  + std::string("\n");

}

// ------------------------------------------------------------

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
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
