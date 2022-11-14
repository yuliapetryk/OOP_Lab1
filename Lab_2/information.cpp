#include "information.h"
#include <QTextStream>
#include <QMessageBox>


//Information::Information(QString title, QDateTime time) : Title(title),Time(time){};

    QString Information:: createInformation()
    {
        QString about_text="";
        about_text="Information about note: ";
       about_text.append(Information::Time.time().toString("hh.mm.ss"));
        about_text.append(" ");
        about_text.append(Information::Time.date().toString("dd.mm.yy"));
        about_text.append(" ");
        about_text.append(Information::Title);
        return about_text;

    }



