#include "airspeedindicator.h"

AirspeedIndicator::AirspeedIndicator(QObject *parent, ExtPlaneConnection *conn) : NeedleInstrument(parent),
_client(this, typeName(), conn) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_pilot", 0.1);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(speedChanged(QString,double)));
    setBars(10, 5);
    setNumbers(20);
    setUnit(VELOCITY_KTS);
    //    setMaxValue(300);
}

void AirspeedIndicator::speedChanged(QString name, double speed) {
    setValue(Units::convertSpeed(VELOCITY_KTS, units, speed));
}

void AirspeedIndicator::setUnit(VelocityUnit unit) {
    units = unit;
    setLabel(Units::unitName(units));
}

void AirspeedIndicator::storeSettings(QSettings &settings) {
    NeedleInstrument::storeSettings(settings);

    settings.setValue("unit", Units::unitName(units));
    //    settings.setValue("maxvalue", QString::number(maxValue));
}

void AirspeedIndicator::loadSettings(QSettings &settings) {
    NeedleInstrument::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    VelocityUnit unit = Units::velocityUnitForName(unitname);
    setUnit(unit);
    //    setMaxValue(settings.value("maxvalue", 300).toFloat());
}

QString AirspeedIndicator::typeName() {
    return typeNameStatic();
}

QString AirspeedIndicator::typeNameStatic() {
    return "indicator/airspeed/round";
}
//void AirspeedIndicator::setMaxValue(float mv) {
    //    maxValue = mv;
    // setScale(25, NeedleInstrument::_zeroValue, 0+330, maxValue);
    //}

void AirspeedIndicator::createSettings(QGridLayout *layout) {
    QLabel *unitsLabel = new QLabel("Unit", layout->parentWidget());
    layout->addWidget(unitsLabel, layout->rowCount(), 0);
    VelocityUnitComboBox *unitsCombo = new VelocityUnitComboBox(layout->parentWidget(), units);
    connect(unitsCombo, SIGNAL(unitSelected(VelocityUnit)), this, SLOT(setUnit(VelocityUnit)));
    layout->addWidget(unitsCombo);

    NeedleInstrument::createSettings(layout);

}
