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


#include "pro380modbusrtuconnection.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcPro380ModbusRtuConnection, "Pro380ModbusRtuConnection")

Pro380ModbusRtuConnection::Pro380ModbusRtuConnection(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent) :
    QObject(parent),
    m_modbusRtuMaster(modbusRtuMaster),
    m_slaveId(slaveId)
{
    
}

ModbusRtuMaster *Pro380ModbusRtuConnection::modbusRtuMaster() const
{
    return m_modbusRtuMaster;
}
quint16 Pro380ModbusRtuConnection::slaveId() const
{
    return m_slaveId;
}
float Pro380ModbusRtuConnection::voltagePhaseA() const
{
    return m_voltagePhaseA;
}

float Pro380ModbusRtuConnection::voltagePhaseB() const
{
    return m_voltagePhaseB;
}

float Pro380ModbusRtuConnection::voltagePhaseC() const
{
    return m_voltagePhaseC;
}

float Pro380ModbusRtuConnection::frequency() const
{
    return m_frequency;
}

float Pro380ModbusRtuConnection::currentPhaseA() const
{
    return m_currentPhaseA;
}

float Pro380ModbusRtuConnection::currentPhaseB() const
{
    return m_currentPhaseB;
}

float Pro380ModbusRtuConnection::currentPhaseC() const
{
    return m_currentPhaseC;
}

float Pro380ModbusRtuConnection::totalCurrentPower() const
{
    return m_totalCurrentPower;
}

float Pro380ModbusRtuConnection::powerPhaseA() const
{
    return m_powerPhaseA;
}

float Pro380ModbusRtuConnection::powerPhaseB() const
{
    return m_powerPhaseB;
}

float Pro380ModbusRtuConnection::powerPhaseC() const
{
    return m_powerPhaseC;
}

float Pro380ModbusRtuConnection::totalEnergyConsumed() const
{
    return m_totalEnergyConsumed;
}

float Pro380ModbusRtuConnection::totalEnergyProduced() const
{
    return m_totalEnergyProduced;
}

float Pro380ModbusRtuConnection::energyConsumedPhaseA() const
{
    return m_energyConsumedPhaseA;
}

float Pro380ModbusRtuConnection::energyConsumedPhaseB() const
{
    return m_energyConsumedPhaseB;
}

float Pro380ModbusRtuConnection::energyConsumedPhaseC() const
{
    return m_energyConsumedPhaseC;
}

float Pro380ModbusRtuConnection::energyProducedPhaseA() const
{
    return m_energyProducedPhaseA;
}

float Pro380ModbusRtuConnection::energyProducedPhaseB() const
{
    return m_energyProducedPhaseB;
}

float Pro380ModbusRtuConnection::energyProducedPhaseC() const
{
    return m_energyProducedPhaseC;
}

void Pro380ModbusRtuConnection::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void Pro380ModbusRtuConnection::update()
{
    updateVoltagePhaseA();
    updateVoltagePhaseB();
    updateVoltagePhaseC();
    updateFrequency();
    updateCurrentPhaseA();
    updateCurrentPhaseB();
    updateCurrentPhaseC();
    updateTotalCurrentPower();
    updatePowerPhaseA();
    updatePowerPhaseB();
    updatePowerPhaseC();
    updateTotalEnergyConsumed();
    updateTotalEnergyProduced();
    updateEnergyConsumedPhaseA();
    updateEnergyConsumedPhaseB();
    updateEnergyConsumedPhaseC();
    updateEnergyProducedPhaseA();
    updateEnergyProducedPhaseB();
    updateEnergyProducedPhaseC();
}

void Pro380ModbusRtuConnection::updateVoltagePhaseA()
{
    // Update registers from Voltage phase L1
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Voltage phase L1\" register:" << 20482 << "size:" << 2;
    ModbusRtuReply *reply = readVoltagePhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Voltage phase L1\" register" << 20482 << "size:" << 2 << values;
                    float receivedVoltagePhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_voltagePhaseA != receivedVoltagePhaseA) {
                        m_voltagePhaseA = receivedVoltagePhaseA;
                        emit voltagePhaseAChanged(m_voltagePhaseA);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Voltage phase L1\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Voltage phase L1\" registers";
    }
}

void Pro380ModbusRtuConnection::updateVoltagePhaseB()
{
    // Update registers from Voltage phase L2
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Voltage phase L2\" register:" << 20484 << "size:" << 2;
    ModbusRtuReply *reply = readVoltagePhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Voltage phase L2\" register" << 20484 << "size:" << 2 << values;
                    float receivedVoltagePhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_voltagePhaseB != receivedVoltagePhaseB) {
                        m_voltagePhaseB = receivedVoltagePhaseB;
                        emit voltagePhaseBChanged(m_voltagePhaseB);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Voltage phase L2\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Voltage phase L2\" registers";
    }
}

