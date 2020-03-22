/********************************************************************************
** Form generated from reading UI file 'Render_UI.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDER_UI_H
#define UI_RENDER_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Render_UIClass
{
public:
    QWidget *centralWidget;
    QPushButton *open_button;
    QPushButton *render_button;
    QPushButton *save_button;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSlider *horizontalSlider;
    QSpinBox *spinBox;
    QGraphicsView *graphicsView;
    QLabel *image_lable;
    QMenuBar *menuBar;
    QMenu *menux3d_render;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Render_UIClass)
    {
        if (Render_UIClass->objectName().isEmpty())
            Render_UIClass->setObjectName(QString::fromUtf8("Render_UIClass"));
        Render_UIClass->resize(600, 426);
        centralWidget = new QWidget(Render_UIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        open_button = new QPushButton(centralWidget);
        open_button->setObjectName(QString::fromUtf8("open_button"));
        open_button->setGeometry(QRect(20, 40, 91, 31));
        render_button = new QPushButton(centralWidget);
        render_button->setObjectName(QString::fromUtf8("render_button"));
        render_button->setGeometry(QRect(20, 80, 91, 31));
        save_button = new QPushButton(centralWidget);
        save_button->setObjectName(QString::fromUtf8("save_button"));
        save_button->setGeometry(QRect(20, 120, 91, 31));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(130, 310, 451, 24));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSlider = new QSlider(layoutWidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMinimum(2);
        horizontalSlider->setMaximum(12);
        horizontalSlider->setSliderPosition(8);
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);

        spinBox = new QSpinBox(layoutWidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(2);
        spinBox->setMaximum(12);
        spinBox->setValue(8);

        horizontalLayout->addWidget(spinBox);

        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(130, 10, 451, 281));
        image_lable = new QLabel(centralWidget);
        image_lable->setObjectName(QString::fromUtf8("image_lable"));
        image_lable->setGeometry(QRect(130, 10, 451, 281));
        Render_UIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Render_UIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menux3d_render = new QMenu(menuBar);
        menux3d_render->setObjectName(QString::fromUtf8("menux3d_render"));
        Render_UIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Render_UIClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Render_UIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Render_UIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Render_UIClass->setStatusBar(statusBar);

        menuBar->addAction(menux3d_render->menuAction());

        retranslateUi(Render_UIClass);
        QObject::connect(horizontalSlider, SIGNAL(sliderMoved(int)), spinBox, SLOT(setValue(int)));
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), horizontalSlider, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(Render_UIClass);
    } // setupUi

    void retranslateUi(QMainWindow *Render_UIClass)
    {
        Render_UIClass->setWindowTitle(QCoreApplication::translate("Render_UIClass", "Render_UI", nullptr));
        open_button->setText(QCoreApplication::translate("Render_UIClass", "Open", nullptr));
        render_button->setText(QCoreApplication::translate("Render_UIClass", "Render", nullptr));
        save_button->setText(QCoreApplication::translate("Render_UIClass", "Save", nullptr));
        label->setText(QCoreApplication::translate("Render_UIClass", "Rendering deep", nullptr));
        image_lable->setText(QString());
        menux3d_render->setTitle(QCoreApplication::translate("Render_UIClass", "Render", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Render_UIClass: public Ui_Render_UIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDER_UI_H
