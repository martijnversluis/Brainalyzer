#ifndef TEXTDATAFILE_H
#define TEXTDATAFILE_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class TextDataFile {

public:
    TextDataFile(QString filename);
    void open();
    void close();
    bool atEnd();
    int line();
    void gotoLine(int lineNumber);
    void reset();

protected:
    QFile* file;
    QTextStream* stream;
    int lineNumber;

    QString readLine();

};

#endif
