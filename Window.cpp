#include "Window.h"
#include <QtWidgets>


Window::Window() {

}


int Window::getFactorVal() {
    return sliderForFactor->value();
}

int Window::getSunValX() {
    return sliderForSunX->value();
}

Window::Window(double min, double max, double step) {


    sliderForFactor = new QSlider(Qt::Horizontal);
    sliderForSunX = new QSlider(Qt::Vertical);
    sliderForSunY = new QSlider(Qt::Vertical);
    sliderForSunZ = new QSlider (Qt::Vertical);

    sliderForSpotX = new QSlider(Qt::Vertical);
    sliderForSpotY = new QSlider(Qt::Vertical);
    sliderForSpotZ = new QSlider (Qt::Vertical);

    Spinframe = new QSpinBox();

    sliderForFactor->setRange(min, max);
    sliderForFactor->setSingleStep(step);
    sliderForFactor->setTickPosition(QSlider::TicksRight);
    sliderForFactor->setValue(min);

    sliderForSunX->setRange(-10, 20);
    sliderForSunX->setSingleStep(1);
    sliderForSunX->setTickPosition(QSlider::TicksRight);
    sliderForSunX->setValue(0);

    sliderForSunY->setRange(-10, 20);
    sliderForSunY->setSingleStep(1);
    sliderForSunY->setTickPosition(QSlider::TicksRight);
    sliderForSunY->setValue(0);

    sliderForSunZ->setRange(-10, 20);
    sliderForSunZ->setSingleStep(1);
    sliderForSunZ->setTickPosition(QSlider::TicksRight);
    sliderForSunZ->setValue(1);

    sliderForSpotX->setRange(-10, 20);
    sliderForSpotX->setSingleStep(5);
    sliderForSpotX->setTickPosition(QSlider::TicksRight);
    sliderForSpotX->setValue(0);

    sliderForSpotY->setRange(-10, 20);
    sliderForSpotY->setSingleStep(5);
    sliderForSpotY->setTickPosition(QSlider::TicksRight);
    sliderForSpotY->setValue(0);

    sliderForSpotZ->setRange(-10, 20);
    sliderForSpotZ->setSingleStep(5);
    sliderForSpotZ->setTickPosition(QSlider::TicksRight);
    sliderForSpotZ->setValue(1);

    PointLightButton = new QPushButton(tr("PointLight"));
    DirectionalLightButton = new QPushButton(tr("DirectionalLight"));
    SpotLightButton = new QPushButton(tr("SpotlightButton"));


    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(sliderForFactor);
    layout->addWidget(sliderForSunX);
    layout->addWidget(sliderForSunY);
    layout->addWidget(sliderForSunZ);

    layout->addWidget(sliderForSpotX);
    layout->addWidget(sliderForSpotY);
    layout->addWidget(sliderForSpotZ);

    layout->addWidget(PointLightButton);
    layout->addWidget(DirectionalLightButton);
    layout->addWidget(SpotLightButton);
    setLayout(layout);


    setWindowTitle(tr("Sliders"));

}

float Window::pointLight() {
    tapOnButton(PointLightButton, isPoint);
    if (isPoint) {
        return 1.0f;
    } else {
        return 0.0f;
    }
}

float Window::dirLight() {
    tapOnButton(DirectionalLightButton, isDir);
    if (isDir) {
        return 1.0f;
    } else {
        return 0.0f;
    }
}

float Window::spotLight() {
    tapOnButton(SpotLightButton, isSpot);
    if (isSpot) {
        return 1.0f;
    } else {
        return 0.0f;
    }
}

void Window::tapOnButton(QPushButton *button, bool& type) {

    if (button->isDown()) {
        if (!type)
            type = true;
        else
            type = false;
    }
}

int Window::getSunValY() {
    return sliderForSunY->value();
}

int Window::getSunValZ() {
    return sliderForSunZ->value();
}

int Window::getSpotValX() {
    return sliderForSpotX->value();
}

int Window::getSpotValY() {
    return sliderForSpotY->value();
}

int Window::getSpotValZ() {
    return sliderForSpotZ->value();
}
