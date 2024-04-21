#ifndef FILE_DROP_WIDGET_H
#define FILE_DROP_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <iostream>


class FileDropWidget : public QLabel
{
    Q_OBJECT

public:
    FileDropWidget(QWidget *parent = nullptr);
    void setAcceptsSingleFile(bool accept);
    QListWidgetItem getFirstItem();
    QListWidget* getFileListWidget();
    int getListCount();
    void clearFileList();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QListWidget *fileListWidget; 
    bool acceptsSingleFile;
};

#endif //FILE_DROP_WIDGET_H