#ifndef ROBOTSIMULATION_HOMEWIDGET_H
#define ROBOTSIMULATION_HOMEWIDGET_H

#include <QLabel>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include "components/fluwidgets/FluWidget.h"
#include "components/fluwidgets/utils/FluUtils.h"
#include "components/fluwidgets/FluVScrollView.h"
#include "components/fluwidgets/FluSettingsSelectBox.h"
#include "components/fluwidgets/FluSettingsLabelBox.h"
#include "components/fluwidgets/FluIconButton.h"
#include "components/fluwidgets/FluPushButton.h"
#include "ddr6robotwidget.h"
#include "tools/robotkinematics.h"
#include "osgwidget.h"
#include <QSettings>
#include <QMutex>

class HomeWidget : public FluWidget {
Q_OBJECT
public:
    explicit HomeWidget(QWidget *parent = nullptr);

    ~HomeWidget();

    void paintEvent(QPaintEvent *event) override;

    QWidget *makePosDisplayWidget();

    QWidget *makeCheckOptionWidget();

    QWidget *makeRobotControlWidget();

    QWidget *makeObservationWidget();

    QWidget *makePointsWidget();

    void connectSignsSlots();

public slots:

    void onThemeChanged() override;

    void slotUpdateCheckOpt(bool arg);

    void slotUpdateJVarsValue(int var);

    void slotUpdateStepMove();


signals:

    void sigCheckOptChanged(QString, bool);

    void sigJoinVarChanged(int, float);

    void sigKinCalc();

    void sigJointMoveAnimation(std::vector<double>);

    void sigJointMoveAnimations(std::vector<std::vector<double>>);

    void sigOsgDrawPosText(QString, QString, QString, QString, QString, QString);


protected:
    QHBoxLayout *m_mainLayout;

private:
    QWidget *checkboxesWidget;
    QWidget *robotControlWidget;
    FluPushButton *pubBtn;
    FluPushButton *allIKSolBtn;


    OsgWidget *osgWidget;
    std::unique_ptr<RobotKinematics> robotKinematics;

    std::pair<std::string, std::vector<float>> m_points;
    QSettings *m_pointSetting;

    std::vector<std::string> mJointNameVec = {
            "shoulder",
            "upperarm",
            "forearm",
            "wrist1",
            "wrist2",
            "wrist3"
    };

    int mCount;

    bool mAniStart = false;

    QMutex _m;

    std::vector<double> mJointV = {0, 0, 0, 0, 0, 0};

//    std::vector<std::pair<QString, std::vector<double>>> mPointMap;
    std::map<QString, std::vector<double>> mPointMap;
};

#endif //ROBOTSIMULATION_HOMEWIDGET_H