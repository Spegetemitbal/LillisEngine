#ifndef FILEDATAWRAPPER_H
#define FILEDATAWRAPPER_H

class FileDataWrapper {
public:
    explicit FileDataWrapper(const char* file);
    FileDataWrapper() {};
    ~FileDataWrapper() {};

    std::string getFileName() {return fileName;};
    std::string getFileType() {return fileType;};
    std::string getFilePath() {return filePath;};

private:
    std::string filePath;
    std::string fileName;
    std::string fileType;
};

#endif //FILEDATAWRAPPER_H
