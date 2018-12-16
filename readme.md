# PolyMapBackend

## Installation
> the following installation guide is for macOS

### C++ binaries:
The intersection and union functions are written in c++. To compile this, both the boost and nlohmann/json packages are needed.

To install boost: 
``` bash
brew install boost
``` 
To install nlohmann/json: 
```bash
brew tap nlohmann/json
brew install nlohmann_json
``` 

For other platforms, see https://github.com/nlohmann/json#package-managers
If you install nlohmann_json with another package manager or build from sources, note that the makefile variable "JSONPACKAGE" needs to point to the include directory of the nlohmann_json source. 

**Compile the C++ program**
``` bash 
make compute
```


### Python: 
Use pip to install the required packages noted in the requirements.txt file.


## Usage: 
Start the API, navigate to the project folder and type:
 
``` bash
python main.py
``` 


Test the api by using the predefined requests found in "requests.http"
