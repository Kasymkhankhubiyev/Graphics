#pragma once

#include <memory>
#include <unordered_set>

#include <QKeyEvent>
#include <QVector3D>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

#include "GeometryEngine.h"

class InputController {
public:
  void keyPressEvent(QKeyEvent *event);

  unsigned int vertexFactor = 1U;

  enum Shaders { Phong = 0, Gouraud = 1 };

  unsigned int currentShader = Phong;
  bool swapKeyFlag = false;

private:
  std::unordered_set<int> pressedKeys;
  int lastPressedKey = 0x00;

};
