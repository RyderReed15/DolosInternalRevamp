#include "JsonObject.h"

JsonObject::JsonObject() {
    m_szName = "";
}

JsonObject::JsonObject(JsonObject& copyObject) {
    std::unordered_map<std::string, JsonValue*>::iterator iterator = copyObject.m_mValues.begin();
    while (iterator != copyObject.m_mValues.end()) {
        m_mValues[iterator->first] = new JsonValue(*iterator->second);
        iterator++;
    }
    m_szName = std::string(copyObject.m_szName);
}

JsonObject::~JsonObject() {
    std::unordered_map<std::string, JsonValue*>::iterator iterator = m_mValues.begin();
    while (iterator != m_mValues.end()) {
        delete iterator->second;
        iterator++;
    }

    m_mValues.clear();
    m_szName.clear();

}

bool JsonObject::SetNumber(const std::string& szName, const double num) {
    JsonValue* pValue = m_mValues[szName];
    if (pValue) {
        pValue->m_dbValue = num;
        return true;
    }
    else {
        pValue = new JsonValue();
        pValue->m_tType = VALUE_TYPE::NUMBER;
        pValue->m_szName = std::string(szName);
        pValue->m_dbValue = num;
        m_mValues[szName] = pValue;
        return true;
    }
    return false;
}

bool JsonObject::GetBoolean(const std::string& szName) {
    JsonValue* pValue = m_mValues[szName];
    if (pValue) {
        return pValue->m_bValue;
    }
    return false;
}
bool JsonObject::SetBoolean(const std::string& szName, const bool bValue) {
    JsonValue* pValue = m_mValues[szName];
    if (pValue) {
        pValue->m_bValue = bValue;
        return true;
    }
    else {
        pValue = new JsonValue();
        pValue->m_tType = VALUE_TYPE::BOOL;
        pValue->m_szName = std::string(szName);
        pValue->m_bValue = bValue;
        m_mValues[szName] = pValue;
        return true;
    }
    return false;
}

std::string JsonObject::GetString(const std::string& szName) {
    JsonValue* pValue = m_mValues[szName];
    if (pValue) {
        return pValue->m_szValue;
    }
    return "";
}
bool JsonObject::SetString(const std::string& szName, const std::string& szValue) {
    JsonValue* pValue = m_mValues[szName];
    if (pValue) {
        pValue->m_szValue = szValue;
        return true;
    }
    else {
        pValue = new JsonValue();
        pValue->m_tType = VALUE_TYPE::STRING;
        pValue->m_szName = std::string(szName);
        pValue->m_szValue = std::string(szValue);
        m_mValues[szName] = pValue;
        return true;
    }
    return false;
}

JsonObject* JsonObject::GetJsonObject(const std::string& szName) {
    JsonValue* pValue = m_mValues[szName];
    if (pValue) {
        return pValue->m_pObject;
    }
    return nullptr;
}
bool JsonObject::SetJsonObject(const std::string& szName, JsonObject* pObject) {
    JsonValue* pValue = m_mValues[szName];
    if (pValue) {
        pValue->m_pObject = pObject;
        return true;
    }
    else {
        pValue = new JsonValue();
        pValue->m_tType = VALUE_TYPE::OBJECT;
        pValue->m_szName = std::string(szName);
        pValue->m_pObject = pObject;
        m_mValues[szName] = pValue;
        return true;
    }
    return false;
}

JsonArray* JsonObject::GetJsonArray(const std::string& szName) {
    JsonValue* pValue = m_mValues[szName];
    if (pValue) {
        return pValue->m_pArray;
    }
    return nullptr;
}
bool JsonObject::SetJsonArray(const std::string& szName, JsonArray* pArray) {
    JsonValue* pValue = m_mValues[szName];
    if (pValue) {
        pValue->m_pArray = pArray;
        return true;
    }
    else {
        pValue = new JsonValue();
        pValue->m_tType = VALUE_TYPE::ARRAY;
        pValue->m_szName = std::string(szName);
        pValue->m_pArray = pArray;
        m_mValues[szName] = pValue;
        return true;
    }
    return false;
}

void JsonObject::RemoveValue(const std::string& szName) {
    m_mValues.erase(szName);
}

JsonObject JsonObject::operator=(JsonObject& rhs) {
    if (this == &rhs) {
        return *this;
    }
    std::unordered_map<std::string, JsonValue*>::iterator iterator = m_mValues.begin();
    while (iterator != m_mValues.end()) {
        delete iterator->second;
        iterator++;
    }
    m_mValues.clear();
    std::unordered_map<std::string, JsonValue*>::iterator copyIterator = rhs.m_mValues.begin();
    while (copyIterator != rhs.m_mValues.end()) {
        m_mValues[copyIterator->first] = new JsonValue(*copyIterator->second);
        copyIterator++;
    }
    return *this;
}
