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

#ifndef NEURON_H
#define NEURON_H

#include <QThread>
#include <QObject>
#include <QHash>

// Base class for a single unipi sysfs I/O endpoint. Each instance owns a raw
// file descriptor for its /value (or similar) node. All I/O goes through
// pread(2)/pwrite(2) at offset 0, which are atomic with respect to the file
// position; there is no shared seek offset between concurrent readers and
// writers, so no mutex is needed to make the polling thread safe against
// writes from the main thread.
class NeuronIO : public QObject
{
    Q_OBJECT

public:
    explicit NeuronIO(QObject *parent = nullptr);
    ~NeuronIO() override;

    bool isAvailable() const;
    QString directory() const;

protected:
    bool openPath(const QString &path, int flags);

    QString m_circuit;
    QString m_filePath;
    int m_fd = -1;
    const QString m_unipiSysFsDir = "/run/unipi-plc/by-sys/";
};

class NeuronDigitalOutput : public NeuronIO
{
    Q_OBJECT
public:
    explicit NeuronDigitalOutput(const QString &circuit, QObject *parent = nullptr);

    void setValue(bool value);
    bool getValue();

private:
    bool m_currentValue = false;

signals:
    void valueChanged(const QString &circuit, bool value);
};

class NeuronDigitalInput : public NeuronIO
{
    Q_OBJECT
public:
    explicit NeuronDigitalInput(const QString &circuit, QObject *parent = nullptr);

    bool getValue();

private:
    bool m_currentValue = false;

signals:
    void valueChanged(const QString &circuit, bool value);
};

class NeuronAnalogInput : public NeuronIO
{
    Q_OBJECT
public:
    explicit NeuronAnalogInput(const QString &circuit, QObject *parent = nullptr);

    double getValue();

private:
    double m_currentValue = 0.0;

signals:
    void valueChanged(const QString &circuit, double value);
};

class NeuronAnalogOutput : public NeuronIO
{
    Q_OBJECT
public:
    explicit NeuronAnalogOutput(const QString &circuit, QObject *parent = nullptr);

    void setValue(double value);

private:
    double m_currentValue = 0.0;

signals:
    void valueChanged(const QString &circuit, double value);
};

class Neuron : public QThread
{
    Q_OBJECT
public:

    explicit Neuron(QObject *parent = nullptr);
    ~Neuron() override;

    bool init();

    QList<QString> digitalInputs();
    QList<QString> relayOutputs();
    QList<QString> digitalOutputs();
    QList<QString> analogInputs();
    QList<QString> analogOutputs();

    bool setRelayOutput(const QString &circuit, bool value);
    bool setDigitalOutput(const QString &circuit, bool value);
    bool setAnalogOutput(const QString &circuit, double value);

protected:
    void run() override;

private:
    const QString m_unipiSysFsDir = "/run/unipi-plc/by-sys/";

    // Base cycle for digital I/O polling, in milliseconds.
    static constexpr int c_cycleIntervalMs = 50;
    // Poll analog inputs every N cycles. 10 × 50 ms = 500 ms is plenty for
    // 0-10 V inputs that physically can't change that fast.
    static constexpr int c_analogPollDivider = 10;

    QHash<QString, NeuronDigitalOutput *> m_relayOutputs;
    QHash<QString, NeuronDigitalOutput *> m_digitalOutputs;
    QHash<QString, NeuronDigitalInput *> m_digitalInputs;
    QHash<QString, NeuronAnalogOutput *> m_analogOutputs;
    QHash<QString, NeuronAnalogInput *> m_analogInputs;

signals:
    void relayOutputStatusChanged(const QString &circuit, bool value);
    void digitalOutputStatusChanged(const QString &circuit, bool value);
    void digitalInputStatusChanged(const QString &circuit, bool value);

    void analogInputStatusChanged(const QString &circuit, double value);
    void analogOutputStatusChanged(const QString &circuit, double value);
};
#endif // NEURON_H
