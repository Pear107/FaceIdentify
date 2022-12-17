#include "train_thread.h"

QImage TrainThread::toQImage(cv::Mat srcFrame){
    cv::Mat dstFrame;
    cv::cvtColor(srcFrame, dstFrame, cv::COLOR_BGR2RGB);
    QImage img((const uchar*)dstFrame.data,dstFrame.cols,dstFrame.rows,dstFrame.step,QImage::Format_RGB888);
    img.bits(); //深拷贝，避免线程退出内存错误
    return img;
}

void TrainThread::run(){
    std::vector<cv::Mat> images(30);
    std::vector<int> labels(30, 1);
    cv::VideoCapture capture = cv::VideoCapture(0);
    if(!capture.isOpened()){
        emit sendError(QStringLiteral("打开摄像头失败，请检查是否安装摄像头设备"));
        return;
    }
    capture.set(cv::CAP_PROP_FRAME_WIDTH,640);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT,480);
    cv::CascadeClassifier cascadeClassifier;
    if(!cascadeClassifier.load("assets/classifier/haarcascade_frontalface_alt.xml"))
    {
        qDebug("Error loading cascade file. Exiting!");
        return;
    }
    QDir trainPath;
    if(!trainPath.exists("train")){
        qDebug("not exists");
        trainPath.mkdir("train");
        trainPath.cd("train");
    }else{
        qDebug("exists");
        trainPath.cd("train");
        QList<QFileInfo> *fileInfo = new QList<QFileInfo>(trainPath.entryInfoList());
        for(int i = 0; i < fileInfo->count(); i++)
        {
            trainPath.remove(fileInfo->at(i).fileName());
        }
    }
    cv::Mat frame, frameGray;
    cv::Mat face, faceResize;
    cv::Mat equalizeFrame;
    std::vector<cv::Rect> faceRects;
    quint8 count = 0;
    while(!isInterruptionRequested()){
        capture>>frame;
        cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(frameGray, equalizeFrame);
        cascadeClassifier.detectMultiScale(equalizeFrame, faceRects, 1.1, 3, 0, cv::Size(30, 30));
        for (cv::Rect &i:faceRects) {
            cv::rectangle(frame, i, cv::Scalar(0, 255, 0), 2, 8);
            face = equalizeFrame(i);
            cv::resize(face, faceResize, cv::Size(128,128), 0, 0, cv::INTER_LINEAR);
            images[count] = faceResize;
            count++;
//            QString path = trainPath.canonicalPath() +"/" + QString::number(count) + ".jpg";
//            cv::imwrite(path.toStdString(), faceResize);
            break;
        }
        emit sendImage(toQImage(frame));
        cv::waitKey(30);
        if(count == 30){
            cv::Ptr<cv::face::LBPHFaceRecognizer> recognizer = cv::face::LBPHFaceRecognizer::create();
            recognizer->train(images, labels);
            recognizer->write("train/train.yml");
            break;
        }
    }
}

TrainThread::TrainThread(QObject *parent){}

TrainThread::~TrainThread(){}
