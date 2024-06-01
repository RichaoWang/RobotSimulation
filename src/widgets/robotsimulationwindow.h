#include <QStackedLayout>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QApplication>
#include <FramelessHelper/Core/framelessmanager.h>
#include <FramelessHelper/Widgets/framelesswidgetshelper.h>
#include <FramelessHelper/Widgets/standardsystembutton.h>
#include <FramelessHelper/Widgets/standardtitlebar.h>

#include "components/fluwidgets/FluFrameLessWidget.h"
#include "components/fluwidgets/FluVNavigationView.h"
#include "components/fluwidgets/FluVNavigationIconTextItem.h"
#include "components/fluwidgets/FluStackedLayout.h"
#include "components/fluwidgets/FluVNavigationSettingsItem.h"
#include "components/fluwidgets/FluMessageBox.h"
#include "components/fluwidgets/FluMessageBox.h"

#include "widgets/homewidget.h"
#include "widgets/codeeditwidget.h"
#include "widgets/settingwidget.h"


class RobotSimulationWindow : public FluFrameLessWidget {
Q_OBJECT
public:
    RobotSimulationWindow(QWidget *parent = nullptr);

    void makeHomeNavItem();

    void makeCodeEditNavItem();

    void makeSettingsNavItem();

//    void resizeEvent(QResizeEvent *event);

    void closeEvent(QCloseEvent *event);

    void moveToCenter();

    void paintEvent(QPaintEvent *event);

public slots:

    void onThemeChanged();

protected:
    FluVNavigationView *m_navView;
    FluStackedLayout *m_sLayout;
};
