#include "cameraWidget.h"
#include <QPainter>
#include "../ApplicationModule/application_utils.h"

CameraWidget::CameraWidget(QWidget *parent) : QWidget(parent)
{
    bool result = initCamera();

    if(false == result)
    {
        logFile << log_time() << "[Camera Module] Error: There is an error with initializing Camera Module (cameraWidget)" << endl;
    }
}

CameraWidget::~CameraWidget() 
{
    delete videoCapture;
}

void CameraWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QSize widgetSize = size();
    
    qreal scaleX = (qreal)widgetSize.width() / frame.cols;
    qreal scaleY = (qreal)widgetSize.height() / frame.rows;
    
    qreal scaleFactor = qMin(scaleX, scaleY);
    QImage scaledImage = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888).scaled(widgetSize, Qt::KeepAspectRatio);
    
    painter.drawImage(QPoint(0, 0), scaledImage);    
}

/* @ToDo TTS - Solve what to do if camera is not available! */
bool CameraWidget::initCamera()
{

    videoCapture = new cv::VideoCapture(0);
    if (!videoCapture->isOpened()) 
    {
       return false;
    }

    *videoCapture >> frame;

    QTimer *timer = new QTimer(this);
    
    connect(timer, &QTimer::timeout, [this]() 
    {
        *videoCapture >> frame;
        update();
    });
    timer->start(30); 
    return true;
}
