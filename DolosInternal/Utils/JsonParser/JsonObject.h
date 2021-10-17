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

    template <typename T>
    T           GetNumber       (const std::string& szName);
    bool        SetNumber       (const std::string& szName, const double num);

    std::string GetString       (const std::string& szName);
    bool        SetString       (const std::string& szName, const std::string& szValue);

    JsonObject* GetJsonObject   (const std::string& szName);
    bool        SetJsonObject   (const std::string& szName, JsonObject* pObject);

    JsonArray*  GetJsonArray    (const std::string& szName);
    bool        SetJsonArray    (const std::string& szName, JsonArray* pArray);

    void        RemoveValue     (const std::string& szName);

    JsonObject  operator=       (JsonObject& rhs);

    std::unordered_map<std::string, JsonValue*> m_mValues;
    std::string                                 m_szName;

};

#include "JsonValue.h"
#include "JsonArray.h"

template <typename T>
T JsonObject::GetNumber(const std::string& szName) {
    JsonValue* pValue = m_mValues[szName];
    if (pValue) {
        return (T)m_mValues[szName]->m_dbValue;;
    }
    return 0;
}

#endif // !JSON_OBJECT_H
