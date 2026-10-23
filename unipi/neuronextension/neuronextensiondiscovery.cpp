/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2024, nymea GmbH
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

#include "neuronextensiondiscovery.h"
#include "extern-plugininfo.h"

#include <QTimer>

NeuronExtensionDiscovery::NeuronExtensionDiscovery(NeuronExtensionBus *neuronExtensionBus, int startAddress, int endAddress, QObject *parent) :
    QObject(parent),
    m_neuronExtensionBus(neuronExtensionBus),
    m_startAddress(startAddress),
    m_endAddress(endAddress)
{
}

bool NeuronExtensionDiscovery::startDiscovery()
{
    qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: start Discovery, start Address" << m_startAddress << "end address" << m_endAddress;

    if (m_discoveryOngoing) {
        qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: Discovery is already in progress";
        return false;
    }
    m_sweepingAddress = m_startAddress;
    m_discoveredExtensions.clear();
    m_discoveryOngoing = true;
    getNext(m_startAddress);
    return true;
}

void NeuronExtensionDiscovery::stopDiscovery()
{
    qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: stopping discovery";
    m_sweepingAddress = m_startAddress;
    m_discoveryOngoing = false;
}

void NeuronExtensionDiscovery::getNext(int address)
{
    NeuronExtensionRequest request(NeuronExtensionRegisterType::RegisterTypeRegisters, address, 1000, 7, false);
    auto *reply = m_neuronExtensionBus->sendRequest(request);
    if (!reply) {
        qCWarning(dcUniPi()) << "NeuronExtensionDiscovery: Discovery probe returned no reply";
        m_discoveryOngoing = false;
        emit finished(m_discoveredExtensions);
        return;
    }
    if (reply->isFinished()) {
        processResponse(reply);
        reply->deleteLater();
    } else {
        connect(reply, &NeuronExtensionReply::finished, reply, &NeuronExtensionReply::deleteLater);
        connect(reply, &NeuronExtensionReply::finished, this, [this, reply] {
            processResponse(reply);
        });
    }
}

void NeuronExtensionDiscovery::processResponse(NeuronExtensionReply *reply)
{
    if (reply->slaveAddress() == m_sweepingAddress) {
        m_sweepingAddress = reply->slaveAddress()+1;
    } else if (reply->slaveAddress() < m_sweepingAddress){
        // A reply returns multiple finish signals depending on the retry
        qCWarning(dcUniPi()) << "NeuronExtensionDiscovery: Got modbus reply from previous request, ignoring";
        return;
    }

    if (reply->error() == NeuronExtensionReply::Error::ErrorNoError) {
        QVector<quint16> result = reply->result();
        if (result.length() == 7) {
            const int hwId = result[4];
            const quint32 serial = (static_cast<quint32>(result[6]) << 16) | result[5];

            NeuronExtension::ExtensionTypes model = NeuronExtension::ExtensionTypes::Unknown;
            if (hwId == 1) {
                model = NeuronExtension::ExtensionTypes::xS10;
            } else if (hwId == 784) {
                model = NeuronExtension::ExtensionTypes::xS30;
            } else if (hwId == 528) {
                model = NeuronExtension::ExtensionTypes::xS40;
            } else if (hwId == 5) {
                model = NeuronExtension::ExtensionTypes::xS50;
            } else if (hwId == 272) {
                model = NeuronExtension::ExtensionTypes::xS11;
            } else if (hwId == 273) {
                model = NeuronExtension::ExtensionTypes::xS51;
            }

            if (model != NeuronExtension::ExtensionTypes::Unknown) {
                qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: Found" << NeuronExtension::stringFromType(model)
                                   << "at slave" << reply->slaveAddress()
                                   << "on" << m_neuronExtensionBus->serialPort()
                                   << "(serial" << serial << ")";
                m_discoveredExtensions.insert(reply->slaveAddress(), model);
                emit deviceFound(reply->slaveAddress(), model);
            } else {
                qCWarning(dcUniPi()) << "NeuronExtensionDiscovery: Unknown Neuron extension model, hwId"
                                     << hwId << "at slave" << reply->slaveAddress();
            }
        }
    }
    if (reply->slaveAddress() >= m_endAddress) {
        m_discoveryOngoing = false;
        qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: Discovery finished, found" << m_discoveredExtensions.count() << "devices";
        emit finished(m_discoveredExtensions);
    } else {
        if (m_discoveryOngoing) {
            // Defer the next probe to the event loop. The bus runs synchronously,
            // so a direct recursive call here would stack-grow across the full
            // sweep window and block the thread for the whole sweep duration.
            const int next = m_sweepingAddress;
            QTimer::singleShot(0, this, [this, next] {
                if (m_discoveryOngoing)
                    getNext(next);
            });
        }
    }
}
