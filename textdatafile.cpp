#include "textdatafile.h"

TextDataFile::TextDataFile(QString filename):
    lineNumber(0)
{
    this->file = new QFile(filename);
    this->open();
}

void TextDataFile::open() {
    if (this->file->exists()) {
        this->file->open(QFile::ReadOnly);
        this->stream = new QTextStream(file);
    } else {
        qDebug() << "File " << this->file->fileName() << " does not exist";
    }
}

void TextDataFile::close() {
    this->file->close();
}

void TextDataFile::reset() {
    this->close();
    lineNumber = 0;
    this->open();
}

bool TextDataFile::atEnd() {
    return this->stream->atEnd();
}

QString TextDataFile::readLine() {
    lineNumber++;
    return this->stream->readLine();
}

int TextDataFile::line() {
    return this->lineNumber;
}

void TextDataFile::gotoLine(int lineNumber) {
    if (lineNumber > this->lineNumber) {
        this->reset();
    }

    while (this->lineNumber < lineNumber) {
        this->readLine();
    }
}
