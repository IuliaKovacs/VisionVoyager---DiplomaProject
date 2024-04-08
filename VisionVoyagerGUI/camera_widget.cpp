#include "camera_widget.h"
#include <QPainter>

CameraWidget::CameraWidget(QWidget *parent) : QWidget(parent)
{
    initCamera();
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

void CameraWidget::initCamera()
{

    videoCapture = new cv::VideoCapture(0);
    if (!videoCapture->isOpened()) {
       /* @ToDo - Solve what to do if camera is not available! */
    }
    // Inițializare frame video
    *videoCapture >> frame;

    // Inițializare și pornire update continuu al frame-ului video
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        *videoCapture >> frame;
        update();
    });
    timer->start(30); // Interval de update în milisecunde
}
