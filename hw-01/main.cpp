#include "dependencies/smhasher/src/MurmurHash3.h"
#include "dependencies/smhasher/src/SpeedTest.h"

int main() {
    double outCycles;
    const int hashSize = 100;
    const int keySize = 100;
    const int seed = 1;
    const bool verbose = true;
    TinySpeedTest(MurmurHash3_x64_128, hashSize, keySize, seed, verbose, outCycles);
    return 0;
}
