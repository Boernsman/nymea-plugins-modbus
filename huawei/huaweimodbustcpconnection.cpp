/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
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


#include "huaweimodbustcpconnection.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcHuaweiModbusTcpConnection, "HuaweiModbusTcpConnection")

HuaweiModbusTcpConnection::HuaweiModbusTcpConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :
    ModbusTCPMaster(hostAddress, port, parent),
    m_slaveId(slaveId)
{
    
}

float HuaweiModbusTcpConnection::inverterActivePower() const
{
    return m_inverterActivePower;
}

float HuaweiModbusTcpConnection::inverterGridFrequency() const
{
    return m_inverterGridFrequency;
}

float HuaweiModbusTcpConnection::inverterInternalTemperature() const
{
    return m_inverterInternalTemperature;
}

HuaweiModbusTcpConnection::InverterDeviceStatus HuaweiModbusTcpConnection::inverterDeviceStatus() const
{
    return m_inverterDeviceStatus;
}

float HuaweiModbusTcpConnection::inverterAccumulatedEnergyYield() const
{
    return m_inverterAccumulatedEnergyYield;
}

float HuaweiModbusTcpConnection::inverterDailyEnergyYield() const
{
    return m_inverterDailyEnergyYield;
}

qint32 HuaweiModbusTcpConnection::powerMeterActivePower() const
{
    return m_powerMeterActivePower;
}

QString HuaweiModbusTcpConnection::model() const
{
    return m_model;
}

QString HuaweiModbusTcpConnection::serialNumber() const
{
    return m_serialNumber;
}

QString HuaweiModbusTcpConnection::productNumber() const
{
    return m_productNumber;
}

quint16 HuaweiModbusTcpConnection::modelId() const
{
    return m_modelId;
}

quint16 HuaweiModbusTcpConnection::numberOfPvString() const
{
    return m_numberOfPvString;
}

quint16 HuaweiModbusTcpConnection::numberOfMppTracks() const
{
    return m_numberOfMppTracks;
}

float HuaweiModbusTcpConnection::inverterVoltagePhaseA() const
{
    return m_inverterVoltagePhaseA;
}

float HuaweiModbusTcpConnection::inverterVoltagePhaseB() const
{
    return m_inverterVoltagePhaseB;
}

float HuaweiModbusTcpConnection::inverterVoltagePhaseC() const
{
    return m_inverterVoltagePhaseC;
}

float HuaweiModbusTcpConnection::inverterPhaseACurrent() const
{
    return m_inverterPhaseACurrent;
}

float HuaweiModbusTcpConnection::inverterPhaseBCurrent() const
{
    return m_inverterPhaseBCurrent;
}

float HuaweiModbusTcpConnection::inverterPhaseCCurrent() const
{
    return m_inverterPhaseCCurrent;
}

void HuaweiModbusTcpConnection::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void HuaweiModbusTcpConnection::update()
{
    updateInverterActivePower();
    updateInverterGridFrequency();
    updateInverterInternalTemperature();
    updateInverterDeviceStatus();
    updateInverterAccumulatedEnergyYield();
    updateInverterDailyEnergyYield();
    updatePowerMeterActivePower();
    updateInverterInformationBlock();
    updateInverterConfigurationBlock();
    updateInverterVoltageBlock();
}

