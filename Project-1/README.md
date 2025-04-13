# CS-351 Project-1: Performance Evaluation Report

Here lies the performance evaluation of the five implementations for computing 1,000,000 hashes using two compiler options: a non-optimized default build (`-g`) and an optimized build (`-O2`).

---

## Experiment 1: Non-Optimized Build (`-g`)

The following timings were recorded using the command:
/usr/bin/time -f "%e real\t%U user\t%S sys\t%M memory (KB)" ./hash-0X

| Program  | Optimization | Real Time (s) | User Time (s) | Sys Time (s) | Memory (KB) | Throughput (hashes/s)     | Improvement vs hash-00 |
|----------|--------------|---------------|---------------|--------------|-------------|---------------------------|------------------------|
| hash-00  | -g           | 337.20        | 330.51        | 4.60         | 2892        | 1,000,000 / 337.20 ≈ 2964 | 1.00                   |
| hash-01  | -g           | 17.87         | 16.51         | 1.21         | 2880        | 1,000,000 / 17.87 ≈ 55958 | 337.20 / 17.87 ≈ 18.87 |
| hash-02  | -g           | 15.61         | 14.32         | 1.17         | 3464        | 1,000,000 / 15.61 ≈ 64063 | 337.20 / 15.61 ≈ 21.60 |
| hash-03  | -g           | 16.46         | 15.07         | 1.28         | 3468        | 1,000,000 / 16.46 ≈ 60747 | 337.20 / 16.46 ≈ 20.48 |
| hash-04  | -g           | 14.35         | 13.78         | 0.46         | 5011784     | 1,000,000 / 14.35 ≈ 69720 | 337.20 / 14.35 ≈ 23.50 |

*Notes:*  
- The baseline is **hash-00**, which reads the text file one value at a time and takes a very very long time.
- The improvements for the other programs are calculated as:
  **Improvement = (hash-00 real time) / (current program real time)**.

---

## Experiment 2: Optimized Build (`-O2`)

After cleaning the old executables with `make rmtargets`, the programs were rebuilt with:
make OPT="-O2"

The following timings were recorded:

| Program  | Optimization | Real Time (s) | User Time (s) | Sys Time (s) | Memory (KB) | Throughput (hashes/s)     | Improvement vs hash-00 |
|----------|--------------|---------------|---------------|--------------|-------------|---------------------------|------------------------|
| hash-00  | -O2          | 337.02        | 330.71        | 4.58         | 2888        | 1,000,000 / 337.02 ≈ 2966 | 1.00                   |
| hash-01  | -O2          | 8.22          | 6.93          | 1.20         | 3468        | 1,000,000 / 8.22 ≈ 121673 | 337.02 / 8.22 ≈ 41.0   |
| hash-02  | -O2          | 8.16          | 6.85          | 1.23         | 3456        | 1,000,000 / 8.16 ≈ 122549 | 337.02 / 8.16 ≈ 41.3   |
| hash-03  | -O2          | 8.05          | 6.76          | 1.20         | 2888        | 1,000,000 / 8.05 ≈ 124223 | 337.02 / 8.05 ≈ 41.8   |
| hash-04  | -O2          | 7.13          | 6.60          | 0.45         | 5012352     | 1,000,000 / 7.13 ≈ 140210 | 337.02 / 7.13 ≈ 47.3   |

*Notes:*  
- The optimized builds of hash-01, hash-02, hash-03, and hash-04 show dramatic performance improvements compared to the baseline hash-00, which is almost unchanged.
- The improvements for hash-04 suggest that while it benefits from optimization, its high memory usage remains as a result of memory mapping.

---

## Analysis Questions

### 1. What operation accounts for most of hash-00's runtime?

The hash-00 implementation reads each value from the file individually. Reading each value one at a time is very inefficient resulting in long execution times.

### 2. Comparison of hash-01 vs. hash-02 dynamic allocation methods

Both **hash-01** and **hash-02** allocate memory for each hash computation:
- **hash-01** uses `operator new`.
- **hash-02** uses `calloc()` which does allocation on the stack versus operator new's allocation in the heap.

The timing results show that hash-02 is slightly faster (15.61 s vs. 17.87 s in the non-optimized build, and 8.16 s vs. 8.22 s in the optimized build). The difference is not massive, but the consistency in the data still shows that the allocation method in hash-02 offers a small improvement in performance.

### 3. Impact of using a fixed-size array in hash-03

The **hash-03** implementation avoids dynamic memory allocation by using a fixed-size array.
- The timings (16.46 s in non-optimized, 8.05 s in optimized) are very close to those of hash-01 and hash-02.
- The fixed array method does not have any speed difference when compared to hash-01 and hash-02, in fact, it is slower than hash-02 in the non-optimised test. This suggests that the increasing in size of the array in this situation does not have a major performance impact.

### 4. Reason for high memory usage in hash-04

**Hash-04** uses memory mapping (`mmap`) to map the entire binary file into the process's address space.  
- This approach reads the file into its own buffer making it availible to the program as a pointer to an array of bytes.
- As a result, the system memory usage is high because the file is being mapped and remains in memory. This is why memory usage is so high with this method.

### 5. Other Compiler Options Experimented With

In addition to the standard `-O2` flag, other optimization flags such as `-funroll-loops` were experimented with.  
- These flags had small improvements in timings for some versions of the programs, while these small optimizations can help, the large changes in numbers occurred when switching from dynamic allocation or from inefficient algorithms in hash-00 to more optimized memory handling in the other versions.
