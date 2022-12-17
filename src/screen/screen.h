#ifndef SCREEN_H
#define SCREEN_H

#include <QQuickItem>
#include <QImage>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

#include "detect_thread.h"
#include "reco_thread.h"
#include "train_thread.h"

class Screen:public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(Screen)
    Q_PROPERTY(bool isOpen READ isOpen WRITE setIsOpen NOTIFY isOpenChanged FINAL)
    Q_PROPERTY(qint32 model READ model WRITE setModel NOTIFY modelChanged FINAL)
    Q_PROPERTY(QImage img READ img WRITE setImg NOTIFY imgChanged FINAL)
    Q_PROPERTY(QString error READ error() NOTIFY errorChanged)
private:
    bool _isOpen;
    qint32 _model;
    QImage _img;
    QString _error;
    DetectThread *detect;
    RecoThread *reco;
    TrainThread *train;
    QImage toQImage(cv::Mat);
protected:
    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override;
public:
    Screen(QQuickItem *parent = nullptr);
    ~Screen();
    bool isOpen() const;
    qint32 model() const;
    QImage img() const;
    QString error() const;
    void setIsOpen(bool);
    void setModel(qint32);
    Q_INVOKABLE void detectPhoto(const QString);
    Q_INVOKABLE void recoPhoto(const QString);
    Q_INVOKABLE void trainPhoto(const QString);
private slots:
    void setImg(QImage&);
    void receiveError(QString);
    void receiveFinish();
signals:
    void isOpenChanged();
    void modelChanged();
    void imgChanged();
    void errorChanged();
    void sendFinish();
    void setCover(QString);
    void setClassifier(QString);
};

#endif // SCREEN_H
