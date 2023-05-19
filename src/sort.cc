#include "CLI11.hpp"
#include "delay.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cinttypes>
#include <random>
#include <vector>

static bool kSpacePressed = false;
static bool kAuto = false;
static int32_t kSpeed = false;
static std::vector<float> kArray;

void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    kSpacePressed = true;
  }
}

void WaitKey(GLFWwindow *window) {
  while (!kSpacePressed && !glfwWindowShouldClose(window)) {
    glfwWaitEvents();
  }
  kSpacePressed = false;
}

void Wait(GLFWwindow *window) {
  if (kAuto) {
    delay(0.01 * kSpeed, [=]() { return !glfwWindowShouldClose(window); });
  } else {
    WaitKey(window);
  }
  if (glfwWindowShouldClose(window)) {
    exit(0);
  }
}

void DrawArray(GLFWwindow *window, size_t a, size_t b, size_t c) {
  glClear(GL_COLOR_BUFFER_BIT);

  float width = 2.0f / static_cast<float>(kArray.size());
  // 定义间隔宽度，可以根据需要进行调整
  float gap = width * 0.0f;

  for (int i = 0; i < kArray.size(); ++i) {
    glColor3f(1.0, 1.0, 1.0);

    if (a != -1 && i == a) {
      glColor3f(0.0, 1.0, 0.0);
    }

    if (b != -1 && i == b) {
      glColor3f(0.0, 0.0, 1.0);
    }
    if (c != -1 && i == c) {
      glColor3f(1.0, 0.0, 0.0);
    }

    auto x = static_cast<float>(i) * (width + gap) - 1;
    auto height = static_cast<float>(
        kArray[i] * 2.0 / static_cast<float>(kArray.size()) + 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(x, -1.0);
    glVertex2f(x, height - 1);
    glVertex2f(x + width, height - 1);
    glVertex2f(x + width, -1.0);
    glEnd();
  }

  glfwSwapBuffers(window);
  glfwPollEvents();
  Wait(window);
}

void InsertionSort(GLFWwindow *window) {
  for (int i = 1; i < kArray.size(); ++i) {
    for (int j = i; j > 0; --j) {
      // 比较相邻元素，如果arr[j-1]比arr[j]大，则交换它们
      if (kArray[j - 1] > kArray[j]) {
        DrawArray(window, j, j - 1, -1);
        std::swap(kArray[j - 1], kArray[j]);
        DrawArray(window, j - 1, j, -1);
      } else {
        DrawArray(window, j - 1, j, -1);
        break;
      }
    }
  }

  // 最后一次绘制数组状态
  DrawArray(window, -1, -1, -1);
}

void SelectionSort(GLFWwindow *window) {
  int i, j, minIndex;
  float temp;

  // 遍历数组元素
  for (i = 0; i + 1 < kArray.size(); i++) {
    // 找到未排序部分的最小元素下标
    minIndex = i;

    for (j = i + 1; j < kArray.size(); j++) {
      DrawArray(window, j, minIndex, i);
      if (kArray[j] < kArray[minIndex]) {
        minIndex = j;
        DrawArray(window, j, minIndex, i);
      }
    }

    // 将最小元素与当前位置交换
    temp = kArray[i];
    kArray[i] = kArray[minIndex];
    kArray[minIndex] = temp;
    DrawArray(window, j, i, minIndex);
  }

  // 最后一次绘制数组状态
  DrawArray(window, -1, -1, -1);
}

void BubbleSort(GLFWwindow *window) {
  size_t i, j;
  float temp;
  bool swapped; // 标志位，表示是否进行了交换

  for (i = 0; i + 1 < kArray.size(); i++) {
    swapped = false; // 每轮开始时将标志位设置为0

    for (j = 0; j + i + 1 < kArray.size(); j++) {
      DrawArray(window, j, j + 1, -1);
      // 如果相邻元素逆序，则交换它们的位置
      if (kArray[j] > kArray[j + 1]) {
        temp = kArray[j];
        kArray[j] = kArray[j + 1];
        kArray[j + 1] = temp;
        swapped = true; // 设置标志位为1，表示进行了交换
        DrawArray(window, j + 1, j, -1);
      }
    }

    // 如果没有进行交换，表示数组已经有序，提前退出循环
    if (swapped == 0) {
      break;
    }
  }

  DrawArray(window, -1, -1, -1);
}

void RandomArray(size_t count) {
  kArray.resize(count);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0,
                                      static_cast<int32_t>(kArray.size()) - 1);

  for (float &i : kArray) {
    i = static_cast<float>(dis(gen));
  }
}

int main(int argc, char *argv[]) {
  CLI::App app("sort");

  int32_t n, algotithm;
  app.add_option("-n,--size", n, "Number of presentation elements")->required();
  app.add_option("--auto", kAuto, "Whether to execute automatically");
  app.add_option("--speed", kSpeed, "Automatic execution speed(1-x)");
  app.add_option("--algorithm", algotithm, "Select sort algorithm")
      ->default_val(0);

  CLI11_PARSE(app, argc, argv);

  if (n < 1) {
    std::cout << "There must be more than two presentation elements.";
    return -1;
  }

  if (kAuto && kSpeed == 0) {
    std::cout << "The execution speed must be set.";
    return -1;
  }

  RandomArray(n);

  if (!glfwInit()) {
    exit(-1);
  }

  GLFWwindow *window =
      glfwCreateWindow(640, 480, "Sort Visualization", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    exit(-1);
  }

  glfwSetKeyCallback(window, KeyCallback);

  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window)) {
    switch (algotithm) {
    case 0:
      InsertionSort(window);
      break;
    case 1:
      BubbleSort(window);
      break;
    case 2:
      SelectionSort(window);
      break;
    }

    WaitKey(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
