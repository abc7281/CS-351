
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <fstream>

const char* path = DATA_PATH "/Data.bin";

template <typename Type>
Type* loadData(const char* path) {
    int fd = open(path, O_RDONLY);
    off_t size = lseek(fd, 0, SEEK_END);

    void* memory = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    
    return reinterpret_cast<Type*>(memory);
}

int main(int argc, char* argv[]) {
    using Byte = unsigned char;
    using Bytes = Byte*;
    Bytes data = loadData<Byte>(path);

    size_t numHashes = *reinterpret_cast<size_t*>(data);
    data += sizeof(numHashes);

    std::string outpath{argv[0]};
    outpath += ".txt";
    std::ofstream hashes{outpath};

    for (auto i = 0; i < numHashes; ++i) {
        size_t numBytes = *reinterpret_cast<size_t*>(data);
        data += sizeof(numBytes);

        Bytes bytes = reinterpret_cast<Bytes>(data);

        using Hash = unsigned long long;
        Hash hash = 104395301;
        const Hash multiplier = 2654435789;
        for (auto j = 0; j < numBytes; ++j) {
            hash += (multiplier * bytes[j]) ^ (hash >> 23);
        }

        data += numBytes;

        hashes << hash << "\n";
    }

    hashes.close();
}