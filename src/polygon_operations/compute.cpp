//
// Created by Magnus Poppe Wang on 11/12/2018.
//

#include "conversion.hpp"
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>

const static std::string UNION = "-u";
const static std::string INTERSECTION = "-i";

int main(int argv, char **argc) {
    // Defining boost types:
    using boost::geometry::correct;
    using boost::geometry::get;
    using boost::geometry::model::polygon;
    using boost::geometry::model::d2::point_xy;
    typedef polygon<point_xy<double>> boost_polygon;

    // Input validation:
    if (argv < 4) {
        std::cerr
            << "Arguments should be: \n"
            << "1.\tProgram mode (union or intersection)\n"
            << "2.\tInput file path\n"
            << "3.\tOutput file path"
            << std::endl;
        return 1;
    }

    std::string mode(argc[1]);
    if (mode != UNION and mode != INTERSECTION) {
        std::cerr << "Mode must be \"union\" or \"intersection\"" << std::endl;
    }


    // Interpreting input file:
    std::string filepath(argc[2]);
    std::string out_filepath(argc[3]);
    std::vector<std::string> features = geoJsonToWkt(filepath);

    //mode != "-u" Setup:
    boost_polygon poly1;
    boost::geometry::read_wkt(features.at(0), poly1);
    correct(poly1);
    std::vector<boost_polygon> output;

    // Finding intersection of all features (polygons):
    for (unsigned int i = 1; i < features.size(); i++) {
        boost_polygon poly2;
        boost::geometry::read_wkt(features.at(i), poly2);
        correct(poly2);
        if (mode == INTERSECTION)
            boost::geometry::intersection(poly1, poly2, output);
        else if (mode == UNION)
            boost::geometry::union_(poly1, poly2, output);
        poly1 = output.at(0);
    }

    // Saving output:
    if (output.size() > 0) {
        std::vector<std::string> polygons;
        for (auto out : output) {
            std::ostringstream boost_out;
            boost_out << boost::geometry::wkt(output.at(0));
            polygons.push_back(boost_out.str());
        }
        wktToGeoJsonFile(polygons, out_filepath);
    }
    std::cout << out_filepath << std::endl;
    exit(0);
}
