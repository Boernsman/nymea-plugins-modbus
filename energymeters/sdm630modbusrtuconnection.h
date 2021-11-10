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

#ifndef SDM630MODBUSRTUCONNECTION_H
#define SDM630MODBUSRTUCONNECTION_H

#include <QObject>

#include "../modbus/modbusdatautils.h"
#include <hardware/modbus/modbusrtumaster.h>

class Sdm630ModbusRtuConnection : public QObject
{
    Q_OBJECT
public:
    explicit Sdm630ModbusRtuConnection(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent = nullptr);
    ~Sdm630ModbusRtuConnection() = default;

    ModbusRtuMaster *modbusRtuMaster() const;
    quint16 slaveId() const;

    /* Voltage phase L1 [V] - Address: 30001, Size: 2 */
    float voltagePhaseA() const;

    /* Voltage phase L2 [V] - Address: 30003, Size: 2 */
    float voltagePhaseB() const;

    /* Voltage phase L3 [V] - Address: 30005, Size: 2 */
    float voltagePhaseC() const;

    /* Current phase L1 [A] - Address: 30007, Size: 2 */
    float currentPhaseA() const;

    /* Current phase L2 [A] - Address: 30009, Size: 2 */
    float currentPhaseB() const;

    /* Current phase L3 [A] - Address: 30011, Size: 2 */
    float currentPhaseC() const;

    /* Power phase L1 [W] - Address: 30013, Size: 2 */
    float powerPhaseA() const;

    /* Power phase L2 [W] - Address: 30015, Size: 2 */
    float powerPhaseB() const;

    /* Power phase L3 [W] - Address: 30017, Size: 2 */
    float powerPhaseC() const;

    /* Total system power [W] - Address: 30053, Size: 2 */
    quint16 totalCurrentPower() const;

    /* Total system power [W] - Address: 30053, Size: 2 */
    quint16 phaseAImportet() const;

    virtual void initialize();
    virtual void update();

    void updateVoltagePhaseA();
    void updateVoltagePhaseB();
    void updateVoltagePhaseC();
    void updateCurrentPhaseA();
    void updateCurrentPhaseB();
    void updateCurrentPhaseC();
    void updatePowerPhaseA();
    void updatePowerPhaseB();
    void updatePowerPhaseC();
    void updateTotalCurrentPower();
    void updatePhaseAImportet();

signals:
    void initializationFinished();

    void voltagePhaseAChanged(float voltagePhaseA);
    void voltagePhaseBChanged(float voltagePhaseB);
    void voltagePhaseCChanged(float voltagePhaseC);
    void currentPhaseAChanged(float currentPhaseA);
    void currentPhaseBChanged(float currentPhaseB);
    void currentPhaseCChanged(float currentPhaseC);
    void powerPhaseAChanged(float powerPhaseA);
    void powerPhaseBChanged(float powerPhaseB);
    void powerPhaseCChanged(float powerPhaseC);
    void totalCurrentPowerChanged(quint16 totalCurrentPower);
    void phaseAImportetChanged(quint16 phaseAImportet);

private:
    ModbusRtuMaster *m_modbusRtuMaster = nullptr;
    quint16 m_slaveId = 1;
    QVector<ModbusRtuReply *> m_pendingInitReplies;

    float m_voltagePhaseA = 0;
    float m_voltagePhaseB = 0;
    float m_voltagePhaseC = 0;
    float m_currentPhaseA = 0;
    float m_currentPhaseB = 0;
    float m_currentPhaseC = 0;
    float m_powerPhaseA = 0;
    float m_powerPhaseB = 0;
    float m_powerPhaseC = 0;
    quint16 m_totalCurrentPower = 0;
    quint16 m_phaseAImportet = 0;

    void verifyInitFinished();

    ModbusRtuReply *readVoltagePhaseA();
    ModbusRtuReply *readVoltagePhaseB();
    ModbusRtuReply *readVoltagePhaseC();
    ModbusRtuReply *readCurrentPhaseA();
    ModbusRtuReply *readCurrentPhaseB();
    ModbusRtuReply *readCurrentPhaseC();
    ModbusRtuReply *readPowerPhaseA();
    ModbusRtuReply *readPowerPhaseB();
    ModbusRtuReply *readPowerPhaseC();
    ModbusRtuReply *readTotalCurrentPower();
    ModbusRtuReply *readPhaseAImportet();


};

QDebug operator<<(QDebug debug, Sdm630ModbusRtuConnection *sdm630ModbusRtuConnection);

#endif // SDM630MODBUSRTUCONNECTION_H
