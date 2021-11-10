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

float Sdm630ModbusRtuConnection::totalCurrentPower() const
{
    return m_totalCurrentPower;
}

float Sdm630ModbusRtuConnection::frequency() const
{
    return m_frequency;
}

float Sdm630ModbusRtuConnection::totalEnergyConsumed() const
{
    return m_totalEnergyConsumed;
}

float Sdm630ModbusRtuConnection::totalEnergyProduced() const
{
    return m_totalEnergyProduced;
}

float Sdm630ModbusRtuConnection::energyProducedPhaseA() const
{
    return m_energyProducedPhaseA;
}

float Sdm630ModbusRtuConnection::energyProducedPhaseB() const
{
    return m_energyProducedPhaseB;
}

float Sdm630ModbusRtuConnection::energyProducedPhaseC() const
{
    return m_energyProducedPhaseC;
}

float Sdm630ModbusRtuConnection::energyConsumedPhaseA() const
{
    return m_energyConsumedPhaseA;
}

float Sdm630ModbusRtuConnection::energyConsumedPhaseB() const
{
    return m_energyConsumedPhaseB;
}

float Sdm630ModbusRtuConnection::energyConsumedPhaseC() const
{
    return m_energyConsumedPhaseC;
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
    updateFrequency();
    updateTotalEnergyConsumed();
    updateTotalEnergyProduced();
    updateEnergyProducedPhaseA();
    updateEnergyProducedPhaseB();
    updateEnergyProducedPhaseC();
    updateEnergyConsumedPhaseA();
    updateEnergyConsumedPhaseB();
    updateEnergyConsumedPhaseC();
}

