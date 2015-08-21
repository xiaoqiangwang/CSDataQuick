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

/* Concantenate path */
std::string FileInfo::createFilePath(std::string pathname, std::string filename)
{
    std::string filepath;
    if (pathname.empty())
        filepath = filename;
    else {
        filepath = pathname;
        if (filepath.at(filepath.size()-1) != path_sep)
            filepath += path_sep;
        filepath += filename;
    }
    return filepath;
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
    if (pos == std::string::npos)
        return std::string("");
    else
        return filename.substr(0, pos + 1);
}

std::string FileInfo::baseName(bool strip_ext)
{
    size_t bpos = filename.find_last_of(path_sep);
    if (bpos == std::string::npos)
        bpos = 0;
    else
        bpos += 1;
    std::string basename = filename.substr(bpos);

    if (!strip_ext)
        return basename;

    size_t spos = basename.find_last_of('.');
    return basename.substr(0, spos);
}

std::string FileInfo::extName()
{
    size_t pos = filename.find_last_of('.');
    return filename.substr(pos);

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
