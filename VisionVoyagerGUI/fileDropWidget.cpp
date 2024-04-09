#include "fileDropWidget.h"
#include <iostream>
#include <QVBoxLayout>

using namespace std;

FileDropWidget::FileDropWidget(QWidget  *parent)
    : QLabel(parent)
{
    setAcceptDrops(true);

    QVBoxLayout *innerLayout = new QVBoxLayout(this); 
    fileListWidget = new QListWidget(this); 
    innerLayout->addWidget(fileListWidget); 
}

void FileDropWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) 
    {
        event->acceptProposedAction();
    }
}

void FileDropWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls()) 
    {
        event->acceptProposedAction();
    }
}

void FileDropWidget::dropEvent(QDropEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();

    for (const QUrl &url : urls) 
    {
        QString filePath = url.toLocalFile();
        fileListWidget->addItem(filePath);
    }
    event->acceptProposedAction();
}