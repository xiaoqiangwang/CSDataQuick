#ifndef FILEINFO_H
#define FILEINFO_H

#include <string>
#include <vector>

class FileInfo;

class FileInfo
{
public:
    FileInfo(std::string filename);

    bool exists();

    bool isAbsolutePath();
    std::string absolutePath();

    std::string dirName();
    std::string baseName(bool strip_ext=false);
    std::string extName();

    FileInfo getFile(std::string filename);

    static std::string createFilePath(std::string pathname, std::string filename);
private:
    std::string filename;
    static std::vector<std::string> paths;
};

#endif // FILEINFO_H
