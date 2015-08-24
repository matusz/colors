#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QSettings>

#include <QVBoxLayout>
#include <QGridLayout>

#include <QPalette>

#include <QLabel>
#include <QSlider>

#include <QKeyEvent>
#include <QResizeEvent>

const QString redValueKey = "red";
const QString greenValueKey = "green";
const QString blueValueKey = "blue";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pSettings_(NULL),
    redValue_(0),
    greenValue_(0),
    blueValue_(0),
    pRedLabel_(NULL),
    pGreenLabel_(NULL),
    pBlueLabel_(NULL),
    pRedValueLabel_(NULL),
    pGreenValueLabel_(NULL),
    pBlueValueLabel_(NULL),
    pRedSlider_(NULL),
    pGreenSlider_(NULL),
    pBlueSlider_(NULL)
{
    ui->setupUi(this);

    QString settingsFilePath = QDir::homePath() + "/.colorsrc";
    pSettings_ = new QSettings(settingsFilePath, QSettings::NativeFormat, this);

    redValue_ = static_cast<uint8_t>(pSettings_->value(redValueKey).toInt());
    greenValue_ = static_cast<uint8_t>(pSettings_->value(greenValueKey).toInt());
    blueValue_ = static_cast<uint8_t>(pSettings_->value(blueValueKey).toInt());

    // hides all main window normal GUI related stuff
    QObjectList childObjects = children();
    foreach (QObject* pChildObject, childObjects) {
        QToolBar* pToolBar = qobject_cast<QToolBar*>(pChildObject);
        if (pToolBar)
            pToolBar->setVisible(false);

        QStatusBar* pStatusBar = qobject_cast<QStatusBar*>(pChildObject);
        if (pStatusBar)
            pStatusBar->setVisible(false);

        QMenuBar* pMenuBar = qobject_cast<QMenuBar*>(pChildObject);
        if (pMenuBar)
            pMenuBar->setVisible(false);
    }

    QWidget* pCentralWidget = centralWidget();
    if (!pCentralWidget) {
        pCentralWidget = new QWidget(this);
        setCentralWidget(pCentralWidget);
    }

    // creates a vertical layout what will contain
    // a vertical spaces and a grid layout for sliders
    // and labels
    QVBoxLayout* pMainVBoxLayout = new QVBoxLayout();
    pMainVBoxLayout->setSpacing(0);

    pCentralWidget->setLayout(pMainVBoxLayout);

    pMainVBoxLayout->addStretch();

    QGridLayout* pControlGridLayout = new QGridLayout();
    pControlGridLayout->setHorizontalSpacing(0);
    pControlGridLayout->setVerticalSpacing(0);

    pMainVBoxLayout->addLayout(pControlGridLayout);

    pRedLabel_ = new QLabel(this);

    QFont redFont = pRedLabel_->font();
    redFont.setPointSize(32);
    redFont.setBold(true);
    pRedLabel_->setFont(redFont);

    pRedLabel_->setText(QString::fromUtf8("Piros - Red: "));

    pRedValueLabel_ = new QLabel(this);
    pRedValueLabel_->setFont(redFont);

    pRedValueLabel_->setText(QString("(%1)").arg(redValue_, 3, 10, QChar('0')));

    pGreenLabel_ = new QLabel(this);

    QFont greenFont = pGreenLabel_->font();
    greenFont.setPointSize(32);
    greenFont.setBold(true);
    pGreenLabel_->setFont(greenFont);

    pGreenLabel_->setText(QString::fromUtf8("Zöld - Green: "));

    pGreenValueLabel_ = new QLabel(this);
    pGreenValueLabel_->setFont(greenFont);

    pGreenValueLabel_->setText(QString("(%1)").arg(greenValue_, 3, 10, QChar('0')));

    pBlueLabel_ = new QLabel(this);

    QFont blueFont = pBlueLabel_->font();
    blueFont.setPointSize(32);
    blueFont.setBold(true);
    pBlueLabel_->setFont(blueFont);

    pBlueLabel_->setText(QString::fromUtf8("Kék - Blue: "));

    pBlueValueLabel_ = new QLabel(this);
    pBlueValueLabel_->setFont(blueFont);

    pRedSlider_ = new QSlider(Qt::Horizontal, this);
    pRedSlider_->setRange(0, 255);

    pGreenSlider_ = new QSlider(Qt::Horizontal, this);
    pGreenSlider_->setRange(0, 255);

    pBlueSlider_ = new QSlider(Qt::Horizontal, this);
    pBlueSlider_->setRange(0, 255);

    pControlGridLayout->addWidget(pRedLabel_, 0, 0);
    pControlGridLayout->addWidget(pGreenLabel_, 1, 0);
    pControlGridLayout->addWidget(pBlueLabel_, 2, 0);

    pControlGridLayout->addWidget(pRedValueLabel_, 0, 1);
    pControlGridLayout->addWidget(pGreenValueLabel_, 1, 1);
    pControlGridLayout->addWidget(pBlueValueLabel_, 2, 1);

    pControlGridLayout->addWidget(pRedSlider_, 0, 2);
    pControlGridLayout->addWidget(pGreenSlider_, 1, 2);
    pControlGridLayout->addWidget(pBlueSlider_, 2, 2);

    pControlGridLayout->setColumnStretch(0, 0);
    pControlGridLayout->setColumnStretch(1, 0);
    pControlGridLayout->setColumnStretch(2, 1);

    QPalette palette;
    palette.setColor(backgroundRole(), QColor(redValue_, greenValue_, blueValue_, 255));
    setPalette(palette);

    connect(pRedSlider_, SIGNAL(valueChanged(int)), this, SLOT(redValueChanged(int)));
    connect(pGreenSlider_, SIGNAL(valueChanged(int)), this, SLOT(greenValueChanged(int)));
    connect(pBlueSlider_, SIGNAL(valueChanged(int)), this, SLOT(blueValueChanged(int)));

    showFullScreen();

    pRedSlider_->setValue(redValue_);
    pGreenSlider_->setValue(greenValue_);
    pBlueSlider_->setValue(blueValue_);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::redValueChanged(int redValue)
{
    redValue_ = static_cast<uint8_t>(redValue);

    pSettings_->setValue(redValueKey, redValue);
    pSettings_->sync();

    pRedValueLabel_->setText(QString("(%1)").arg(redValue_, 3, 10, QChar('0')));

    QString redString = QString("rgb(%1, 0, 0)").arg(redValue_);

    pRedLabel_->setStyleSheet(QString("QLabel { background-color: %1; color: white; }").arg(redString));
    pRedValueLabel_->setStyleSheet(QString("QLabel { background-color: %1; color: white; }").arg(redString));

    int redSliderHeight = (pRedValueLabel_->size().height() > pRedLabel_->size().height()) ? pRedValueLabel_->size().height() : pRedLabel_->size().height();
    pRedSlider_->setStyleSheet(QString("QSlider { height: %1px; } QSlider::groove:horizontal { background-color: %2; } QSlider::handle:horizontal { background: #d0d0d0; width: %3px; }").arg(redSliderHeight).arg(redString).arg(redSliderHeight));

    QPalette palette;
    palette.setColor(backgroundRole(), QColor(redValue_, greenValue_, blueValue_, 255));
    setPalette(palette);
}

void MainWindow::greenValueChanged(int greenValue)
{
    greenValue_ = static_cast<uint8_t>(greenValue);

    pSettings_->setValue(greenValueKey, greenValue);
    pSettings_->sync();

    pGreenValueLabel_->setText(QString("(%1)").arg(greenValue_, 3, 10, QChar('0')));

    QString greenString = QString("rgb(0, %1, 0)").arg(greenValue_);

    pGreenLabel_->setStyleSheet(QString("QLabel { background-color: %1; color: white; }").arg(greenString));
    pGreenValueLabel_->setStyleSheet(QString("QLabel { background-color: %1; color: white; }").arg(greenString));

    int greenSliderHeight = (pGreenValueLabel_->size().height() > pGreenLabel_->size().height()) ? pGreenValueLabel_->size().height() : pGreenLabel_->size().height();
    pGreenSlider_->setStyleSheet(QString("QSlider { height: %1px; } QSlider::groove:horizontal { background-color: %2; } QSlider::handle:horizontal { background: #d0d0d0; width: %3px; }").arg(greenSliderHeight).arg(greenString).arg(greenSliderHeight));

    QPalette palette;
    palette.setColor(backgroundRole(), QColor(redValue_, greenValue_, blueValue_, 255));
    setPalette(palette);
}

void MainWindow::blueValueChanged(int blueValue)
{
    blueValue_ = static_cast<uint8_t>(blueValue);

    pSettings_->setValue(blueValueKey, blueValue);
    pSettings_->sync();

    pBlueValueLabel_->setText(QString("(%1)").arg(blueValue_, 3, 10, QChar('0')));

    QString blueString = QString("rgb(0, 0, %1)").arg(blueValue_);

    pBlueLabel_->setStyleSheet(QString("QLabel { background-color: %1; color: white; }").arg(blueString));
    pBlueValueLabel_->setStyleSheet(QString("QLabel { background-color: %1; color: white; }").arg(blueString));

    int blueSliderHeight = (pBlueValueLabel_->size().height() > pBlueLabel_->size().height()) ? pBlueValueLabel_->size().height() : pBlueLabel_->size().height();
    pBlueSlider_->setStyleSheet(QString("QSlider { height: %1px; } QSlider::groove:horizontal { background-color: %2; } QSlider::handle:horizontal { background: #d0d0d0; width: %3px; }").arg(blueSliderHeight).arg(blueString).arg(blueSliderHeight));

    QPalette palette;
    palette.setColor(backgroundRole(), QColor(redValue_, greenValue_, blueValue_, 255));
    setPalette(palette);
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    QMainWindow::keyPressEvent(event);

    switch (event->key()) {
    case Qt::Key_Escape : {
        close();
        break;
    }
    }
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);

    redValueChanged(redValue_);
    greenValueChanged(greenValue_);
    blueValueChanged(blueValue_);
}
