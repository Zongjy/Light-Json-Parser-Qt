#include "JsonHandler.h"

QString JsonHandler::rawString(const QString &str) {
    QString result = "\"";
    for (int i = 0; i < str.length(); ++i) {
        if (str[i] == '\"') {
            result += "\\\"";
        } else if (str[i] == '\\') {
            result += "\\\\";
        } else if (str[i] == '/') {
            result += "\\/";
        } else if (str[i] == '\b') {
            result += "\\b";
        } else if (str[i] == '\f') {
            result += "\\f";
        } else if (str[i] == '\n') {
            result += "\\n";
        } else if (str[i] == '\r') {
            result += "\\r";
        } else if (str[i] == '\t') {
            result += "\\t";
        } else {
            result += str[i];
        }
    }
    result += "\"";
    return result;
}

// 将 jsonObject 转换为 QStandardItem
QStandardItem *JsonHandler::jsonObjectToItem(const QJsonObject &jsonObject) {
    QStandardItem *parentItem = new QStandardItem();
    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        QStandardItem *childItem;
        switch(it.value().type()) {
        case QJsonValue::Object:
            childItem = jsonObjectToItem(it.value().toObject());
            childItem->setText(it.key() + ": {Object}");
            break;
        case QJsonValue::Array:
            childItem = jsonArrayToItem((it.value().toArray()));
            childItem->setText(it.key() + ": [Array]");
            break;
        case QJsonValue::Null:
            childItem = new QStandardItem(it.key() + ": null");
            break;
        case QJsonValue::String:
            childItem = new QStandardItem(it.key() + ": " + rawString(it.value().toVariant().toString()));
            break;
        case QJsonValue::Bool:
            childItem = new QStandardItem(it.key() + ": " + (it.value().toBool() ? "true" : "false"));
            break;
        case QJsonValue::Double:
            childItem = new QStandardItem(it.key() + ": " + QString::number(it.value().toDouble()));
            break;
        case QJsonValue::Undefined:
            childItem = new QStandardItem(it.key() + ": undefined");
            break;
        default:
            childItem = new QStandardItem(it.key() + ": unknown type");
            break;
        }
        parentItem->appendRow(childItem);
    }
    return parentItem;
}

// 将 jsonArray 转换为 QStandardItem
QStandardItem *JsonHandler::jsonArrayToItem(const QJsonArray &jsonArray) {
    QStandardItem *parentItem = new QStandardItem();
    int index = 0;
    for (auto it = jsonArray.begin(); it != jsonArray.end(); ++it, ++index) {
        QStandardItem *childItem;
        switch(it->type()) {
        case QJsonValue::Object:
            childItem = jsonObjectToItem(it->toObject());
            childItem->setText(QString::number(index) + ": {Object}");
            break;
        case QJsonValue::Array:
            childItem = jsonArrayToItem(it->toArray());
            childItem->setText(QString::number(index) + ": [Array]");
            break;
        case QJsonValue::Null:
            childItem = new QStandardItem(QString::number(index) + ": null");
            break;
        case QJsonValue::String:
            childItem = new QStandardItem(QString::number(index) + ": " + rawString(it->toVariant().toString()));
            break;
        case QJsonValue::Bool:
            childItem = new QStandardItem(QString::number(index) + ": " + (it->toBool() ? "true" : "false"));
            break;
        case QJsonValue::Double:
            childItem = new QStandardItem(QString::number(index) + ": " + QString::number(it->toDouble()));
            break;
        case QJsonValue::Undefined:
            childItem = new QStandardItem(QString::number(index) + ": undefined");
            break;
        default:
            childItem = new QStandardItem(QString::number(index) + ": unknown type");
            break;
        }
        parentItem->appendRow(childItem);
    }
    return parentItem;
}

// 将 json 字符串反序列化为一个便于处理的 QJsonValue 格式，如果解析失败抛出 std::runtime_error
QJsonValue JsonHandler::deserializeToJsonValue(const QString &jsonString) {
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    if(parseError.error != QJsonParseError::NoError) {
        if(parseError.error == QJsonParseError::IllegalValue) {
            QJsonValue value = QJsonDocument::fromJson(QString("{\"value\":%1}").arg(jsonString).toUtf8(), &parseError)["value"];
            if(parseError.error == QJsonParseError::NoError) {
                return value;
            } else {
                throw std::runtime_error(QString("%1").arg(parseError.errorString()).toStdString());
            }
        }
        else {
            throw std::runtime_error(QString("%1").arg(parseError.errorString()).toStdString());
        }
    }

    if(document.isObject()) {
        return document.object();
    } else if(document.isArray()) {
        return document.array();
    } else {
        throw std::runtime_error("Unsupported JSON structure type");
    }
}


// 将 jsonValue 转换为通用的数据类型 QStandardItemModel，以便使用 QTreeView 展示树状结构
QStandardItemModel* JsonHandler::valueToModel(const QJsonValue &value) {
    QStandardItem *rootItem = new QStandardItem("JSON");
    if (value.isObject()) {
        QStandardItem *item = jsonObjectToItem(value.toObject());
        while(item->rowCount() > 0) {
            rootItem->appendRow(item->takeRow(0));
        }
        delete item;
    } else if (value.isArray()) {
        QStandardItem *item = jsonArrayToItem(value.toArray());
        while(item->rowCount() > 0) {
            rootItem->appendRow(item->takeRow(0));
        }
        delete item;
    } else if (value.isDouble()) {
        QStandardItem *item = new QStandardItem(QString::number(value.toDouble()));
        rootItem->appendRow(item);
    } else if (value.isBool()) {
        QStandardItem *item = new QStandardItem(value.toBool() ? "true" : "false");
        rootItem->appendRow(item);
    } else if (value.isNull()) {
        QStandardItem *item = new QStandardItem("null");
        rootItem->appendRow(item);
    } else if (value.isString()){
        QStandardItem *item = new QStandardItem("\"" + value.toString() + "\"");
        rootItem->appendRow(item);
    } else {
        throw std::runtime_error("Unsupported JSON value type");
    }

    QStandardItemModel *model = new QStandardItemModel();
    model->appendRow(rootItem);
    return model;
}


// 通过 QJsonValue 序列化为 Json 字符串
QString JsonHandler::serializeValue(const QJsonValue &value, QJsonDocument::JsonFormat formatType) {
    QJsonDocument doc;

    if (value.isObject()) {
        doc.setObject(value.toObject());
        return QString(doc.toJson(formatType));
    } else if (value.isArray()) {
        doc.setArray(value.toArray());
        return QString(doc.toJson(formatType));
    } else if (value.isDouble()) {
        return QString::number(value.toDouble());
    } else if (value.isBool()) {
        return value.toBool() ? "true" : "false";
    } else if (value.isNull()) {
        return "null";
    } else if (value.isString()) {
        return rawString(value.toString());
    } else {
        throw std::runtime_error("Unsupported JSON value type");
    }
}
