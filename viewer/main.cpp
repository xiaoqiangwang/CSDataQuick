
#include "common.h"
#include <sstream>
#include <iostream>

int main(int argc, char **argv)
{
    std::string input("");
    std::istringstream isstream(input);
    char token[MAX_TOKEN_LENGTH];

    while (getToken(isstream, token) != T_EOF)
        std::cout << token << std::endl;
}
