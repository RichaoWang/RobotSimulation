#pragma once

#include <QMenuBar>
#include "components/fluwidgets/utils/FluUtils.h"
#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QVBoxLayout>

class FluMenuBar : public QMenuBar
{
    Q_OBJECT
  public:
    FluMenuBar(QWidget* parent = nullptr) : QMenuBar(parent)
    {
        setMouseTracking(true);
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluMenuBar.qss", this);
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluMenuBar.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluMenuBar.qss", this);
        }
    }
};
