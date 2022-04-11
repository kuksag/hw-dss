#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zstd.h>
#include <lz4.h>
#include "dependencies/zstd/examples/common.h"
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

struct ZSTD_COMPRESSOR {
public:
    double compression_rate;
    long compress_time_in_millis;

    void loadFile(const char *fname) {
        buffer = mallocAndLoadFile_orDie(fname, &buffer_size);
        compressed_size = ZSTD_compressBound(buffer_size);
        compressed = malloc_orDie(compressed_size);
    }

    void compress_loaded_file(int level) {
        clock_t start, stop;
        start = clock();
        size_t cSize = ZSTD_compress(compressed, compressed_size, buffer, buffer_size, level);
        stop = clock();
        CHECK_ZSTD(cSize);
        compression_rate = (double) buffer_size / cSize;
        compress_time_in_millis = stop - start;
        free_memory();
    }

private:
    void *buffer;
    size_t compressed_size;
    size_t buffer_size;
    void *compressed;

    void free_memory() const {
        free(buffer);
        free(compressed);
    }

};

void ZSTD_TEST() {
    fs::path dataset_dir_path = "../dataset";
    //fs::path result_dir_path = "../res";
    ZSTD_COMPRESSOR compressor{};
    printf("file \t compression rate \t time(milliseconds)\n");
    for (const auto &entity : fs::directory_iterator(dataset_dir_path)) {
        std::string filename = entity.path().string();
        compressor.loadFile(filename.c_str());
        compressor.compress_loaded_file(7);
        printf("%s\t %.3f %ld\n", filename.c_str(), compressor.compression_rate, compressor.compress_time_in_millis);
    }
}


int main(int argc, const char **argv) {
    ZSTD_TEST();
    return 0;
}
