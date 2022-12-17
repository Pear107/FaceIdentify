#include "screen.h"

QImage Screen::toQImage(cv::Mat src){
    cv::Mat dst;
    cv::cvtColor(src, dst, cv::COLOR_BGR2RGB);
    QImage img((const uchar*)dst.data,dst.cols,dst.rows,dst.step,QImage::Format_RGB888);
    img.bits(); //深拷贝，避免线程退出内存错误
    return img;
}

Screen::Screen(QQuickItem *parent)
{
    setFlag(Flag::ItemHasContents,true);
    detect = new DetectThread(this);
    reco = new RecoThread(this);
    train = new TrainThread(this);
    qRegisterMetaType<QImage>("QImage&");
    connect(detect, SIGNAL(sendImage(QImage&)),this,SLOT(setImg(QImage&)));
    connect(detect, SIGNAL(sendError(QString)),this,SLOT(receiveError(QString)));
    connect(reco, SIGNAL(sendImage(QImage&)),this,SLOT(setImg(QImage&)));
    connect(reco, SIGNAL(sendError(QString)),this,SLOT(receiveError(QString)));
    connect(train, SIGNAL(sendImage(QImage&)),this,SLOT(setImg(QImage&)));
    connect(train, SIGNAL(sendError(QString)),this,SLOT(receiveError(QString)));
    connect(train, SIGNAL(sendFinish()), this,SLOT(receiveFinish()));
    connect(this, SIGNAL(setCover(QString)), detect, SLOT(receiveCover(QString)));
    connect(this, SIGNAL(setClassifier(QString)), detect, SLOT(receiveClassifier(QString)));
}

Screen::~Screen(){
    detect->requestInterruption();
    detect->wait();
    reco->requestInterruption();
    reco->wait();
    train->requestInterruption();
    train->wait();
}

QSGNode* Screen::updatePaintNode(QSGNode* node, UpdatePaintNodeData *){
    QSGSimpleTextureNode *texture = static_cast<QSGSimpleTextureNode *>(node);
    if (!texture) {
        texture = new QSGSimpleTextureNode();
    }

    if(_img.isNull()){
        _img = QImage(boundingRect().size().toSize(), QImage::Format_RGB888);
        _img.fill(QColor(Qt::white));
    }
    QSGTexture *sgTexture = window()->createTextureFromImage(_img.scaled(boundingRect().size().toSize()));
    if(sgTexture){
        QSGTexture *tt = texture->texture();
        if(tt){
            tt->deleteLater();
        }
        texture->setRect(boundingRect());
        texture->setTexture(sgTexture);
    }
    return texture;
}

bool Screen::isOpen() const{
    return _isOpen;
}

qint32 Screen::model() const{
    return _model;
}

QImage Screen::img() const{
    return _img;
}

QString Screen::error() const{
    return _error;
}

void Screen::setIsOpen(bool flag){
    _isOpen = flag;
    if(_isOpen){
        switch (_model) {
        case 0:
            detect->start();
            break;
        case 1:
            reco->start();
            break;
        case 2:
            train->start();
            break;
        default:
            break;
        }
    }else{
        switch (_model) {
        case 0:
            detect->requestInterruption();
            detect->wait();
            break;
        case 1:
            reco->requestInterruption();
            reco->wait();
            break;
        case 2:
            train->requestInterruption();
            train->wait();
            break;
        default:
            break;
        }
    }
}

void Screen::setModel(qint32 model){
    _model = model;
}

void Screen::detectPhoto(const QString){

}

void Screen::recoPhoto(const QString src){
    cv::CascadeClassifier cascadeClassifier;
    if(!cascadeClassifier.load("assets/classifier/haarcascade_frontalface_alt.xml"))
    {
        qDebug("Error loading cascade file. Exiting!");
        return;
    }
    cv::Ptr<cv::face::LBPHFaceRecognizer> recognizer = cv::Algorithm::load<cv::face::LBPHFaceRecognizer>("train/train.yml");
    try{
        recognizer->read("train/train.yml");
    }catch(cv::Exception& e){

    }
    cv::Mat frame, frameGray;
    cv::Mat face, faceResize;
    cv::Mat equalizeFrame;
    std::vector<cv::Rect> faceRects;
    frame = cv::imread(src.toStdString());
    cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(frameGray, equalizeFrame);
    cascadeClassifier.detectMultiScale(equalizeFrame, faceRects, 1.1, 3, 0, cv::Size(30, 30));
    for(cv::Rect &i:faceRects){
        cv::rectangle(frame, i, cv::Scalar(0, 255, 0), 2, 8);
        face = equalizeFrame(i);
        cv::resize(face, faceResize, cv::Size(128,128), 0, 0, cv::INTER_LINEAR);
        recognizer->setThreshold(0.0);
        int predictedLabel = recognizer->predict(faceResize);
        std::cout<<predictedLabel<<std::endl;
        break;
    }
    _img = toQImage(frame);
    update();
}

void Screen::trainPhoto(const QString){

}

void Screen::setImg(QImage & srcImg){
    _img = srcImg;
    update();
}

void Screen::receiveFinish(){
    detect->requestInterruption();
    detect->wait();
    emit sendFinish();
}

void Screen::receiveError(QString str){
    _error = str;
    detect->quit();
    detect->wait();
    emit errorChanged();
}
