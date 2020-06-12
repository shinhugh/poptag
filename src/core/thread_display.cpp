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

static void printState(Game&);

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



  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_src, 0);
  glCompileShader(vertex_shader);

  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_src, 0);
  glCompileShader(fragment_shader);

  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };

  unsigned int vertex_buffer_obj;
  glGenBuffers(1, &vertex_buffer_obj);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
  (void *) 0);
  glEnableVertexAttribArray(0);



  // While game is ongoing
  while (!(game.isExit()) && !glfwWindowShouldClose(window))
  {

    // Print state - THIS IS A TEMPORARY REPLACEMENT FOR GRAPHICS
    // printState(game);

    // Read game state
    DataPacket packet = game.readState();
    StateData *state_data = static_cast<StateData *>(packet.getData());

    // Update location of square
    vertices[0] = -1 + ((state_data->character_x + 0.5) / 5);
    vertices[1] = 1 - ((state_data->character_y - 0.5) / 5);
    vertices[3] = -1 + ((state_data->character_x + 0.5) / 5);
    vertices[4] = 1 - ((state_data->character_y + 0.5) / 5);
    vertices[6] = -1 + ((state_data->character_x - 0.5) / 5);
    vertices[7] = 1 - ((state_data->character_y - 0.5) / 5);
    vertices[9] = -1 + ((state_data->character_x - 0.5) / 5);
    vertices[10] = 1 - ((state_data->character_y + 0.5) / 5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // Get frame dimensions and specify viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Clear display
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw
    glUseProgram(shader_program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 1, 4);

    // Swap front/back buffers (to display new render)
    glfwSwapBuffers(window);

    // Handle input events in queue
    glfwPollEvents();

  }

  // If window close wasn't triggered by in-game exit, trigger in-game exit too
  game.exit();

  // Close window
  glfwDestroyWindow(window);

  // Free resources used by OpenGL
  glDeleteBuffers(1, &vertex_buffer_obj);
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

  // Escape
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    game_instance->exit();
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

static void printState(Game& game) {

  DataPacket packet = game.readState();
  StateData *state_data = static_cast<StateData *>(packet.getData());

  std::cerr << "Character: (" + std::to_string(state_data->character_y) + ", "
  + std::to_string(state_data->character_x) + ")\n";

}
