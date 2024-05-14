//
// Created by JerryWang on 24-5-13.
//

#ifndef ROBOTSIMULATION_ROBOTCONTROLWIDGET_H
#define ROBOTSIMULATION_ROBOTCONTROLWIDGET_H

#include <QWidget>


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

signals:

    void sigJoinValueChanged(int, int);

    void sigCheckOptChanged(QString, bool);

private:
    Ui::RobotControlWidget *ui;
};


#endif //ROBOTSIMULATION_ROBOTCONTROLWIDGET_H
