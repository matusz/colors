#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <stdint.h>

class QSettings;

class QLabel;
class QSlider;

class QKeyEvent;
class QResizeEvent;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void redValueChanged(int redValue);
    void greenValueChanged(int greenValue);
    void blueValueChanged(int blueValue);

protected:
    void keyPressEvent(QKeyEvent * event);
    void resizeEvent(QResizeEvent * event);

private:
    Ui::MainWindow *ui;

    QSettings* pSettings_;

    uint8_t redValue_;
    uint8_t greenValue_;
    uint8_t blueValue_;

    QLabel* pRedLabel_;
    QLabel* pGreenLabel_;
    QLabel* pBlueLabel_;

    QLabel* pRedValueLabel_;
    QLabel* pGreenValueLabel_;
    QLabel* pBlueValueLabel_;

    QSlider* pRedSlider_;
    QSlider* pGreenSlider_;
    QSlider* pBlueSlider_;
};

#endif // MAINWINDOW_H
