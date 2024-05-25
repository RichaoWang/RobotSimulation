#ifndef ROBOTSIMULATION_HOMEWIDGET_H
#define ROBOTSIMULATION_HOMEWIDGET_H

#include <QLabel>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include "components/FluWidget.h"
#include "components/utils/FluUtils.h"
#include "components/FluVScrollView.h"
#include "components/FluSettingsSelectBox.h"
#include "components/FluSettingsLabelBox.h"
#include "components/FluIconButton.h"
#include "ddr6robotwidget.h"
#include "tools/robotkinematics.h"

class HomeWidget : public FluWidget {
Q_OBJECT
public:
    explicit HomeWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

    QWidget *makePosDisplayWidget();

    QWidget *makeCheckOptionWidget();

    QWidget *makeRobotControlWidget();

    QWidget *makePointsWidget();

    void connectSignsSlots();

public slots:

    void onThemeChanged() override;

    void slotUpdateCheckOpt(bool arg);

    void slotUpdateJVarsValue(int var);

signals:

    void sigCheckOptChanged(QString, bool);

    void sigJoinVarChanged(int, int);

protected:
    QHBoxLayout *m_mainLayout;

//    QVBoxLayout *m_vDisPlayLayout;

//    QLabel *m_titleLabel;
//    FluVScrollView *m_vScrollView;
private:
    QWidget *checkboxesWidget;
    QWidget *robotControlWidget;
    DDR6RobotWidget *ddr6widget;
    std::unique_ptr<RobotKinematics> robotKinematics;
};

#endif //ROBOTSIMULATION_HOMEWIDGET_H