#pragma once

#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H




#include <string>
#include <vector>

class JsonObject;
class JsonValue;

class JsonArray {
public:
                JsonArray           (void);
                JsonArray           (const JsonArray& copyArray);
                ~JsonArray          (void);

    bool        GetBoolean          (int index) const;
    bool        SetBoolean          (int index, const bool bValue);
    int         AddBoolean          (bool bValue, int index = -1);

    template <typename T>
    T           GetNumber           (int index) const;
    bool        SetNumber           (int index, const double num);
    int         AddNumber           (const double num, int index = -1);

    std::string GetString           (int index) const;
    bool        SetString           (int index, const std::string& szValue);
    int         AddString           (const std::string& szValue, int index = -1);

    JsonObject* GetJsonObject       (int index) const;
    bool        SetJsonObject       (int index, JsonObject* pObject);
    int         AddJsonObject       (JsonObject* pObject, int index = -1);

    template <typename T>
    JsonObject* GetJsonObjectByValue(const std::string& szValueName, T tValue) const;

    JsonArray*  GetJsonArray        (int index) const;
    bool        SetJsonArray        (int index, JsonArray* pArray);
    int         AddJsonArray        (JsonArray* pArray, int index = -1);

    void        RemoveValue         (int index);

    int         GetSize             (void);

    JsonArray   operator=           (const JsonArray& rhs);

    JsonValue*& operator[]          (int index);


    std::vector<JsonValue*> m_vValues;
    std::string             m_szName;
};

#include "JsonValue.h"
#include "JsonObject.h"

template <typename T>
T JsonArray::GetNumber(int index) const {
    if (index >= 0 && index < m_vValues.size()) {
        return (T)m_vValues[index]->m_dbValue;
    }
    return 0;
}

template <typename T>
JsonObject* JsonArray::GetJsonObjectByValue(const std::string& szValueName, T tValue) const {
    for (int i = 0; i < m_vValues.size(); i++) {
        if (m_vValues[i]->m_tType == VALUE_TYPE::OBJECT) {
            if (tValue == (T)(m_vValues[i]->m_pObject->m_mValues[szValueName]->m_dbValue)) {
                return m_vValues[i]->m_pObject;
            }
        }
    }
    return nullptr;
}
#endif // !JSON_ARRAY_H
