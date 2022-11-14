#ifndef INFORMATION_H
#define INFORMATION_H

#include <QString>
#include <QDateTime>


class Information
{
public:
    QString Title ;
     QDateTime Time;

    friend class MainWindow;
    static QString createInformation();
    Information(QString title, QDateTime time);

};

#endif // INFORMATION_H
