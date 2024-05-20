#pragma once

#include <QLineEdit>
#include "FluDef.h"
#include "./utils/FluUtils.h"

class FluLineEdit : public QLineEdit
{
    Q_OBJECT
  public:
    FluLineEdit(QWidget* parent = nullptr) : QLineEdit(parent)
    {
        FluStyleSheetUitls::setQssByFileName(":/stylesheet/light/FluLineEdit.qss", this);
    }
};
