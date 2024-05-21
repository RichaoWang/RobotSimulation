#pragma once

#include <QStackedLayout>
#include <QPainter>
#include <QStyleOption>

#include "../components/FluFrameLessWidget.h"
#include "../components/FluVNavigationView.h"
#include "../components/FluVNavigationIconTextItem.h"
#include "../components/FluStackedLayout.h"
#include "../components/FluVNavigationSettingsItem.h"
#include "../components/FluMessageBox.h"


class RobotSimulationWindow : public FluFrameLessWidget {
Q_OBJECT
public:
    RobotSimulationWindow(QWidget *parent = nullptr);

    void makeHomeNavItem();

    void makeCodeEditNavItem();

    void makeSettingsNavItem();

    void resizeEvent(QResizeEvent *event);

    void closeEvent(QCloseEvent *event);

//    void paintEvent(QPaintEvent *event) {
//        QStyleOption opt;
//        opt.initFrom(this);
//        QPainter painter(this);
//        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
//    }

public slots:

    void onThemeChanged();

protected:
    FluVNavigationView *m_navView;
    FluStackedLayout *m_sLayout;
};
