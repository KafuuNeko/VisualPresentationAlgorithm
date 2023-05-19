#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

const float PI = 3.14159265f;

struct Node {
  float x, y;
  int index;
};

struct Edge {
  int node1, node2;
  float weight;
};

std::vector<Node> nodes;
std::vector<Edge> edges;

void drawNode(const Node &node) {
  glBegin(GL_LINE_LOOP);
  // 以线循环模式开始绘制图形
  // 循环 100 次，每次绘制一个点
  for (int i = 0; i < 100; i++) {
    // 计算当前点的极角
    float theta = 2.0f * PI * i / 100;
    // 计算当前点相对于圆心的 x 坐标偏移量
    float dx = 0.1 * cosf(theta);
    // 计算当前点相对于圆心的 y 坐标偏移量
    float dy = 0.1 * sinf(theta);
    // 在当前点的位置画一个点
    glVertex2f(node.x + dx, node.y + dy);
  }
  // 结束绘制图形
  glEnd();

  // 在节点内部绘制数字
  // 设置绘制文本的位置
  glRasterPos2f(node.x, node.y);
  // 将节点的索引转换为字符串
  std::string s = std::to_string(node.index);
  // 设置绘制文本的字体
  void *font = (void *)GLUT_BITMAP_9_BY_15;
  // 遍历字符串的每一个字符
  for (std::string::iterator i = s.begin(); i != s.end(); ++i) {
    glutBitmapCharacter(font, *i); // 在当前位置绘制该字符
  }
}

void drawEdge(const Edge &edge) {
  // 获取边的起始节点
  const Node &startNode = nodes[edge.node1];
  // 获取边的终止节点
  const Node &endNode = nodes[edge.node2];

  // 计算绘制边所需的参数
  float dx = endNode.x - startNode.x;
  float dy = endNode.y - startNode.y;
  float length = sqrt(dx * dx + dy * dy);
  float cosTheta = dx / length;
  float sinTheta = dy / length;
  float radius = 0.1;
  float x1 = startNode.x + cosTheta * radius;
  float y1 = startNode.y + sinTheta * radius;
  float x2 = endNode.x - cosTheta * radius;
  float y2 = endNode.y - sinTheta * radius;

  // 绘制边
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();

  // 在边的中点绘制边的权重数字
  glRasterPos2f((startNode.x + endNode.x) / 2, (startNode.y + endNode.y) / 2);
  std::string s = std::to_string(edge.weight);
  void *font = (void *)GLUT_BITMAP_9_BY_15;
  for (std::string::iterator i = s.begin(); i != s.end() && i != s.begin() + 4;
       ++i) {
    char c = *i;
    glutBitmapCharacter(font, c);
  }

  // 在边的末端绘制箭头
  float arrowLength = 0.05f;
  float arrowDegrees = 30.0f;
  float angle = atan2(endNode.y - startNode.y, endNode.x - startNode.x);
  float x3 = x2 - arrowLength * cos(angle - arrowDegrees * PI / 180);
  float y3 = y2 - arrowLength * sin(angle - arrowDegrees * PI / 180);
  float x4 = x2 - arrowLength * cos(angle + arrowDegrees * PI / 180);
  float y4 = y2 - arrowLength * sin(angle + arrowDegrees * PI / 180);
  glBegin(GL_TRIANGLES);
  glVertex2f(x2, y2);
  glVertex2f(x3, y3);
  glVertex2f(x4, y4);
  glEnd();
}

void display(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0f, 1.0f, 1.0f);
  for (const Node &node : nodes) {
    drawNode(node);
  }
  for (const Edge &edge : edges) {
    drawEdge(edge);
  }
  glfwSwapBuffers(window);
}

int main(int argc, char **argv) {
  // initialize nodes and edges
  Node node1 = {0.0f, 0.0f, 1};
  Node node2 = {0.5f, 0.5f, 2};
  Node node3 = {-0.5f, 0.5f, 3};

  Edge edge1 = {0, 1, 1.0f};
  Edge edge2 = {0, 2, 2.0f};
  edges.push_back(edge1);
  edges.push_back(edge2);
  nodes.push_back(node1);
  nodes.push_back(node2);
  nodes.push_back(node3);

  if (!glfwInit()) {
    return -1;
  }

  glutInit(&argc, argv);

  GLFWwindow *window =
      glfwCreateWindow(500, 500, "Directed Weighted Graph", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

  while (!glfwWindowShouldClose(window)) {
    display(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