void HuaweiModbusTcpConnection::updateInverterActivePower()
{
    // Update registers from Inverter active power
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Inverter active power\" register:" << 32080 << "size:" << 2;
    QModbusReply *reply = readInverterActivePower();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Inverter active power\" register" << 32080 << "size:" << 2 << values;
                    float receivedInverterActivePower = ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -3);
                    if (m_inverterActivePower != receivedInverterActivePower) {
                        m_inverterActivePower = receivedInverterActivePower;
                        emit inverterActivePowerChanged(m_inverterActivePower);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Inverter active power\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Inverter active power\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateInverterGridFrequency()
{
    // Update registers from Inverter grid frequency
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Inverter grid frequency\" register:" << 32085 << "size:" << 1;
    QModbusReply *reply = readInverterGridFrequency();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Inverter grid frequency\" register" << 32085 << "size:" << 1 << values;
                    float receivedInverterGridFrequency = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -3);
                    if (m_inverterGridFrequency != receivedInverterGridFrequency) {
                        m_inverterGridFrequency = receivedInverterGridFrequency;
                        emit inverterGridFrequencyChanged(m_inverterGridFrequency);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Inverter grid frequency\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Inverter grid frequency\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateInverterInternalTemperature()
{
    // Update registers from Inverter internal temperature
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Inverter internal temperature\" register:" << 32087 << "size:" << 1;
    QModbusReply *reply = readInverterInternalTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Inverter internal temperature\" register" << 32087 << "size:" << 1 << values;
                    float receivedInverterInternalTemperature = ModbusDataUtils::convertToInt16(values) * 1.0 * pow(10, -1);
                    if (m_inverterInternalTemperature != receivedInverterInternalTemperature) {
                        m_inverterInternalTemperature = receivedInverterInternalTemperature;
                        emit inverterInternalTemperatureChanged(m_inverterInternalTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Inverter internal temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Inverter internal temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateInverterDeviceStatus()
{
    // Update registers from Inverter device status
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Inverter device status\" register:" << 32089 << "size:" << 1;
    QModbusReply *reply = readInverterDeviceStatus();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Inverter device status\" register" << 32089 << "size:" << 1 << values;
                    InverterDeviceStatus receivedInverterDeviceStatus = static_cast<InverterDeviceStatus>(ModbusDataUtils::convertToUInt16(values));
                    if (m_inverterDeviceStatus != receivedInverterDeviceStatus) {
                        m_inverterDeviceStatus = receivedInverterDeviceStatus;
                        emit inverterDeviceStatusChanged(m_inverterDeviceStatus);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Inverter device status\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Inverter device status\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateInverterAccumulatedEnergyYield()
{
    // Update registers from Inverter accumulated energy yield
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Inverter accumulated energy yield\" register:" << 32106 << "size:" << 2;
    QModbusReply *reply = readInverterAccumulatedEnergyYield();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Inverter accumulated energy yield\" register" << 32106 << "size:" << 2 << values;
                    float receivedInverterAccumulatedEnergyYield = ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -2);
                    if (m_inverterAccumulatedEnergyYield != receivedInverterAccumulatedEnergyYield) {
                        m_inverterAccumulatedEnergyYield = receivedInverterAccumulatedEnergyYield;
                        emit inverterAccumulatedEnergyYieldChanged(m_inverterAccumulatedEnergyYield);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Inverter accumulated energy yield\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Inverter accumulated energy yield\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateInverterDailyEnergyYield()
{
    // Update registers from Inverter daily energy yield
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Inverter daily energy yield\" register:" << 32114 << "size:" << 2;
    QModbusReply *reply = readInverterDailyEnergyYield();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Inverter daily energy yield\" register" << 32114 << "size:" << 2 << values;
                    float receivedInverterDailyEnergyYield = ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -2);
                    if (m_inverterDailyEnergyYield != receivedInverterDailyEnergyYield) {
                        m_inverterDailyEnergyYield = receivedInverterDailyEnergyYield;
                        emit inverterDailyEnergyYieldChanged(m_inverterDailyEnergyYield);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Inverter daily energy yield\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Inverter daily energy yield\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updatePowerMeterActivePower()
{
    // Update registers from Power meter active power
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read \"Power meter active power\" register:" << 37113 << "size:" << 2;
    QModbusReply *reply = readPowerMeterActivePower();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"Power meter active power\" register" << 37113 << "size:" << 2 << values;
                    qint32 receivedPowerMeterActivePower = ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerMeterActivePower != receivedPowerMeterActivePower) {
                        m_powerMeterActivePower = receivedPowerMeterActivePower;
                        emit powerMeterActivePowerChanged(m_powerMeterActivePower);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"Power meter active power\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"Power meter active power\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateInverterInformationBlock()
{
    // Update register block "inverterInformation"
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read block \"inverterInformation\" registers from:" << 30000 << "size:" << 35;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 30000, 35);
    QModbusReply *reply = sendReadRequest(request, m_slaveId);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    QVector<quint16> blockValues = unit.values();
                    QVector<quint16> values;
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"inverterInformation\" register" << 30000 << "size:" << 35 << values;
                    values = blockValues.mid(0, 15);
                    QString receivedModel = ModbusDataUtils::convertToString(values);
                    if (m_model != receivedModel) {
                        m_model = receivedModel;
                        emit modelChanged(m_model);
                    }

                    values = blockValues.mid(15, 10);
                    QString receivedSerialNumber = ModbusDataUtils::convertToString(values);
                    if (m_serialNumber != receivedSerialNumber) {
                        m_serialNumber = receivedSerialNumber;
                        emit serialNumberChanged(m_serialNumber);
                    }

                    values = blockValues.mid(25, 10);
                    QString receivedProductNumber = ModbusDataUtils::convertToString(values);
                    if (m_productNumber != receivedProductNumber) {
                        m_productNumber = receivedProductNumber;
                        emit productNumberChanged(m_productNumber);
                    }

                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"inverterInformation\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"inverterInformation\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateInverterConfigurationBlock()
{
    // Update register block "inverterConfiguration"
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read block \"inverterConfiguration\" registers from:" << 30070 << "size:" << 3;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 30070, 3);
    QModbusReply *reply = sendReadRequest(request, m_slaveId);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    QVector<quint16> blockValues = unit.values();
                    QVector<quint16> values;
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"inverterConfiguration\" register" << 30070 << "size:" << 3 << values;
                    values = blockValues.mid(0, 1);
                    quint16 receivedModelId = ModbusDataUtils::convertToUInt16(values);
                    if (m_modelId != receivedModelId) {
                        m_modelId = receivedModelId;
                        emit modelIdChanged(m_modelId);
                    }

                    values = blockValues.mid(1, 1);
                    quint16 receivedNumberOfPvString = ModbusDataUtils::convertToUInt16(values);
                    if (m_numberOfPvString != receivedNumberOfPvString) {
                        m_numberOfPvString = receivedNumberOfPvString;
                        emit numberOfPvStringChanged(m_numberOfPvString);
                    }

                    values = blockValues.mid(2, 1);
                    quint16 receivedNumberOfMppTracks = ModbusDataUtils::convertToUInt16(values);
                    if (m_numberOfMppTracks != receivedNumberOfMppTracks) {
                        m_numberOfMppTracks = receivedNumberOfMppTracks;
                        emit numberOfMppTracksChanged(m_numberOfMppTracks);
                    }

                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"inverterConfiguration\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"inverterConfiguration\" registers from" << hostAddress().toString() << errorString();
    }
}

void HuaweiModbusTcpConnection::updateInverterVoltageBlock()
{
    // Update register block "inverterVoltage"
    qCDebug(dcHuaweiModbusTcpConnection()) << "--> Read block \"inverterVoltage\" registers from:" << 32069 << "size:" << 9;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 32069, 9);
    QModbusReply *reply = sendReadRequest(request, m_slaveId);
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    QVector<quint16> blockValues = unit.values();
                    QVector<quint16> values;
                    qCDebug(dcHuaweiModbusTcpConnection()) << "<-- Response from \"inverterVoltage\" register" << 32069 << "size:" << 9 << values;
                    values = blockValues.mid(0, 1);
                    float receivedInverterVoltagePhaseA = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -10);
                    if (m_inverterVoltagePhaseA != receivedInverterVoltagePhaseA) {
                        m_inverterVoltagePhaseA = receivedInverterVoltagePhaseA;
                        emit inverterVoltagePhaseAChanged(m_inverterVoltagePhaseA);
                    }

                    values = blockValues.mid(1, 1);
                    float receivedInverterVoltagePhaseB = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -10);
                    if (m_inverterVoltagePhaseB != receivedInverterVoltagePhaseB) {
                        m_inverterVoltagePhaseB = receivedInverterVoltagePhaseB;
                        emit inverterVoltagePhaseBChanged(m_inverterVoltagePhaseB);
                    }

                    values = blockValues.mid(2, 1);
                    float receivedInverterVoltagePhaseC = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_inverterVoltagePhaseC != receivedInverterVoltagePhaseC) {
                        m_inverterVoltagePhaseC = receivedInverterVoltagePhaseC;
                        emit inverterVoltagePhaseCChanged(m_inverterVoltagePhaseC);
                    }

                    values = blockValues.mid(3, 2);
                    float receivedInverterPhaseACurrent = ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -3);
                    if (m_inverterPhaseACurrent != receivedInverterPhaseACurrent) {
                        m_inverterPhaseACurrent = receivedInverterPhaseACurrent;
                        emit inverterPhaseACurrentChanged(m_inverterPhaseACurrent);
                    }

                    values = blockValues.mid(5, 2);
                    float receivedInverterPhaseBCurrent = ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -3);
                    if (m_inverterPhaseBCurrent != receivedInverterPhaseBCurrent) {
                        m_inverterPhaseBCurrent = receivedInverterPhaseBCurrent;
                        emit inverterPhaseBCurrentChanged(m_inverterPhaseBCurrent);
                    }

                    values = blockValues.mid(7, 2);
                    float receivedInverterPhaseCCurrent = ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -3);
                    if (m_inverterPhaseCCurrent != receivedInverterPhaseCCurrent) {
                        m_inverterPhaseCCurrent = receivedInverterPhaseCCurrent;
                        emit inverterPhaseCCurrentChanged(m_inverterPhaseCCurrent);
                    }

                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcHuaweiModbusTcpConnection()) << "Modbus reply error occurred while updating \"inverterVoltage\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcHuaweiModbusTcpConnection()) << "Error occurred while reading \"inverterVoltage\" registers from" << hostAddress().toString() << errorString();
    }
}

