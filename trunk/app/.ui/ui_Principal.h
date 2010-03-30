/********************************************************************************
** Form generated from reading ui file 'Principal.ui'
**
** Created: Fri Mar 26 10:46:06 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PRINCIPAL_H
#define UI_PRINCIPAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_Principal
{
public:
    QHBoxLayout *hboxLayout;
    GLWidget *gLWidget1;
    QVBoxLayout *vboxLayout;
    QSpacerItem *spacerItem;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem1;
    QPushButton *pushButton1;

    void setupUi(QWidget *Principal)
    {
        if (Principal->objectName().isEmpty())
            Principal->setObjectName(QString::fromUtf8("Principal"));
        Principal->resize(764, 581);
        hboxLayout = new QHBoxLayout(Principal);
        hboxLayout->setSpacing(6);
        hboxLayout->setMargin(11);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        gLWidget1 = new GLWidget(Principal);
        gLWidget1->setObjectName(QString::fromUtf8("gLWidget1"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gLWidget1->sizePolicy().hasHeightForWidth());
        gLWidget1->setSizePolicy(sizePolicy);
        gLWidget1->setMinimumSize(QSize(100, 100));

        hboxLayout->addWidget(gLWidget1);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setMargin(11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        spacerItem = new QSpacerItem(20, 365, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setMargin(11);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacerItem1 = new QSpacerItem(101, 31, QSizePolicy::Fixed, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);

        pushButton1 = new QPushButton(Principal);
        pushButton1->setObjectName(QString::fromUtf8("pushButton1"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton1->sizePolicy().hasHeightForWidth());
        pushButton1->setSizePolicy(sizePolicy1);
        pushButton1->setMinimumSize(QSize(75, 0));

        hboxLayout1->addWidget(pushButton1);


        vboxLayout->addLayout(hboxLayout1);


        hboxLayout->addLayout(vboxLayout);


        retranslateUi(Principal);
        QObject::connect(pushButton1, SIGNAL(clicked()), Principal, SLOT(close()));

        QMetaObject::connectSlotsByName(Principal);
    } // setupUi

    void retranslateUi(QWidget *Principal)
    {
        Principal->setWindowTitle(QApplication::translate("Principal", "Form1", 0, QApplication::UnicodeUTF8));
        pushButton1->setText(QApplication::translate("Principal", "&Sortir", 0, QApplication::UnicodeUTF8));
        pushButton1->setShortcut(QApplication::translate("Principal", "Alt+S", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Principal);
    } // retranslateUi

};

namespace Ui {
    class Principal: public Ui_Principal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINCIPAL_H
