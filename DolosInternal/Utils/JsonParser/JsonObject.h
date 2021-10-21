#pragma once

#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H




#include <string>
#include <unordered_map>

class JsonArray;
class JsonValue;

class JsonObject {
public:
                JsonObject      (void);
                JsonObject      (JsonObject& copyObject);
                ~JsonObject     (void);

    bool        GetBoolean      (const std::string& szName);
    bool        SetBoolean      (const std::string& szName, const bool bValue);
    bool        AddBoolean      (const std::string& szName, const bool bValue);

    template <typename T>
    T           GetNumber       (const std::string& szName);
    bool        SetNumber       (const std::string& szName, const double num);
    bool        AddNumber       (const std::string& szName, const double num);

    std::string GetString       (const std::string& szName);
    bool        SetString       (const std::string& szName, const std::string& szValue);
    bool        AddString       (const std::string& szName, const std::string& szValue);

    JsonObject* GetJsonObject   (const std::string& szName);
    bool        SetJsonObject   (const std::string& szName, JsonObject* pObject);
    bool        AddJsonObject   (const std::string& szName, JsonObject* pObject);

    JsonArray*  GetJsonArray    (const std::string& szName);
    bool        SetJsonArray(const std::string& szName, JsonArray* pArray);
    bool        AddJsonArray(const std::string& szName, JsonArray* pArray);

    void        RemoveValue     (const std::string& szName);

    JsonObject  operator=       (JsonObject& rhs);

    std::vector<JsonValue*>                 m_vValues;
    std::unordered_map<std::string, int>    m_mValues;
    std::string                             m_szName;

};

#include "JsonValue.h"
#include "JsonArray.h"

template <typename T>
T JsonObject::GetNumber(const std::string& szName) {
    if (!this) return (T)0;
    JsonValue* pValue = m_vValues[m_mValues[szName]];
    if (pValue && pValue->m_tType == VALUE_TYPE::NUMBER) {
        return (T)m_vValues[m_mValues[szName]]->m_dbValue;;
    }
    return 0;
}

#endif // !JSON_OBJECT_H
