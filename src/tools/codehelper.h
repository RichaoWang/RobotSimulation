//
// Created by 12168 on 2024/5/29.
//

#ifndef ROBOTSIMULATION_CODEHELPER_H
#define ROBOTSIMULATION_CODEHELPER_H

#include <QString>
#include <QFile>

namespace CodeHelper {
    static QString loadCode(QString path) {
        QFile fl(path);

        if (!fl.open(QIODevice::ReadOnly)) {
            return QString();
        }

        return fl.readAll();
    }

    static QSyntaxStyle *loadStyle(QString path, QWidget *p) {
        QFile fl(path);

        if (!fl.open(QIODevice::ReadOnly)) {
            return nullptr;
        }

        auto style = new QSyntaxStyle(p);

        if (!style->load(fl.readAll())) {
            delete style;
            return nullptr;
        }

//    m_styles.append({style->name(), style});
        return style;
    }
}


#endif //ROBOTSIMULATION_CODEHELPER_H
