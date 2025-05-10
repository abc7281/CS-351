
#include <fstream>
#include <iostream>

const char* path = DATA_PATH "/Data.txt";

int main(int argc, char* argv[]) {
    std::ifstream input{path};
    std::string outpath{argv[0]};
    outpath += ".txt";
    std::ofstream hashes{outpath};

    size_t numHashes;
    input >> numHashes;

    for (auto i = 0; i < numHashes; ++i) {
        size_t numBytes;
        input >> numBytes;

        using Hash = unsigned long long;
        Hash hash = 104395301;
        const Hash multiplier = 2654435789;
        
        using Byte = unsigned short;
        Byte byte;
        for (auto j = 0; j < numBytes; ++j) {
            input >> byte;
            hash += (multiplier * byte) ^ (hash >> 23);
        }

        hashes << hash << "\n";
    }

    hashes.close();
    input.close();
}