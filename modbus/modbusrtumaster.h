/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MODBUSRTUMASTER_H
#define MODBUSRTUMASTER_H

#include <QObject>
#include <QtSerialBus>
#include <QSerialPort>
#include <QTimer>
#include <QUuid>

class ModbusRTUMaster : public QObject
{
    Q_OBJECT
public:
    explicit ModbusRTUMaster(QString serialPort, uint baudrate, QSerialPort::Parity parity, uint dataBits, uint stopBits, QObject *parent = nullptr);
    ~ModbusRTUMaster();

    bool connectDevice();
    void setNumberOfRetries(int number);
    void setTimeout(int timeout);

    int timeout();
    int numberOfRetries();

    QUuid readCoil(uint slaveAddress, uint registerAddress, uint size = 1);
    QUuid readDiscreteInput(uint slaveAddress, uint registerAddress, uint size = 1);
    QUuid readInputRegister(uint slaveAddress, uint registerAddress, uint size = 1);
    QUuid readHoldingRegister(uint slaveAddress, uint registerAddress, uint size = 1);

    QUuid writeCoil(uint slaveAddress, uint registerAddress, bool status);
    QUuid writeCoils(uint slaveAddress, uint registerAddress, const QVector<quint16> &values);

    QUuid writeHoldingRegister(uint slaveAddress, uint registerAddress, quint16 value);
    QUuid writeHoldingRegisters(uint slaveAddress, uint registerAddress, const QVector<quint16> &values);

    QString serialPort();

private:
    QModbusRtuSerialMaster *m_modbusRtuSerialMaster;
    QTimer *m_reconnectTimer = nullptr;

private slots:
    void onReconnectTimer();

    void onModbusErrorOccurred(QModbusDevice::Error error);
    void onModbusStateChanged(QModbusDevice::State state);

signals:
    void connectionStateChanged(bool status);

    void requestExecuted(QUuid requestId, bool success);
    void requestError(QUuid requestId, const QString &error);

    void receivedCoil(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values);
    void receivedDiscreteInput(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values);
    void receivedHoldingRegister(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values);
    void receivedInputRegister(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values);
};

#endif // MODBUSRTUMASTER_H
