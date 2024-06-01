#ifndef ROBOTSIMULATION_SETTINGWIDGET_H
#define ROBOTSIMULATION_SETTINGWIDGET_H

#include <QLabel>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include "components/fluwidgets/FluWidget.h"
#include "components/fluwidgets/utils/FluUtils.h"
#include "components/fluwidgets/FluVScrollView.h"
#include "components/fluwidgets/FluSettingsSelectBox.h"
#include "components/fluwidgets/FluSettingsLabelBox.h"

class SettingWidget : public FluWidget {
Q_OBJECT
public:
    explicit SettingWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

public slots:

    void onThemeChanged() override;

protected:
    QVBoxLayout *m_mainLayout;

    QLabel *m_titleLabel;
    QLabel *m_appBehaviorLabel;
    QLabel *m_aboutLabel;
    FluVScrollView *m_vScrollView;
public:
    FluSettingsSelectBox *appThemeSelectBox;
    FluSettingsSelectBox *appLanguageSelectBox;
};

#endif //ROBOTSIMULATION_SETTINGWIDGET_H