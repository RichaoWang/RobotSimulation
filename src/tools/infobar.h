#include <QWidget>
#include "components/FluShortInfoBar.h"
#include "components/FluInfoBarMgr.h"

namespace InfoBar {
    static void showInfoBar(FluShortInfoBarType type, QString text, QWidget *p, int w = 270) {
        auto infobar = new FluShortInfoBar(type, text, p->window());
        infobar->setFixedWidth(w);
        FluInfoBarMgr::getInstance()->addInfoBar(p->window(), infobar);
    }
}


