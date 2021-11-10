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


#include "sdm630modbusrtuconnection.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcSdm630ModbusRtuConnection, "Sdm630ModbusRtuConnection")

Sdm630ModbusRtuConnection::Sdm630ModbusRtuConnection(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent) :
    QObject(parent),
    m_modbusRtuMaster(modbusRtuMaster),
    m_slaveId(slaveId)
{
    
}

ModbusRtuMaster *Sdm630ModbusRtuConnection::modbusRtuMaster() const
{
    return m_modbusRtuMaster;
}
quint16 Sdm630ModbusRtuConnection::slaveId() const
{
    return m_slaveId;
}
float Sdm630ModbusRtuConnection::voltagePhaseA() const
{
    return m_voltagePhaseA;
}

float Sdm630ModbusRtuConnection::voltagePhaseB() const
{
    return m_voltagePhaseB;
}

float Sdm630ModbusRtuConnection::voltagePhaseC() const
{
    return m_voltagePhaseC;
}

float Sdm630ModbusRtuConnection::currentPhaseA() const
{
    return m_currentPhaseA;
}

float Sdm630ModbusRtuConnection::currentPhaseB() const
{
    return m_currentPhaseB;
}

float Sdm630ModbusRtuConnection::currentPhaseC() const
{
    return m_currentPhaseC;
}

float Sdm630ModbusRtuConnection::powerPhaseA() const
{
    return m_powerPhaseA;
}

float Sdm630ModbusRtuConnection::powerPhaseB() const
{
    return m_powerPhaseB;
}

float Sdm630ModbusRtuConnection::powerPhaseC() const
{
    return m_powerPhaseC;
}

quint16 Sdm630ModbusRtuConnection::totalCurrentPower() const
{
    return m_totalCurrentPower;
}

quint16 Sdm630ModbusRtuConnection::phaseAImportet() const
{
    return m_phaseAImportet;
}

void Sdm630ModbusRtuConnection::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void Sdm630ModbusRtuConnection::update()
{
    updateVoltagePhaseA();
    updateVoltagePhaseB();
    updateVoltagePhaseC();
    updateCurrentPhaseA();
    updateCurrentPhaseB();
    updateCurrentPhaseC();
    updatePowerPhaseA();
    updatePowerPhaseB();
    updatePowerPhaseC();
    updateTotalCurrentPower();
    updatePhaseAImportet();
}

