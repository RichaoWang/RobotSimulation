
#include <FramelessHelper/Core/framelessmanager.h>
#include <FramelessHelper/Widgets/framelesswidgetshelper.h>
#include <FramelessHelper/Widgets/standardsystembutton.h>
#include <FramelessHelper/Widgets/standardtitlebar.h>
#include <QApplication>

#include "../components/FluMessageBox.h"
#include "robotsimulationwindow.h"
// todo test
#include "FluAEmptyPage.h"
#include "widgets/homewidget.h"
#include "widgets/settingwidget.h"

FRAMELESSHELPER_USE_NAMESPACE

RobotSimulationWindow::RobotSimulationWindow(QWidget *parent /*= nullptr*/) : FluFrameLessWidget(parent) {
    setWindowTitle("RobotSimulation");

    setWindowIcon(QIcon(":/icon/icon.ico"));

    resize(1200, 800);

    m_titleBar->setObjectName("titleBar");
    m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
    m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
    m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::black);
    m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::black);
    m_titleBar->setFixedHeight(35);

    QString qss = FluStyleSheetUitls::getQssByFileName(":/stylesheet/light/RobotSimulationWindow.qss");
    setStyleSheet(qss);

    m_navView = new FluVNavigationView(this);
    m_sLayout = new FluStackedLayout(this);

    m_contentLayout->addWidget(m_navView);
    m_contentLayout->addSpacing(0);
    m_contentLayout->addLayout(m_sLayout, 1);

    // home
    makeHomeNavItem();

    // code edit
    makeCodeEditNavItem();

    // settings
    makeSettingsNavItem();

    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, [=](FluTheme theme) { onThemeChanged(); });

    moveToCenter();
}


void RobotSimulationWindow::moveToCenter() {
    int screenWidth = QApplication::desktop()->width();
    int screenHeight = QApplication::desktop()->height();
    int windowWidth = width();
    int windowHeight = height();
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;
    move(x, y);
}


void RobotSimulationWindow::closeEvent(QCloseEvent *event) {
//    FluMessageBox messageBox("Close RobotSimulation?", "Choose \"Ok\" to close window, Choose \"Cancel\" return.",
//                             this);
//
//    int nExec = messageBox.exec();
//    if (nExec == QDialog::Rejected) {
//        m_titleBar->show();
//        event->ignore(); // can't run it! has some bug.
//        return;
//    } else if (nExec == QDialog::Accepted) {
//        event->accept();
//    }
}

void RobotSimulationWindow::onThemeChanged() {
    if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light) {
        m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::black);
        m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::black);

        m_titleBar->minimizeButton()->setActiveForegroundColor(Qt::black);
        m_titleBar->closeButton()->setActiveForegroundColor(Qt::black);
        m_titleBar->maximizeButton()->setActiveForegroundColor(Qt::black);

        m_titleBar->show();
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/RobotSimulationWindow.qss", this);
    } else {
        m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::white);
        m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::white);

        m_titleBar->minimizeButton()->setActiveForegroundColor(Qt::white);
        m_titleBar->closeButton()->setActiveForegroundColor(Qt::white);
        m_titleBar->maximizeButton()->setActiveForegroundColor(Qt::white);

        m_titleBar->show();
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/RobotSimulationWindow.qss", this);
    }
}

void RobotSimulationWindow::makeHomeNavItem() {
    FluVNavigationIconTextItem *item = new FluVNavigationIconTextItem(":/img/visual-light.png", "Display",
                                                                      this);
    m_navView->addItemToMidLayout(item);

    QWidget *homePage = new HomeWidget(this);

    m_sLayout->addWidget("HomePage", homePage);
    connect(item, &FluVNavigationIconTextItem::itemClicked, [=]() {
        m_sLayout->setCurrentWidget("HomePage");
    });
}

void RobotSimulationWindow::makeCodeEditNavItem() {
    FluVNavigationIconTextItem *item = new FluVNavigationIconTextItem(":/img/code-light.png", "CodeEdit",
                                                                      this);
    m_navView->addItemToMidLayout(item);

    auto typographyPage = new FluAEmptyPage(this);
    m_sLayout->addWidget("codeEditPage", typographyPage);
    connect(item, &FluVNavigationIconTextItem::itemClicked, [=]() { m_sLayout->setCurrentWidget("codeEditPage"); });
}

void RobotSimulationWindow::makeSettingsNavItem() {
    FluVNavigationIconTextItem *item = new FluVNavigationIconTextItem(":/img/setting-light.png", "Setting", this);
    m_navView->addItemToMidLayout(item);

    auto settingPage = new SettingWidget();
    m_sLayout->addWidget("settingPage", settingPage);
    connect(item, &FluVNavigationIconTextItem::itemClicked, [=]() { m_sLayout->setCurrentWidget("settingPage"); });
}

//void RobotSimulationWindow::resizeEvent(QResizeEvent *event) {
//    // if (event->size().width() == width())
//    //     return;
//
////        if (width() > 1000)
////        {
////            // check it
////            if (!m_navView->isLong())
////                m_navView->onMenuItemClicked();
////        }
////        else
////        {
////            if (m_navView->isLong())
////                m_navView->onMenuItemClicked();
////        }
//}