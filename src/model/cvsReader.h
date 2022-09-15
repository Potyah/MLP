#ifndef SRC_MODEL_CVSREADER_H_
#define SRC_MODEL_CVSREADER_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace s21 {
class CvsReader {
private:
    std::vector<std::vector<double> > _data;
    void setData(std::vector<std::vector<double> > data) { _data = data; }

public:
    CvsReader() {}
    void readFile(std::string path);
    std::vector<std::vector<double> > getVector() { return _data; }
    unsigned long getCountString() { return _data.size(); }
};
}  // namespace s21

#endif  //  SRC_MODEL_CVSREADER_H_
