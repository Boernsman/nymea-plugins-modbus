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

#include "neuron.h"
#include "extern-plugininfo.h"

#include <QByteArray>
#include <QDir>
#include <QElapsedTimer>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

Neuron::Neuron(QObject *parent) : QThread{parent}
{
    qCDebug(dcUniPi()) << "Neuron: Creating Neuron connection";
}

Neuron::~Neuron()
{
    qCDebug(dcUniPi()) << "Neuron: Deleting Neuron connection";
}

bool Neuron::init()
{
    QDir test = QDir("/run/unipi/") ;
    if (test.exists()) {
        qCWarning(dcUniPi()) << "Neuron: SysFs driver is deprecated, update the UniPi kernel module";
        return false;
    }

    QDir sysFsDir = QDir(m_unipiSysFsDir);
    if (!sysFsDir.exists()) {
        qCWarning(dcUniPi()) << "Neuron: SysFs driver is not available";
        return false;
    }

    foreach (auto circuit, m_relayOutputs.keys())
        m_relayOutputs.take(circuit)->deleteLater();
    qCDebug(dcUniPi()) << "Neuron: Discover all relay outputs";
    foreach (auto circuit, sysFsDir.entryList({"RO*"}, QDir::NoFilter, QDir::SortFlag::Name)) {
        NeuronDigitalOutput *relay = new NeuronDigitalOutput(circuit, this);
        connect(relay, &NeuronDigitalOutput::valueChanged, this, &Neuron::relayOutputStatusChanged);
        m_relayOutputs.insert(circuit, relay);
    }

    foreach (auto circuit, m_digitalOutputs.keys())
        m_digitalOutputs.take(circuit)->deleteLater();
    qCDebug(dcUniPi()) << "Neuron: Discover all digital outputs";
    foreach (auto circuit, sysFsDir.entryList({"DO*"}, QDir::NoFilter, QDir::SortFlag::Name)) {
        NeuronDigitalOutput *output = new NeuronDigitalOutput(circuit, this);
        connect(output, &NeuronDigitalOutput::valueChanged, this, &Neuron::digitalOutputStatusChanged);
        m_digitalOutputs.insert(circuit, output);
    }

    foreach (auto circuit, m_digitalInputs.keys())
        m_digitalInputs.take(circuit)->deleteLater();
    qCDebug(dcUniPi()) << "Neuron: Discover all digital inputs";
    foreach (auto circuit, sysFsDir.entryList({"DI*"}, QDir::NoFilter, QDir::SortFlag::Name)) {
        NeuronDigitalInput *input = new NeuronDigitalInput(circuit, this);
        connect(input, &NeuronDigitalInput::valueChanged, this, &Neuron::digitalInputStatusChanged);
        m_digitalInputs.insert(circuit, input);
    }

    foreach (auto circuit, m_analogInputs.keys())
        m_analogInputs.take(circuit)->deleteLater();
    qCDebug(dcUniPi()) << "Neuron: Discover all analog inputs";
    foreach (auto circuit, sysFsDir.entryList({"AI*"}, QDir::NoFilter, QDir::SortFlag::Name)) {
        NeuronAnalogInput *input = new NeuronAnalogInput(circuit, this);
        connect(input, &NeuronAnalogInput::valueChanged, this, &Neuron::analogInputStatusChanged);
        m_analogInputs.insert(circuit, input);
    }

    foreach (auto circuit, m_analogOutputs.keys())
        m_analogOutputs.take(circuit)->deleteLater();
    qCDebug(dcUniPi()) << "Neuron: Discover all analog outputs";
    foreach (auto circuit, sysFsDir.entryList({"AO*"}, QDir::NoFilter, QDir::SortFlag::Name)) {
        NeuronAnalogOutput *output = new NeuronAnalogOutput(circuit, this);
        connect(output, &NeuronAnalogOutput::valueChanged, this, &Neuron::analogOutputStatusChanged);
        m_analogOutputs.insert(circuit, output);
    }
    return true;
}

