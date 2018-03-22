#include "player.h"
#include <QDebug>


Player::Player(QObject *parent)
 : QThread(parent)
{
    stop = true;
    camera =false;
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

        if(!capture.open("rtsp://fab:fab@192.168.0.20:554/live.sdp"))
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
            this->Play();
        }
    }else{
        int delay = (1000/frameRate);

        while(!stop){
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
}
