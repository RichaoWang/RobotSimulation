//
// Created by JerryWang on 24-5-25.
//

#ifndef ROBOTSIMULATION_CONTROLWIDGET_H
#define ROBOTSIMULATION_CONTROLWIDGET_H

#include <QWidget>
#include <QDebug>

class ControlWidget : public QWidget {
Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = nullptr);

    ~ControlWidget() override {  } ;

private:
    void initWindow();
};


#endif //ROBOTSIMULATION_CONTROLWIDGET_H
