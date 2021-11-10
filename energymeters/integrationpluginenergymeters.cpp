/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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

#include "integrationpluginenergymeters.h"
#include "plugininfo.h"

IntegrationPluginEnergyMeters::IntegrationPluginEnergyMeters()
{
    m_slaveIdParamTypeIds.insert(pro380ThingClassId, pro380ThingSlaveAddressParamTypeId);
    m_slaveIdParamTypeIds.insert(sdm630ThingClassId, sdm630ThingSlaveAddressParamTypeId);

    m_modbusUuidParamTypeIds.insert(pro380ThingClassId, pro380ThingModbusMasterUuidParamTypeId);
    m_modbusUuidParamTypeIds.insert(sdm630ThingClassId, sdm630ThingModbusMasterUuidParamTypeId);

    m_discoverySlaveAddressParamTypeIds.insert(pro380ThingClassId, pro380DiscoverySlaveAddressParamTypeId);
    m_discoverySlaveAddressParamTypeIds.insert(sdm630ThingClassId, sdm630DiscoverySlaveAddressParamTypeId);

    m_connectionStateTypeIds.insert(pro380ThingClassId, pro380ConnectedStateTypeId);
    m_connectionStateTypeIds.insert(sdm630ThingClassId, sdm630ConnectedStateTypeId);
}

void IntegrationPluginEnergyMeters::init()
{
    connect(hardwareManager()->modbusRtuResource(), &ModbusRtuHardwareResource::modbusRtuMasterRemoved, this, [=] (const QUuid &modbusUuid){
        qCDebug(dcEnergyMeters()) << "Modbus RTU master has been removed" << modbusUuid.toString();

        foreach (Thing *thing, myThings()) {
            if (m_modbusUuidParamTypeIds.contains(thing->thingClassId())) {
                if (thing->paramValue(m_modbusUuidParamTypeIds.value(thing->thingClassId())) == modbusUuid) {
                    qCWarning(dcEnergyMeters()) << "Modbus RTU hardware resource removed for" << thing << ". The thing will not be functional any more until a new resource has been configured for it.";
                    thing->setStateValue(m_connectionStateTypeIds[thing->thingClassId()], false);

//                    EnergyMeter *meter = m_energyMeters.value(thing);
//                    if (!meter)
//                        return;
//                    meter->setModbusMaster(nullptr);
                }
            }
        }
    });
}

void IntegrationPluginEnergyMeters::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcEnergyMeters()) << "Discover things";
    QList<ThingDescriptor> thingDescriptors;

    if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No Modbus RTU interface available."));
        return;
    }

    if (!m_connectionStateTypeIds.contains(info->thingClassId())) {
        Q_ASSERT_X(false, "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
    }
    uint slaveAddress = info->params().paramValue(m_discoverySlaveAddressParamTypeIds.value(info->thingClassId())).toUInt();
    if (slaveAddress > 254 || slaveAddress == 0) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("Modbus slave address must be between 1 and 254"));
        return;
    }
    Q_FOREACH(ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
        qCDebug(dcEnergyMeters()) << "Found RTU master resource" << modbusMaster << "connected" << modbusMaster->connected();
        if (!modbusMaster->connected()) {
            continue;
        }
        ThingDescriptor descriptor(info->thingClassId(), QT_TR_NOOP("Energy meter"), QT_TR_NOOP("Slave address ") +QString::number(slaveAddress)+" "+modbusMaster->serialPort());
        ParamList params;
        params << Param(m_slaveIdParamTypeIds.value(info->thingClassId()), slaveAddress);
        params << Param(m_modbusUuidParamTypeIds.value(info->thingClassId()), modbusMaster->modbusUuid());
        descriptor.setParams(params);
        info->addThingDescriptor(descriptor);
    }
    info->finish(Thing::ThingErrorNoError);
    return;
}

void IntegrationPluginEnergyMeters::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcEnergyMeters()) << "Setup thing" << thing->name();

    if (!m_connectionStateTypeIds.contains(thing->thingClassId())) {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }

//    if (m_energyMeters.contains(thing)) {
//        qCDebug(dcEnergyMeters()) << "Setup after rediscovery, cleaning up ...";
//        m_energyMeters.take(thing)->deleteLater();
//    }
    uint address = thing->paramValue(m_slaveIdParamTypeIds.value(thing->thingClassId())).toUInt();
    if (address > 254 || address == 0) {
        qCWarning(dcEnergyMeters()) << "Setup failed, slave address is not valid" << address;
        info->finish(Thing::ThingErrorSetupFailed, tr("Slave address not valid, must be between 1 and 254"));
        return;
    }
    QUuid uuid = thing->paramValue(m_modbusUuidParamTypeIds.value(thing->thingClassId())).toUuid();
    if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(uuid)) {
        qCWarning(dcEnergyMeters()) << "Setup failed, hardware manager not available";
        info->finish(Thing::ThingErrorSetupFailed, tr("Modbus RTU resource not available."));
        return;
    }

