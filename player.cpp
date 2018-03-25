#include "player.h"
#include <QDebug>
#include <iostream>

using namespace std;


Player::Player(QObject *parent)
 : QThread(parent)
{
    stop = true;
    camera =false;
    pathToSave = "../savedVideos/out.avi";
    adresseCamera = "rtsp://fab:fab@192.168.0.20:554/live.sdp";
}

bool Player::loadVideo(string filename) {
    capture.open(filename);
    if (capture.isOpened())
    {
        frameRate = (int) capture.get(CV_CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

//Methode fromCamera
void Player::fromCamera(){

   camera = true;
   //capture.open(adresseCamera);
   capture.open(0);
   //capture.open(0);
   frame_width=   capture.get(CV_CAP_PROP_FRAME_WIDTH);
   frame_height=   capture.get(CV_CAP_PROP_FRAME_HEIGHT);
   videoWriter = VideoWriter(pathToSave, CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height),true);

}

void Player::Play()
{
    if (!isRunning()) {
        if (isStopped()){
            stop = false;
        }
        start(LowPriority);
    }
}

void Player::run()
{
    if(camera){
        //capture.release();
        //this->Stop();

        if(!capture.isOpened())
                return ;

        for(;;)
        {
            if(isStopped()){
                break;
            }

            Mat frame;
            capture >> frame;
            if (frame.channels()== 3){
                cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
                img = QImage((const unsigned char*)(RGBframe.data),
                                  RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
            }
            else
            {
                img = QImage((const unsigned char*)(frame.data),
                                     frame.cols,frame.rows,QImage::Format_Indexed8);
            }
            emit processedImage(img);
            videoWriter.write(frame);
            this->Play();
        }
    }else{
        int delay = (1000/frameRate);
        int time = (int) ((capture.get(CV_CAP_PROP_POS_FRAMES)/capture.get(CV_CAP_PROP_FRAME_COUNT))*100);

        while(!stop){

            time = (int) ((capture.get(CV_CAP_PROP_POS_FRAMES)/capture.get(CV_CAP_PROP_FRAME_COUNT))*100);
            //cout << time;

            if (!capture.read(frame))
            {
                stop = true;
            }
            if (frame.channels()== 3){
                cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
                img = QImage((const unsigned char*)(RGBframe.data),
                                  RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
            }
            else
            {
                img = QImage((const unsigned char*)(frame.data),
                                     frame.cols,frame.rows,QImage::Format_Indexed8);
            }
            emit processedImage(img);
            emit updateProgressBarSignal(time);
            this->msleep(delay);
        }
    }

}


Player::~Player()
{
    mutex.lock();
    stop = true;
    capture.release();
    condition.wakeOne();
    mutex.unlock();
    wait();
}


void Player::Stop()
{
    stop = true;
}


void Player::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}


bool Player::isStopped() const{
    return this->stop;
}

void Player::desactiverCamera(){
    camera = false;
    capture.release();
}

void Player::activerCamera(){
    camera = true;

}

void Player::setPathToSave(QString p){
    this->pathToSave = p.toStdString();
    //cout << pathToSave;
    videoWriter = VideoWriter(pathToSave, CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height),true);
}
