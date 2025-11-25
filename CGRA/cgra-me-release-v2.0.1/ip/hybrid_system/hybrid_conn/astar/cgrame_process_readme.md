# A* Pathfinding to CGRA-ME Conversion

## Files Created

### 1. astar_silent.cpp (Original C++ Version)
- Full parallel A* implementation
- Uses C++ STL, futures, and threading
- Runs 4 shortest path problems on a 20-node graph
- No console output

### 2. astar.c (Full C Conversion)
- Complete C99 conversion of the A* algorithm
- Sequential (non-parallel) implementation
- Maintains all functionality
- Uses malloc/free for dynamic memory

### 3. astar_kernel.c (CGRA-Ready Kernel)
- **THIS IS THE FILE FOR CGRA-ME**
- Extracts the hot compute loop: neighbor expansion
- Marked with `//DFGLOOP: loop` for CGRA-ME DFG generation
- Uses `__attribute__((noinline))` to prevent aggressive optimization
- Suitable for hardware acceleration

## CGRA-ME Integration Steps

### Step 1: Generate DFG from the Kernel

Place `astar_kernel.c` in a CGRA-ME benchmark directory:

```bash
cd cgra-me-release-v2.0.1/benchmarks/userbench
mkdir astar
cp astar_kernel.c astar/
```

Create a Makefile in the astar directory:

```makefile
include ../rules/DFG_from_source_code.mk
```

Generate the DFG:

```bash
cd astar
make
```

This will create `graph_loop.dot` which represents the dataflow graph of the neighbor expansion loop.

### Step 2: Map to CGRA Architecture

Run the mapper (example using HyCUBE architecture):

```bash
cgrame -c 2 -g graph_loop.dot -m ILPHeuristicMapper \
  --mapper-opts "ILPHeuristicMapper.allow_recomputation=true" \
  --arch-opts "pe_conn=255 cols=4 rows=4" \
  --gen-testbench --fixed-ports fixed_ports.txt
```

### Step 3: Create Fixed Ports File

Create `fixed_ports.txt` to map specific DFG operations to CGRA resources:

```
# Example fixed ports mapping
# Format: operation_name:resource_name
i4_load0:mem_0
i4_load1:mem_1
i4_store0:mem_2
```

### Step 4: Generate Hybrid System Files

Generate Verilog and control files:

```bash
cgrame -c 2 -m ILPHeuristicMapper \
  --mapper-opts "ILPHeuristicMapper.allow_recomputation=true" \
  --arch-opts "pe_conn=255 cols=4 rows=4" \
  --gen-verilog . \
  --verilog-opts "hybrid=on memsize=22"
```

This creates:
- `cgrame.v`: CGRA architecture Verilog
- `control.sv`: Connection/control logic
- `hybrid.h`: Header file for RISC-V firmware
- `testbench.v`: Configuration bitstream

### Step 5: Create RISC-V Firmware

Create an assembly file (e.g., `astar_kernel.S`) that:
1. Loads graph data into shared memory
2. Configures the CGRA
3. Starts the CGRA accelerator
4. Reads results from CGRA output

Example firmware structure:

```assembly
# Load memory-mapped register addresses from hybrid.h
.include "hybrid.h"

.text
.globl app

app:
    # Load graph data to memory banks
    la t0, graph_data
    # ... load data ...
    
    # Start CGRA via counter
    li t1, CGRA_CYCLES  # Number of cycles needed
    sw t1, counter_port
    
    # Wait for completion
wait_loop:
    lw t2, status_port
    beqz t2, wait_loop
    
    # Read results
    # ... read from memory ...
    
    ret
```

## Key Loop for CGRA Acceleration

The core loop being accelerated is:

```c
//DFGLOOP: loop
for (int e = edge_start; e < edge_end; e++) {
    int neighbor = col_idx[e];
    int weight = weights[e];
    
    // Compute tentative g score
    double tentative_g = g_current + weight;
    
    // Compute heuristic (h score)  
    int neighbor_x = positions_x[neighbor];
    int neighbor_y = positions_y[neighbor];
    int dx = neighbor_x - goal_x;
    int dy = neighbor_y - goal_y;
    double h_score = sqrt((double)(dx * dx + dy * dy));
    
    // Store results
    output_neighbors[count] = neighbor;
    output_g_values[count] = tentative_g;
    
    count++;
}
```

This loop:
- Loads graph data from memory (col_idx, weights, positions)
- Performs arithmetic operations (addition, subtraction)
- Computes Euclidean distance (multiply, add, sqrt)
- Stores results back to memory

## Computational Characteristics

- **Memory accesses**: 5 loads, 2 stores per iteration
- **Operations**: 
  - Integer arithmetic: 3 additions, 2 subtractions
  - Floating point: 2 multiplies, 1 add, 1 sqrt
- **Data dependencies**: Minimal between iterations (loop-carried only through count)
- **Parallelization potential**: High - can pipeline iterations

## Notes for CGRA Mapping

1. **Sqrt operation**: May need to approximate or replace with lookup table if CGRA doesn't support sqrt
2. **Memory bandwidth**: Loop requires ~7 memory operations per iteration
3. **Integer/Float mix**: CGRA needs to support both int and double operations
4. **Variable bounds**: Loop bound (edge_end - edge_start) varies per node

## Alternative: Simpler Fixed-Iteration Kernel

If the variable loop bounds are problematic, consider this fixed-iteration version:

```c
//DFGLOOP: loop
for (int e = 0; e < MAX_NEIGHBORS; e++) {
    if (e < num_edges) {
        // ... compute ...
    }
}
```

This trades off some performance for easier CGRA mapping with fixed II.

## Expected Performance

For a 4x4 CGRA:
- **Initiation Interval (II)**: 1-2 cycles (with pipelining)
- **Latency**: ~5-10 cycles per iteration
- **Speedup**: 2-5x over sequential RISC-V execution (depends on memory bandwidth)

## Testing

Compile and run the kernel:

```bash
gcc -std=c99 -O3 astar_kernel.c -lm -o astar_kernel
./astar_kernel
```

Expected output: Silent execution, returns 0 on success.
