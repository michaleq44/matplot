#pragma once
#include <main.hpp>

#include <QWidget>
#include <QPushButton>
#include <QPainter>

#include <iostream>

class Window final : public QWidget{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent* event) override;
private:

};