void Pro380ModbusRtuConnection::updateVoltagePhaseC()
{
    // Update registers from Voltage phase L3
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Voltage phase L3\" register:" << 20486 << "size:" << 2;
    ModbusRtuReply *reply = readVoltagePhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Voltage phase L3\" register" << 20486 << "size:" << 2 << values;
                    float receivedVoltagePhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_voltagePhaseC != receivedVoltagePhaseC) {
                        m_voltagePhaseC = receivedVoltagePhaseC;
                        emit voltagePhaseCChanged(m_voltagePhaseC);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Voltage phase L3\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Voltage phase L3\" registers";
    }
}

void Pro380ModbusRtuConnection::updateFrequency()
{
    // Update registers from Frequency
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Frequency\" register:" << 20488 << "size:" << 2;
    ModbusRtuReply *reply = readFrequency();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Frequency\" register" << 20488 << "size:" << 2 << values;
                    float receivedFrequency = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_frequency != receivedFrequency) {
                        m_frequency = receivedFrequency;
                        emit frequencyChanged(m_frequency);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Frequency\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Frequency\" registers";
    }
}

void Pro380ModbusRtuConnection::updateCurrentPhaseA()
{
    // Update registers from Current phase L1
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Current phase L1\" register:" << 20492 << "size:" << 2;
    ModbusRtuReply *reply = readCurrentPhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Current phase L1\" register" << 20492 << "size:" << 2 << values;
                    float receivedCurrentPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseA != receivedCurrentPhaseA) {
                        m_currentPhaseA = receivedCurrentPhaseA;
                        emit currentPhaseAChanged(m_currentPhaseA);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Current phase L1\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Current phase L1\" registers";
    }
}

void Pro380ModbusRtuConnection::updateCurrentPhaseB()
{
    // Update registers from Current phase L2
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Current phase L2\" register:" << 20494 << "size:" << 2;
    ModbusRtuReply *reply = readCurrentPhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Current phase L2\" register" << 20494 << "size:" << 2 << values;
                    float receivedCurrentPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseB != receivedCurrentPhaseB) {
                        m_currentPhaseB = receivedCurrentPhaseB;
                        emit currentPhaseBChanged(m_currentPhaseB);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Current phase L2\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Current phase L2\" registers";
    }
}

void Pro380ModbusRtuConnection::updateCurrentPhaseC()
{
    // Update registers from Current phase L3
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Current phase L3\" register:" << 20496 << "size:" << 2;
    ModbusRtuReply *reply = readCurrentPhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Current phase L3\" register" << 20496 << "size:" << 2 << values;
                    float receivedCurrentPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_currentPhaseC != receivedCurrentPhaseC) {
                        m_currentPhaseC = receivedCurrentPhaseC;
                        emit currentPhaseCChanged(m_currentPhaseC);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Current phase L3\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Current phase L3\" registers";
    }
}

void Pro380ModbusRtuConnection::updateTotalCurrentPower()
{
    // Update registers from Total system power
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Total system power\" register:" << 20498 << "size:" << 2;
    ModbusRtuReply *reply = readTotalCurrentPower();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Total system power\" register" << 20498 << "size:" << 2 << values;
                    float receivedTotalCurrentPower = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalCurrentPower != receivedTotalCurrentPower) {
                        m_totalCurrentPower = receivedTotalCurrentPower;
                        emit totalCurrentPowerChanged(m_totalCurrentPower);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total system power\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Total system power\" registers";
    }
}

void Pro380ModbusRtuConnection::updatePowerPhaseA()
{
    // Update registers from Power phase L1
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Power phase L1\" register:" << 20500 << "size:" << 2;
    ModbusRtuReply *reply = readPowerPhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Power phase L1\" register" << 20500 << "size:" << 2 << values;
                    float receivedPowerPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseA != receivedPowerPhaseA) {
                        m_powerPhaseA = receivedPowerPhaseA;
                        emit powerPhaseAChanged(m_powerPhaseA);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Power phase L1\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Power phase L1\" registers";
    }
}

void Pro380ModbusRtuConnection::updatePowerPhaseB()
{
    // Update registers from Power phase L2
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Power phase L2\" register:" << 20502 << "size:" << 2;
    ModbusRtuReply *reply = readPowerPhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Power phase L2\" register" << 20502 << "size:" << 2 << values;
                    float receivedPowerPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseB != receivedPowerPhaseB) {
                        m_powerPhaseB = receivedPowerPhaseB;
                        emit powerPhaseBChanged(m_powerPhaseB);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Power phase L2\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Power phase L2\" registers";
    }
}

