#pragma once

#include "components/fluwidgets/utils/FluUtils.h"
#include <QPushButton>

class FluStyleButton : public QPushButton
{
    Q_OBJECT
  public:
    FluStyleButton(QWidget* parent = nullptr) : QPushButton(parent)
    {
        setFixedSize(200, 30);
        QString qss = FluStyleSheetUitls::getQssByFileName(":/stylesheet/light/FluStyleButton.qss");
        setStyleSheet(qss);
    }
};
