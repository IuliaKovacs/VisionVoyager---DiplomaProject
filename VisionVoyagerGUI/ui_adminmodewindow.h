/********************************************************************************
** Form generated from reading UI file 'adminmodewindowNdRzQg.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ADMINMODEWINDOWNDRZQG_H
#define ADMINMODEWINDOWNDRZQG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminModeWindow
{
public:
    QWidget *main_window;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *layout_2;
    QPushButton *routeRegistrationButton;
    QLineEdit *lineEdit;
    QWidget *tab_2;
    QPushButton *pushButton;
    QLabel *label;
    QWidget *tab_3;

    void setupUi(QMainWindow *AdminModeWindow)
    {
        if (AdminModeWindow->objectName().isEmpty())
            AdminModeWindow->setObjectName("AdminModeWindow");
        AdminModeWindow->resize(795, 610);
        AdminModeWindow->setStyleSheet(QString::fromUtf8(""));
        main_window = new QWidget(AdminModeWindow);
        main_window->setObjectName("main_window");
        tabWidget = new QTabWidget(main_window);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(-10, 0, 801, 621));
        tab = new QWidget();
        tab->setObjectName("tab");
        horizontalLayoutWidget = new QWidget(tab);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(50, 100, 301, 121));
        layout_2 = new QHBoxLayout(horizontalLayoutWidget);
        layout_2->setObjectName("layout_2");
        layout_2->setContentsMargins(0, 0, 0, 0);
        routeRegistrationButton = new QPushButton(tab);
        routeRegistrationButton->setObjectName("routeRegistrationButton");
        routeRegistrationButton->setGeometry(QRect(490, 100, 151, 41));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(routeRegistrationButton->sizePolicy().hasHeightForWidth());
        routeRegistrationButton->setSizePolicy(sizePolicy);
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(490, 180, 151, 31));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        pushButton = new QPushButton(tab_2);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(50, 250, 83, 29));
        label = new QLabel(tab_2);
        label->setObjectName("label");
        label->setGeometry(QRect(190, 100, 63, 20));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        tabWidget->addTab(tab_3, QString());
        AdminModeWindow->setCentralWidget(main_window);

        retranslateUi(AdminModeWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(AdminModeWindow);
    } // setupUi

    void retranslateUi(QMainWindow *AdminModeWindow)
    {
        AdminModeWindow->setWindowTitle(QCoreApplication::translate("AdminModeWindow", "AdminModeWindow", nullptr));
        routeRegistrationButton->setText(QCoreApplication::translate("AdminModeWindow", "Route Registration", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("AdminModeWindow", "Manual Mode", nullptr));
        pushButton->setText(QCoreApplication::translate("AdminModeWindow", "PushButton", nullptr));
        label->setText(QCoreApplication::translate("AdminModeWindow", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("AdminModeWindow", "Add Recognized Face", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("AdminModeWindow", "Statistics", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminModeWindow: public Ui_AdminModeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ADMINMODEWINDOWNDRZQG_H
