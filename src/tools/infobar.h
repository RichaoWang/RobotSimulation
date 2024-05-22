//
// Created by 12168 on 2024/5/22.
//

#ifndef ROBOTSIMULATION_INFOBAR_H
#define ROBOTSIMULATION_INFOBAR_H

#include <QWidget>
#include "components/FluShortInfoBar.h"
#include "components/FluInfoBarMgr.h"

namespace InfoBar {
    void showInfoBar(FluShortInfoBarType type, QString text, QWidget *p, int w = 270) {
        auto infobar = new FluShortInfoBar(type, text, p->window());
        infobar->setFixedWidth(w);
        FluInfoBarMgr::getInstance()->addInfoBar(p->window(), infobar);
    }
}


#endif //ROBOTSIMULATION_INFOBAR_H
