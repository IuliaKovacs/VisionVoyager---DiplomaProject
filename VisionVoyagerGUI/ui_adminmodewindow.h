/********************************************************************************
** Form generated from reading UI file 'adminmodewindowUjidTH.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ADMINMODEWINDOWUJIDTH_H
#define ADMINMODEWINDOWUJIDTH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminModeWindow
{
public:
    QWidget *main_window;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *layout_2;
    QPushButton *routeRegistrationButton;

    void setupUi(QMainWindow *AdminModeWindow)
    {
        if (AdminModeWindow->objectName().isEmpty())
            AdminModeWindow->setObjectName("AdminModeWindow");
        AdminModeWindow->resize(788, 600);
        AdminModeWindow->setStyleSheet(QString::fromUtf8(""));
        main_window = new QWidget(AdminModeWindow);
        main_window->setObjectName("main_window");
        horizontalLayoutWidget = new QWidget(main_window);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(70, 110, 400, 200));
        layout_2 = new QHBoxLayout(horizontalLayoutWidget);
        layout_2->setObjectName("layout_2");
        layout_2->setContentsMargins(0, 0, 0, 0);
        routeRegistrationButton = new QPushButton(main_window);
        routeRegistrationButton->setObjectName("routeRegistrationButton");
        routeRegistrationButton->setGeometry(QRect(540, 110, 200, 41));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(routeRegistrationButton->sizePolicy().hasHeightForWidth());
        routeRegistrationButton->setSizePolicy(sizePolicy);
        AdminModeWindow->setCentralWidget(main_window);

        retranslateUi(AdminModeWindow);

        QMetaObject::connectSlotsByName(AdminModeWindow);
    } // setupUi

    void retranslateUi(QMainWindow *AdminModeWindow)
    {
        AdminModeWindow->setWindowTitle(QCoreApplication::translate("AdminModeWindow", "AdminModeWindow", nullptr));
        routeRegistrationButton->setText(QCoreApplication::translate("AdminModeWindow", "Route Registration", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminModeWindow: public Ui_AdminModeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ADMINMODEWINDOWUJIDTH_H