//    EnergyMeter *meter = new EnergyMeter(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), address, m_registerMaps.value(thing->thingClassId()), this);
//    connect(info, &ThingSetupInfo::aborted, meter, &EnergyMeter::deleteLater);
//    connect(meter, &EnergyMeter::consumedEnergyReceived, info, [this, info, meter] (double energy) {
//        qCDebug(dcEnergyMeters()) << "Consumed energy received" << energy << "Setup finished";
//        connect(meter, &EnergyMeter::connectedChanged, this, &IntegrationPluginEnergyMeters::onConnectionStateChanged);
//        connect(meter, &EnergyMeter::voltageL1Received, this, &IntegrationPluginEnergyMeters::onVoltageL1Received);
//        connect(meter, &EnergyMeter::voltageL2Received, this, &IntegrationPluginEnergyMeters::onVoltageL2Received);
//        connect(meter, &EnergyMeter::voltageL3Received, this, &IntegrationPluginEnergyMeters::onVoltageL3Received);
//        connect(meter, &EnergyMeter::currentL1Received, this, &IntegrationPluginEnergyMeters::onCurrentL1Received);
//        connect(meter, &EnergyMeter::currentL2Received, this, &IntegrationPluginEnergyMeters::onCurrentL2Received);
//        connect(meter, &EnergyMeter::currentL3Received, this, &IntegrationPluginEnergyMeters::onCurrentL3Received);
//        connect(meter, &EnergyMeter::activePowerReceived, this, &IntegrationPluginEnergyMeters::onActivePowerReceived);
//        connect(meter, &EnergyMeter::powerFactorReceived, this, &IntegrationPluginEnergyMeters::onPowerFactorReceived);
//        connect(meter, &EnergyMeter::frequencyReceived, this, &IntegrationPluginEnergyMeters::onFrequencyReceived);
//        connect(meter, &EnergyMeter::producedEnergyReceived, this, &IntegrationPluginEnergyMeters::onProducedEnergyReceived);
//        connect(meter, &EnergyMeter::consumedEnergyReceived, this, &IntegrationPluginEnergyMeters::onConsumedEnergyReceived);

//        m_energyMeters.insert(info->thing(), meter);
//        info->finish(Thing::ThingErrorNoError);
//    });
//    meter->getEnergyConsumed();
    return;
}

void IntegrationPluginEnergyMeters::postSetupThing(Thing *thing)
{
    qCDebug(dcEnergyMeters) << "Post setup thing" << thing->name();
    if (m_connectionStateTypeIds.contains(thing->thingClassId())) {
        thing->setStateValue(m_connectionStateTypeIds.value(thing->thingClassId()), true);

//        if (m_energyMeters.contains(thing)) {
//            startUpdateCycle(m_energyMeters.value(thing));
//        } else {
//            qCWarning(dcEnergyMeters()) << "Thing has no energy meter connection and will not work properly";
//        }
    }

//    if (!m_reconnectTimer) {
//        qCDebug(dcEnergyMeters()) << "Starting reconnect timer";
//        m_reconnectTimer = hardwareManager()->pluginTimerManager()->registerTimer(5);
//        connect(m_reconnectTimer, &PluginTimer::timeout, this, [this] {
//            foreach (Thing *thing, myThings()) {
//                if (m_connectionStateTypeIds.contains(thing->thingClassId())) {
//                    if (!thing->stateValue(m_connectionStateTypeIds.value(thing->thingClassId())).toBool()) {
//                        EnergyMeter *meter = m_energyMeters.value(thing);
//                        if (!meter) {
//                            qCWarning(dcEnergyMeters()) << "On reconnect timer, could not find any EnergyMeter connection for" << thing->name();
//                            continue;
//                        }
//                        qCDebug(dcEnergyMeters()) << "On reconnect timer, restarting update cycle for" << thing->name();
//                        startUpdateCycle(meter);
//                    }
//                }
//            }
//        });

//    }
}

void IntegrationPluginEnergyMeters::thingRemoved(Thing *thing)
{
    qCDebug(dcEnergyMeters()) << "Thing removed" << thing->name();

//    if (m_energyMeters.contains(thing)) {
//        EnergyMeter *meter = m_energyMeters.take(thing);
//        m_updateCycleInProgress.remove(meter);
//        meter->deleteLater();
//    }

    if (myThings().isEmpty() && m_reconnectTimer) {
        qCDebug(dcEnergyMeters()) << "Stopping reconnect timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_reconnectTimer);
        m_reconnectTimer = nullptr;
    }
}
