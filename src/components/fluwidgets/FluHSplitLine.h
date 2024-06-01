#pragma once

#include "FluWidget.h"
#include <QStyleOption>
#include <QPainter>
#include "components/fluwidgets/utils/FluUtils.h"

class FluHSplitLine : public FluWidget
{
    Q_OBJECT
  public:
    FluHSplitLine(QWidget* parent = nullptr) : FluWidget(parent)
    {
        setFixedWidth(1);
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluHSplitLine.qss", this);
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
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluHSplitLine.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluHSplitLine.qss", this);
        }
    }
};
