//
// Created by sol on 6/24/2025.
//

#include "PWEParser.h"

void PWEParser::pack(std::string configfile, std::string outputfile) {
    std::ifstream read(configfile);
    std::ofstream write(outputfile);
    std::string content;

    for (std::string line; getline( read, line );) {
        write << base64_encode(line) << std::endl;
        printf("%s : %s\n", line.c_str(), base64_encode(line).data());
    }
}

void PWEParser::unpack(std::string pwefile, std::string outputfile) {
    std::ifstream read(pwefile);
    std::ofstream write(outputfile);
    std::string content;

    for (std::string line; getline( read, line );) {
        write << base64_decode(line) << std::endl;
        printf("%s : %s\n", line.c_str(), base64_decode(line).data());
    }
}