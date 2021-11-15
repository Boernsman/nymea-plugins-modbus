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

#ifndef HUAWEIMODBUSTCPCONNECTION_H
#define HUAWEIMODBUSTCPCONNECTION_H

#include <QObject>

#include "../modbus/modbusdatautils.h"
#include "../modbus/modbustcpmaster.h"

class HuaweiModbusTcpConnection : public ModbusTCPMaster
{
    Q_OBJECT
public:
    enum InverterDeviceStatus {
        InverterDeviceStatusStandbyInitializing = 0,
        InverterDeviceStatusStandbyDetectingInsulationResistance = 1,
        InverterDeviceStatusStandbyDetectingIrradiation = 2,
        InverterDeviceStatusStandbyDridDetecting = 3,
        InverterDeviceStatusStarting = 256,
        InverterDeviceStatusOnGrid = 512,
        InverterDeviceStatusPowerLimited = 513,
        InverterDeviceStatusSelfDerating = 514,
        InverterDeviceStatusShutdownFault = 768,
        InverterDeviceStatusShutdownCommand = 769,
        InverterDeviceStatusShutdownOVGR = 770,
        InverterDeviceStatusShutdownCommunicationDisconnected = 771,
        InverterDeviceStatusShutdownPowerLimit = 772,
        InverterDeviceStatusShutdownManualStartupRequired = 773,
        InverterDeviceStatusShutdownInputUnderpower = 774,
        InverterDeviceStatusGridSchedulingPCurve = 1025,
        InverterDeviceStatusGridSchedulingQUCurve = 1026,
        InverterDeviceStatusGridSchedulingPFUCurve = 1027,
        InverterDeviceStatusGridSchedulingDryContact = 1028,
        InverterDeviceStatusGridSchedulingQPCurve = 1029,
        InverterDeviceStatusSpotCheckReady = 1280,
        InverterDeviceStatusSpotChecking = 1281,
        InverterDeviceStatusInspecting = 1536,
        InverterDeviceStatusAfciSelfCheck = 1792,
        InverterDeviceStatusIVScanning = 2048,
        InverterDeviceStatusDCInputDetection = 2304,
        InverterDeviceStatusRunningOffGridCharging = 2560,
        InverterDeviceStatusStandbyNoIrradiation = 40960
    };
    Q_ENUM(InverterDeviceStatus)

    explicit HuaweiModbusTcpConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);
    ~HuaweiModbusTcpConnection() = default;

    /* Inverter active power [kW] - Address: 32080, Size: 2 */
    float inverterActivePower() const;

    /* Inverter grid frequency [Hz] - Address: 32085, Size: 1 */
    float inverterGridFrequency() const;

    /* Inverter internal temperature [°C] - Address: 32087, Size: 1 */
    float inverterInternalTemperature() const;

    /* Inverter device status - Address: 32089, Size: 1 */
    InverterDeviceStatus inverterDeviceStatus() const;

    /* Inverter accumulated energy yield [kWh] - Address: 32106, Size: 2 */
    float inverterAccumulatedEnergyYield() const;

    /* Inverter daily energy yield [kWh] - Address: 32114, Size: 2 */
    float inverterDailyEnergyYield() const;

    /* Power meter active power [W] - Address: 37113, Size: 2 */
    qint32 powerMeterActivePower() const;

    /* Model - Address: 30000, Size: 15 */
    QString model() const;

    /* Serial number - Address: 30015, Size: 10 */
    QString serialNumber() const;

    /* Product number - Address: 30025, Size: 10 */
    QString productNumber() const;

    /* Read block from start addess 30000 with size of 35 registers containing following 3 properties:
    -- Model - Address: 30000, Size: 15
    -- Serial number - Address: 30015, Size: 10
    -- Product number - Address: 30025, Size: 10
    */ 
    void updateInverterInformationBlock();
    /* Model identifier - Address: 30070, Size: 1 */
    quint16 modelId() const;

    /* Number of PV strings - Address: 30071, Size: 1 */
    quint16 numberOfPvString() const;

    /* Number of MPP tracks - Address: 30072, Size: 1 */
    quint16 numberOfMppTracks() const;

    /* Read block from start addess 30070 with size of 3 registers containing following 3 properties:
    -- Model identifier - Address: 30070, Size: 1
    -- Number of PV strings - Address: 30071, Size: 1
    -- Number of MPP tracks - Address: 30072, Size: 1
    */ 
    void updateInverterConfigurationBlock();
    /* Inverter phase A voltage [V] - Address: 32069, Size: 1 */
    float inverterVoltagePhaseA() const;

    /* Inverter phase B voltage [V] - Address: 32070, Size: 1 */
    float inverterVoltagePhaseB() const;

    /* Inverter phase C voltage [V] - Address: 32071, Size: 1 */
    float inverterVoltagePhaseC() const;

    /* Inverter phase A current [A] - Address: 32072, Size: 2 */
    float inverterPhaseACurrent() const;

    /* Inverter phase B current [A] - Address: 32074, Size: 2 */
    float inverterPhaseBCurrent() const;

    /* Inverter phase C current [A] - Address: 32076, Size: 2 */
    float inverterPhaseCCurrent() const;

    /* Read block from start addess 32069 with size of 9 registers containing following 6 properties:
      - Inverter phase A voltage [V] - Address: 32069, Size: 1
      - Inverter phase B voltage [V] - Address: 32070, Size: 1
      - Inverter phase C voltage [V] - Address: 32071, Size: 1
      - Inverter phase A current [A] - Address: 32072, Size: 2
      - Inverter phase B current [A] - Address: 32074, Size: 2
      - Inverter phase C current [A] - Address: 32076, Size: 2
    */ 
    void updateInverterVoltageBlock();

    virtual void initialize();
    virtual void update();

    void updateInverterActivePower();
    void updateInverterGridFrequency();
    void updateInverterInternalTemperature();
    void updateInverterDeviceStatus();
    void updateInverterAccumulatedEnergyYield();
    void updateInverterDailyEnergyYield();
    void updatePowerMeterActivePower();

