#pragma once

#include "./utils/FluStyleSheetUitls.h"
#include <QTableWidget>

class FluTabWidget : public QTabWidget
{
    Q_OBJECT
  public:
    FluTabWidget(QWidget *parent = nullptr) : QTabWidget(parent)
    {
        QString qss = FluStyleSheetUitls::getQssByFileName(":/stylesheet/light/FluTabWidget.qss");
        setStyleSheet(qss);
    }
};
