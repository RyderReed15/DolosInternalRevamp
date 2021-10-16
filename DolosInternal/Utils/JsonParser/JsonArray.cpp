#include "JsonArray.h"


JsonArray::JsonArray() {
    m_szName = "";
}

JsonArray::JsonArray(const JsonArray& copyArray) {
    m_szName = std::string(copyArray.m_szName);
    for (int i = 0; i < copyArray.m_vValues.size(); i++) {
        m_vValues.push_back(new JsonValue(*m_vValues[i]));
    }
}

JsonArray::~JsonArray() {
    for (int i = 0; i < m_vValues.size(); i++) {
        delete m_vValues[i];
    }
    m_vValues.clear();

    m_szName.clear();
}

bool JsonArray::SetNumber(int index, const double num) {
    if (index >= 0 && index < m_vValues.size()) {
        m_vValues[index]->m_dbValue = num;
        return true;
    }
    return false;
}
int JsonArray::AddNumber(const double num, int index) {

    JsonValue* pValue = new JsonValue();
    pValue->m_tType = VALUE_TYPE::NUMBER;
    pValue->m_dbValue = num;
    if (index == -1) {
        m_vValues.push_back(pValue);
        return m_vValues.size() - 1;
    }
    else {
        std::vector<JsonValue*>::iterator it = m_vValues.begin();
        if (index > m_vValues.size()) {
            index = m_vValues.size();
        }
        m_vValues.insert(it + index, pValue);
        return index;
    }


}

bool JsonArray::GetBoolean(int index) const {
    if (index >= 0 && index < m_vValues.size()) {
        return m_vValues[index]->m_bValue;
    }
    return false;
}
bool JsonArray::SetBoolean(int index, const bool bValue) {
    if (index >= 0 && index < m_vValues.size()) {
        m_vValues[index]->m_bValue = bValue;
        return true;
    }
    return false;
}
int JsonArray::AddBoolean(const bool bValue, int index) {

    JsonValue* pValue = new JsonValue();
    pValue->m_tType = VALUE_TYPE::BOOL;
    pValue->m_bValue = bValue;
    if (index == -1) {
        m_vValues.push_back(pValue);
        return m_vValues.size() - 1;
    }
    else {
        std::vector<JsonValue*>::iterator it = m_vValues.begin();
        if (index > m_vValues.size()) {
            index = m_vValues.size();
        }
        m_vValues.insert(it + index, pValue);
        return index;
    }


}

std::string JsonArray::GetString(int index) const {
    if (index >= 0 && index < m_vValues.size()) {
        return m_vValues[index]->m_szValue;
    }
    return "";
}
bool JsonArray::SetString(int index, const std::string& szValue) {
    if (index >= 0 && index < m_vValues.size()) {
        m_vValues[index]->m_szValue = szValue;
        return true;
    }
    return false;
}
int JsonArray::AddString(const std::string& szValue, int index) {

    JsonValue* pValue = new JsonValue();
    pValue->m_tType = VALUE_TYPE::STRING;
    pValue->m_szValue = std::string(szValue);
    if (index == -1) {
        m_vValues.push_back(pValue);
        return m_vValues.size() - 1;
    }
    else {
        std::vector<JsonValue*>::iterator it = m_vValues.begin();
        if (index > m_vValues.size()) {
            index = m_vValues.size();
        }
        m_vValues.insert(it + index, pValue);
        return index;
    }


}

JsonObject* JsonArray::GetJsonObject(int index) const {
    if (index >= 0 && index < m_vValues.size()) {
        return m_vValues[index]->m_pObject;
    }
    return nullptr;
}
bool JsonArray::SetJsonObject(int index, JsonObject* pObject) {
    if (index >= 0 && index < m_vValues.size()) {
        m_vValues[index]->m_pObject = pObject;
        return true;
    }
    return false;
}
int JsonArray::AddJsonObject(JsonObject* pObject, int index) {
    if (pObject) {
        JsonValue* pValue = new JsonValue();
        pValue->m_szName = pObject->m_szName;
        pValue->m_tType = VALUE_TYPE::OBJECT;
        pValue->m_pObject = pObject;
        if (index == -1) {
            m_vValues.push_back(pValue);
            return m_vValues.size() - 1;
        }
        else {
            std::vector<JsonValue*>::iterator it = m_vValues.begin();
            if (index > m_vValues.size()) {
                index = m_vValues.size();
            }
            m_vValues.insert(it + index, pValue);
            return index;
        }
    }
    return -1;
}


JsonArray* JsonArray::GetJsonArray(int index) const {
    if (index >= 0 && index < m_vValues.size()) {
        return m_vValues[index]->m_pArray;
    }
    return nullptr;
}
bool JsonArray::SetJsonArray(int index, JsonArray* pArray) {
    if (index >= 0 && index < m_vValues.size()) {
        m_vValues[index]->m_pArray = pArray;
        return true;
    }
    return false;
}
int JsonArray::AddJsonArray(JsonArray* pArray, int index) {
    if (pArray) {
        JsonValue* pValue = new JsonValue();
        pValue->m_szName = pArray->m_szName;
        pValue->m_tType = VALUE_TYPE::ARRAY;
        pValue->m_pArray = pArray;
        if (index == -1) {
            m_vValues.push_back(pValue);
            return m_vValues.size() - 1;
        }
        else {
            std::vector<JsonValue*>::iterator it = m_vValues.begin();
            if (index > m_vValues.size()) {
                index = m_vValues.size();
            }
            m_vValues.insert(it + index, pValue);
            return index;
        }
    }
    return -1;

}

void JsonArray::RemoveValue(int index) {
    if (index >= 0 && index < m_vValues.size()) {
        std::vector<JsonValue*>::iterator it = m_vValues.begin();
        m_vValues.erase(it + index);
    }

}

int JsonArray::GetSize() {
    return m_vValues.size();
}

JsonArray JsonArray::operator= (const JsonArray& rhs) {
    if (this == &rhs) {
        return *this;
    }
    m_szName = "";
    for (int i = 0; i < m_vValues.size(); i++) {
        delete m_vValues[i];
    }
    m_vValues.clear();
    for (int i = 0; i < rhs.m_vValues.size(); i++) {
        m_vValues.push_back(new JsonValue(*rhs.m_vValues[i]));
    }
    return *this;
}

JsonValue*& JsonArray::operator[](int index) {

    return m_vValues[index];
}
