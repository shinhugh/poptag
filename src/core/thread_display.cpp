#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "thread_display.h"
#include "data_packet.h"
#include "event_data.h"
#include "state_data.h"

#define FRAMES_PER_SEC 5

// ------------------------------------------------------------

// Game instance being rendered; pointer is updated when thread is initialized
static Game* game_instance;

// ------------------------------------------------------------

static void error_callback(int, const char*);

static void key_callback(GLFWwindow *, int, int, int, int);

static void printState(Game&);

// ------------------------------------------------------------

void threadRoutine_Display(Game& game) {

  // Update game instance pointer
  game_instance = &game;

  GLuint program;

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
  GLFWwindow* window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
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

  // OpenGL code
  program = glCreateProgram();

  // While game is ongoing
  while (!(game.isExit()))
  {

    // Print state
    printState(game); // THIS IS A TEMPORARY REPLACEMENT FOR GRAPHICS

    // TODO: You have access to the game variable here (as an argument into this
    // function). Call 'game.readState()' to fetch the game state data, and
    // paint a representation of it using OpenGL here.

    // Get frame dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    // Swap front/back buffers (to display new render)
    glfwSwapBuffers(window);
    // Handle input events in queue
    glfwPollEvents();

  }

  // Close window
  glfwDestroyWindow(window);

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

  // Escape
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    game_instance->exit();
  }

  // W, pressed
  else if(key == GLFW_KEY_W && action == GLFW_PRESS) {
    // std::cerr << "Character is moving up.\n";
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
      // std::cerr << "Character is not moving.\n";
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
    // std::cerr << "Character is moving right.\n";
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
      // std::cerr << "Character is not moving.\n";
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
    // std::cerr << "Character is moving down.\n";
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
      // std::cerr << "Character is not moving.\n";
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
    // std::cerr << "Character is moving left.\n";
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
      // std::cerr << "Character is not moving.\n";
      current_direction = 0;
      DataPacket packet;
      EventData_MoveStop event_data;
      event_data.character_id = 0;
      packet.setType(moveStop);
      packet.setData(&event_data, sizeof(EventData_MoveStop));
      game_instance->queueEvent(packet);
    }
  }

}

// ------------------------------------------------------------

static void printState(Game& game) {

  DataPacket packet = game.readState();
  StateData *state_data = static_cast<StateData *>(packet.getData());

  std::cerr << "Character: (" + std::to_string(state_data->character_y) + ", "
  + std::to_string(state_data->character_x) + ")\n";

}
