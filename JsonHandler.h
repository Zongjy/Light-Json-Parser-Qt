#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QStandardItemModel>
// 定义 JsonHandler 类
class JsonHandler
{
public:
    static QString serializeValue(const QJsonValue &value, QJsonDocument::JsonFormat formatType);
    static QStandardItem* jsonValueToItem(const QJsonValue& jsonValue);
    static QStandardItem* jsonObjectToItem(const QJsonObject& jsonObject);
    static QStandardItem* jsonArrayToItem(const QJsonArray& jsonArray);
    static QJsonValue deserializeToJsonValue(const QString &jsonString);
    static QStandardItemModel* valueToModel(const QJsonValue &value);
    static QString rawString(const QString &str);
};

#endif // JSONHANDLER_H
