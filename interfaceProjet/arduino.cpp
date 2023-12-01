#include "arduino.h"
#include <QSerialPortInfo>
#include <QDebug>


QString arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *arduino::getserial()
{
   return serial;
}

arduino::arduino()
{
    data="";
    arduino_port_name = "COM4";
    arduino_is_available=false;
    serial=new QSerialPort;
}

int arduino::connect_arduino()
{
        // Recherche du port sur lequel la carte Arduino est connectée
        foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
            if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
                if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                    serial_port_info.productIdentifier() == arduino_uno_producy_id) {
                    arduino_is_available = true;
                    arduino_port_name = serial_port_info.portName();
                }
            }
        }

        qDebug() << "arduino_port_name is :" << arduino_port_name;

        if (arduino_is_available) {
            // Configuration de la communication (débit, etc.)
            serial->setPortName(arduino_port_name);
            serial->setBaudRate(QSerialPort::Baud115200);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);

            // Tentative d'ouverture du port série
            if (serial->open(QSerialPort::ReadWrite)) {
                qDebug() << "Serial port opened successfully.";
                return 0;  // Port opened successfully
            } else {
                qDebug() << "Failed to open serial port. Error: " << serial->errorString();
                return 1;  // Failed to open port
            }
        }

        return -1;  // Arduino not available


}

int arduino::close_arduino()

{

    if(serial->isOpen()){
            serial->close();
            return 0;
        }
    return 1;


}


 QByteArray arduino::read_from_arduino()
{
    if(serial->isReadable()){
         data=serial->readAll(); //récupérer les données reçues

         return data;
    }



     return QByteArray(); // Return an empty QByteArray if no data is available
 }


int arduino::write_to_arduino( QByteArray d)

{

    if(serial->isWritable()){
        serial->write(d);  // envoyer des donnés vers Arduino
    }else{
        qDebug() << "Couldn't write to serial!";
    }


}
