# Project 4 – CUDA **iota** & Julia-set Generator  
_CS 351 • Spring 2025_

---

## Deliverables in this folder

| File / program | What it is |
|----------------|------------|
| `iota.cpp` / `iota.cpu` | CPU reference version of `std::iota` |
| `iota.cu`  / `iota.gpu` | CUDA version with a six-line kernel |
| `julia.cpp` / `julia.cpu` | CPU Julia-set renderer (PPM out) |
| `julia.cu`  / `julia.gpu` | CUDA Julia-set renderer |
| `runTrials.sh` | Script that produced the iota timing tables |
| `cpu_times.txt`, `gpu_times.txt` | Raw timing logs for iota |
| `images/julia.ppm` | Image produced by `julia.gpu` (embedded below) |
| `README.md` | **this** file |

Everything lives inside **Project-4** in my GitHub repo; nothing is submitted on Canvas.

---

## iota results & discussion

### Timing tables  
(_Generated with `./runTrials.sh` on an RTX A6000 cloud VM._)

#### CPU (`iota.cpu`)

| Vector<br>Length | Wall Clock (s) | User (s) | Sys (s) |
|:--:|--:|--:|--:|
| 10 | 0.00 | 0.00 | 0.00 |
| 100 | 0.00 | 0.00 | 0.00 |
| 1 000 | 0.00 | 0.00 | 0.00 |
| 10 000 | 0.00 | 0.00 | 0.00 |
| 100 000 | 0.00 | 0.00 | 0.00 |
| 1 000 000 | 0.00 | 0.00 | 0.00 |
| 5 000 000 | 0.02 | 0.00 | 0.02 |
| 100 000 000 | 0.59 | 0.09 | 0.50 |
| 500 000 000 | 2.98 | 0.46 | 2.51 |
| 1 000 000 000 | 5.93 | 0.91 | 5.01 |
| 5 000 000 000 | 33.64 | 5.92 | 27.72 |

#### GPU (`iota.gpu`)

| Vector<br>Length | Wall Clock (s) | User (s) | Sys (s) |
|:--:|--:|--:|--:|
| 10 | 0.31 | 0.02 | 0.28 |
| 100 | 0.21 | 0.01 | 0.19 |
| 1 000 | 0.21 | 0.01 | 0.20 |
| 10 000 | 0.21 | 0.00 | 0.20 |
| 100 000 | 0.21 | 0.00 | 0.20 |
| 1 000 000 | 0.21 | 0.01 | 0.20 |
| 5 000 000 | 0.25 | 0.02 | 0.22 |
| 100 000 000 | 0.87 | 0.17 | 0.69 |
| 500 000 000 | 3.28 | 0.78 | 2.49 |
| 1 000 000 000 | 6.82 | 1.78 | 5.04 |
| 5 000 000 000 | 48.06 | 9.42 | 38.65 |

**Are the results what I expected?  Why is CUDA a poor fit here?**  
Yes. Each GPU launch incurs significant latency plus two PCIe transfers; the per-element work is only one addition, so the overhead dominates for small/medium arrays. Only at billions of elements does the GPU approach CPU time, and even there the CPU’s cache-friendly loop remains competitive. CUDA excels when each thread does _lots_ of math—not one integer add.

---

## CUDA Julia-set image

The CUDA kernel (one thread = one pixel):

```cpp
__global__ void julia(Complex d, Complex center, Color* pixels) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= Width || y >= Height) return;

    float real = -center.x + x * d.x;
    float imag = -center.y + y * d.y;
    Complex z{ real, imag };

    int iter = 0;
    while (iter < MaxIterations && magnitude(z) <= 2.0f) {
        z = z * z + center;
        ++iter;
    }
    pixels[y * Width + x] = setColor(iter);
}
```

ll      = (-2.1, -2.1)

ur      = ( 2.1,  2.1)

d       = ((ur.x-ll.x)/Width , (ur.y-ll.y)/Height)  // pixel size

center  = (2.1, 2.1)   // constant  c

block   = (32,32)

grid    = (Width/32 , Height/32)



iota.cpu	sequential loop filling a vector

iota.gpu	six-line CUDA kernel, one thread per element

julia.cpu	nested loops compute escape count, write PPM

julia.gpu	GPU kernel computes each pixel in parallel
