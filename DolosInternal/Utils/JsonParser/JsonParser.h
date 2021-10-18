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

inline const char* chIndent = "\t"; // Indent string

//Parses the json file and returns a JsonObject holding the parsed values - delete the object before exiting
JsonObject*     ParseJsonString (const std::string& szJson);
//Parses the json string and returns a JsonObject holding the parsed values - delete the object before exiting
JsonObject*     ParseJsonFile   (const char* szPath);
//Helper to decide which value to parse as
JsonValue*      ParseJsonValue  (std::istream& fJson, const std::string& szName);
bool            ParseBool       (std::istream& fJson, char chPrev);
double          ParseNumber     (std::istream& fJson, char chPrev);
std::string     ParseString     (std::istream& fJson, char chDelim);
JsonArray*      ParseJsonArray  (std::istream& fJson, const std::string& szName);
JsonObject*     ParseJsonObject (std::istream& fJson);

//Writes to a file given a pointer to a JsonObject
bool            WriteJsonFile   (const char* szPath, JsonObject* pJsonObject);
//Returns a json formatted string and is give a pointer to a jsonObject
std::string     WriteJsonString (JsonObject* pJsonObject);

//Helper for writing json values - decides whihc value something is
bool            WriteJsonValue  (std::ostream& fJson, JsonValue* pValue, const std::string& indent = "");
bool            WriteJsonArray  (std::ostream& fJson, JsonArray* pArray, const std::string& indent = "");
bool            WriteJsonObject (std::ostream& fJson, JsonObject* pJsonObject, const std::string& indent = "");


#endif // !JSON_PARSER_H