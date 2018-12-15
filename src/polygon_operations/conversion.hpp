//
// Created by Magnus Poppe Wang on 11/12/2018.
//
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <nlohmann/json.hpp>

// GeoJson conversion to and from boost:
std::vector<std::string> geoJsonToWkt(std::string filepath);
void wktToGeoJsonFile(std::vector<std::string> polygons, std::string filepath);
