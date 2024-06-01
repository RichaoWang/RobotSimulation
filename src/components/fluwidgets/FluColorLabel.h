#pragma once

#include <QLabel>
#include "components/fluwidgets/utils/FluUtils.h"

class FluColorLabel : public QLabel
{
    Q_OBJECT
  public:
    FluColorLabel(QWidget* parent = nullptr) : QLabel(parent)
    {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluColorLabel.qss", this);
    }
};
