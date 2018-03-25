#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    myPlayer = new Player();
    QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI(QImage)));
    QObject::connect(myPlayer, SIGNAL(updateProgressBarSignal(int)),
                              this, SLOT(updateProgressBar(int)));
    //QObject::connect(myPlayer, SIGNAL(updateProgressBar(int)),
    //                          this, SLOT(updateProgressBar(int)));
    ui->setupUi(this);
    //this->setFixedSize();
    this->setWindowTitle("Application de detection et de suivi d'objets dans une scene");
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
    ui->connectToCamera->setEnabled(true);
    //ui->loadButton->setEnabled(false);
    ui->playStopButton->setEnabled(true);
    ui->saveVideo->setEnabled(false);

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
        ui->progressBar->setVisible(true);
        }else{
            ui->loadButton->setEnabled(true);
            ui->connectToCamera->setEnabled(true);
            ui->comboBox->setEnabled(false);
            ui->saveVideo->setEnabled(false);
            ui->playStopButton->setText(tr("Play"));
            ui->playStopButton->setIcon(QIcon("/home/rubens/stage/images/go.png"));
            ui->playStopButton->setEnabled(false);
        }
}


void MainWindow::on_connectToCamera_clicked()
{
    ui->loadButton->setEnabled(true);
    ui->connectToCamera->setEnabled(false);
    ui->playStopButton->setEnabled(true);
    ui->saveVideo->setEnabled(true);
    ui->progressBar->setVisible(false);
    ui->comboBox->setEnabled(true);

    ui->playStopButton->setText("Play");
    myPlayer->fromCamera();
}


void MainWindow::on_playStopButton_clicked()
{
    ui->playStopButton->setEnabled(true);

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
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer la video", QString(), "Videos (*.avi )");
    qDebug() << fichier;
    myPlayer->setPathToSave(fichier);

}

void MainWindow::updateProgressBar(int t){
   ui->progressBar->setValue(t);
}
