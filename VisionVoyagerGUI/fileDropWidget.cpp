#include "fileDropWidget.h"
#include "adminModeWindow.h"
#include <iostream>
#include <QVBoxLayout>

using namespace std;

FileDropWidget::FileDropWidget(QWidget  *parent)
    : QLabel(parent)
{
    setAcceptDrops(true);

    setStyleSheet("background-image: url(../VisionVoyagerGUI/dragNdropBG.png); "
                    "background-repeat: no-repeat; "
                    "background-position: center; "
                    "background-origin: content-box;");

    QVBoxLayout *innerLayout = new QVBoxLayout(this); 

    fileListWidget = new QListWidget(this); 
    fileListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    
    acceptsSingleFile = false;
    innerLayout->addWidget(fileListWidget); 
}

void FileDropWidget::setAcceptsSingleFile(bool accept) 
{
    acceptsSingleFile = accept;
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
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) 
    {
        event->acceptProposedAction();
        QList<QUrl> urls = mimeData->urls();
        if (acceptsSingleFile && (fileListWidget->count() == 1))
        {
            /* Do not accept item, there is already one in place */
        }
        else if(acceptsSingleFile && (urls.size() == 1)) 
        {   
            QString filePath = urls.at(0).toLocalFile();
            fileListWidget->addItem(filePath); 
        } 
        else 
        {   
            for (const QUrl &url : urls) 
            {
                QString filePath = url.toLocalFile();
                fileListWidget->addItem(filePath); 
            }
        }
    } 
    else 
    {
        QLabel::dropEvent(event);
    }
}

QListWidgetItem FileDropWidget::getFirstItem()
{
    return *(fileListWidget->item(0));
}

int FileDropWidget::getListCount()
{
    return fileListWidget->count();
}

void FileDropWidget::clearFileList()
{
    fileListWidget->clear();
}