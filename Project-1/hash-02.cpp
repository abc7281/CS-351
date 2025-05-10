
#include <fstream>

const char* path = DATA_PATH "/Data.bin";

int main(int argc, char* argv[]) {
    std::ifstream input{path, std::ios::binary};
    std::string outpath{argv[0]};
    outpath += ".txt";
    std::ofstream hashes{outpath};

    size_t numHashes;
    input.read(reinterpret_cast<char*>(&numHashes), sizeof(numHashes));

    for (auto i = 0; i < numHashes; ++i) {
        size_t numBytes;
        input.read(reinterpret_cast<char*>(&numBytes), sizeof(numBytes));

        [&]() {
            using Byte = unsigned char;
            using Bytes = Byte*;
            Bytes bytes = (Bytes) alloca(numBytes);
            input.read(reinterpret_cast<char*>(bytes), numBytes);

            using Hash = unsigned long long;
            Hash hash = 104395301;
            const Hash multiplier = 2654435789;
            for (auto j = 0; j < numBytes; ++j) {
                hash += (multiplier * bytes[j]) ^ (hash >> 23);
            }

            hashes << hash << "\n";
        }();
    }

    hashes.close();
    input.close();
}