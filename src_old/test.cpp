// Test program

#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <GLFW/glfw3.h>

// ------------------------------------------------------------

#define BOARD_HEIGHT 2
#define BOARD_WIDTH 2

// ------------------------------------------------------------

class Board {

  private:
    bool **grid;
    unsigned int height, width;
    unsigned int location[2];

  public:

    Board(unsigned int height = BOARD_HEIGHT, unsigned int width = BOARD_WIDTH)
    : height(height), width(width) {
      this->grid = new bool *[this->height]();
      for(unsigned int row = 0; row < this->height; row++) {
        this->grid[row] = new bool[this->width]();
      }
      grid[0][0] = true;
      location[0] = 0;
      location[1] = 0;
    };

    Board(const Board& src) : height(src.height), width(src.width) {
      this->grid = new bool *[this->height]();
      for(unsigned int row = 0; row < this->height; row++) {
        this->grid[row] = new bool[this->width]();
        for(unsigned int col = 0; col < this->width; col++) {
          this->grid[row][col] = src.grid[row][col];
        }
      }
      this->location[0] = src.location[0];
      this->location[1] = src.location[1];
    }

    Board& operator=(const Board& src) {
      this->height = src.height;
      this->width = src.width;
      for(unsigned int row = 0; row < this->height; row++) {
        for(unsigned int col = 0; col < this->width; col++) {
          this->grid[row][col] = src.grid[row][col];
        }
      }
      this->location[0] = src.location[0];
      this->location[1] = src.location[1];
      return *this;
    }

    ~Board() {
      for(unsigned int row = 0; row < this->height; row++) {
        delete[] this->grid[row];
      }
      delete[] this->grid;
    }

    void moveUp() {
      this->grid[this->location[0]][this->location[1]] = false;
      this->location[0] = this->location[0] > 0 ? this->location[0] - 1 : 0;
      this->grid[this->location[0]][this->location[1]] = true;
    }

    void moveDown() {
      this->grid[this->location[0]][this->location[1]] = false;
      this->location[0] = this->location[0] < this->height - 1
      ? this->location[0] + 1 : this->height - 1;
      this->grid[this->location[0]][this->location[1]] = true;
    }

    void moveLeft() {
      this->grid[this->location[0]][this->location[1]] = false;
      this->location[1] = this->location[1] > 0 ? this->location[1] - 1 : 0;
      this->grid[this->location[0]][this->location[1]] = true;
    }

    void moveRight() {
      this->grid[this->location[0]][this->location[1]] = false;
      this->location[1] = this->location[1] < this->width - 1
      ? this->location[1] + 1 : this->width - 1;
      this->grid[this->location[0]][this->location[1]] = true;
    }

    const bool ** getGrid() const {
      return const_cast<const bool **>(this->grid);
    }

};

// ------------------------------------------------------------

static void printGrid(const bool ** grid, unsigned int height,
unsigned int width) {
  for(unsigned int row = 0; row < height; row++) {
    for(unsigned int col = 0; col < width; col++) {
      printf("%c", grid[row][col] ? 'x' : '-');
    }
    printf("\n");
  }
}

// ------------------------------------------------------------

static void handle_error(int error, const char* description) {
  fprintf(stderr, "Error: %s\n", description);
}

// ------------------------------------------------------------

static void handle_keypress(GLFWwindow* window, int key, int scancode,
int action, int mods) {
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

// ------------------------------------------------------------

int main() {

  Board board;
  const bool ** grid = static_cast<const bool **>(board.getGrid());

  GLFWwindow *window;

  // Handle error
  glfwSetErrorCallback(handle_error);

  // Initialize GLFW library
  if (!glfwInit()) {
    return -1;
  }

  // Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(480, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Handle keypress
  glfwSetKeyCallback(window, handle_keypress);

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Render here
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;

}

// ------------------------------------------------------------

/*

// Function declarations

// Thread tasks
void threadTask_A();
void threadTask_B(int n, int *val);

// ------------------------------------------------------------

// Main function
int main() {

  int vals[2];

  // std::thread thread_1(threadTask_A);
  // std::thread thread_2(threadTask_A);
  std::thread thread_3(threadTask_B, 2, &vals[0]);
  std::thread thread_4(threadTask_B, 4, &vals[1]);

  // thread_1.join();
  // thread_2.join();
  thread_3.join();
  thread_4.join();

  std::cout << "All threads complete." << std::endl;

  std::cout << "vals[0]: " << vals[0] << "\n";
  std::cout << "vals[1]: " << vals[1] << "\n";

  return 0;

}

// ------------------------------------------------------------

void threadTask_A() {
  std::cout << "Starting task A.\n";
  std::cout << "Hello world!\n";
  std::cout << "Task A complete.\n";
}

void threadTask_B(int n, int* val) {
  int output;
  std::cout << "Starting task B.\n";
  output = n * 2;
  std::cout << "Task B complete.\n";
  *val = output;
}

*/