signals:
    void initializationFinished();

    void inverterActivePowerChanged(float inverterActivePower);
    void inverterGridFrequencyChanged(float inverterGridFrequency);
    void inverterInternalTemperatureChanged(float inverterInternalTemperature);
    void inverterDeviceStatusChanged(InverterDeviceStatus inverterDeviceStatus);
    void inverterAccumulatedEnergyYieldChanged(float inverterAccumulatedEnergyYield);
    void inverterDailyEnergyYieldChanged(float inverterDailyEnergyYield);
    void powerMeterActivePowerChanged(qint32 powerMeterActivePower);
    void modelChanged(const QString &model);
    void serialNumberChanged(const QString &serialNumber);
    void productNumberChanged(const QString &productNumber);
    void modelIdChanged(quint16 modelId);
    void numberOfPvStringChanged(quint16 numberOfPvString);
    void numberOfMppTracksChanged(quint16 numberOfMppTracks);
    void inverterVoltagePhaseAChanged(float inverterVoltagePhaseA);
    void inverterVoltagePhaseBChanged(float inverterVoltagePhaseB);
    void inverterVoltagePhaseCChanged(float inverterVoltagePhaseC);
    void inverterPhaseACurrentChanged(float inverterPhaseACurrent);
    void inverterPhaseBCurrentChanged(float inverterPhaseBCurrent);
    void inverterPhaseCCurrentChanged(float inverterPhaseCCurrent);

private:
    quint16 m_slaveId = 1;
    QVector<QModbusReply *> m_pendingInitReplies;

    float m_inverterActivePower = 0;
    float m_inverterGridFrequency = 0;
    float m_inverterInternalTemperature = 0;
    InverterDeviceStatus m_inverterDeviceStatus = InverterDeviceStatusStandbyInitializing;
    float m_inverterAccumulatedEnergyYield = 0;
    float m_inverterDailyEnergyYield = 0;
    qint32 m_powerMeterActivePower = 0;
    QString m_model;
    QString m_serialNumber;
    QString m_productNumber;
    quint16 m_modelId = 0;
    quint16 m_numberOfPvString = 0;
    quint16 m_numberOfMppTracks = 0;
    float m_inverterVoltagePhaseA = 0;
    float m_inverterVoltagePhaseB = 0;
    float m_inverterVoltagePhaseC = 0;
    float m_inverterPhaseACurrent = 0;
    float m_inverterPhaseBCurrent = 0;
    float m_inverterPhaseCCurrent = 0;

    void verifyInitFinished();

    QModbusReply *readInverterActivePower();
    QModbusReply *readInverterGridFrequency();
    QModbusReply *readInverterInternalTemperature();
    QModbusReply *readInverterDeviceStatus();
    QModbusReply *readInverterAccumulatedEnergyYield();
    QModbusReply *readInverterDailyEnergyYield();
    QModbusReply *readPowerMeterActivePower();


};

QDebug operator<<(QDebug debug, HuaweiModbusTcpConnection *huaweiModbusTcpConnection);

#endif // HUAWEIMODBUSTCPCONNECTION_H
