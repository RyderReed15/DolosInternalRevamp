#include "JsonValue.h"

JsonValue::JsonValue() {
    m_szName = "";
    m_tType = VALUE_TYPE::INVALID;

}

JsonValue::JsonValue(const JsonValue& copyValue) {
    m_szName = std::string(copyValue.m_szName);
    m_tType = copyValue.m_tType;
    switch (m_tType) {
    case VALUE_TYPE::BOOL:
        m_bValue = copyValue.m_bValue;
        break;
    case VALUE_TYPE::NUMBER:
        m_dbValue = copyValue.m_dbValue;
        break;
    case VALUE_TYPE::STRING:
        m_szValue = std::string(copyValue.m_szValue);
        break;
    case VALUE_TYPE::ARRAY:
        m_pObject = new JsonObject(*copyValue.m_pObject);
        break;
    case VALUE_TYPE::OBJECT:
        m_pArray = new JsonArray(*copyValue.m_pArray);
        break;
    }

}

JsonValue::~JsonValue() {
    switch (m_tType) {
    case VALUE_TYPE::ARRAY:
        delete m_pArray;
        break;
    case VALUE_TYPE::OBJECT:
        delete m_pObject;
        break;
    default:
        break;
    }

    m_szName.clear();


}

JsonValue JsonValue::operator=(const JsonValue& rhs) {
    if (&rhs == this) {
        return *this;
    }
    switch (m_tType) {
    case VALUE_TYPE::BOOL:
        m_bValue = 0;
        break;
    case VALUE_TYPE::NUMBER:
        m_dbValue = 0;
        break;
    case VALUE_TYPE::STRING:
        m_szValue = "";
        break;
    case VALUE_TYPE::ARRAY:
        delete m_pObject;
        m_pObject = nullptr;;
        break;
    case VALUE_TYPE::OBJECT:
        delete m_pArray;
        m_pArray = nullptr;;
        break;
    }
    m_szName = std::string(rhs.m_szName);
    m_tType = rhs.m_tType;
    switch (m_tType) {
    case VALUE_TYPE::BOOL:
        m_bValue = rhs.m_bValue;
        break;
    case VALUE_TYPE::NUMBER:
        m_dbValue = rhs.m_dbValue;
        break;
    case VALUE_TYPE::STRING:
        m_szValue = std::string(rhs.m_szValue);
        break;
    case VALUE_TYPE::ARRAY:
        m_pObject = new JsonObject(*rhs.m_pObject);
        break;
    case VALUE_TYPE::OBJECT:
        m_pArray = new JsonArray(*rhs.m_pArray);
        break;
    }
    return *this;
}

bool JsonValue::operator== (const JsonValue& rhs) {
    if (this == &rhs) {
        return true;
    }
    if (rhs.m_tType == m_tType) {
        switch (m_tType) {


        case VALUE_TYPE::BOOL:
            return m_bValue == rhs.m_bValue;
        case VALUE_TYPE::NUMBER:
            return m_dbValue == rhs.m_dbValue;
        case VALUE_TYPE::STRING:
            return m_szValue == rhs.m_szValue;
        case VALUE_TYPE::ARRAY:
            return m_pArray == rhs.m_pArray;
        case VALUE_TYPE::OBJECT:
            return m_pObject == rhs.m_pObject;
        default:
            return true;
        }
    }
    return false;
}


std::ostream& operator<< (std::ostream& out, const JsonValue& rhs) {
    std::string newString = "";

    switch (rhs.m_tType) {


    case VALUE_TYPE::BOOL:
        out << "\"" << rhs.m_szName << "\": " << (rhs.m_bValue ? "true" : "false");
        break;
    case VALUE_TYPE::NUMBER:
        if ((int)rhs.m_dbValue == rhs.m_dbValue) {
            out << "\"" << rhs.m_szName << "\": " << std::fixed << std::setprecision(0) << rhs.m_dbValue;
            break;
        }
        out << "\"" << rhs.m_szName << "\": " << std::fixed << std::setprecision(5) << rhs.m_dbValue;
        break;
    case VALUE_TYPE::STRING:
        for (int i = 0; i < rhs.m_szValue.size(); i++) {
            newString += UnEscapeCharacter(rhs.m_szValue[i]);
        }
        out << "\"" << rhs.m_szName << "\": " << "\"" << newString << "\"";
        break;
    case VALUE_TYPE::NULLTYPE:
        out << "\"" << rhs.m_szName << "\": " << "null";
        break;
    }
    return out;

}

std::string UnEscapeCharacter(const char chUnEscape) {
    switch (chUnEscape) {
    case '\\': return "\\\\";
    case '\'': return "\\\'";
    case '\"': return "\\\"";
    case '\?': return "\\?";
    case '\a': return "\\a";
    case '\b': return "\\b";
    case '\f': return "\\f";
    case '\n': return "\\n";
    case '\r': return "\\r";
    case '\t': return "\\t";
    case '\v': return "\\v";
    default:   return std::string(1, chUnEscape);
    }
}