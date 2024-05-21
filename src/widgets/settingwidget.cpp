
#include "settingwidget.h"

SettingWidget::SettingWidget(QWidget *parent) : FluWidget(parent) {
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setAlignment(Qt::AlignTop);
    setLayout(m_mainLayout);

    m_mainLayout->setContentsMargins(35, 35, 35, 35);

    m_titleLabel = new QLabel;
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setText("Settings");
    m_mainLayout->addWidget(m_titleLabel);

    m_vScrollView = new FluVScrollView;
    m_vScrollView->setObjectName("vScrollView");
    m_mainLayout->addWidget(m_vScrollView, 1);

    m_vScrollView->getMainLayout()->setAlignment(Qt::AlignTop);

    m_appBehaviorLabel = new QLabel;
    m_appBehaviorLabel->setObjectName("appBehaviorLabel");
    m_appBehaviorLabel->setText("Appearance & behavior");
    m_vScrollView->getMainLayout()->addWidget(m_appBehaviorLabel);

    auto appThemeSelectBox = new FluSettingsSelectBox;
    appThemeSelectBox->setTitleInfo("App theme", "Select which app theme to display.");
    appThemeSelectBox->setIcon(FluAwesomeType::Color);

    appThemeSelectBox->getComboBox()->addItem("Light");
    appThemeSelectBox->getComboBox()->addItem("Dark");

    connect(appThemeSelectBox->getComboBox(), &FluComboBoxEx::currentIndexChanged, [=](int index) {
        if (index == 0)
            FluThemeUtils::getUtils()->setTheme(FluTheme::Light);
        else
            FluThemeUtils::getUtils()->setTheme(FluTheme::Dark);

        // LOG_DEBUG << FluThemeUtils::getUtils()->getTheme();
    });

    m_vScrollView->getMainLayout()->addWidget(appThemeSelectBox);

    auto navStyleSelectBox = new FluSettingsSelectBox;
    navStyleSelectBox->hideInfoLabel();
    navStyleSelectBox->setTitleInfo("Navigation style", "");
    navStyleSelectBox->setIcon(FluAwesomeType::HolePunchLandscapeLeft);
    navStyleSelectBox->getComboBox()->addItem("Left");
    navStyleSelectBox->getComboBox()->addItem("Top");
    m_vScrollView->getMainLayout()->addWidget(navStyleSelectBox);

    m_vScrollView->getMainLayout()->addSpacing(30);

    m_aboutLabel = new QLabel;
    m_aboutLabel->setObjectName("aboutLabel");
    m_aboutLabel->setText("About");
    m_vScrollView->getMainLayout()->addWidget(m_aboutLabel);

    auto aboutLabelBox = new FluSettingsLabelBox;
    aboutLabelBox->setTitleInfo("CppQtFluent888 Gallery", "©2023-2024 FluentUI For Qt & Cpp. All rights reserved.");
    aboutLabelBox->setIcon(QIcon(":icon/icon.ico"));
    aboutLabelBox->setVersion("");  // empty it!;
    m_vScrollView->getMainLayout()->addWidget(aboutLabelBox);
    FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/SettingWidget.qss", this);
}

void SettingWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void SettingWidget::onThemeChanged() {
    if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light) {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/SettingWidget.qss", this);
    } else {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/SettingWidget.qss", this);
    }
}
