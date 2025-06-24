#include <iostream>
#include <yaml-cpp/yaml.h>
#include "pwe_parser/PWEParser.h"

int main() {
    PWEParser pweParser;
    pweParser.pack("C:\\Users\\sol\\CLionProjects\\windows-pwe\\uv.yaml",
                   "C:\\Users\\sol\\CLionProjects\\windows-pwe\\uv.pwe");
    pweParser.unpack("C:\\Users\\sol\\CLionProjects\\windows-pwe\\uv.pwe",
                     "C:\\Users\\sol\\CLionProjects\\windows-pwe\\uvunpackked.yaml");

    YAML::Node config = YAML::LoadFile("C:\\Users\\sol\\CLionProjects\\windows-pwe\\uv.yaml");
    for (auto steps : config["steps"]) {
        std::cout << steps.first << std::endl;
    }
    return 0;
}