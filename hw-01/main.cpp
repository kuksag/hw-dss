#include "dependencies/smhasher/src/MurmurHash3.h"
#include "dependencies/smhasher/src/SpeedTest.h"
#include "dependencies/xxHash/xxhash.h"
#include "dependencies/xxHash/xxh3.h"

#include <filesystem>
#include <fstream>

class Benchmark {
    enum {
        NOT_USED,
    };
    const static bool VERBOSE = false;
    const static char *TEST_SMALL;
    const static char *TEST_BIG;

    pfHash handle;
    std::filesystem::path path;
    int seed;

    void smallTest() {
        std::fstream file;
        file.open(path.string() + TEST_SMALL, std::fstream::out);
        for (int keySize = LOW; keySize < MIDDLE; keySize += SMALL_STEP) {
            double d;
            TinySpeedTest(handle, NOT_USED, keySize, seed, VERBOSE, d);
            file << keySize << " " << d << '\n';
        }
        file.close();
    }

    void bigTest() {
        std::fstream file;
        file.open(path.string() + TEST_BIG, std::fstream::out);
        for (int keySize = MIDDLE; keySize < HIGH; keySize += BIG_STEP) {
            double d;
            TinySpeedTest(handle, NOT_USED, keySize, seed, VERBOSE, d);
            file << keySize << " " << d << '\n';
        }
        file.close();
    };

public:

    enum Step {
        SMALL_STEP = 1,
        BIG_STEP = 75,
    };

    enum Constraint {
        LOW = 1,
        MIDDLE = 100,
        HIGH = 1000,
    };

    Benchmark(pfHash handle_, std::filesystem::path path_, int seed_ = 1) : handle(handle_), path(std::move(path_)),
                                                                            seed(seed_) {
    }

    void operator()() {
        smallTest();
        bigTest();
    }
};

const char *Benchmark::TEST_SMALL = "_small";
const char *Benchmark::TEST_BIG = "_big";


void *mem;

void memcpyWrapper(const void *blob, const int len, [[maybe_unused]] const uint32_t seed, void *out) {
    memcpy(mem, blob, len);
    ((uint64_t *) out)[0] = 0;
    ((uint64_t *) out)[1] = 0;
}

void xxHashWrapper(const void *blob, const int len, const uint32_t seed, void *out) {
    XXH128_hash_t hash = XXH128(blob, len, seed);
    ((uint64_t *) out)[0] = hash.low64;
    ((uint64_t *) out)[1] = hash.high64;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }
    mem = malloc(Benchmark::HIGH);

    Benchmark(MurmurHash3_x64_128, std::filesystem::path(argv[1]) / "linux_murmur_x64_128")();
    Benchmark(memcpyWrapper, std::filesystem::path(argv[1]) / "linux_memcpy")();
    Benchmark(xxHashWrapper, std::filesystem::path(argv[1]) / "linux_xxhash_128")();

    free(mem);
}
