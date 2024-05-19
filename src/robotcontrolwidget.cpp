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
    validator = new QDoubleValidator(this);
    initWindow();
    robotKinematics = std::make_unique<RobotKinematics>();
}

RobotControlWidget::~RobotControlWidget() {
    delete ui;
}

void RobotControlWidget::initWindow() {

    auto lineEdits = this->findChildren<QLineEdit *>();
    for (int i = 0; i < lineEdits.size(); ++i) {
        QLineEdit *le = lineEdits.at(i);
        le->setValidator(validator);
    }

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

    connect(ui->sendPushButton, &QPushButton::clicked, this, &RobotControlWidget::slotRobotIKSolution);


    connect(ui->zeroPosePushButton, &QPushButton::clicked, [=]() {
        ui->j1Slider->setValue(0);
        ui->j2Slider->setValue(0);
        ui->j3Slider->setValue(0);
        ui->j4Slider->setValue(0);
        ui->j5Slider->setValue(0);
        ui->j6Slider->setValue(0);
    });

    connect(ui->initPosePushButton, &QPushButton::clicked, [=]() {
        ui->j1Slider->setValue(45);
        ui->j2Slider->setValue(-90);
        ui->j3Slider->setValue(90);
        ui->j4Slider->setValue(0);
        ui->j5Slider->setValue(90);
        ui->j6Slider->setValue(0);
    });

    // default check opt
    ui->gridCheckBox->setChecked(true);
    ui->baseCoordCheckBox->setChecked(true);
    ui->endCheckBox->setChecked(true);
    ui->initPosePushButton->clicked(true);
}

void RobotControlWidget::slotUpdateJVarsValue(int var) {
    QSlider *senderSlider = (QSlider *) sender();
    QString sliderName = senderSlider->objectName();
    QString idx = sliderName.at(1);
    auto label = this->findChildren<QLabel *>("j" + idx + "Label");
    label.at(0)->setText(QString::number(var));

    auto v1 = ui->j1Slider->value() * (M_PI / 180.0);
    auto v2 = ui->j2Slider->value() * (M_PI / 180.0);
    auto v3 = ui->j3Slider->value() * (M_PI / 180.0);
    auto v4 = ui->j4Slider->value() * (M_PI / 180.0);
    auto v5 = ui->j5Slider->value() * (M_PI / 180.0);
    auto v6 = ui->j6Slider->value() * (M_PI / 180.0);
    double q[6] = {v1, v2, v3, v4, v5, v6};

    auto xyzrxryrz = robotKinematics->forwardSolution(q);
    double x = xyzrxryrz[0];
    double y = xyzrxryrz[1];
    double z = xyzrxryrz[2];
    double rx = xyzrxryrz[3];
    double ry = xyzrxryrz[4];
    double rz = xyzrxryrz[5];
    ui->xLineEdit->setText(QString::number(x * 1000));
    ui->yLineEdit->setText(QString::number(y * 1000));
    ui->zLineEdit->setText(QString::number(z * 1000));

    ui->rollLineEdit->setText(QString::number(rx * 180 / M_PI));
    ui->pitchLineEdit->setText(QString::number(ry * 180 / M_PI));
    ui->yawLineEdit->setText(QString::number(rz * 180 / M_PI));

    emit sigJoinValueChanged(idx.toInt(), var);
}

void RobotControlWidget::slotUpdateCheckOpt(bool arg) {
    QCheckBox *cb = (QCheckBox *) sender();
    QString cbName = cb->objectName();
    auto key = cbName.split("CheckBox").at(0);
    emit sigCheckOptChanged(key, arg);
}

void RobotControlWidget::slotRobotIKSolution(bool arg) {
//    qDebug() << "slotRobotIKSolution";
    double x = ui->xLineEdit->text().toDouble();
    double y = ui->yLineEdit->text().toDouble();
    double z = ui->zLineEdit->text().toDouble();
    double rx = ui->rollLineEdit->text().toDouble();
    double ry = ui->pitchLineEdit->text().toDouble();
    double rz = ui->yawLineEdit->text().toDouble();
    double xyzrpy[6]{x / 1000, y / 1000, z / 1000, rx * M_PI / 180, ry * M_PI / 180, rz * M_PI / 180};
    auto res = robotKinematics->inverseSolutionFromEulerAngle(xyzrpy);
    if (res.size() == 0) {
        QMessageBox::warning(this, "IK Solutions Fail", "IK Solutions List is Empty!");
        return;
    }
    // todo test
    qDebug() << res[0];
    emit sigUpdateRobotPose(res[0]);
}

void RobotControlWidget::slotUpdateSlider(QVector<float> var) {
    ui->j1Slider->setValue(var[1]);
    ui->j2Slider->setValue(-var[2]);
    ui->j3Slider->setValue(-var[3]);
    ui->j4Slider->setValue(-var[4]);
    ui->j5Slider->setValue(var[5]);
    ui->j6Slider->setValue(var[6]);
}
