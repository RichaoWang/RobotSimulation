//
// Created by JerryWang on 24-5-13.
//

#ifndef ROBOTSIMULATION_ROBOTCONTROLWIDGET_H
#define ROBOTSIMULATION_ROBOTCONTROLWIDGET_H

#include <QWidget>
#include <QDoubleValidator>
#include <QMessageBox>
#include "tools/robotkinematics.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RobotControlWidget; }
QT_END_NAMESPACE

class RobotControlWidget : public QWidget {
Q_OBJECT

public:
    explicit RobotControlWidget(QWidget *parent = nullptr);

    ~RobotControlWidget() override;

private:
    void initWindow();

public slots:

    void slotUpdateJVarsValue(int var);

    void slotUpdateCheckOpt(bool arg);

    void slotRobotIKSolution(bool arg);

    void slotUpdateSlider(QVector<float>);

signals:

    void sigJoinValueChanged(int, int);

    void sigCheckOptChanged(QString, bool);

    void sigUpdateRobotPose(std::vector<double>);



private:
    Ui::RobotControlWidget *ui;
    std::unique_ptr<RobotKinematics> robotKinematics;
    QDoubleValidator *validator;
};


#endif //ROBOTSIMULATION_ROBOTCONTROLWIDGET_H
