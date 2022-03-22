#define _USE_MATH_DEFINES
#include "dependencies/matplotlibcpp/matplotlibcpp.h"
#include "experimental/filesystem"
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
namespace plt = matplotlibcpp;
namespace fs = std::experimental::filesystem;
int main() {
  std::string path = "./data/";
  for (const auto entity : fs::directory_iterator(path)) {
    std::ifstream fin;
    fin.open(entity.path());
    std::vector<double> x, y;
    while (!fin.eof()) {
      double fst;
      double snd;
      fin >> fst >> snd;
      x.push_back(fst);
      y.push_back(snd);
    }
    plt::figure_size(1200, 780);

    plt::plot(x, y);

    plt::title(entity.path().filename());

    plt::xlabel("object size");
    plt::ylabel("processor cycles");
    std::string filename =
        std::string("plots/") + entity.path().filename().string();
    plt::save(filename);
  }
}
