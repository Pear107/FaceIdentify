#ifndef RECOTHREAD_H
#define RECOTHREAD_H

#include <QThread>
#include <QImage>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

class RecoThread:public QThread
{
    Q_OBJECT
private:
    QImage toQImage(cv::Mat);
protected:
    void run() override;
public:
    RecoThread(QObject *parent = nullptr);
    ~RecoThread();
    RecoThread(const RecoThread &) = delete;
signals:
    void sendImage(QImage);
    void sendError(QString);
};

#endif // RECOTHREAD_H
