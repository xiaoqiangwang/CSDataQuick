#include "fileinfo.h"

#include <cstdlib>

#if _WIN32
#include <io.h>
    const char path_sep = '\\';
    const char path_del = ';';
#else
#include <limits.h>
#include <unistd.h>
    const char path_sep = '/';
    const char path_del = ':';
#endif

std::vector<std::string> getDisplayPath()
{
    std::vector<std::string> paths;
    /* initialize search path */
    if (getenv("EPICS_DISPLAY_PATH") != 0) {
        std::string display_path = getenv("EPICS_DISPLAY_PATH");

        size_t previous = 0;
        size_t index = display_path.find( path_del );
        while( index != std::string::npos )
        {
            paths.push_back( display_path.substr(previous, index-previous));
            previous = index + 1;
            index = display_path.find( path_del, previous );
        }
        paths.push_back( display_path.substr(previous) );
    }
    return paths;
}

std::vector<std::string> FileInfo::paths = getDisplayPath();

FileInfo::FileInfo(std::string filename)
{
    this->filename = filename;
}

bool FileInfo::exists()
{
    return access(this->filename.c_str(), R_OK) == 0;
}

/* Conenate path */
std::string FileInfo::createFilePath(std::string pathname, std::string filename)
{
    return pathname + path_sep + filename;
}

bool FileInfo::isAbsolutePath()
{
    bool test;
#ifdef _WIN32
    test = (this->filename.find(':') == 1 ||
            this->filename.find('\\') == 0);
#else
    test = (this->filename.find('/') == 0);
#endif

    return test;
}

std::string FileInfo::absolutePath()
{
    if (isAbsolutePath())
        return this->filename;

    char currentDirectory[PATH_MAX];
    getcwd(currentDirectory, PATH_MAX);

    return createFilePath(currentDirectory, this->filename);
}

std::string FileInfo::dirName()
{
    size_t pos = filename.find_last_of(path_sep);
    return filename.substr(0, pos + 1);
}

FileInfo FileInfo::getFile(std::string filename)
{
    if (access(filename.c_str(), R_OK) == 0)
        return FileInfo(filename);

    std::string fullpath = createFilePath(dirName(), filename);
    if (access(fullpath.c_str(), R_OK) == 0) {
        return FileInfo(fullpath);
    }

    else {
        /* search in display path */
        for (std::vector<std::string>::iterator it = paths.begin();
             it != paths.end(); it++) {
            std::string fullpath = (*it) + path_sep + filename;
            if (access(fullpath.c_str(), R_OK) == 0) {
                return FileInfo(fullpath);
            }
        }
    }
    return FileInfo("");
}
