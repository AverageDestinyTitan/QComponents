#pragma once
#include <QtQml/qqml.h>
#include <QObject>
#include <QDebug>
#include <QImage>
#include <QImageReader>
#include <QString>
#include <QVarLengthArray>
// #include <QRgb>
#include <QColor>
#include <qquickitem.h>
#include "Windows.h"
// #include <libloaderapi.h>

class Q_ImageSource : public QObject{
    Q_OBJECT
    // QML_ELEMENT
    QML_NAMED_ELEMENT(ImageSource)

public:

    const QImage requestFrame(qint64 idx){

        if(m_frames.length() < idx || idx < 0)
            return err_img;
        return static_cast<const QImage>(m_frames[idx]);
        }

    void readFrames(){
        if(m_filename.isEmpty() || m_filename.isNull()) {
            qDebug() << "Null file!";
            return;}
        m_reader.setFileName(m_filename);
        qDebug() <<"FILENAME: " << m_reader.fileName();

        if(m_reader.canRead() == false || m_reader.imageCount() == -1)  [[unlikely]] {
            qDebug()<< m_reader.errorString();
            return;
        }

        m_frames.clear();
        if(m_reader.supportsAnimation() == false) [[unlikely]] {
            m_frames.append(m_reader.read());
            return;
        }

        if(m_frames.capacity() < m_reader.imageCount()) m_frames.reserve(m_reader.imageCount() - m_frames.capacity());
        QImage current = m_reader.read();
        while(current.isNull() == false){
            m_frames.append(current);
            current = m_reader.read();
        }
    }

    void setFileName(const QString& other){
        m_filename.clear();
        m_filename = other;
    }

    Q_ImageSource(QObject* parent = nullptr){
        DWORD bytesWritten =  GetModuleFileNameW(NULL, path, MAX_PATH);
        qDebug()<<"PATH: " << bytesWritten;
        qDebug()<< QString(path);

        QObject::connect(this, &Q_ImageSource::pathReady, this, [&](){
            delete this->m_resourcePathptr;
            this -> m_resourcePathptr = new QString(path);
            m_resourcePath = m_resourcePathptr->left(m_resourcePathptr->lastIndexOf("\\")  + 1);
            m_resourcePathptr ->clear();
            qDebug() << "EVAL RESOURCE PATH: "<< m_resourcePath;
        });

        emit this->pathReady();
        this ->readFrames();
    }

private:

    QVarLengthArray<QImage, 1> m_frames;
    QString* m_resourcePathptr = nullptr;
    QImageReader m_reader;
    QString m_filename = "C:/Projects/myBrowser/ico/magnifyingGlass001.png";
    QString m_resourcePath = QString((qint64)MAX_PATH, Qt::Initialization::Uninitialized);
    inline static const unsigned char m_errData[8] =  {0x00, 0x00,  0xFF, 0xF9, 0xFF, 0xF9, 0x00, 0x00};
    const QImage err_img = QImage(m_errData, 2,  2, 4,   QImage::Format::Format_RGB16); // m_errData may be defined later in the class..
    wchar_t path[MAX_PATH];
    Q_SIGNAL void pathReady();
};
