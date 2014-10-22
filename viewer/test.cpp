#include <sstream>

#include <QtCore/QFile>
#include <QtDebug>
#include <fstream>

#include "common.h"
#include "element.h"
#include "fileinfo.h"

int main(int argc, char **argv)
{
    std::fstream ifstream;
    std::string filename = "/Users/wang/Development/epicsqml/tests/text.adl";
    filename = "ADBase.adl";

    FileInfo fi = FileInfo("").getFile(filename);
    ifstream.open(fi.absolutePath().c_str(), std::fstream::in);

    Display displayInfo(0);
    displayInfo.setFileName(fi.absolutePath());
    displayInfo.parse(ifstream);
    displayInfo.toQML(std::cout);

    std::fstream ofstream;
    ofstream.open ("/Users/wang/Development/epicsqml/tests/main.qml", std::fstream::out);
    displayInfo.toQML(ofstream);

    return 0;
}