void Sdm630ModbusRtuConnection::updateVoltagePhaseA()
{
    // Update registers from Voltage phase L1
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Voltage phase L1\" register:" << 0 << "size:" << 2;
    ModbusRtuReply *reply = readVoltagePhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Voltage phase L1\" register" << 0 << "size:" << 2 << values;
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
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Voltage phase L2\" register:" << 2 << "size:" << 2;
    ModbusRtuReply *reply = readVoltagePhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Voltage phase L2\" register" << 2 << "size:" << 2 << values;
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
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Voltage phase L3\" register:" << 4 << "size:" << 2;
    ModbusRtuReply *reply = readVoltagePhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Voltage phase L3\" register" << 4 << "size:" << 2 << values;
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
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Current phase L1\" register:" << 6 << "size:" << 2;
    ModbusRtuReply *reply = readCurrentPhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Current phase L1\" register" << 6 << "size:" << 2 << values;
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
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Current phase L2\" register:" << 8 << "size:" << 2;
    ModbusRtuReply *reply = readCurrentPhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Current phase L2\" register" << 8 << "size:" << 2 << values;
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
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Current phase L3\" register:" << 10 << "size:" << 2;
    ModbusRtuReply *reply = readCurrentPhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Current phase L3\" register" << 10 << "size:" << 2 << values;
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
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Power phase L1\" register:" << 10 << "size:" << 2;
    ModbusRtuReply *reply = readPowerPhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Power phase L1\" register" << 10 << "size:" << 2 << values;
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
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Power phase L2\" register:" << 14 << "size:" << 2;
    ModbusRtuReply *reply = readPowerPhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Power phase L2\" register" << 14 << "size:" << 2 << values;
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
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Power phase L3\" register:" << 16 << "size:" << 2;
    ModbusRtuReply *reply = readPowerPhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Power phase L3\" register" << 16 << "size:" << 2 << values;
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
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Total system power\" register:" << 52 << "size:" << 2;
    ModbusRtuReply *reply = readTotalCurrentPower();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Total system power\" register" << 52 << "size:" << 2 << values;
                    float receivedTotalCurrentPower = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
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

void Sdm630ModbusRtuConnection::updateFrequency()
{
    // Update registers from Frequency
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Frequency\" register:" << 70 << "size:" << 2;
    ModbusRtuReply *reply = readFrequency();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Frequency\" register" << 70 << "size:" << 2 << values;
                    float receivedFrequency = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_frequency != receivedFrequency) {
                        m_frequency = receivedFrequency;
                        emit frequencyChanged(m_frequency);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Frequency\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Frequency\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateTotalEnergyConsumed()
{
    // Update registers from Total energy consumed
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Total energy consumed\" register:" << 72 << "size:" << 2;
    ModbusRtuReply *reply = readTotalEnergyConsumed();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Total energy consumed\" register" << 72 << "size:" << 2 << values;
                    float receivedTotalEnergyConsumed = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalEnergyConsumed != receivedTotalEnergyConsumed) {
                        m_totalEnergyConsumed = receivedTotalEnergyConsumed;
                        emit totalEnergyConsumedChanged(m_totalEnergyConsumed);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total energy consumed\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Total energy consumed\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateTotalEnergyProduced()
{
    // Update registers from Total energy produced
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Total energy produced\" register:" << 74 << "size:" << 2;
    ModbusRtuReply *reply = readTotalEnergyProduced();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Total energy produced\" register" << 74 << "size:" << 2 << values;
                    float receivedTotalEnergyProduced = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_totalEnergyProduced != receivedTotalEnergyProduced) {
                        m_totalEnergyProduced = receivedTotalEnergyProduced;
                        emit totalEnergyProducedChanged(m_totalEnergyProduced);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Total energy produced\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Total energy produced\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateEnergyProducedPhaseA()
{
    // Update registers from Energy produced phase A
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Energy produced phase A\" register:" << 346 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyProducedPhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Energy produced phase A\" register" << 346 << "size:" << 2 << values;
                    float receivedEnergyProducedPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyProducedPhaseA != receivedEnergyProducedPhaseA) {
                        m_energyProducedPhaseA = receivedEnergyProducedPhaseA;
                        emit energyProducedPhaseAChanged(m_energyProducedPhaseA);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy produced phase A\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Energy produced phase A\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateEnergyProducedPhaseB()
{
    // Update registers from Energy produced phase B
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Energy produced phase B\" register:" << 348 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyProducedPhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Energy produced phase B\" register" << 348 << "size:" << 2 << values;
                    float receivedEnergyProducedPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyProducedPhaseB != receivedEnergyProducedPhaseB) {
                        m_energyProducedPhaseB = receivedEnergyProducedPhaseB;
                        emit energyProducedPhaseBChanged(m_energyProducedPhaseB);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy produced phase B\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Energy produced phase B\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateEnergyProducedPhaseC()
{
    // Update registers from Energy produced phase C
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Energy produced phase C\" register:" << 350 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyProducedPhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Energy produced phase C\" register" << 350 << "size:" << 2 << values;
                    float receivedEnergyProducedPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyProducedPhaseC != receivedEnergyProducedPhaseC) {
                        m_energyProducedPhaseC = receivedEnergyProducedPhaseC;
                        emit energyProducedPhaseCChanged(m_energyProducedPhaseC);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy produced phase C\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Energy produced phase C\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateEnergyConsumedPhaseA()
{
    // Update registers from Energy consumed phase A
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Energy consumed phase A\" register:" << 352 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyConsumedPhaseA();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Energy consumed phase A\" register" << 352 << "size:" << 2 << values;
                    float receivedEnergyConsumedPhaseA = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseA != receivedEnergyConsumedPhaseA) {
                        m_energyConsumedPhaseA = receivedEnergyConsumedPhaseA;
                        emit energyConsumedPhaseAChanged(m_energyConsumedPhaseA);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy consumed phase A\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Energy consumed phase A\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateEnergyConsumedPhaseB()
{
    // Update registers from Energy consumed phase B
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Energy consumed phase B\" register:" << 354 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyConsumedPhaseB();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Energy consumed phase B\" register" << 354 << "size:" << 2 << values;
                    float receivedEnergyConsumedPhaseB = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseB != receivedEnergyConsumedPhaseB) {
                        m_energyConsumedPhaseB = receivedEnergyConsumedPhaseB;
                        emit energyConsumedPhaseBChanged(m_energyConsumedPhaseB);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy consumed phase B\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Energy consumed phase B\" registers";
    }
}

void Sdm630ModbusRtuConnection::updateEnergyConsumedPhaseC()
{
    // Update registers from Energy consumed phase C
    qCDebug(dcSdm630ModbusRtuConnection()) << "--> Read \"Energy consumed phase C\" register:" << 356 << "size:" << 2;
    ModbusRtuReply *reply = readEnergyConsumedPhaseC();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    QVector<quint16> values = reply->result();
                    qCDebug(dcSdm630ModbusRtuConnection()) << "<-- Response from \"Energy consumed phase C\" register" << 356 << "size:" << 2 << values;
                    float receivedEnergyConsumedPhaseC = ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_energyConsumedPhaseC != receivedEnergyConsumedPhaseC) {
                        m_energyConsumedPhaseC = receivedEnergyConsumedPhaseC;
                        emit energyConsumedPhaseCChanged(m_energyConsumedPhaseC);
                    }
                }
            });

            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){
                qCWarning(dcSdm630ModbusRtuConnection()) << "ModbusRtu reply error occurred while updating \"Energy consumed phase C\" registers" << error << reply->errorString();
                emit reply->finished();
            });
        }
    } else {
        qCWarning(dcSdm630ModbusRtuConnection()) << "Error occurred while reading \"Energy consumed phase C\" registers";
    }
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readVoltagePhaseA()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 0, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readVoltagePhaseB()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 2, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readVoltagePhaseC()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 4, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readCurrentPhaseA()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 6, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readCurrentPhaseB()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 8, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readCurrentPhaseC()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 10, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readPowerPhaseA()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 10, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readPowerPhaseB()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 14, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readPowerPhaseC()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 16, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readTotalCurrentPower()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 52, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readFrequency()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 70, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readTotalEnergyConsumed()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 72, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readTotalEnergyProduced()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 74, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readEnergyProducedPhaseA()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 346, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readEnergyProducedPhaseB()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 348, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readEnergyProducedPhaseC()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 350, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readEnergyConsumedPhaseA()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 352, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readEnergyConsumedPhaseB()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 354, 2);
}

ModbusRtuReply *Sdm630ModbusRtuConnection::readEnergyConsumedPhaseC()
{
    return m_modbusRtuMaster->readInputRegister(m_slaveId, 356, 2);
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
    debug.nospace().noquote() << "    - Frequency:" << sdm630ModbusRtuConnection->frequency() << " [Hz]" << "\n";
    debug.nospace().noquote() << "    - Total energy consumed:" << sdm630ModbusRtuConnection->totalEnergyConsumed() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Total energy produced:" << sdm630ModbusRtuConnection->totalEnergyProduced() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase A:" << sdm630ModbusRtuConnection->energyProducedPhaseA() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase B:" << sdm630ModbusRtuConnection->energyProducedPhaseB() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy produced phase C:" << sdm630ModbusRtuConnection->energyProducedPhaseC() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase A:" << sdm630ModbusRtuConnection->energyConsumedPhaseA() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase B:" << sdm630ModbusRtuConnection->energyConsumedPhaseB() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Energy consumed phase C:" << sdm630ModbusRtuConnection->energyConsumedPhaseC() << " [kWh]" << "\n";
    return debug.quote().space();
}

