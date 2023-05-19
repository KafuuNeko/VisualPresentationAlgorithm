#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define N 10
#define M 10

void DrawGrid() {
  glBegin(GL_LINES);

  // 设置颜色为蓝色
  glColor3f(0.0f, 0.0f, 1.0f);
  // 设置线宽为2
  glLineWidth(2.0f);

  for (int i = 0; i <= N; i++) {
    glVertex2f((float)i / N * 2.0f - 1.0f, -1.0f);
    glVertex2f((float)i / N * 2.0f - 1.0f, 1.0f);
  }

  for (int i = 0; i <= M; i++) {
    glVertex2f(-1.0f, (float)i / M * 2.0f - 1.0f);
    glVertex2f(1.0f, (float)i / M * 2.0f - 1.0f);
  }

  glEnd();
}

void DrawPoint(float x, float y) {
  float pointX = (x / N) * 2.0f - 1.0f + (1.0f / N);
  float pointY = (y / M) * 2.0f - 1.0f + (1.0f / M);

  glPointSize(10.0f);
  glBegin(GL_POINTS);
  glVertex2f(pointX, pointY);
  glEnd();
}

int main() {
  if (!glfwInit()) {
    return -1;
  }

  GLFWwindow *window = glfwCreateWindow(640, 480, "Grid", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    return -1;
  }

  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 设置背景颜色为白色
    glClear(GL_COLOR_BUFFER_BIT);

    DrawGrid();
    DrawPoint(0, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
