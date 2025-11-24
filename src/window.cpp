#include <window.hpp>

Window::Window(QWidget *parent) : QWidget(parent) {
    setBaseSize(800, 600);
}

void Window::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::white);
    painter.drawLine(0, 0, size().width(), size().height());
}