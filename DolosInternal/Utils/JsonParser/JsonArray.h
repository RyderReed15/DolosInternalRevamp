#pragma once

#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H




#include <string>
#include <vector>

class JsonObject;
class JsonValue;

class JsonArray {
public:
    //Constructors and Destructors
                JsonArray           (void);
                JsonArray           (const JsonArray& copyArray);
                ~JsonArray          (void);

    //Getters and setters for Boolean Values - Get by index in array
    bool        GetBoolean          (int index) const;
    bool        SetBoolean          (int index, const bool bValue);
    int         AddBoolean          (bool bValue, int index = -1);

    //Getters and setters for Number Values - Get by index in array
    template <typename T>
    T           GetNumber           (int index) const;
    bool        SetNumber           (int index, const double num);
    int         AddNumber           (const double num, int index = -1);

    //Getters and setters for String Values - Get by index in array
    std::string GetString           (int index) const;
    bool        SetString           (int index, const std::string& szValue);
    int         AddString           (const std::string& szValue, int index = -1);

    //Getters and setters for JsonObject Values - Get by index in array
    JsonObject* GetJsonObject       (int index) const;
    bool        SetJsonObject       (int index, JsonObject* pObject);
    int         AddJsonObject       (JsonObject* pObject, int index = -1);

    //Finds a json object with a specific value and returns it
    template <typename T>
    JsonObject* GetJsonObjectByValue(const std::string& szValueName, T tValue) const;

    //Getters and setters for JsonArray Values - Get by index in array
    JsonArray*  GetJsonArray        (int index) const;
    bool        SetJsonArray        (int index, JsonArray* pArray);
    int         AddJsonArray        (JsonArray* pArray, int index = -1);

    //removes a value from the array
    void        RemoveValue         (int index);

    //Returns size of the array
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