QModbusReply *HuaweiModbusTcpConnection::readInverterActivePower()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 32080, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readInverterGridFrequency()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 32085, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readInverterInternalTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 32087, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readInverterDeviceStatus()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 32089, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readInverterAccumulatedEnergyYield()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 32106, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readInverterDailyEnergyYield()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 32114, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *HuaweiModbusTcpConnection::readPowerMeterActivePower()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 37113, 2);
    return sendReadRequest(request, m_slaveId);
}

void HuaweiModbusTcpConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcHuaweiModbusTcpConnection()) << "Initialization finished of HuaweiModbusTcpConnection" << hostAddress().toString();
        emit initializationFinished();
    }
}

QDebug operator<<(QDebug debug, HuaweiModbusTcpConnection *huaweiModbusTcpConnection)
{
    debug.nospace().noquote() << "HuaweiModbusTcpConnection(" << huaweiModbusTcpConnection->hostAddress().toString() << ":" << huaweiModbusTcpConnection->port() << ")" << "\n";
    debug.nospace().noquote() << "    - Inverter active power:" << huaweiModbusTcpConnection->inverterActivePower() << " [kW]" << "\n";
    debug.nospace().noquote() << "    - Inverter grid frequency:" << huaweiModbusTcpConnection->inverterGridFrequency() << " [Hz]" << "\n";
    debug.nospace().noquote() << "    - Inverter internal temperature:" << huaweiModbusTcpConnection->inverterInternalTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Inverter device status:" << huaweiModbusTcpConnection->inverterDeviceStatus() << "\n";
    debug.nospace().noquote() << "    - Inverter accumulated energy yield:" << huaweiModbusTcpConnection->inverterAccumulatedEnergyYield() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Inverter daily energy yield:" << huaweiModbusTcpConnection->inverterDailyEnergyYield() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Power meter active power:" << huaweiModbusTcpConnection->powerMeterActivePower() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Model:" << huaweiModbusTcpConnection->model() << "\n";
    debug.nospace().noquote() << "    - Serial number:" << huaweiModbusTcpConnection->serialNumber() << "\n";
    debug.nospace().noquote() << "    - Product number:" << huaweiModbusTcpConnection->productNumber() << "\n";
    debug.nospace().noquote() << "    - Model identifier:" << huaweiModbusTcpConnection->modelId() << "\n";
    debug.nospace().noquote() << "    - Number of PV strings:" << huaweiModbusTcpConnection->numberOfPvString() << "\n";
    debug.nospace().noquote() << "    - Number of MPP tracks:" << huaweiModbusTcpConnection->numberOfMppTracks() << "\n";
    debug.nospace().noquote() << "    - Inverter phase A voltage:" << huaweiModbusTcpConnection->inverterVoltagePhaseA() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Inverter phase B voltage:" << huaweiModbusTcpConnection->inverterVoltagePhaseB() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Inverter phase C voltage:" << huaweiModbusTcpConnection->inverterVoltagePhaseC() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Inverter phase A current:" << huaweiModbusTcpConnection->inverterPhaseACurrent() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Inverter phase B current:" << huaweiModbusTcpConnection->inverterPhaseBCurrent() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Inverter phase C current:" << huaweiModbusTcpConnection->inverterPhaseCCurrent() << " [A]" << "\n";
    return debug.quote().space();
}

