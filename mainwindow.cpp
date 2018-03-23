#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    myPlayer = new Player();
    QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI(QImage)));
    //QObject::connect(myPlayer, SIGNAL(updateProgressBar(int)),
    //                          this, SLOT(updateProgressBar(int)));
    ui->setupUi(this);
}


void MainWindow::updatePlayerUI(QImage img){
    if (!img.isNull())
    {
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),
                                           Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}


MainWindow::~MainWindow(){
    delete myPlayer;
    delete ui;
}


void MainWindow::on_loadButton_clicked(){
    myPlayer->Stop();
    myPlayer->desactiverCamera();
    ui->playStopButton->setText(tr("Play"));
    QString filename = QFileDialog::getOpenFileName(this,
                                          tr("Open Video"), ".",
                                          tr("Video Files (*.avi *.mpg *.mp4)"));
    if (!filename.isEmpty()){
        if (!myPlayer->loadVideo(filename.toAscii().data()))
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
    }
}


void MainWindow::on_connectToCamera_clicked()
{

    ui->playStopButton->setText("Play");
    myPlayer->fromCamera();
}


void MainWindow::on_playStopButton_clicked()
{

    if (myPlayer->isStopped())
    {
        myPlayer->Play();
        ui->playStopButton->setText(tr("Stop"));
        ui->playStopButton->setIcon(QIcon("/home/rubens/stage/images/stop.png"));
    }else
    {
        myPlayer->Stop();
        ui->playStopButton->setText(tr("Play"));
        ui->playStopButton->setIcon(QIcon("/home/rubens/stage/images/go.png"));
    }
}

void MainWindow::on_saveVideo_clicked()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    qDebug() << fichier;
}
