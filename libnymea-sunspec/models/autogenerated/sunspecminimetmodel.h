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

#ifndef SUNSPECMINIMETMODEL_H
#define SUNSPECMINIMETMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecMiniMetModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecMiniMetModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecMiniMetModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 ghi() const;
    float temp() const;
    float ambientTemperature() const;
    quint16 windSpeed() const;

protected:
    void processBlockData() override;

private:
    quint16 m_ghi;
    float m_temp;
    float m_ambientTemperature;
    quint16 m_windSpeed;

    void initDataPoints();

};

#endif // SUNSPECMINIMETMODEL_H
