#include "robotsimulationwindow.h"

FRAMELESSHELPER_USE_NAMESPACE

RobotSimulationWindow::RobotSimulationWindow(QWidget *parent /*= nullptr*/) : FluFrameLessWidget(parent) {
    setWindowTitle("RobotSimulation");

    setWindowIcon(QIcon(":/icon/icon.ico"));

//    resize(1200, 800);
    setMinimumSize(1200,800);

    m_titleBar->setObjectName("titleBar");

    m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
    m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
    m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::black);
    m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::black);
    QString qss = FluStyleSheetUitls::getQssByFileName(":/stylesheet/light/RobotSimulationWindow.qss");
    setStyleSheet(qss);

    m_titleBar->setFixedHeight(35);

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
#ifdef NDEBUG
    FluMessageBox messageBox("是否关闭RobotSimulation？", "温馨提示：关闭该应用前，请检查已编辑的文件是否保存成功。按下确定按钮退出，按下返回按钮继续当前应用",
                             this);

    int nExec = messageBox.exec();
    if (nExec == QDialog::Rejected) {
        m_titleBar->show();
        event->ignore(); // can't run it! has some bug.
        return;
    } else if (nExec == QDialog::Accepted) {
        event->accept();
    }
#endif
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

        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/RobotSimulationWindow.qss", this);
    } else {
        m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
        m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::white);
        m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::white);

        m_titleBar->minimizeButton()->setActiveForegroundColor(Qt::white);
        m_titleBar->closeButton()->setActiveForegroundColor(Qt::white);
        m_titleBar->maximizeButton()->setActiveForegroundColor(Qt::white);

        FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/RobotSimulationWindow.qss", this);
    }
}

/// 显示页
void RobotSimulationWindow::makeHomeNavItem() {
    FluVNavigationIconTextItem *item = new FluVNavigationIconTextItem(":/img/visual-light.png", "视图与控制",
                                                                      this);
    m_navView->addItemToMidLayout(item);

    QWidget *homePage = new HomeWidget(this);

    m_sLayout->addWidget("HomePage", homePage);
    connect(item, &FluVNavigationIconTextItem::itemClicked, [=]() {
        m_sLayout->setCurrentWidget("HomePage");
    });
    item->itemClicked();
}

/// todo 代码编辑页
void RobotSimulationWindow::makeCodeEditNavItem() {
    FluVNavigationIconTextItem *item = new FluVNavigationIconTextItem(":/img/code-light.png", "编程",
                                                                      this);
    m_navView->addItemToMidLayout(item);

    auto typographyPage = new CodeEditWidget(this);
    m_sLayout->addWidget("CodeEditPage", typographyPage);
    connect(item, &FluVNavigationIconTextItem::itemClicked, [=]() { m_sLayout->setCurrentWidget("CodeEditPage"); });
}

/// 设置页
void RobotSimulationWindow::makeSettingsNavItem() {
    FluVNavigationIconTextItem *item = new FluVNavigationIconTextItem(":/img/setting-light.png", "设置", this);
    m_navView->addItemToMidLayout(item);

    auto settingPage = new SettingWidget();
    m_sLayout->addWidget("SettingPage", settingPage);
    connect(item, &FluVNavigationIconTextItem::itemClicked, [=]() { m_sLayout->setCurrentWidget("SettingPage"); });
}

void RobotSimulationWindow::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
