#pragma once

#include <QProgressBar>
#include "./utils/FluUtils.h"

class FluProgressBar : public QProgressBar
{
    Q_OBJECT
  public:
    FluProgressBar(QWidget* parent = nullptr) : QProgressBar(parent)
    {
        setTextVisible(true);
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluProgressBar.qss", this);
    }

  protected:
};
