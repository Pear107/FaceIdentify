#ifndef TRAIN_THREAD_H
#define TRAIN_THREAD_H

#include <QThread>
#include <QImage>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

class TrainThread:public QThread
{
    Q_OBJECT
private:
    QImage toQImage(cv::Mat);
protected:
    void run() override;
public:
    TrainThread(QObject *parent = nullptr);
    ~TrainThread();
    TrainThread(const TrainThread &) = delete;
signals:
    void sendImage(QImage);
    void sendError(QString);
    void sendFinish();
};

#endif // TRAIN_THREAD_H
