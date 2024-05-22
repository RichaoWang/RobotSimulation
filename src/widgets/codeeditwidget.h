#ifndef ROBOTSIMULATION_CODEEDITWIDGET_H
#define ROBOTSIMULATION_CODEEDITWIDGET_H

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

class CodeEditWidget : public FluWidget {
Q_OBJECT
public:
    explicit CodeEditWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

public slots:

    void onThemeChanged() override;

protected:
    QVBoxLayout *m_mainLayout;

    QLabel *m_titleLabel;
    QLabel *m_appBehaviorLabel;
//    QLabel *m_aboutLabel;
    FluVScrollView *m_vScrollView;
public:
//    FluSettingsSelectBox *appThemeSelectBox;
//    FluSettingsSelectBox *appLanguageSelectBox;
};


#endif //ROBOTSIMULATION_CODEEDITWIDGET_H