void Sdm630ModbusRtuConnection::updateVoltagePhaseA()
{
    // Update registers from Voltage phase L1
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Voltage phase L1\" register:" << 30001 << "size:" << 2;
    ModbusRtuReply *reply = readVoltagePhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Voltage phase L1\" register" << 30001 << "size:" << 2 << values;
                    float receivedVoltagePhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_voltagePhaseA != receivedVoltagePhaseA) {
                        m_voltagePhaseA = receivedVoltagePhaseA;
                        emit voltagePhaseAChanged(m_voltagePhaseA);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Voltage phase L1\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Voltage phase L1\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateVoltagePhaseB()
{
    // Update registers from Voltage phase L2
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Voltage phase L2\" register:" << 30003 << "size:" << 2;
    ModbusRtuReply *reply = readVoltagePhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Voltage phase L2\" register" << 30003 << "size:" << 2 << values;
                    float receivedVoltagePhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_voltagePhaseB != receivedVoltagePhaseB) {
                        m_voltagePhaseB = receivedVoltagePhaseB;
                        emit voltagePhaseBChanged(m_voltagePhaseB);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Voltage phase L2\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Voltage phase L2\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateVoltagePhaseC()
{
    // Update registers from Voltage phase L3
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Voltage phase L3\" register:" << 30005 << "size:" << 2;
    ModbusRtuReply *reply = readVoltagePhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Voltage phase L3\" register" << 30005 << "size:" << 2 << values;
                    float receivedVoltagePhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_voltagePhaseC != receivedVoltagePhaseC) {
                        m_voltagePhaseC = receivedVoltagePhaseC;
                        emit voltagePhaseCChanged(m_voltagePhaseC);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Voltage phase L3\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Voltage phase L3\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateCurrentPhaseA()
{
    // Update registers from Current phase L1
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Current phase L1\" register:" << 30007 << "size:" << 2;
    ModbusRtuReply *reply = readCurrentPhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Current phase L1\" register" << 30007 << "size:" << 2 << values;
                    float receivedCurrentPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseA != receivedCurrentPhaseA) {
                        m_currentPhaseA = receivedCurrentPhaseA;
                        emit currentPhaseAChanged(m_currentPhaseA);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Current phase L1\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Current phase L1\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateCurrentPhaseB()
{
    // Update registers from Current phase L2
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Current phase L2\" register:" << 30009 << "size:" << 2;
    ModbusRtuReply *reply = readCurrentPhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Current phase L2\" register" << 30009 << "size:" << 2 << values;
                    float receivedCurrentPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseB != receivedCurrentPhaseB) {
                        m_currentPhaseB = receivedCurrentPhaseB;
                        emit currentPhaseBChanged(m_currentPhaseB);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Current phase L2\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Current phase L2\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateCurrentPhaseC()
{
    // Update registers from Current phase L3
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Current phase L3\" register:" << 30011 << "size:" << 2;
    ModbusRtuReply *reply = readCurrentPhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Current phase L3\" register" << 30011 << "size:" << 2 << values;
                    float receivedCurrentPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseC != receivedCurrentPhaseC) {
                        m_currentPhaseC = receivedCurrentPhaseC;
                        emit currentPhaseCChanged(m_currentPhaseC);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Current phase L3\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Current phase L3\" registers";
    }
}

void Sdm630ModbusRtuConnection::updatePowerPhaseA()
{
    // Update registers from Power phase L1
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Power phase L1\" register:" << 30013 << "size:" << 2;
    ModbusRtuReply *reply = readPowerPhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Power phase L1\" register" << 30013 << "size:" << 2 << values;
                    float receivedPowerPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseA != receivedPowerPhaseA) {
                        m_powerPhaseA = receivedPowerPhaseA;
                        emit powerPhaseAChanged(m_powerPhaseA);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Power phase L1\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Power phase L1\" registers";
    }
}

void Sdm630ModbusRtuConnection::updatePowerPhaseB()
{
    // Update registers from Power phase L2
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Power phase L2\" register:" << 30015 << "size:" << 2;
    ModbusRtuReply *reply = readPowerPhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Power phase L2\" register" << 30015 << "size:" << 2 << values;
                    float receivedPowerPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseB != receivedPowerPhaseB) {
                        m_powerPhaseB = receivedPowerPhaseB;
                        emit powerPhaseBChanged(m_powerPhaseB);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Power phase L2\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Power phase L2\" registers";
    }
}

void Sdm630ModbusRtuConnection::updatePowerPhaseC()
{
    // Update registers from Power phase L3
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Power phase L3\" register:" << 30017 << "size:" << 2;
    ModbusRtuReply *reply = readPowerPhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Power phase L3\" register" << 30017 << "size:" << 2 << values;
                    float receivedPowerPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseC != receivedPowerPhaseC) {
                        m_powerPhaseC = receivedPowerPhaseC;
                        emit powerPhaseCChanged(m_powerPhaseC);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Power phase L3\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Power phase L3\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateTotalCurrentPower()
{
    // Update registers from Total system power
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Total system power\" register:" << 30053 << "size:" << 2;
    ModbusRtuReply *reply = readTotalCurrentPower();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Total system power\" register" << 30053 << "size:" << 2 << values;
                    quint16 receivedTotalCurrentPower = ModbusDataUtils::convertToUInt16(values);
                    if (m_totalCurrentPower != receivedTotalCurrentPower) {
                        m_totalCurrentPower = receivedTotalCurrentPower;
                        emit totalCurrentPowerChanged(m_totalCurrentPower);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total system power\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Total system power\" registers";
    }
}

void Sdm630ModbusRtuConnection::updatePhaseAImportet()
{
    // Update registers from Total system power
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Total system power\" register:" << 30053 << "size:" << 2;
    ModbusRtuReply *reply = readPhaseAImportet();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    const QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Total system power\" register" << 30053 << "size:" << 2 << values;
                    quint16 receivedPhaseAImportet = ModbusDataUtils::convertToUInt16(values);
                    if (m_phaseAImportet != receivedPhaseAImportet) {
                        m_phaseAImportet = receivedPhaseAImportet;
                        emit phaseAImportetChanged(m_phaseAImportet);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total system power\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Total system power\" registers";
    }
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readVoltagePhaseA()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30001, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readVoltagePhaseB()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30003, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readVoltagePhaseC()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30005, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readCurrentPhaseA()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30007, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readCurrentPhaseB()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30009, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readCurrentPhaseC()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30011, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readPowerPhaseA()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30013, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readPowerPhaseB()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30015, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readPowerPhaseC()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30017, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readTotalCurrentPower()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30053, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readPhaseAImportet()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 30053, 2);
}

void Sdm630ModbusRtuConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcSdm630ModbusRtuConnection()) << "Initialization finished of Sdm630ModbusRtuConnection";
        emit initializationFinished();
    }
}

QDebug operator<<(QDebug debug, Sdm630ModbusRtuConnection *sdm630ModbusRtuConnection)
{
    debug.nospace().noquote() << "Sdm630ModbusRtuConnection(" << sdm630ModbusRtuConnection->modbusRtuMaster()->modbusUuid().toString() << ", " << sdm630ModbusRtuConnection->modbusRtuMaster()->serialPort() << ", slave ID:" << sdm630ModbusRtuConnection->slaveId() << ")" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L1:" << sdm630ModbusRtuConnection->voltagePhaseA() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L2:" << sdm630ModbusRtuConnection->voltagePhaseB() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L3:" << sdm630ModbusRtuConnection->voltagePhaseC() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Current phase L1:" << sdm630ModbusRtuConnection->currentPhaseA() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Current phase L2:" << sdm630ModbusRtuConnection->currentPhaseB() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Current phase L3:" << sdm630ModbusRtuConnection->currentPhaseC() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Power phase L1:" << sdm630ModbusRtuConnection->powerPhaseA() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Power phase L2:" << sdm630ModbusRtuConnection->powerPhaseB() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Power phase L3:" << sdm630ModbusRtuConnection->powerPhaseC() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Total system power:" << sdm630ModbusRtuConnection->totalCurrentPower() << " [W]" << "\n";
    debug.nospace().noquote() << "    - Total system power:" << sdm630ModbusRtuConnection->phaseAImportet() << " [W]" << "\n";
    return debug.quote().space();
}

