#define _USE_MATH_DEFINES
#include "dependencies/matplotlibcpp/matplotlibcpp.h"
#include "experimental/filesystem"
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
namespace plt = matplotlibcpp;
namespace fs = std::experimental::filesystem;
void draw_one(const std::string &suffix) {
  std::string path = "./data/";
  plt::figure_size(1200, 780);
  for (const auto entity : fs::directory_iterator(path)) {
    std::ifstream fin;
    std::string filename = entity.path().filename().string();
    if (filename.substr(filename.size() - suffix.size(), suffix.size()) !=
        suffix) {
      continue;
    }
    fin.open(entity.path());
    std::vector<double> x, y;
    while (!fin.eof()) {
      double fst;
      double snd;
      fin >> fst >> snd;
      x.push_back(fst);
      y.push_back(snd);
    }
    plt::named_plot(filename, x, y);
  }
  plt::title(suffix + " test");
  plt::legend();
  fs::path filename = "plots/results_" + suffix;
  plt::xlabel("object size");
  plt::ylabel("processor cycles");
  plt::save(filename);
}

int main() {
  draw_one("big");
  draw_one("small");
}