void Pro380ModbusRtuConnection::updatePowerPhaseC()
{
    // Update registers from Power phase L3
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Power phase L3\" register:" << 20504 << "size:" << 2;
    ModbusRtuReply *reply = readPowerPhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Power phase L3\" register" << 20504 << "size:" << 2 << values;
                    float receivedPowerPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_powerPhaseC != receivedPowerPhaseC) {
                        m_powerPhaseC = receivedPowerPhaseC;
                        emit powerPhaseCChanged(m_powerPhaseC);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Power phase L3\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Power phase L3\" registers";
    }
}

void Pro380ModbusRtuConnection::updateTotalEnergyConsumed()
{
    // Update registers from Total energy consumed (Forward active energy)
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Total energy consumed (Forward active energy)\" register:" << 24588 << "size:" << 2;
    ModbusRtuReply *reply = readTotalEnergyConsumed();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Total energy consumed (Forward active energy)\" register" << 24588 << "size:" << 2 << values;
                    float receivedTotalEnergyConsumed = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalEnergyConsumed != receivedTotalEnergyConsumed) {
                        m_totalEnergyConsumed = receivedTotalEnergyConsumed;
                        emit totalEnergyConsumedChanged(m_totalEnergyConsumed);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total energy consumed (Forward active energy)\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Total energy consumed (Forward active energy)\" registers";
    }
}

void Pro380ModbusRtuConnection::updateTotalEnergyProduced()
{
    // Update registers from Total energy produced (Reverse active energy)
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Total energy produced (Reverse active energy)\" register:" << 24600 << "size:" << 2;
    ModbusRtuReply *reply = readTotalEnergyProduced();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Total energy produced (Reverse active energy)\" register" << 24600 << "size:" << 2 << values;
                    float receivedTotalEnergyProduced = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalEnergyProduced != receivedTotalEnergyProduced) {
                        m_totalEnergyProduced = receivedTotalEnergyProduced;
                        emit totalEnergyProducedChanged(m_totalEnergyProduced);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total energy produced (Reverse active energy)\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Total energy produced (Reverse active energy)\" registers";
    }
}

void Pro380ModbusRtuConnection::updateEnergyConsumedPhaseA()
{
    // Update registers from Energy consumed phase A
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Energy consumed phase A\" register:" << 24594 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyConsumedPhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Energy consumed phase A\" register" << 24594 << "size:" << 2 << values;
                    float receivedEnergyConsumedPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseA != receivedEnergyConsumedPhaseA) {
                        m_energyConsumedPhaseA = receivedEnergyConsumedPhaseA;
                        emit energyConsumedPhaseAChanged(m_energyConsumedPhaseA);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy consumed phase A\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Energy consumed phase A\" registers";
    }
}

void Pro380ModbusRtuConnection::updateEnergyConsumedPhaseB()
{
    // Update registers from Energy consumed phase B
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Energy consumed phase B\" register:" << 24596 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyConsumedPhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Energy consumed phase B\" register" << 24596 << "size:" << 2 << values;
                    float receivedEnergyConsumedPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseB != receivedEnergyConsumedPhaseB) {
                        m_energyConsumedPhaseB = receivedEnergyConsumedPhaseB;
                        emit energyConsumedPhaseBChanged(m_energyConsumedPhaseB);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy consumed phase B\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Energy consumed phase B\" registers";
    }
}

void Pro380ModbusRtuConnection::updateEnergyConsumedPhaseC()
{
    // Update registers from Energy consumed phase C
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Energy consumed phase C\" register:" << 24598 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyConsumedPhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Energy consumed phase C\" register" << 24598 << "size:" << 2 << values;
                    float receivedEnergyConsumedPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseC != receivedEnergyConsumedPhaseC) {
                        m_energyConsumedPhaseC = receivedEnergyConsumedPhaseC;
                        emit energyConsumedPhaseCChanged(m_energyConsumedPhaseC);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy consumed phase C\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Energy consumed phase C\" registers";
    }
}

void Pro380ModbusRtuConnection::updateEnergyProducedPhaseA()
{
    // Update registers from Energy produced phase A
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Energy produced phase A\" register:" << 24606 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyProducedPhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Energy produced phase A\" register" << 24606 << "size:" << 2 << values;
                    float receivedEnergyProducedPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyProducedPhaseA != receivedEnergyProducedPhaseA) {
                        m_energyProducedPhaseA = receivedEnergyProducedPhaseA;
                        emit energyProducedPhaseAChanged(m_energyProducedPhaseA);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy produced phase A\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Energy produced phase A\" registers";
    }
}

void Pro380ModbusRtuConnection::updateEnergyProducedPhaseB()
{
    // Update registers from Energy produced phase B
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Energy produced phase B\" register:" << 24608 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyProducedPhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Energy produced phase B\" register" << 24608 << "size:" << 2 << values;
                    float receivedEnergyProducedPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyProducedPhaseB != receivedEnergyProducedPhaseB) {
                        m_energyProducedPhaseB = receivedEnergyProducedPhaseB;
                        emit energyProducedPhaseBChanged(m_energyProducedPhaseB);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy produced phase B\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Energy produced phase B\" registers";
    }
}

