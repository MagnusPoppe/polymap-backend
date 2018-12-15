//
// Created by Magnus Poppe Wang on 11/12/2018.
//

#include "conversion.hpp"
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>

int main(int argv, char **argc) {
    // Defining boost types:
    using boost::geometry::correct;
    using boost::geometry::get;
    using boost::geometry::model::polygon;
    using boost::geometry::model::d2::point_xy;
    typedef polygon<point_xy<double>> boost_polygon;

    // Input validation:
    if (argv < 2) {
        std::cerr << "GeoJson file must be given as parameter!" << std::endl;
        return 1;
    }

    // Interpreting input file:
    std::string filepath(argc[1]);
    std::vector<std::string> features = geoJsonToWkt(filepath);

    // Setup:
    boost_polygon poly1;
    boost::geometry::read_wkt(features.at(0), poly1);
    correct(poly1);
    std::vector<boost_polygon> output;

    // Finding intersection of all features (polygons):
    for (int i = 1; i < features.size(); i++) {
        boost_polygon poly2;
        boost::geometry::read_wkt(features.at(i), poly2);
        correct(poly2);
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
        wktToGeoJsonFile(polygons, "data/output.json");
    }
    exit(0);
}
