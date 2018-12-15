//
// Created by Magnus Poppe Wang on 11/12/2018.
//

#include "conversion.hpp"

std::vector<std::string> geoJsonToWkt(std::string filepath) {
    // Reading file to JSON
    std::ifstream stream(filepath);
    nlohmann::json geoJson;
    stream >> geoJson;

    // Parsing GEOJSON contents:
    std::vector<std::string> features;
    for (nlohmann::json feat : geoJson["features"]) {
        if (feat["type"] == "Feature" && feat["geometry"]["type"] == "Polygon") {
            for (nlohmann::json poly : feat["geometry"]["coordinates"]) {

                // Creating the wkt boost string:
                std::ostringstream polygon;
                polygon << "Polygon((";
                int i = 0;
                for (auto coords : poly) {
                    std::ostringstream output;
                    if (i > 0) output << ",";
                    polygon << " " << coords[0] << " " << coords[1] << " ";
                    i++;
                }
                polygon << "))";
                features.push_back(polygon.str());
            }
        }
    }
    return features;
}

void wktToGeoJsonFile(std::vector<std::string> polygons, std::string filepath) {
    // Defining feature collection:
    nlohmann::json featureCollection = nlohmann::json({});
    featureCollection["type"] = "FeatureCollection";
    nlohmann::json features = nlohmann::json::array();

    for (std::string polygon : polygons) {
        // Defining feature:
        nlohmann::json feature = nlohmann::json({});
        feature["type"] = "Feature";
        feature["properties"] = nlohmann::json({});

        // Defining geometry:
        nlohmann::json geometry = nlohmann::json({});
        geometry["type"] = "Polygon";
        geometry["coordinates"] = nlohmann::json::array();
        nlohmann::json coordinates = nlohmann::json::array();

        // Adding coordinates to geometry:
        long start = polygon.find_last_of("(");
        long end = polygon.find_first_of(")");
        std::string substr = polygon.substr(start + 1, end  - 1 - start);

        while (substr.find(",") < polygon.size()) {
            // Getting first coordinate:
            long splitIndex = substr.find_first_of(",");
            std::string point = substr.substr(0, splitIndex);
            substr = substr.substr(splitIndex+1);

            // Separating individual lat + lng:
            long xPos = point.find_first_of(" ");
            std::string x = point.substr(0, xPos);
            std::string y = point.substr(xPos+1);

            // Adding json coordinates:
            nlohmann::json coordinate = nlohmann::json::array();
            coordinate.push_back(std::stod(x));
            coordinate.push_back(std::stod(y));
            coordinates.push_back(coordinate);
        }

        geometry["coordinates"].push_back(coordinates);
        feature["geometry"] = geometry;
        features.push_back(feature);
    }
    featureCollection["features"] = features;

    // Writing to file:
    std::ofstream output(filepath);
    output << featureCollection << std::endl;
}

