
#include "homewidget.h"
#include "components/FluIconButton.h"
#include "tools/infobar.h"

HomeWidget::HomeWidget(QWidget *parent) : FluWidget(parent) {
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setAlignment(Qt::AlignTop);
    setLayout(m_mainLayout);

    m_mainLayout->setContentsMargins(35, 35, 35, 35);

    m_vScrollView = new FluVScrollView;
    m_vScrollView->setObjectName("vScrollView");
    m_mainLayout->addWidget(m_vScrollView, 1);

    m_vScrollView->getMainLayout()->setAlignment(Qt::AlignTop);

    m_appBehaviorLabel = new QLabel;
    m_appBehaviorLabel->setObjectName("appBehaviorLabel");
    m_appBehaviorLabel->setText("Appearance");
    m_vScrollView->getMainLayout()->addWidget(m_appBehaviorLabel);

    appThemeSelectBox = new FluSettingsSelectBox;
    appThemeSelectBox->setTitleInfo("AppTheme", "Select which RobotSimulation theme to display");
    appThemeSelectBox->setIcon(FluAwesomeType::Color);

    appThemeSelectBox->getComboBox()->addItem("Light");
    appThemeSelectBox->getComboBox()->addItem("Dark");

    connect(appThemeSelectBox->getComboBox(), &FluComboBoxEx::currentIndexChanged, [=](int index) {
        if (index == 0)
            FluThemeUtils::getUtils()->setTheme(FluTheme::Light);
        else
            FluThemeUtils::getUtils()->setTheme(FluTheme::Dark);
    });

    appLanguageSelectBox = new FluSettingsSelectBox;
    appLanguageSelectBox->setTitleInfo("Language", "Select which RobotSimulation language to display");
    appLanguageSelectBox->setIcon(FluAwesomeType::LocaleLanguage);

    appLanguageSelectBox->getComboBox()->addItem("English");
    appLanguageSelectBox->getComboBox()->addItem("中文");

    connect(appLanguageSelectBox->getComboBox(), &FluComboBoxEx::currentIndexChanged, [=](int index) {
        if (index == 0)
            qDebug() << "en";
        else {
            /// todo cn Language translation
            InfoBar::showInfoBar(FluShortInfoBarType::Warn, "暂未实现中文翻译", this);
            qDebug() << "cn";
        }

    });


    m_vScrollView->getMainLayout()->addWidget(appThemeSelectBox);
    m_vScrollView->getMainLayout()->addWidget(appLanguageSelectBox);

    m_vScrollView->getMainLayout()->addSpacing(30);

    m_aboutLabel = new QLabel;
    m_aboutLabel->setObjectName("aboutLabel");
    m_aboutLabel->setText("About");
    m_vScrollView->getMainLayout()->addWidget(m_aboutLabel);

    auto aboutLabelBox = new FluSettingsLabelBox;
    aboutLabelBox->setTitleInfo("RobotSimulation", "RobotSimulation© 2024 RichaoWang, All rights reserved.");
    aboutLabelBox->setIcon(QIcon(":icon/icon.ico"), 30, 30);
    aboutLabelBox->setVersion("");  // empty it!;
    m_vScrollView->getMainLayout()->addWidget(aboutLabelBox);
    FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/HomeWidget.qss", this);
}

void HomeWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void HomeWidget::onThemeChanged() {
    if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light) {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/HomeWidget.qss", this);
    } else {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/HomeWidget.qss", this);
    }
}
