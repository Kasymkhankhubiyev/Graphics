#include "MainWindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QScreen>
#include <cmath>
#include <iostream>

void MainWindow::mousePressEvent(QMouseEvent *e) {
  // Save mouse press position
  mousePressPosition = QVector2D(e->localPos());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e) {
  // Mouse release position - mouse press position
  QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

  // Rotation axis is perpendicular to the mouse position difference
  // vector
  QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

  // Accelerate angular speed relative to the length of the mouse sweep
  qreal acc = diff.length() / 100.0;

  // Calculate new rotation axis as weighted sum
  rotationAxis = QVector3D(0.0, 1.0, 0.0).normalized(); //(rotationAxis * angularSpeed + n * acc).normalized();

  // Increase angular speed
  angularSpeed = 1.0;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  inputCtrl->keyPressEvent(event);
}

void MainWindow::wheelEvent(QWheelEvent *event) {
  if (event->angleDelta().y() > 0) {
    m_z += 0.25;
  } else if (event->angleDelta().y() < 0) {
    m_z -= 0.25;
  }
  update();
}

void MainWindow::timerEvent(QTimerEvent *) {
  // Decrease angular speed (friction)
  angularSpeed = 0.2;

  // Stop rotation when speed goes below threshold
  rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
  update();
}

void MainWindow::initializeGL() {
  initializeOpenGLFunctions();

  glClearColor(0.1f, 0.1f, 0.1f, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);

  initShaders(InputController::Phong);

  old =  std::chrono::system_clock::now();

  m_z = -5.0f; // - 2 * morphWindow.getFactorVal();
  inputCtrl = std::make_unique<InputController>();
//  m_objects.append(std::make_unique<GeometryEngine>());

  // Use QBasicTimer because its faster than QTimer
  timer.start(12, this);

  morphWindow.show();
}

void MainWindow::initShaders(unsigned int currentShader) {
  program.removeAllShaders();

  /* Init Phong shader */
  if (currentShader == InputController::Phong) {
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/Shaders/phong_vshader.vsh"))
      close();

    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/Shaders/phong_fshader.fsh"))
      close();
  }
  /* Init Gouraud shader */
  else if (currentShader == InputController::Gouraud) {
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/Shaders/gouraud_vshader.vsh"))
      close();

    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/Shaders/gouraud_fshader.fsh"))
      close();
  }

  // Link shader pipeline
  if (!program.link())
    close();

  // Bind shader pipeline for use
  if (!program.bind())
    close();
}

void MainWindow::resizeGL(int w, int h) {
  // Calculate aspect ratio
  qreal aspect = qreal(w) / qreal(h ? h : 1);

  // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
  const qreal zNear = 1.0, zFar = 50.0, fov = 60.0;

  // Reset projection
  projection.setToIdentity();

  // Set perspective projection
  projection.perspective(fov, aspect, zNear, zFar);
}

void MainWindow::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (inputCtrl->swapKeyFlag) {
    for (auto &m_object : m_objects) {
      m_object->initCubeGeometry(1.0f, inputCtrl->vertexFactor);
    }
    initShaders(inputCtrl->currentShader);
    inputCtrl->swapKeyFlag = false;
  }

  program.bind();

  QVector<std::shared_ptr<GeometryEngine>> m_objects;

  m_objects.append(std::make_unique<GeometryEngine>());

  float step = 2 * morphWindow.getFactorVal();
  int dev = morphWindow.getFactorVal();

  for (auto x = -step/dev; x <= step; x += step/dev) {
    for (auto y = -step/dev; y <= step; y += step/dev) {
      for (auto z = -step/dev; z <= step; z += step/dev) {
        m_objects.append(std::make_shared<GeometryEngine>());
        m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
      }
    }
  }

  // Calculate model view transformation
  QMatrix4x4 matrixView;
  matrixView.translate(0.0f, 0.0f, m_z);
  matrixView.rotate(5.0f * frame_ / screen()->refreshRate(), 0, 1, 0);

  for (int i=0; i++; translateVector.size()){
      m_objects[i]->translate(translateVector[i]);
  }

  program.setUniformValue("projection_matrix", projection);
  program.setUniformValue("view_matrix", matrixView);
  program.setUniformValue("t", static_cast<GLfloat>(frame_));

  program.setUniformValue("u_lightPosition", QVector3D(sin(morphWindow.getSunValX()*0.1)*20,
                                                       sin(morphWindow.getSunValY()*0.1)*20,
                                                       (morphWindow.getSunValZ()*0.1)));
  program.setUniformValue("u_SpotlightPosition", QVector3D(sin(morphWindow.getSpotValX()*0.1)*20,
                                                           sin(morphWindow.getSpotValY()*0.1),
                                                           (morphWindow.getSpotValZ()*0.1)));
  program.setUniformValue("u_lightPower", 1.0f);

  program.setUniformValue("u_PointLightParam", morphWindow.pointLight());
  program.setUniformValue("u_DirLightParam", morphWindow.dirLight());
  program.setUniformValue("u_SpotLightParam", morphWindow.spotLight());

  float distancePoint = std::sqrt(std::pow(morphWindow.getSunValX(), 2) + std::pow(morphWindow.getSunValY(), 2) +
                                  std::pow(morphWindow.getSunValZ(), 2));

  float distanceSpot = std::sqrt(std::pow(morphWindow.getSpotValX(), 2) + std::pow(morphWindow.getSpotValY(), 2) +
                                 std::pow(morphWindow.getSpotValZ(), 2));
  std::cout << distancePoint << std::endl;

  program.setUniformValue("u_distanceForPoint", distancePoint);
  program.setUniformValue("u_distanceForSpot", distanceSpot);

  // Draw cubes

  for (auto &m_object : m_objects) {
    m_object->drawCubeGeometry(&program, context()->functions());
  }

  program.release();

  auto null = std::chrono::system_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(null - old).count();
  old = std::chrono::system_clock::now();
  fps = (10e8/time);
  std::cout << "FPS:  " << std::round(fps) << "   cubes amount:   "<< m_objects.size() -1 << std::endl;

  ++frame_;
}

void MainWindow::changeMorphFactor(int MF){
    morphFactor = static_cast<int>(MF)/100.0;
}
