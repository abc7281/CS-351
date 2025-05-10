# CS-351 Project-1: Performance Evaluation Report

This report details the performance evaluation of five different implementations for computing 1,000,000 hashes using two sets of compiler options: a non-optimized build (default, debug build with `-g`) and an optimized build (`-O2`). Each implementation reads input data in a different manner, and the goal is to measure improvements and analyze where processing time and resource usage are spent.

---

## Experiment 1: Non-Optimized Build (`-g`)

The following timings were recorded using the command:

| Program  | Optimization | Real Time (s) | User Time (s) | Sys Time (s) | Memory (KB) | Throughput (hashes/s) | Improvement vs hash-00 |
|----------|--------------|---------------|---------------|--------------|-------------|------------------------|------------------------|
| hash-00  | -g           | 337.20        | 330.51        | 4.60         | 2892       |1,000,000 / 337.20 ≈ 2964| 1.00                   |
| hash-01  | -g           | 17.87         | 16.51         | 1.21         | 2880       |	1,000,000 / 17.87 ≈ 55958| 337.20 / 17.87 ≈ 18.87  |
| hash-02  | -g           | 15.61         | 14.32         | 1.17         | 3464       |	1,000,000 / 15.61 ≈ 64063| 337.20 / 15.61 ≈ 21.60  |
| hash-03  | -g           | 16.46         | 15.07         | 1.28         | 3468       |1,000,000 / 16.46 ≈ 60747| 337.20 / 16.46 ≈ 20.48  |
| hash-04  | -g           | 14.35         | 13.78         | 0.46         | 5011784    |1,000,000 / 14.35 ≈ 69720| 337.20 / 14.35 ≈ 23.50  |

*Notes:*  
- The baseline is **hash-00**, which reads the text file one value at a time and takes significantly longer.  
- The improvements for the other programs are calculated as:  
  **Improvement = (hash-00 real time) / (current program real time)**.

---

## Experiment 2: Optimized Build (`-O2`)

After cleaning the old executables with `make rmtargets`, the programs were rebuilt with:

The following timings were recorded:

| Program  | Optimization | Real Time (s) | User Time (s) | Sys Time (s) | Memory (KB) |	Throughput (hashes/s)| Improvement vs hash-00 |
|----------|--------------|---------------|---------------|--------------|-------------|------------------------|------------------------|
| hash-00  | -O2          | 337.02        | 330.71        | 4.58         | 2888       |	1,000,000 / 337.02 ≈ 2966| 1.00                   |
| hash-01  | -O2          | 8.22          | 6.93          | 1.20         | 3468       |1,000,000 / 8.22 ≈ 121673| 337.02 / 8.22 ≈ 41.0   |
| hash-02  | -O2          | 8.16          | 6.85          | 1.23         | 3456       |1,000,000 / 8.16 ≈ 122549| 337.02 / 8.16 ≈ 41.3   |
| hash-03  | -O2          | 8.05          | 6.76          | 1.20         | 2888       |	1,000,000 / 8.05 ≈ 124223| 337.02 / 8.05 ≈ 41.8   |
| hash-04  | -O2          | 7.13          | 6.60          | 0.45         | 5012352    |	1,000,000 / 7.13 ≈ 140210| 337.02 / 7.13 ≈ 47.3   |

*Notes:*  
- The optimized builds of hash-01, hash-02, hash-03, and hash-04 show dramatic performance improvements compared to the baseline hash-00, which appears unchanged.  
- The improvements for hash-04, for instance, suggest that while it benefits from optimization, its very high memory usage remains as a characteristic of memory mapping.

---

## Analysis Questions

### 1. What operation accounts for most of hash-00's runtime?

The hash-00 implementation reads each value from the ASCII file individually. This frequent file I/O is considerably slower due to system call overhead and inefficiencies in reading one value at a time, which explains its significantly longer execution time relative to the others.

### 2. Comparison of hash-01 vs. hash-02 dynamic allocation methods

Both **hash-01** and **hash-02** allocate memory for each hash computation:
- **hash-01** uses `operator new`.
- **hash-02** uses `calloc()` within a lambda function (to leverage stack allocation constraints).

The timing results show that hash-02 is slightly faster (15.61 s vs. 17.87 s in the non-optimized build, and 8.16 s vs. 8.22 s in the optimized build). While the difference is not huge, it suggests that the allocation method in hash-02 might have a slight efficiency gain, but overall both methods demonstrate a similar performance profile.

### 3. Impact of using a fixed-size array in hash-03

The **hash-03** implementation avoids dynamic memory allocation by using a fixed-size array (allocated on the stack).  
- The timings (16.46 s in non-optimized, 8.05 s in optimized) are very close to those of hash-01 and hash-02.  
- This indicates that while avoiding per-iteration dynamic allocation might reduce overhead, the performance gain is modest. The fixed array method may help in predictability of allocation time, but here the difference isn’t drastic.

### 4. Reason for high memory usage in hash-04

**Hash-04** uses memory mapping (`mmap`) to map the entire binary file into the process's address space.  
- This approach leverages the operating system's mechanism to read the file into its own buffer (the page cache) and then provides the application with a pointer into that memory.
- As a result, no extra memory copy is made for the application's use, but the underlying system memory usage appears very high since the entire file is being mapped and remains resident in memory. This is why you see the memory usage at around 5 GB, compared to a few MB for the others.

### 5. Other Compiler Options Experimented With

In addition to the standard `-O2` flag, additional optimization flags such as `-funroll-loops` were experimented with.  
- These additional flags resulted in marginal improvements in execution time for some versions of the programs, indicating that while loop unrolling and similar micro-optimizations can help, the primary gains were achieved by the switch from dynamic allocation (or inefficient I/O in hash-00) to more optimized memory handling in the other versions.

---

## Conclusion

The experiments reveal the following key insights:

Hash-00 is significantly slower due to inefficient per-value file reading.

Hash-01, hash-02, and hash-03 benefit from more efficient memory allocation techniques, achieving far higher throughput compared to hash-00.

Hash-04 delivers the best throughput, despite its high memory usage, by leveraging memory mapping to access file contents directly from the OS's cache.

Compiler optimizations (using -O2 and additional flags) greatly enhance performance, particularly for the implementations that already use more efficient I/O and memory allocation methods.

---
