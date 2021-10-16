#include "JsonParser.h"


void        StringToLower   (std::string& szUpper);
char        EscapeCharacter (std::istream& fJson, char chEscape);

JsonObject* ParseJsonFile(const char* szPath) {
    std::ifstream fJson;
    fJson.open(szPath);

    if (fJson.is_open()) {
        char chFirst = '\0';
        fJson.get(chFirst);
        if (chFirst == '[') {
            JsonObject* pJsonFile = new JsonObject();
            pJsonFile->SetJsonArray("1", ParseArray(fJson, ""));
            return pJsonFile;
        }
        else {
            JsonObject* pJsonFile = ParseObject(fJson);
            return pJsonFile;
        }



    }
    return nullptr;

}

JsonObject* ParseJsonString(const std::string& szJson) {
    std::istringstream sJson(szJson);
    if (sJson.good()) {
        char chFirst = '\0';
        sJson.get(chFirst);
        if (chFirst == '[') {
            JsonObject* pJsonFile = new JsonObject();
            pJsonFile->SetJsonArray("1", ParseArray(sJson, ""));
            return pJsonFile;
        }
        else {
            JsonObject* pJsonFile = ParseObject(sJson);
            return pJsonFile;
        }
    }
    return nullptr;
}

JsonValue* GetValue(std::istream& fJson, const std::string& szName) {
    bool bSkip = false;
    JsonValue* pValue = new JsonValue();
    pValue->m_szName = szName;
    char chCurr = '\0';
    while (fJson.good() && !fJson.eof()) {

        fJson.get(chCurr);
        switch (chCurr) {
        case ',':
        case '=':
        case ':':
        case ' ':
            break;
        case ']':
            delete pValue;
            return nullptr;
        case '\"':
            pValue->m_szValue = ParseString(fJson, '\"');
            pValue->m_tType = VALUE_TYPE::STRING;
            return pValue;
        case '[':

            pValue->m_pArray = ParseArray(fJson, szName);
            pValue->m_tType = VALUE_TYPE::ARRAY;
            return pValue;
        case '{':

            pValue->m_pObject = ParseObject(fJson);
            pValue->m_pObject->m_szName = pValue->m_szName;
            pValue->m_tType = VALUE_TYPE::OBJECT;
            return pValue;
        case 't':
        case 'T':
        case 'f':
        case 'F':
            pValue->m_bValue = ParseBool(fJson, chCurr);
            pValue->m_tType = VALUE_TYPE::BOOL;
            return pValue;
        case 'n':
        case 'N':
            pValue->m_bValue = false;
            pValue->m_tType = VALUE_TYPE::NULLTYPE;
            return pValue;
        default:
            if (chCurr >= '-' && chCurr <= '9') {
                pValue->m_dbValue = ParseNumber(fJson, chCurr);
                pValue->m_tType = VALUE_TYPE::NUMBER;
                return pValue;
            }

        }

    }

    return pValue;
}

bool ParseBool(std::istream& fJson, char chPrev) {
    std::string szValue; szValue += chPrev;
    char chCurr = '\0';
    while (!fJson.eof() && fJson.good()) {
        fJson.get(chCurr);
        if (chCurr == ' ' || chCurr == ',' || chCurr == '\n' || chCurr == ';' || chCurr == ']' || chCurr == '}') break;

        szValue += chCurr;

    }
    StringToLower(szValue);
    if (!szValue.compare("true")) {
        return true;
    }
    return false;
}

double ParseNumber(std::istream& fJson, char chPrev) {
    std::string szValue; szValue += chPrev;
    char chCurr = '\0';
    while (!fJson.eof() && fJson.good()) {
        fJson.get(chCurr);
        if (chCurr == ' ' || chCurr == ',' || chCurr == '\n' || chCurr == ';' || chCurr == ']' || chCurr == '}') break;

        szValue += chCurr;

    }
    return strtod(szValue.c_str(), 0);
}

std::string ParseString(std::istream& fJson, char chDelim) {
    std::string szValue; char chCurr = '\0';
    while (!fJson.eof() && fJson.good()) {

        fJson.get(chCurr);
        if (chCurr == chDelim) break;
        if (chCurr == '\\') {
            fJson.get(chCurr);
            chCurr = EscapeCharacter(fJson, chCurr);

        }
        szValue += chCurr;

    }
    return szValue;
}

JsonArray* ParseArray(std::istream& fJson, const std::string& szName) {
    JsonArray* pArray = new JsonArray();
    pArray->m_szName = szName;
    char chCurr = '\0';
    while (!fJson.eof() && fJson.good()) {

        JsonValue* pValue = GetValue(fJson, "");
        if (!pValue) return pArray;
        pArray->m_vValues.push_back(pValue);
        if (pValue->m_tType == VALUE_TYPE::NUMBER || pValue->m_tType == VALUE_TYPE::BOOL) {
            fJson.unget(); fJson.get(chCurr);

            if (chCurr == ']') return pArray;
        }


    }
    return pArray;
}

JsonObject* ParseObject(std::istream& fJson) {
    std::string line;
    JsonObject* pObject = new JsonObject();
    JsonValue* pValue;
    char chCurr = '\0';
    while (!fJson.eof() && fJson.good()) {
        fJson.get(chCurr);
        switch (chCurr) {
        case '}':


            return pObject;
        case '\"':
            std::getline(fJson, line, '\"');
            pValue = GetValue(fJson, line);
            if (!pValue->m_szName.compare("")) {
                pObject->m_mValues[std::to_string((int)pValue)] = pValue;
            }
            else {
                pObject->m_mValues[pValue->m_szName] = pValue;
            }



            if (pValue->m_tType == VALUE_TYPE::NUMBER || pValue->m_tType == VALUE_TYPE::BOOL) {
                fJson.unget(); fJson.get(chCurr);
                if (chCurr == '}') {

                    return pObject;
                }
            }
            break;
        case '{':
            pValue = new JsonValue();
            pValue->m_tType = VALUE_TYPE::OBJECT;
            pValue->m_pObject = ParseObject(fJson);
            if (pValue->m_pObject->m_szName.compare("")) {
                pValue->m_szName = pValue->m_pObject->m_szName;

                pObject->m_mValues[pValue->m_szName] = pValue;
            }
            else {
                pObject->m_mValues[std::to_string((int)pValue)] = pValue;

            }
            break;
        default:
            break;
        }


    }

    return pObject;
}

