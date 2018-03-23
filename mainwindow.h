#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <player.h>
#include <QtGui>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //Display video frame in player UI
    void updatePlayerUI(QImage img);
    //Slot for the load video push button.
    void on_loadButton_clicked();
    //Slot for connecting to camera
    void on_connectToCamera_clicked();
    // Slot for the play push button.
    void on_playStopButton_clicked();
    //TO save the video streaming of the camera
    void on_saveVideo_clicked();

private:
    Ui::MainWindow *ui;
    Player* myPlayer;
};

#endif // MAINWINDOW_H
