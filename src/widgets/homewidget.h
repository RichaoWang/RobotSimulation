//
// Created by 12168 on 2024/5/21.
//

#ifndef ROBOTSIMULATION_HOMEWIDGET_H
#define ROBOTSIMULATION_HOMEWIDGET_H

#include <QWidget>
#include "components/FluWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HomeWidget; }
QT_END_NAMESPACE

class HomeWidget : public FluWidget {
Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);

    ~HomeWidget() override;

private:
    Ui::HomeWidget *ui;
};


#endif //ROBOTSIMULATION_HOMEWIDGET_H
