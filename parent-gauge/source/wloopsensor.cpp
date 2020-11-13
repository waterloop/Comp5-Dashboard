#include "wloopsensor.h"

wloopSensor::wloopSensor(QJsonObject obj){
    if(obj.isEmpty()){
        valid = 0;
        return;
    }
    for(QJsonObject::iterator it = obj.begin(); it!= obj.end(); it++){
        if(it.key() == "name"){
            if(it.value().type() != QJsonValue::String) {
                this->valid = 0;
                return;
            }
            this->name = it.value().toString();
        }
        else if(it.key() == "units"){
            if(it.value().type() != QJsonValue::String) {
                this->valid = 0;
                return;
            }
            this->units = it.value().toString();
        }
        else if(it.key() == "maxRange"){
            if(it.value().type() != QJsonValue::Array){
                this->valid = 0;
                return;
            }
            QJsonArray temp = it.value().toArray();
            if(temp.size() != 2){
                this->valid = 0;
                return;
            }
            if(temp.at(0).type() != QJsonValue::Double){
                this->valid = 0;
                return;
            }
            this->maxRange.first = temp.at(0).toDouble();
            if(temp.at(1).type() != QJsonValue::Double){
                this->valid = 0;
            }
            this->maxRange.second = temp.at(1).toDouble();

        }
        else if(it.key() == "accRange"){
            if(it.value().type() != QJsonValue::Array){
                this->valid = 0;
                return;
            }
            QJsonArray temp = it.value().toArray();
            if(temp.size() != 2){
                this->valid = 0;
                return;
            }
            if(temp.at(0).type() != QJsonValue::Double){
                this->valid = 0;
                return;
            }
            this->accRange.first = temp.at(0).toDouble();
            if(temp.at(1).type() != QJsonValue::Double){
                this->valid = 0;
            }
            this->accRange.second = temp.at(1).toDouble();
        }
        else if(it.key() == "id"){
            if(it.value().type()!= QJsonValue::Double){
                this->valid = 0;
                return;
            }
            this->id =static_cast<qint16>(it.value().toDouble());
        }
    }
}

QString wloopSensor::getName(){
    if(this->valid){
        return this->name;
    }
    return "";
}

QString wloopSensor::getUnits(){
    if(this->valid){
        return this->units;
    }
    return "";
}

enum sensorState wloopSensor::getState(){
    if(this->valid){
        return this->state;
    }
    return sensorState::unsafe;
}

void wloopSensor::setValItem(QTableWidgetItem *item){
    if(this->valid){
        this->valItem = item;
    }
}

void wloopSensor::setNameItem(QTableWidgetItem *item){
    if(this->valid){
        this->nameItem = item;
    }
}

QTableWidgetItem * wloopSensor::getValItem(){
    return this->valItem;
}

QTableWidgetItem * wloopSensor::getNameItem(){
    return this->nameItem;
}

void wloopSensor::changeValue(qreal val){
    if(this->valid){
        this->value = val;
        emit valueChanged(this->valItem, val, this->units);
        if(this->state == sensorState::uninitialized){
            if(val >= this->accRange.first && val<= this->accRange.second) this->state = sensorState::safe;
            else this->state = sensorState::unsafe;
            emit stateChanged(this->valItem, this->state);
        }
        else{
            enum sensorState oldState = this->state;
            if(val >= this->accRange.first && val<= this->accRange.second) this->state = sensorState::safe;
            else this->state = sensorState::unsafe;
            if(this->state != oldState){
                emit stateChanged(this->valItem, this->state);
            }
        }

    }
}

void wloopSensor::changeValueSlider(qreal newVal){
    changeValue(this->maxRange.first + static_cast<qreal>(newVal) / 99 * (this->maxRange.second - this->maxRange.first));
}

qreal wloopSensor::getValue(){
    if(this->valid){
        return  this->value;
    }
    return  -1;
}

QVector<wloopSensor *> getSensorArray(QJsonDocument doc){
    QVector<wloopSensor *> res;
    if(doc.isNull()) return res;
    if(!doc.isArray()) return res;
    QJsonArray temp = doc.array();
    for(QJsonArray::iterator it = temp.begin(); it!=temp.end(); it++){
        if((*it).type() != QJsonValue::Object){
            continue;
        }
        else{
            res.push_back(new wloopSensor((*it).toObject()));
        }
    }
    return res;
}
