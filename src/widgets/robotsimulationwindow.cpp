
#include <FramelessHelper/Core/framelessmanager.h>
#include <FramelessHelper/Widgets/framelesswidgetshelper.h>
#include <FramelessHelper/Widgets/standardsystembutton.h>
#include <FramelessHelper/Widgets/standardtitlebar.h>
#include <QApplication>

#include "../components/FluMessageBox.h"
#include "robotsimulationwindow.h"
// todo test
#include "FluAEmptyPage.h"
#include "FluAEmptyPage2.h"
#include "FluAEmptyPage3.h"

FRAMELESSHELPER_USE_NAMESPACE

RobotSimulationWindow::RobotSimulationWindow(QWidget *parent /*= nullptr*/) : FluFrameLessWidget(parent) {
    setWindowTitle("RobotSimulation");

    setWindowIcon(QIcon(":/icon/icon.ico"));

    resize(1200, 800);

    m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
    m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
    m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::black);
    m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::black);
    m_titleBar->setFixedHeight(35);

    QString qss = FluStyleSheetUitls::getQssByFileName(":/stylesheet/light/FluGalleryWindow.qss");
    setStyleSheet(qss);

    m_navView = new FluVNavigationView(this);
    m_sLayout = new FluStackedLayout(this);

    m_contentLayout->addWidget(m_navView);
    m_contentLayout->addSpacing(20);
    m_contentLayout->addLayout(m_sLayout, 1);

    // home
    makeHomeNavItem();

    // code edit
    makeCodeEditNavItem();

    // settings
    makeSettingsNavItem();

//    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, [=](FluTheme theme) { onThemeChanged(); });

//    connect(m_titleBar->closeButton(), &QPushButton::clicked, [=]() {
////        FluMessageBox messageBox("", "", this);
////        messageBox.exec();
//    });
}

void RobotSimulationWindow::closeEvent(QCloseEvent *event) {
    FluMessageBox messageBox("Close", "Close RobotSimulation Window?",
                             this);

    int nExec = messageBox.exec();
    if (nExec == QDialog::Rejected) {
        m_titleBar->show();
        event->ignore(); // can't run it! has some bug.
        return;
    } else if (nExec == QDialog::Accepted) {
        event->accept();
    }
}

void RobotSimulationWindow::onThemeChanged() {
    // LOG_DEBUG << "Func Beg";
    if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light) {
        m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::black);
        m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::black);

        m_titleBar->minimizeButton()->setActiveForegroundColor(Qt::black);
        m_titleBar->closeButton()->setActiveForegroundColor(Qt::black);
        m_titleBar->maximizeButton()->setActiveForegroundColor(Qt::black);

        // m_titleBar->update();
        // m_titleBar->style()->polish(m_titleBar);
        m_titleBar->show();
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluGalleryWindow.qss", this);
        // repaint();
        // QApplication::processEvents();
    } else {
        m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::white);
        m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::white);
        // m_titleBar->update();
        // m_titleBar->style()->polish(m_titleBar);

        m_titleBar->minimizeButton()->setActiveForegroundColor(Qt::white);
        m_titleBar->closeButton()->setActiveForegroundColor(Qt::white);
        m_titleBar->maximizeButton()->setActiveForegroundColor(Qt::white);

        m_titleBar->show();
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluGalleryWindow.qss", this);
        // repaint();
        // QApplication::processEvents();
    }
    // LOG_DEBUG << "Func End";
}

void RobotSimulationWindow::makeHomeNavItem() {
    FluVNavigationIconTextItem *item = new FluVNavigationIconTextItem(FluAwesomeType::Home, "Home", this);
    m_navView->addItemToMidLayout(item);

    auto homePage = new FluAEmptyPage();

//        auto homePage = new FluHomePage;
    m_sLayout->addWidget("HomePage", homePage);
    connect(item, &FluVNavigationIconTextItem::itemClicked, [=]() {
        m_sLayout->setCurrentWidget("HomePage");

        //  FluMessageBox messageBox("Close Gallery Window?", "choose \"Ok\" to close. choose \"Cancel\" do nothing.", this);
        // int nExec = messageBox.exec();
    });
}

void RobotSimulationWindow::makeCodeEditNavItem() {
    FluVNavigationIconTextItem *item = new FluVNavigationIconTextItem(FluAwesomeType::Code, "Code",
                                                                      this);
    m_navView->addItemToMidLayout(item);

    auto typographyPage = new FluAEmptyPage2(this);
    m_sLayout->addWidget("TypographyPage", typographyPage);
    connect(item, &FluVNavigationIconTextItem::itemClicked, [=]() { m_sLayout->setCurrentWidget("TypographyPage"); });
//
//        auto iconsPage = new FluIconsPage;
//        m_sLayout->addWidget("IconsPage", iconsPage);
//        connect(item2, &FluVNavigationIconTextItem::itemClicked, [=]() { m_sLayout->setCurrentWidget("IconsPage"); });
}

void RobotSimulationWindow::makeSettingsNavItem() {
    FluVNavigationSettingsItem *item = new FluVNavigationSettingsItem(FluAwesomeType::Settings, "Set", this);
    m_navView->addItemToMidLayout(item);

    auto settingPage = new FluAEmptyPage3(this);
    m_sLayout->addWidget("settingPage", settingPage);
    connect(item, &FluVNavigationSettingsItem::itemClicked, [=]() { m_sLayout->setCurrentWidget("settingPage"); });

//        m_navView->addItemToBottomLayout(item);

//        auto settingsPage = new FluSettingPage;
//        m_sLayout->addWidget("SettingPage", settingsPage);
//        connect(item, &FluVNavigationSettingsItem::itemClicked, [=]() { m_sLayout->setCurrentWidget("SettingPage"); });
}

void RobotSimulationWindow::resizeEvent(QResizeEvent *event) {
    // if (event->size().width() == width())
    //     return;

//        if (width() > 1000)
//        {
//            // check it
//            if (!m_navView->isLong())
//                m_navView->onMenuItemClicked();
//        }
//        else
//        {
//            if (m_navView->isLong())
//                m_navView->onMenuItemClicked();
//        }
}