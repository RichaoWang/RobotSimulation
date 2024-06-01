#pragma once

#include "FluWidget.h"
#include <QStyleOption>
#include <QPainter>
#include "components/fluwidgets/utils/FluUtils.h"

class FluVSplitLine : public FluWidget
{
    Q_OBJECT
  public:
    FluVSplitLine(QWidget* parent = nullptr) : FluWidget(parent)
    {
        setFixedHeight(1);
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluVSplitLine.qss", this);
    }

    void paintEvent(QPaintEvent* event)
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    }
  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluVSplitLine.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluVSplitLine.qss", this);
        }
    }
};
