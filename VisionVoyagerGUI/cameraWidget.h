#ifndef CAMERA_WIDGET_H
#define CAMERA_WIDGET_H

#include <opencv2/opencv.hpp> 
#include <QWidget>
#include <QTimer>


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

    void initCamera();
};

#endif // CAMERA_WIDGET_H
