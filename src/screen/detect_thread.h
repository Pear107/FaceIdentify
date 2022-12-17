#ifndef DETECT_THREAD_H
#define DETECT_THREAD_H

#include <QThread>
#include <QImage>
#include <QFile>
#include <QTextStream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <json/json.h>

class DetectThread:public QThread
{
    Q_OBJECT
private:
    QImage toQImage(cv::Mat);
    ushort _cover = 0;
    ushort _classifier = 0;
    QVector<QVector<QString>> covers = {{":/assets/cover/mask.jpg"}, {":/assets/cover/glasses.jpg"}, {""}, {":/assets/cover/redNose.jpg"}};
protected:
    void run() override;
public:
    DetectThread(QObject *parent = nullptr);
    ~DetectThread();
    DetectThread(const DetectThread &) = delete;
    ushort cover() const;
    ushort classifier() const;
signals:
    void sendImage(QImage);
    void sendError(QString);
public slots:
    void receiveCover(QString);
    void receiveClassifier(QString);
};

#endif // DETECT_THREAD_H
