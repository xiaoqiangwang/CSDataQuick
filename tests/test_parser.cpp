#include <iostream>
#include "EDLParser.h"

int main(int argc, char **argv)
{
    std::string filename = std::string(argv[1]);

    std::string qml = parseEDLDisplay(filename, std::map<std::string, std::string>());

    std::cout << qml << std::endl;
}
