#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <zstd.h>
#include <lz4.h>
#include "dependencies/zstd/examples/common.h"
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

struct ZSTD_COMPRESSOR {
public:
    double compression_rate{};
    long compress_time_in_millis{};


    explicit ZSTD_COMPRESSOR(const char *fname) {
        buffer = mallocAndLoadFile_orDie(fname, &buffer_size);
        compressed_size = ZSTD_compressBound(buffer_size);
        compressed = malloc_orDie(compressed_size);
    }

    ZSTD_COMPRESSOR(void *data, size_t buffer_size) : buffer(data), buffer_size(buffer_size) {
        compressed_size = ZSTD_compressBound(buffer_size);
        compressed = malloc_orDie(compressed_size);
    }

    void compress_loaded_file(int level) {
        clock_t start, stop;
        start = clock();
        compressed_size = ZSTD_compress(compressed, compressed_size, buffer, buffer_size, level);
        stop = clock();
        CHECK_ZSTD(compressed_size);
        compression_rate = (double) buffer_size / compressed_size;
        compress_time_in_millis = stop - start;
    }

    void *decompress() {
        unsigned long long const rSize = ZSTD_getFrameContentSize(compressed, compressed_size);
        decompressed = malloc_orDie((size_t) rSize);

        /* Decompress.
         * If you are doing many decompressions, you may want to reuse the context
         * and use ZSTD_decompressDCtx(). If you want to set advanced parameters,
         * use ZSTD_DCtx_setParameter().
         */
        decompressed_size = ZSTD_decompress(decompressed, rSize, compressed, compressed_size);
        CHECK_ZSTD(decompressed_size);
        /* When zstd knows the content size, it will error if it doesn't match. */
        CHECK(decompressed_size == rSize, "Impossible because zstd will check this condition!");

        return decompressed;
    }

    size_t buffer_size{};
    void *buffer;

    size_t compressed_size;
    void *compressed;

    size_t decompressed_size{};
    void *decompressed{};

    ~ZSTD_COMPRESSOR() {
        if (buffer) {
            free(buffer);
        }
        if (compressed) {
            free(compressed);
        }
        if (decompressed) {
            free(decompressed);
        }
    }
};

void CHECK_CORRECTNESS_ZSTD() {
    const size_t BUF_SIZE = 5;
    void *data = calloc(BUF_SIZE, sizeof(char));
    ((char *) data)[0] = 'a';
    ZSTD_COMPRESSOR compressor(data, BUF_SIZE);
    compressor.compress_loaded_file(1);
    compressor.decompress();
    assert(((char *) compressor.decompressed)[0] == 'a');
}

void ZSTD_TEST(const char *dataset) {
    fs::path dataset_dir_path = dataset;
    for (const auto &entity: fs::directory_iterator(dataset_dir_path)) {
        std::string filename = entity.path().string();
        ZSTD_COMPRESSOR compressor(filename.data());
        compressor.compress_loaded_file(7);
        printf("%s\t %.3f %ld\n", entity.path().filename().string().data(), compressor.compression_rate, compressor.compress_time_in_millis);
    }
}


int main(int argc, const char **argv) {
    if (argc != 3) {
        return 1;
    }
    const char ZSTD[] = "zstd";
    const char LZ4[] = "lz4";
    CHECK_CORRECTNESS_ZSTD();
    if (strcmp(argv[2], ZSTD) == 0) {
        ZSTD_TEST(argv[1]);
    } else if (strcmp(argv[2], LZ4) == 0) {
        //
    } else {
        fprintf(stderr, "bad opt\n");
        return 1;
    }
    return 0;
}
