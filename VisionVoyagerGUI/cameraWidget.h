#ifndef CAMERA_WIDGET_H
#define CAMERA_WIDGET_H

#include <opencv2/opencv.hpp> 
#include <QWidget>
#include <QTimer>

#ifdef USE_SIMULATION
    #include <rclcpp/rclcpp.hpp>
    #include <sensor_msgs/msg/image.hpp>
    #include <cv_bridge/cv_bridge.hpp>
#endif



class CameraWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CameraWidget(QWidget *parent = nullptr);
    ~CameraWidget();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    cv::VideoCapture *videoCapture; 
    cv::Mat frame; 

#ifdef USE_SIMULATION
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr ros_subscription_;
    std::shared_ptr<rclcpp::Node> ros_node_;
#endif

    bool initCamera();
};

#endif // CAMERA_WIDGET_H
