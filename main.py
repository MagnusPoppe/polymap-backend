import json
import os
import subprocess
import time

if not os.path.isfile("out/compute.o"):
    raise Exception("C++ compute program not compiled...")

from flask import Flask, request
from flask_restful import Resource, Api, reqparse
from flask_cors import CORS

app = Flask(__name__)
api = Api(app)
app.config['DATASETS_FOLDER'] = "data"
app.config['UPLOAD_FOLDER'] = "files/uploaded"
app.config['COMPUTE_FOLDER'] = "files/computed"
os.makedirs(app.config['UPLOAD_FOLDER'], exist_ok=True)
os.makedirs(app.config['COMPUTE_FOLDER'], exist_ok=True)
parser = reqparse.RequestParser()

# Ignoring CORS
headers = {'Access-Control-Allow-Origin': '*'}
cors = CORS(app, resources={r"*": {"origins": "*"}})

def validate_geo_json(geo: dict) -> bool:
    """
    Checks that the input GeoJSON is correctly formatted
    and that it only contains two features.
    :param geo:
    :return:
    """
    try:
        return geo['type'] == "FeatureCollection" \
           and len(geo['features']) == 2 \
           and all([
            feature['type'] == 'Feature' and
            feature['geometry']['type'] == 'Polygon' and
            len(feature['geometry']['coordinates'][0]) >= 3
            for feature in geo['features']
        ])
    except KeyError:
        return False

def parse_request(content:dict , mode: str) -> dict or None:
    """
    Saves the input geojson to file and runs the C++
    program to compute the intersection or union of the
    geojson polygons.
    :param content:
    :param mode:
    :return:
    """
    # Writing file:
    filename = f"{time.time()}.json"
    filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
    with open(filepath, "w") as f:
        json.dump(content, f)

    # Running C++ compute:
    computed_file = os.path.join(app.config['COMPUTE_FOLDER'], filename)
    output = subprocess.check_output(["out/compute.o", mode, filepath, computed_file])
    output = output.decode("utf-8").strip("\n")

    # Reading results:
    if output == computed_file:
        with open(computed_file, "r") as f:
            computed = json.load(f)
        return computed
    else: return None


class Datasets(Resource):
    def get(self):
        """ Returns all datasets and their contents """
        files = os.listdir(app.config['DATASETS_FOLDER'])
        names = [x[:-5] for x in os.listdir(app.config['DATASETS_FOLDER'])]
        output = {}
        for i, file in enumerate(files):
            with open(os.path.join(app.config['DATASETS_FOLDER'], file), "r") as f:
                output[names[i]] = json.load(f)
        return output, 200, headers


class Union(Resource):
    """ Computes the union of geojson features """
    def post(self):
        if validate_geo_json(request.json):
            computed = parse_request(request.json, "-u")
            result = computed if computed else {}
            return result, 200, headers
        else:
            return None, 400, headers


class Intersection(Resource):
    """ Computes the intersection of geojson features """
    def post(self):
        if validate_geo_json(request.json):
            computed = parse_request(request.json, "-i")
            result = computed if computed else {}
            return result, 200, headers
        else:
            return None, 400, headers

api.add_resource(Datasets, '/datasets')
api.add_resource(Union, '/union')
api.add_resource(Intersection, '/intersection')

if __name__ == '__main__':
    app.run(debug=False)
