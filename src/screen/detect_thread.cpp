#include "detect_thread.h"

QImage DetectThread::toQImage(cv::Mat srcFrame){
    cv::Mat dstFrame;
    cv::cvtColor(srcFrame, dstFrame, cv::COLOR_BGR2RGB);
    QImage img((const uchar*)dstFrame.data,dstFrame.cols,dstFrame.rows,dstFrame.step,QImage::Format_RGB888);
    img.bits(); //深拷贝，避免线程退出内存错误
    return img;
}

void DetectThread::run(){
    cv::VideoCapture capture = cv::VideoCapture(0);
    if(!capture.isOpened()){
        emit sendError(QStringLiteral("打开摄像头失败，请检查是否安装摄像头设备"));
        return;
    }
    capture.set(cv::CAP_PROP_FRAME_WIDTH,640);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT,480);
    cv::CascadeClassifier cascadeClassifier;
    cv::Mat frame, frameGray;
    cv::Mat faceCover, faceCoverResize, faceCoverGray, faceThreshold, faceCoverBitwise;
    cv::Mat frameROI;
    cv::Mat equalizeFrame;
    int x, y, w, h;
    ushort cover = _cover;
    ushort classifier = _classifier;
    QFile file(":/assets/classifier/classifier.json");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray array = file.readAll();
        Json::Reader reader;
        Json::Value value;
        if(reader.parse(array.toStdString(), value)){
            std::string a = "./assets/classifier/";
            std::string b = value[std::to_string(_classifier)].asString();
            if(!cascadeClassifier.load(a + b)){
                qDebug("Error loading cascade file. Exiting!");
                return;
            }
        }else{
            return;
        }
    }else{
        return;
    }
    file.close();
    while(!isInterruptionRequested()){
        //从文件中读取，保证当前文件有demo.json文件
        if(classifier != _classifier){
            classifier = _classifier;
            QFile file(":/assets/classifier/classifier.json");
            if(file.open(QIODevice::ReadOnly)){
                QByteArray array = file.readAll();
                Json::Reader reader;
                Json::Value value;
                if(reader.parse(array.toStdString(), value)){
                    std::string a = "./assets/classifier/";
                    std::string b = value[std::to_string(_classifier)].asString();
                    cascadeClassifier.load(a + b);
                }else{
                    return;
                }
            }else{
                return;
            }
            file.close();
        }
        capture>>frame;
        cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(frameGray, equalizeFrame);
        std::vector<cv::Rect> faceRects;
        cascadeClassifier.detectMultiScale(equalizeFrame, faceRects, 1.1, 3, 0, cv::Size(30, 30));
        QVector<cv::Point> centers;
        for(auto &i: faceRects){
            if(_cover == 0){
                cover = _cover;
                cv::rectangle(frame, i, cv::Scalar(0, 255, 0), 2, 8);
            }else{
                if(faceCover.empty() || cover != _cover){
                    cover = _cover;
                    QFile file(covers[_classifier][_cover - 1]);
                    if(file.open(QIODevice::ReadOnly))
                    {
                        qint64 sz = file.size();
                        std::vector<uchar> buf(sz);
                        file.read((char*)buf.data(), sz);
                        faceCover = imdecode(buf, cv::IMREAD_COLOR);
                    }
                    file.close();
                    QVector<cv::Point> centers;
                }
                if(_classifier == 1 && _cover == 1){
                    cv::Point center(i.x + int(i.width * 0.5), i.y + int(i.height));
                    centers.push_back(center);
                }else{
                    cv::Rect faceRect(i.x, i.y, i.width, i.height);
                    cv::resize(faceCover, faceCoverResize, i.size());
                    cv::cvtColor(faceCoverResize, faceCoverGray, cv::COLOR_RGB2GRAY);
                    cv::threshold(faceCoverGray, faceThreshold, 233, 255, cv::THRESH_BINARY_INV);
                    frameROI = frame(i);
                    faceCoverResize.copyTo(frameROI, faceThreshold);
                }
            }
            if(centers.size() == 2){
                cv::Point leftPoint, rightPoint;
                if(centers[0].x < centers[1].x){
                    leftPoint = centers[0];
                    rightPoint = centers[1];
                }else{
                    leftPoint = centers[1];
                    rightPoint = centers[0];
                }
                int w = 2.3 * (rightPoint.x - leftPoint.x);
                int h = int(0.4 * w);
                int x = leftPoint.x - 0.25*w;
                int y = leftPoint.y - 0.5*h;
                if(x + w < frame.cols && y + h < frame.rows && x > 0 && y > 0){
                    cv::resize(faceCover, faceCoverResize, cv::Size(w, h));
                    cv::cvtColor(faceCoverResize, faceCoverGray, cv::COLOR_RGB2GRAY);
                    cv::threshold(faceCoverGray, faceThreshold, 233, 255, cv::THRESH_BINARY_INV);
                    frameROI = frame(cv::Rect(x, y, w, h));
                    faceCoverResize.copyTo(frameROI, faceThreshold);
                }
            }
        }
        emit sendImage(toQImage(frame));
        cv::waitKey(30);
    }
}

DetectThread::DetectThread(QObject *parent){}

DetectThread::~DetectThread(){}

ushort DetectThread::cover() const{
    return _cover;
}

ushort DetectThread::classifier() const{
    return _classifier;
}

void DetectThread::receiveCover(QString cover){
    _cover = cover.toUShort();
}

void DetectThread::receiveClassifier(QString classifier){
    _classifier = classifier.toUShort();
}
