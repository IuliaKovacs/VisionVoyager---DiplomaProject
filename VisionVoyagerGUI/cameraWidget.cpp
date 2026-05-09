#include "cameraWidget.h"
#include <QPainter>
#include "../ApplicationModule/application_utils.h"
#include "../VisionVoyagerMoves/visionVoyager.h"

#ifdef USE_SIMULATION
CameraWidget::CameraWidget(rclcpp::Node::SharedPtr node, QWidget *parent
#else
CameraWidget::CameraWidget(QWidget *parent
#endif
) :
#ifdef USE_SIMULATION
    ros_node_(node),
#endif  
    QWidget(parent)
{
    bool result = initCamera();

    if(false == result)
    {
        logFile << log_time() << "[Camera Module] Error: There is an error with initializing Camera Module (cameraWidget)" << endl;
    }
}

CameraWidget::~CameraWidget() 
{
    /* Destroy the subscription */
    ros_subscription_.reset(); 
    if (videoCapture) delete videoCapture;
}

void CameraWidget::paintEvent(QPaintEvent *event)
{
    if (frame.empty()) return;
    
    QPainter painter(this);
    QSize widgetSize = size();
    
    qreal scaleX = (qreal)widgetSize.width() / frame.cols;
    qreal scaleY = (qreal)widgetSize.height() / frame.rows;
    
    qreal scaleFactor = qMin(scaleX, scaleY);
    QImage scaledImage = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888)
                     .scaled(widgetSize, Qt::KeepAspectRatio)
                     .copy();
    
    painter.drawImage(QPoint(0, 0), scaledImage);    
}

/* @ToDo TTS - Solve what to do if camera is not available! */
bool CameraWidget::initCamera()
{
#ifndef USE_SIMULATION
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
#else    
    ros_subscription_ = ros_node_->create_subscription<sensor_msgs::msg::Image>(
        "/camera/image_raw", 
        10,
        [this](const sensor_msgs::msg::Image::SharedPtr msg) 
        {
            try {
                // cv_bridge face conversia automată la cv::Mat
                frame = cv_bridge::toCvCopy(msg, "bgr8")->image;
                update(); // Apelează paintEvent pentru a redesena widget-ul
            } catch (cv_bridge::Exception& e) {
                // Log error
            }
        });
#endif
    return true;
}