void Neuron::run()
{
    QElapsedTimer cycleTimer;
    QElapsedTimer statsTimer;
    statsTimer.start();
    int cycleCounter = 0;
    int analogDividerCounter = 0;

    while (!isInterruptionRequested()) {
        cycleTimer.start();

        for (NeuronDigitalOutput *output : qAsConst(m_relayOutputs))
            output->getValue();
        for (NeuronDigitalInput *input : qAsConst(m_digitalInputs))
            input->getValue();
        for (NeuronDigitalOutput *output : qAsConst(m_digitalOutputs))
            output->getValue();

        // Analog inputs change slowly compared to the digital polling rate,
        // so sample them once every c_analogPollDivider cycles.
        if (analogDividerCounter == 0) {
            for (NeuronAnalogInput *input : qAsConst(m_analogInputs))
                input->getValue();
        }
        analogDividerCounter = (analogDividerCounter + 1) % c_analogPollDivider;

        cycleCounter++;
        if (cycleCounter >= 100) {
            qCDebug(dcUniPi()) << "Neuron polling cycle interval"
                               << statsTimer.elapsed() / cycleCounter << "milliseconds";
            statsTimer.restart();
            cycleCounter = 0;
        }

        // Monotonic cycle deadline: sleep only the remainder of this cycle,
        // so a slow iteration does not stretch the next one.
        const qint64 elapsed = cycleTimer.elapsed();
        if (elapsed < c_cycleIntervalMs)
            msleep(c_cycleIntervalMs - elapsed);
    }
    qCDebug(dcUniPi()) << "Neuron: Polling thread stopped";
}

QList<QString> Neuron::relayOutputs()
{
    return m_relayOutputs.keys();
}

QList<QString> Neuron::digitalInputs()
{
    return m_digitalInputs.keys();
}

QList<QString> Neuron::digitalOutputs()
{
    return m_digitalOutputs.keys();
}

QList<QString> Neuron::analogInputs()
{
    return m_analogInputs.keys();
}

QList<QString> Neuron::analogOutputs()
{
    return m_analogOutputs.keys();
}

bool Neuron::setRelayOutput(const QString &circuit, bool value)
{
    NeuronDigitalOutput *relay = m_relayOutputs.value(circuit);
    if (!relay) {
        qCWarning(dcUniPi()) << "Neuron: Relay" << circuit << "not available";
        return false;
    }
    relay->setValue(value);
    return true;
}

bool Neuron::setDigitalOutput(const QString &circuit, bool value)
{
    NeuronDigitalOutput *output = m_digitalOutputs.value(circuit);
    if (!output) {
        qCWarning(dcUniPi()) << "Neuron: Digital output" << circuit << "not available";
        return false;
    }
    output->setValue(value);
    return true;
}

bool Neuron::setAnalogOutput(const QString &circuit, double value)
{
    NeuronAnalogOutput *output = m_analogOutputs.value(circuit);
    if (!output) {
        qCWarning(dcUniPi()) << "Neuron: Analog output" << circuit << "not available";
        return false;
    }
    output->setValue(value);
    return true;
}

NeuronIO::NeuronIO(QObject *parent) : QObject(parent)
{
}

NeuronIO::~NeuronIO()
{
    if (m_fd >= 0) {
        ::close(m_fd);
        m_fd = -1;
    }
}

bool NeuronIO::isAvailable() const
{
    return m_fd >= 0;
}

QString NeuronIO::directory() const
{
    return m_filePath;
}

bool NeuronIO::openPath(const QString &path, int flags)
{
    m_filePath = path;
    m_fd = ::open(path.toLocal8Bit().constData(), flags | O_CLOEXEC);
    if (m_fd < 0) {
        qCWarning(dcUniPi()) << "NeuronIO: Could not open" << path << ":" << strerror(errno);
        return false;
    }
    return true;
}

NeuronDigitalInput::NeuronDigitalInput(const QString &circuit, QObject *parent) : NeuronIO{parent}
{
    m_circuit = circuit;
    openPath(m_unipiSysFsDir + circuit + "/value", O_RDONLY);
    qCDebug(dcUniPi()) << "NeuronDigitalInput: File name" << m_filePath;
}

bool NeuronDigitalInput::getValue()
{
    if (m_fd < 0)
        return m_currentValue;

    char value = 0;
    const ssize_t n = ::pread(m_fd, &value, 1, 0);
    if (n != 1) {
        qCWarning(dcUniPi()) << "NeuronDigitalInput: Read failed for" << m_circuit << ":" << strerror(errno);
        return m_currentValue;
    }

    const bool bValue = (value == '1');
    if (m_currentValue != bValue) {
        m_currentValue = bValue;
        emit valueChanged(m_circuit, bValue);
    }
    return bValue;
}

