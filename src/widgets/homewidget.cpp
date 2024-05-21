//
// Created by 12168 on 2024/5/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_HomeWidget.h" resolved

#include "homewidget.h"
#include "../../froms/ui_homewidget.h"

HomeWidget::HomeWidget(QWidget *parent) :
        FluWidget(parent), ui(new Ui::HomeWidget) {
    ui->setupUi(this);
}

HomeWidget::~HomeWidget() {
    delete ui;
}
