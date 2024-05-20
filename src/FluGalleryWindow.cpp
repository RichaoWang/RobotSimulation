#include "FluGalleryWindow.h"
//#include "FluAEmptyPage.h"
#include <FramelessHelper/Core/framelessmanager.h>
#include <FramelessHelper/Widgets/framelesswidgetshelper.h>
#include <FramelessHelper/Widgets/standardsystembutton.h>
#include <FramelessHelper/Widgets/standardtitlebar.h>
#include "./components/FluMessageBox.h"
#include <QApplication>

FRAMELESSHELPER_USE_NAMESPACE

FluGalleryWindow::FluGalleryWindow(QWidget *parent /*= nullptr*/) : FluFrameLessWidget(parent)
{
    setWindowTitle("CppQt WinUI3 Gallery Dev");

//#ifndef _DEBUG
////    setWindowTitle("CppQt WinUI3 Gallery");
//#endif

    setWindowIcon(QIcon(":/icon/icon.ico"));
    qDebug()<<"1111111111111111111111";
    // resize
    resize(1200, 800);

//    m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
//    m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
//    m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::black);
//    m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::black);
    m_titleBar->setFixedHeight(48);
    qDebug()<<"22222222222222222222222222";

    QString qss = FluStyleSheetUitls::getQssByFileName(":/stylesheet/light/FluGalleryWindow.qss");
    setStyleSheet(qss);
    qDebug()<<"3333333333333333333333333333";

//    m_navView = new FluVNavigationView(this);
//    m_sLayout = new FluStackedLayout;
    qDebug()<<"444444444444444444444444444444444";

    // if nav long will be short, short will be long after clicked it.
    // m_navView->onMenuItemClicked();

//    m_contentLayout->addWidget(m_navView);
    //  m_contentLayout->addSpacing(20);
//    m_contentLayout->addLayout(m_sLayout, 1);

    //  auto homePage = new FluHomePage(this);
    //  m_sLayout->addWidget(homePage);

    //  auto aEmptyPage = new FluAEmptyPage(this);
    //  m_sLayout->addWidget(aEmptyPage);

    // home
//    makeHomeNavItem();

    // design guidance
//    makeDesignGuidanceNavItem();

    // samples
//    makeSamplesNavItem();

    // basic input
//    makeBasicInputNavItem();

    // collections
//    makeCollectionsNavItem();

    // date & time
//    makDateTimeNavItem();

    // dialogs & flyouts
//    makeDialogsFlyouts();

    // layout
//    makeLayoutNavItem();

    // media
//    makeMediaNavItem();

    // menus & toolbars
//    makeMenuToolBarsNavItem();

    // navigation
//    makeNavigationNavItem();

    // srolling

//    makeScrollingNavItem();

    // status & info

//    makeStatusInfoNavItem();

    // text
//    makeTextNavItem();

    // settings
//    makeSettingsNavItem();

//    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, [=](FluTheme theme) { onThemeChanged(); });

    ///* connect(m_titleBar->closeButton(), &QPushButton::clicked, [=]() { FluMessageBox messageBox;
    //     messageBox.exec();
    // });*/
}

void FluGalleryWindow::closeEvent(QCloseEvent *event)
{
    // FluMessageBox messageBox("Close Gallery Window?", "choose \"Ok\" to close. choose \"Cancel\" do nothing.", this);
    // messageBox.exec();

    // int nExec = messageBox.exec();
    // if (nExec == QDialog::Rejected)
    // {
    // m_titleBar->show();
    // event->ignore(); // can't run it! has some bug.
    // return;
    //  }
    //  else if (nExec == QDialog::Accepted)
    //  {
    //     event->accept();
    // }
}

void FluGalleryWindow::onThemeChanged()
{
    // LOG_DEBUG << "Func Beg";
    if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
    {
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
        FluStyleSheetUitls::setQssByFileName(":/StyleSheet/light/FluGalleryWindow.qss", this);
        // repaint();
        // QApplication::processEvents();
    }
    else
    {
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
        FluStyleSheetUitls::setQssByFileName("../StyleSheet/dark/FluGalleryWindow.qss", this);
        // repaint();
        // QApplication::processEvents();
    }
    // LOG_DEBUG << "Func End";
}
