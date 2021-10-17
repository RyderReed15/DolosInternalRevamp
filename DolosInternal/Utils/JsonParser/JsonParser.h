#pragma once

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "JsonValue.h"
#include "JsonObject.h"
#include "JsonArray.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>

const char* chIndent = "\t";

JsonObject*     ParseJsonString (const std::string& szJson);
JsonObject*     ParseJsonFile   (const char* szPath);
JsonValue*      GetValue        (std::istream& fJson, const std::string& szName);
bool            ParseBool       (std::istream& fJson, char chPrev);
double          ParseNumber     (std::istream& fJson, char chPrev);
std::string     ParseString     (std::istream& fJson, char chDelim);
JsonArray*      ParseArray      (std::istream& fJson, const std::string& szName);
JsonObject*     ParseObject     (std::istream& fJson);

bool            WriteJsonFile   (const char* szPath, JsonObject* pJsonObject);
std::string     WriteJsonString (JsonObject* pJsonObject);
bool            WriteValue      (std::ostream& fJson, JsonValue* pValue, const std::string& indent = "");
bool            WriteJsonArray  (std::ostream& fJson, JsonArray* pArray, const std::string& indent = "");
bool            WriteJsonObject (std::ostream& fJson, JsonObject* pJsonObject, const std::string& indent = "");


#endif // !JSON_PARSER_H