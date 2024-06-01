#pragma once

#include "components/fluwidgets/utils/FluUtils.h"
#include "FluDef.h"
#include <QPushButton>

class FluGraphicalButton : public QPushButton
{
    Q_OBJECT
  public:
    FluGraphicalButton(QWidget* parent = nullptr) : QPushButton(parent)
    {
        setFixedSize(50, 50);
        setIconSize(QSize(28, 28));
        // FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluGraphicalButton.qss", this);

        onThemeChanged();
        connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, this, [=](FluTheme theme) { onThemeChanged(); });
    }

  public slots:
    void onThemeChanged()
    {
        if (FluThemeUtils::getUtils()->getTheme() == FluTheme::Light)
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluGraphicalButton.qss", this);
        }
        else
        {
            FluStyleSheetUitls::setQssByFileName(":/stylesheet/dark/FluGraphicalButton.qss", this);
        }
    }
};