NeuronDigitalOutput::NeuronDigitalOutput(const QString &circuit, QObject *parent) : NeuronIO{parent}
{
    m_circuit = circuit;
    openPath(m_unipiSysFsDir + circuit + "/value", O_RDWR);
    qCDebug(dcUniPi()) << "NeuronDigitalOutput: File name" << m_filePath;
}

void NeuronDigitalOutput::setValue(bool value)
{
    if (m_fd < 0) {
        qCWarning(dcUniPi()) << "NeuronDigitalOutput: Not initialized, cannot set value for" << m_circuit;
        return;
    }
    qCDebug(dcUniPi()) << "NeuronDigitalOutput: Set value" << value << "to" << m_filePath;
    const char payload = value ? '1' : '0';
    const ssize_t n = ::pwrite(m_fd, &payload, 1, 0);
    if (n != 1)
        qCWarning(dcUniPi()) << "NeuronDigitalOutput: Write failed for" << m_circuit << ":" << strerror(errno);
}

bool NeuronDigitalOutput::getValue()
{
    if (m_fd < 0)
        return m_currentValue;

    char value = 0;
    const ssize_t n = ::pread(m_fd, &value, 1, 0);
    if (n != 1) {
        qCWarning(dcUniPi()) << "NeuronDigitalOutput: Read failed for" << m_circuit << ":" << strerror(errno);
        return m_currentValue;
    }

    const bool bValue = (value == '1');
    if (m_currentValue != bValue) {
        m_currentValue = bValue;
        emit valueChanged(m_circuit, bValue);
    }
    return bValue;
}

NeuronAnalogOutput::NeuronAnalogOutput(const QString &circuit, QObject *parent) : NeuronIO(parent)
{
    m_circuit = circuit;
    openPath(m_unipiSysFsDir + circuit + "/out_voltage_raw", O_WRONLY);
    qCDebug(dcUniPi()) << "NeuronAnalogOutput: File name" << m_filePath;
}

void NeuronAnalogOutput::setValue(double value)
{
    if (m_fd < 0) {
        qCWarning(dcUniPi()) << "NeuronAnalogOutput: Not initialized, cannot set value for" << m_circuit;
        return;
    }
    qCDebug(dcUniPi()) << "NeuronAnalogOutput: Set value" << value << "to" << m_filePath;
    const QByteArray payload = QByteArray::number(value * 1000);
    const ssize_t n = ::pwrite(m_fd, payload.constData(), payload.size(), 0);
    if (n != payload.size()) {
        qCWarning(dcUniPi()) << "NeuronAnalogOutput: Write failed for" << m_circuit << ":" << strerror(errno);
        return;
    }

    if (m_currentValue != value) {
        m_currentValue = value;
        emit valueChanged(m_circuit, value);
    }
}

NeuronAnalogInput::NeuronAnalogInput(const QString &circuit, QObject *parent) : NeuronIO(parent)
{
    m_circuit = circuit;
    openPath(m_unipiSysFsDir + circuit + "/in_voltage_input", O_RDONLY);
    qCDebug(dcUniPi()) << "NeuronAnalogInput: File name" << m_filePath;
}

double NeuronAnalogInput::getValue()
{
    if (m_fd < 0)
        return m_currentValue;

    char buf[32];
    const ssize_t n = ::pread(m_fd, buf, sizeof(buf) - 1, 0);
    if (n < 0) {
        qCWarning(dcUniPi()) << "NeuronAnalogInput: Read failed for" << m_circuit << ":" << strerror(errno);
        return m_currentValue;
    }

    bool ok;
    double voltage = QByteArray(buf, static_cast<int>(n)).trimmed().toDouble(&ok) / 1000.0;
    if (!ok) {
        qCWarning(dcUniPi()) << "NeuronAnalogInput: Could not parse value for" << m_circuit
                             << ":" << QByteArray(buf, static_cast<int>(n));
        return m_currentValue;
    }
    voltage = qMax(voltage, 0.0);

    if (!qFuzzyCompare(m_currentValue, voltage)) {
        m_currentValue = voltage;
        emit valueChanged(m_circuit, voltage);
    }
    return voltage;
}
