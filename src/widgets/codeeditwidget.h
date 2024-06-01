#ifndef ROBOTSIMULATION_CODEEDITWIDGET_H
#define ROBOTSIMULATION_CODEEDITWIDGET_H

#include <QLabel>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include "components/fluwidgets/FluWidget.h"
#include "components/fluwidgets/utils/FluUtils.h"
#include "components/fluwidgets/FluVScrollView.h"
#include "components/fluwidgets/FluSettingsSelectBox.h"
#include "components/fluwidgets/FluSettingsLabelBox.h"
#include "components/fluwidgets/FluIconButton.h"
#include "components/qcodeeditor/QCodeEditor.hpp"
#include "components/qcodeeditor/QRSHighlighter.hpp"
#include "components/qcodeeditor/QRSCompleter.hpp"
#include "components/qcodeeditor/QSyntaxStyle.hpp"
#include "tools/codehelper.h"

class CodeEditWidget : public FluWidget {
Q_OBJECT
public:
    explicit CodeEditWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

    QWidget *makeCodeEditPage();

    QCodeEditor *makeRSCodeEditWidget();

public slots:

    void onThemeChanged() override;

protected:
    QVBoxLayout *m_mainLayout;
    QCodeEditor *rsCodeEdit;
public:

};


#endif //ROBOTSIMULATION_CODEEDITWIDGET_H
