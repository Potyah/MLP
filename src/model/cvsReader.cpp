#include "cvsReader.h"

namespace s21 {
void CvsReader::readFile(
    std::string path) {  //  сделать отдельным метадом инициализацию
  std::ifstream fs;
  fs.open(path, std::fstream::in);

  if (!fs.is_open()) {
    throw std::runtime_error("The file to load the neural network is not open");
  }

  std::vector<std::vector<double> > data;
  std::string line;

  while (getline(fs, line)) {
    std::vector<double> tempVector;
    std::stringstream lineStream(line);
    std::string value;

    while (getline(lineStream, value, ',')) {
      tempVector.push_back(stod(value));
    }
    data.push_back(tempVector);
  }

  setData(data);
  fs.close();
}
}  // namespace s21
