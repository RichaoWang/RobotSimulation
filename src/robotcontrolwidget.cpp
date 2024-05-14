//
// Created by JerryWang on 24-5-13.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RobotControlWidget.h" resolved

#include "robotcontrolwidget.h"
#include "../froms/ui_robotcontrolwidget.h"
#include <QDebug>

RobotControlWidget::RobotControlWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::RobotControlWidget) {
    ui->setupUi(this);
    initWindow();
}

RobotControlWidget::~RobotControlWidget() {
    delete ui;
}

void RobotControlWidget::initWindow() {
    auto jSlider = this->findChildren<QSlider *>();
    for (int i = 0; i < jSlider.size(); ++i) {
        QSlider *slider = jSlider.at(i);
        slider->setMinimum(-180);
        slider->setMaximum(180);
        slider->setTickInterval(1);
        slider->setValue(0);
        connect(slider, &QSlider::valueChanged, this, &RobotControlWidget::slotUpdateJVarsValue,
                Qt::ConnectionType::QueuedConnection);

    }
    auto checkBoxs = this->findChildren<QCheckBox *>();
    for (int i = 0; i < checkBoxs.size(); ++i) {
        QCheckBox *checkBox = checkBoxs.at(i);

        connect(checkBox, &QCheckBox::toggled, this, &RobotControlWidget::slotUpdateCheckOpt,
                Qt::ConnectionType::QueuedConnection);

    }
    // default check opt
    ui->gridCheckBox->setChecked(true);
    ui->baseCoordCheckBox->setChecked(true);
}

void RobotControlWidget::slotUpdateJVarsValue(int var) {
    QSlider *senderSlider = (QSlider *) sender();
    QString sliderName = senderSlider->objectName();
    QString idx = sliderName.at(1);
    auto label = this->findChildren<QLabel *>("j" + idx + "Label");
    label.at(0)->setText(QString::number(var));
    emit sigJoinValueChanged(idx.toInt(), var);
}

void RobotControlWidget::slotUpdateCheckOpt(bool arg) {
    QCheckBox *cb = (QCheckBox *) sender();
    QString cbName = cb->objectName();
    auto key = cbName.split("CheckBox").at(0);
    emit sigCheckOptChanged(key, arg);
}
