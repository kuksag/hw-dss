#define _USE_MATH_DEFINES

#include "dependencies/matplotlib-cpp/matplotlibcpp.h"
#include "experimental/filesystem"
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

namespace plt = matplotlibcpp;
namespace fs = std::experimental::filesystem;
static const std::string COMPRESSION_SUFFIX = "compression";
static const std::string TIME_SUFFIX = "time";

void draw(const std::string& algorithm, const std::string& metric) {
    std::string path = "../artifacts/";
    plt::figure_size(1800, 780);
    std::vector<std::string> names;
    std::vector<double> compression, time;
    for (const auto &entity : fs::directory_iterator(path)) {
        std::ifstream fin;
        std::string filename = entity.path().filename().string();
        if (filename != algorithm || filename.substr(filename.size() - 3, 3) ==
            "png") {
            continue;
        }

        fin.open(entity.path());
        while (!fin.eof()) {
            double fst = 0;
            double snd = 0;
            std::string name;
            fin >> name >> fst >> snd;
            if(fst == 0 || snd == 0)continue;
            names.push_back(name + "_" + filename);
            time.push_back(snd);
            compression.push_back(fst);

        }
        fin.close();
    }
    if(metric == COMPRESSION_SUFFIX){
        plt::bar(compression);
        plt::ylabel("compression rate");
        plt::title("compression test");
    }else{
        plt::ylabel("time in milliseconds");
        plt::title("time test");
        plt::bar(time);
    }
    std::vector<double> range(names.size());
    for (int i = 0; i < range.size(); i++) {
        range[i] = i;
    }
    plt::xticks(range, names);
    fs::path filename = "../artifacts/results_"+algorithm+metric;
    plt::save(filename);
}

int main() {
    draw("zstd1", COMPRESSION_SUFFIX);
    draw("zstd1", TIME_SUFFIX);
    draw("zstd7", COMPRESSION_SUFFIX);
    draw("zstd7", TIME_SUFFIX);
}

