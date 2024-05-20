#pragma once

#include <QLabel>
#include "./utils/FluUtils.h"

class FluColorLabel : public QLabel
{
    Q_OBJECT
  public:
    FluColorLabel(QWidget* parent = nullptr) : QLabel(parent)
    {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluColorLabel.qss", this);
    }
};