void Pro380ModbusRtuConnection::updateEnergyProducedPhaseC()
{
    // Update registers from Energy produced phase C
    qCDebug(dcPro380ModbusRtuConnection()) << "--> Read \"Energy produced phase C\" register:" << 24610 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyProducedPhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcPro380ModbusRtuConnection()) << "<-- Response from \"Energy produced phase C\" register" << 24610 << "size:" << 2 << values;
                    float receivedEnergyProducedPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyProducedPhaseC != receivedEnergyProducedPhaseC) {
                        m_energyProducedPhaseC = receivedEnergyProducedPhaseC;
                        emit energyProducedPhaseCChanged(m_energyProducedPhaseC);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcPro380ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy produced phase C\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcPro380ModbusRtuConnection()) << "Error occurred while reading \"Energy produced phase C\" registers";
    }
}

ModbusRtuReply *Pro380ModbusRtuConnection::readVoltagePhaseA()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20482, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readVoltagePhaseB()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20484, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readVoltagePhaseC()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20486, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readFrequency()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20488, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readCurrentPhaseA()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20492, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readCurrentPhaseB()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20494, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readCurrentPhaseC()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20496, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readTotalCurrentPower()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20498, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readPowerPhaseA()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20500, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readPowerPhaseB()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20502, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readPowerPhaseC()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 20504, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readTotalEnergyConsumed()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24588, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readTotalEnergyProduced()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24600, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readEnergyConsumedPhaseA()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24594, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readEnergyConsumedPhaseB()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24596, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readEnergyConsumedPhaseC()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24598, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readEnergyProducedPhaseA()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24606, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readEnergyProducedPhaseB()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24608, 2);
}

ModbusRtuReply *Pro380ModbusRtuConnection::readEnergyProducedPhaseC()
{
    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, 24610, 2);
}

void Pro380ModbusRtuConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcPro380ModbusRtuConnection()) << "Initialization finished of Pro380ModbusRtuConnection";
        emit initializationFinished();
    }
}

QDebug operator<<(QDebug debug, Pro380ModbusRtuConnection *pro380ModbusRtuConnection)
{
    debug.nospace().noquote() << "Pro380ModbusRtuConnection(" << pro380ModbusRtuConnection->modbusRtuMaster()->modbusUuid().toString() << ", " << pro380ModbusRtuConnection->modbusRtuMaster()->serialPort() << ", slave ID:" << pro380ModbusRtuConnection->slaveId() << ")" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L1:" << pro380ModbusRtuConnection->voltagePhaseA() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L2:" << pro380ModbusRtuConnection->voltagePhaseB() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Voltage phase L3:" << pro380ModbusRtuConnection->voltagePhaseC() << " [V]" << "\n";
    debug.nospace().noquote() << "    - Frequency:" << pro380ModbusRtuConnection->frequency() << " [Hz]" << "\n";
    debug.nospace().noquote() << "    - Current phase L1:" << pro380ModbusRtuConnection->currentPhaseA() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Current phase L2:" << pro380ModbusRtuConnection->currentPhaseB() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Current phase L3:" << pro380ModbusRtuConnection->currentPhaseC() << " [A]" << "\n";
    debug.nospace().noquote() << "    - Total system power:" << pro380ModbusRtuConnection->totalCurrentPower() << " [kW]" << "\n";
    debug.nospace().noquote() << "    - Power phase L1:" << pro380ModbusRtuConnection->powerPhaseA() << " [kW]" << "\n";
    debug.nospace().noquote() << "    - Power phase L2:" << pro380ModbusRtuConnection->powerPhaseB() << " [kW]" << "\n";
    debug.nospace().noquote() << "    - Power phase L3:" << pro380ModbusRtuConnection->powerPhaseC() << " [kW]" << "\n";
    debug.nospace().noquote() << "    - Total energy consumed (Forward active energy):" << pro380ModbusRtuConnection->totalEnergyConsumed() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Total energy produced (Reverse active energy):" << pro380ModbusRtuConnection->totalEnergyProduced() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase A:" << pro380ModbusRtuConnection->energyConsumedPhaseA() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase B:" << pro380ModbusRtuConnection->energyConsumedPhaseB() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase C:" << pro380ModbusRtuConnection->energyConsumedPhaseC() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase A:" << pro380ModbusRtuConnection->energyProducedPhaseA() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase B:" << pro380ModbusRtuConnection->energyProducedPhaseB() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase C:" << pro380ModbusRtuConnection->energyProducedPhaseC() << " [kWh]" << "\n";
    return debug.quote().space();
}

