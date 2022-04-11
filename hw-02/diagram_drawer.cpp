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

void draw(const std::string &metric, const std::string& deprecated_suffix = "") {
    std::string path = "../artifacts/";
    plt::figure_size(3500, 780);
    double shift = -0.5;
    int index = 0;
    std::vector <std::string> all_names;
    std::vector<std::vector<double>> ranges;
    std::vector<std::vector<double>> compressions;
    std::vector<std::vector<double>> times;

    for (const auto &entity : fs::directory_iterator(path)) {

        std::vector <std::string> names;
        std::vector<double> compression, time;
        std::ifstream fin;
        std::string filename = entity.path().filename().string();
        if (filename.substr(filename.size() - 3, 3) ==
            "png" || filename == deprecated_suffix) {
            continue;
        }
        fin.open(entity.path());
        while (!fin.eof()) {
            double fst = 0;
            double snd = 0;
            std::string name;
            fin >> name >> fst >> snd;
            names.push_back(name);
            time.push_back(snd);
            compression.push_back(fst);
        }
        std::vector<double> range(names.size());
        for (int i = 0; i < range.size(); i++) {
            range[i] = i + shift;
        }
        ranges.push_back(range);
        compressions.push_back(compression);
        times.push_back(time);
        if (metric == COMPRESSION_SUFFIX) {
            plt::bar(ranges[index], compressions[index]);
        } else {
            plt::bar(ranges[index], times[index]);
        }
        shift += 0.25;
        all_names = names;
        fin.close();
        index++;
        }
    std::vector<double> range(all_names.size());
    for (int i = 0; i < range.size(); i++) {
        range[i] = i;
    }
    if (metric == COMPRESSION_SUFFIX) {
        plt::ylabel("compression rate");
        plt::title("compression test");
    } else {
        plt::ylabel("time in milliseconds");
        plt::title("time test");
     }
    plt::xticks(range, all_names);
    fs::path filename = "../artifacts/results_" + metric + deprecated_suffix;
    plt::save(filename);
}

int main() {
    draw(COMPRESSION_SUFFIX);
    draw(TIME_SUFFIX);
    draw(TIME_SUFFIX, "zstd7");

}

