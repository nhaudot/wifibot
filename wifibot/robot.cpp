#include "robot.h"

Robot::Robot(QObject *parent) : QObject(parent) {
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x0;
    DataToSend[3] = 0x0;
    DataToSend[4] = 0x0;
    DataToSend[5] = 0x0;
    DataToSend[6] = 0x0;
    DataToSend[7] = 0x0;
    DataToSend[8] = 0x0;
    DataReceived.resize(21);
    TimerEnvoi = new QTimer();
    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer
}


void Robot::doConnect() {
    socket = new QTcpSocket(this); // socket creation
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    qDebug() << "connecting..."; // this is not blocking call
    //socket->connectToHost("LOCALHOST", 15020);
    socket->connectToHost("192.168.1.11", 15020); // connection to wifibot
    // we need to wait...
    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return;
    }
    TimerEnvoi->start(75);
}

void Robot::disConnect() {
    TimerEnvoi->stop();
    socket->close();
}

void Robot::connected() {
    qDebug() << "connected..."; // Hey server, tell me about you.
}

void Robot::disconnected() {
    qDebug() << "disconnected...";
}

void Robot::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void Robot::readyRead() {
    qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll();
    emit updateUI(DataReceived);
    qDebug() << DataReceived[0] << DataReceived[1] << DataReceived[2];
}

void Robot::MyTimerSlot() {
    qDebug() << "Timer...";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}

short Robot::Crc16(unsigned char *_Adresse_tab, unsigned char Taille_Max){
    unsigned int Crc = 0xFFFF;
    unsigned int Polynome = 0xA001;
    unsigned int CptOctet = 0;
    unsigned int CptBit = 0;
    unsigned int Parity = 0;

    Crc = 0xFFFF;
    Polynome = 0xA001;
    for (CptOctet=0; CptOctet<Taille_Max;CptOctet++){
        Crc ^= *(_Adresse_tab + CptOctet);

        for(CptBit=0;CptBit<=7;CptBit++){
            Parity = Crc;
            Crc >>=1;
            if(Parity%2 == true) Crc ^= Polynome;
        }
    }
    return (Crc);
}


void Robot::Avant(short speed1, short speed2){
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = (unsigned char) speed1;
    DataToSend[3] = (unsigned char)(speed1 >> 8);
    DataToSend[4] = (unsigned char) speed2;
    DataToSend[5] = (unsigned char)(speed2 >> 8);
    DataToSend[6] = (unsigned char)(80+1);
    short mycrcsend = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char) mycrcsend;
    DataToSend[8] = (unsigned char) (mycrcsend >> 8);

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
}

void Robot::Arriere(short speed1, short speed2){
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = (unsigned char) speed1;
    DataToSend[3] = (unsigned char)(speed1 >> 8);
    DataToSend[4] = (unsigned char) speed2;
    DataToSend[5] = (unsigned char)(speed2 >> 8);
    DataToSend[6] = (unsigned char)(0+1);
    short mycrcsend = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char) mycrcsend;
    DataToSend[8] = (unsigned char) (mycrcsend >> 8);

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
}

void Robot::Droite(short speed1, short speed2){
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = (unsigned char) speed1;
    DataToSend[3] = (unsigned char)(speed1 >> 8);
    DataToSend[4] = (unsigned char) speed2;
    DataToSend[5] = (unsigned char)(speed2 >> 8);
    DataToSend[6] = (unsigned char)(64+1);
    short mycrcsend = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char) mycrcsend;
    DataToSend[8] = (unsigned char) (mycrcsend >> 8);

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
}

void Robot::Gauche(short speed1, short speed2){
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = (unsigned char) speed1;
    DataToSend[3] = (unsigned char)(speed1 >> 8);
    DataToSend[4] = (unsigned char) speed2;
    DataToSend[5] = (unsigned char)(speed2 >> 8);
    DataToSend[6] = (unsigned char)(16+1);
    short mycrcsend = Crc16((unsigned char *)DataToSend.data()+1,6);
    DataToSend[7] = (unsigned char) mycrcsend;
    DataToSend[8] = (unsigned char) (mycrcsend >> 8);

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
}

void Robot::Stop(){
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x00;
    DataToSend[3] = 0x00;
    DataToSend[4] = 0x00;
    DataToSend[5] = 0x00;
    DataToSend[6] = 0x00;
    DataToSend[7] = 0x00;
    DataToSend[8] = 0x00;

    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
}
