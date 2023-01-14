# [Part 2](https://fgiesen.wordpress.com/2011/07/02/a-trip-through-the-graphics-pipeline-2011-part-2/)

## The memory subsystem

GPU memory substystems are very different from CPU's:
* **Memory bandwidth**, how fast memory could be read and stored, is faster on a GPU
  * [Intel Core i7 13700](https://www.intel.com/content/www/us/en/products/sku/230490/intel-core-i713700-processor-30m-cache-up-to-5-20-ghz/specifications.html): 89.6 GB/s
  * [Intel Arc A550 graphics](https://www.intel.com/content/www/us/en/products/sku/228343/intel-arc-a550m-graphics/specifications.html): 224 GB/s
  * [Nvidia GeForce RTX 4090](https://www.techpowerup.com/gpu-specs/geforce-rtx-4090.c3889): 1,008 GB/s
* **Memory latency**, the time between the processor requesting the data from memory and obtaining the data, is much slower on a GPU

* GPUs prioritize throughput over latency.
* DRAM access
  * Access by row, column
  * Always reads/write the entire row at once
    * Faster to access the same row than multiple rows

## The PCIe host interface
* **PCIe**: Peripherical component interconnect express
  * An extension bus standard, interface to components such as WiFi/Ethernet components, SSDs, graphics cards
* Allows the CPU to access GPU registers, video memory
  * CPU submits GPU commands in the command buffer, PCIe then sends those commands to the GPU
* Has a decent latency cost
* Older, phased out alternative: AGP (Accelerated Graphics Port)
  * Unlike PCIe, AGP did not have *symmetrical point-to-point link*: bandwidth of CPU -> GPU != bandwidth of GPU -> CPU

## Some final memory bits and pieces

Two types of memory:
* (local) video memory
* Mapped system memory
  * Slower than local video memory

**Memory management unit** (MMU)
* Can defragment video memory address space
* Not required in the GPU pipeline?
  
**Direct memory access** (DMA): allows transfers of memory between video and system memory without having the processor involved (in this case, the 3D hardware and shader cores)

## The command processor
Memory paths in the GPU is both high-bandwidth and high-latency.

Submit a command -> command processing front-end which parses the command
* Some pipelines have separate units to parse 2D and 3D commands

State change (ex. updating a variable) in a GPU is more complicated because it is a parallel system
* Various methods to handle state change depending on the chip
  * A lot HW components collectively try to minimize latency
  * Use cases allow for different trade-offs

## Synchronization
Synchonizing between CPU/GPU and GPU/GPU

* **Push-model**: GPU reports changes to CPU immediately
  * Done with interrupts of infrequent, high-priority events
  * Expensive
* **Pull-model**: GUP remembers that a change happened, and then the CPU can ask about those changes/state later when needed
  * Write updates to GPU registers that CPU has access to

* Synchornize with **wait-style instruction**: wait for register M to have a value N
* Other methods: fence/barrier instructions