bool WriteJsonFile(const char* szPath, JsonObject* pJsonObject) {
    std::ofstream fJson;
    fJson.open(szPath);
    if (fJson.is_open()) {
        return WriteJsonObject(fJson, pJsonObject);
        return true;
    }
    return false;
}

std::string WriteJsonString(JsonObject* pJsonObject) {
    std::ostringstream sJson;
    WriteJsonObject(sJson, pJsonObject);
    return sJson.str();
}

bool WriteValue(std::ostream& fJson, JsonValue* pValue, const std::string& indent) {
    if (fJson.good() && pValue) {

        switch (pValue->m_tType) {

        case VALUE_TYPE::BOOL:
        case VALUE_TYPE::NUMBER:
        case VALUE_TYPE::STRING:
            fJson << indent << *pValue;
            return true;
        case VALUE_TYPE::ARRAY:
            return WriteJsonArray(fJson, pValue->m_pArray, indent);
        case VALUE_TYPE::OBJECT:
            return WriteJsonObject(fJson, pValue->m_pObject, indent);
        case VALUE_TYPE::NULLTYPE:
            fJson << indent << "\"" << pValue->m_szName << "\": " << "null";
            return true;
        default:
            return false;
        }
    }
    return false;
}

bool WriteJsonArray(std::ostream& fJson, JsonArray* pArray, const std::string& indent) {
    if (fJson.good() && pArray) {

        if (pArray->m_szName.compare("")) {
            fJson << indent << "\"" << pArray->m_szName << "\": [\n";
        }
        else {
            fJson << indent << "[\n";
        }

        for (int i = 0; i < pArray->m_vValues.size(); i++) {

            JsonValue* pValue = (*pArray)[i];
            std::string szArrayName;

            switch (pValue->m_tType) {

            case VALUE_TYPE::BOOL:
                fJson << indent + chIndent << (pValue->m_bValue ? "true" : "false");
                break;
            case VALUE_TYPE::NUMBER:
                if ((int)pValue->m_dbValue == pValue->m_dbValue) {
                    fJson << indent << std::fixed << std::setprecision(0) << pValue->m_dbValue;
                    break;
                }
                fJson << indent << std::fixed << std::setprecision(5) << pValue->m_dbValue;
                break;
            case VALUE_TYPE::STRING:
                fJson << indent + chIndent + "\"" << pValue->m_szValue << "\"";
                break;
            case VALUE_TYPE::ARRAY:
                szArrayName = pValue->m_pArray->m_szName;
                pValue->m_pArray->m_szName = "";
                WriteJsonArray(fJson, pValue->m_pArray, indent + chIndent);
                pValue->m_pArray->m_szName = szArrayName;
                break;
            case VALUE_TYPE::OBJECT:
                WriteJsonObject(fJson, pValue->m_pObject, indent + chIndent);
                break;
            case VALUE_TYPE::NULLTYPE:
                fJson << indent << chIndent << "null";
                break;
            default:
                return false;
            }
            if (i != pArray->m_vValues.size() - 1) {
                fJson << ",\n";
            }
        }

        fJson << "\n" << indent << "]";
        return true;
    }
    return false;
}

bool WriteJsonObject(std::ostream& fJson, JsonObject* pJsonObject, const std::string& indent) {
    if (fJson.good() && pJsonObject) {
        if (pJsonObject->m_szName.compare("")) {
            fJson << indent << "\"" << pJsonObject->m_szName << "\": {\n";
        }
        else {
            fJson << indent << "{\n";
        }

        std::unordered_map<std::string, JsonValue*>::iterator iterator = pJsonObject->m_mValues.begin();
        while (true) {
            if (!WriteValue(fJson, iterator->second, indent + chIndent)) {
                iterator++;
                if (iterator == pJsonObject->m_mValues.end()) {
                    break;
                }
                continue;
            }
            iterator++;
            if (iterator == pJsonObject->m_mValues.end()) {
                break;
            }
            fJson << ",\n";
        }
        fJson << "\n" << indent << "}";
        return true;
    }
    return false;
}


char EscapeCharacter(std::istream& fJson, char chEscape) {
    char chFixed = '\0';
    char acValue[4]{ '\0' };
    switch (chEscape) {
    case '\\':
    case '/':  return chEscape;
    case '\'': return '\'';
    case '\"': return '\"';
    case '?':  return '\?';
    case 'a':  return '\a';
    case 'b':  return '\b';
    case 'f':  return '\f';
    case 'n':  return '\n';
    case 'r':  return '\r';
    case 't':  return '\t';
    case 'u':
        for (int i = 0; i < 4 && fJson.good() && !fJson.eof(); i++) {
            char chTemp; fJson.get(chTemp);
            acValue[i] = chTemp;
        }
        return strtol(acValue, 0, 16);
    case 'v':  return '\v';
    default:   return chEscape;
    }

}


void StringToLower(std::string& szUpper) {
    //Works for letters but anything else below A will be converted but that doesnt matter for the one use in this
    for (int i = 0; i < szUpper.size(); i++) {
        szUpper[i] += 0x20 * (szUpper[i] <= 0x5A);
    }

}



