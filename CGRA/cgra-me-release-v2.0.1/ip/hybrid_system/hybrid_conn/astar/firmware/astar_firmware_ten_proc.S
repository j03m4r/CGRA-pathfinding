# A* Pathfinding CGRA Accelerator Firmware (CORRECTED VERSION)
# RISC-V Assembly for HyCUBE Hybrid System
# This version handles 4 outputs: neighbor, g-score, h-score, f-score

.include "hybrid.h"

.data
    # Sample 20-node graph data for A* pathfinding
    
    # CSR format: row_ptr[21], col_idx[edges], weights[edges]
    row_ptr:
        .word 0, 2, 5, 8, 11, 14, 17, 20, 23, 26, 29
        .word 32, 35, 38, 41, 44, 47, 50, 53, 56, 58, 60
    
    col_idx:
        .word 1, 4, 0, 2, 5, 1, 3, 6, 2, 7, 11
        .word 0, 5, 8, 1, 4, 6, 9, 2, 5, 7, 10
        .word 3, 6, 11, 4, 9, 12, 5, 8, 10, 13
        .word 6, 9, 14, 3, 7, 15, 8, 13, 16, 9, 12, 14, 17
        .word 10, 13, 18, 11, 16, 19, 12, 17, 18, 14, 16, 19
        .word 15, 18, 17, 19
    
    weights:
        .word 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
        .word 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
        .word 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
        .word 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
        .word 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
        .word 100, 100, 100, 100
    
    # Node positions (x, y coordinates) - 4x5 grid
    positions_x:
        .word 0, 100, 200, 300, 0, 100, 200, 300, 0, 100
        .word 200, 300, 0, 100, 200, 300, 0, 100, 200, 300
    
    positions_y:
        .word 0, 0, 0, 0, 100, 100, 100, 100, 200, 200
        .word 200, 200, 300, 300, 300, 300, 400, 400, 400, 400
    
    # Goal coordinates (node 19 at position 300, 400)
    goal_x: .word 300
    goal_y: .word 400
    
    # Current node being processed
    current_node: .word 0
    current_g_score: .double 0.0
    
    # Output buffers (increased sizes for 4 arrays)
    output_neighbors: .space 240  # Max 60 neighbors (4 bytes each)
    output_g_values: .space 480   # Max 60 g-values (8 bytes each, double)
    output_h_values: .space 480   # Max 60 h-values (8 bytes each, double) - NEW
    output_f_values: .space 480   # Max 60 f-values (8 bytes each, double) - NEW
    
    # Configuration
    num_iterations: .word 10  # Process 10 nodes for testing

.text
.globl app

app:
    # Save return address and callee-saved registers
    addi sp, sp, -32
    sw ra, 28(sp)
    sw s0, 24(sp)
    sw s1, 20(sp)
    sw s2, 16(sp)
    sw s3, 12(sp)
    sw s4, 8(sp)
    
    # Initialize iteration counter
    la s0, num_iterations
    lw s0, 0(s0)           # s0 = number of nodes to process
    li s1, 0               # s1 = current iteration
    
main_loop:
    # Check if done
    bge s1, s0, done
    
    # Load current node parameters
    la t0, current_node
    lw s2, 0(t0)           # s2 = current_node
    
    la t0, row_ptr
    slli t1, s2, 2         # t1 = current_node * 4
    add t1, t0, t1
    lw s3, 0(t1)           # s3 = edge_start = row_ptr[current_node]
    lw s4, 4(t1)           # s4 = edge_end = row_ptr[current_node + 1]
    
    # === STEP 1: Load data into CGRA memory banks ===
    
    # mem_0: col_idx array (neighbor indices)
    li t0, mem_0
    la t1, col_idx
    li t2, 60              # Load all 60 edges
    call load_array_int
    
    # mem_1: weights array
    li t0, mem_1
    la t1, weights
    li t2, 60
    call load_array_int
    
    # mem_2: positions_x array
    li t0, mem_2
    la t1, positions_x
    li t2, 20              # 20 nodes
    call load_array_int
    
    # mem_3: positions_y array
    li t0, mem_3
    la t1, positions_y
    li t2, 20
    call load_array_int
    
    # Note: We don't use mem_4-7 as inputs anymore - they're all outputs now
    # Configuration data (goal_x, goal_y, edge_start, edge_end, g_current)
    # needs to be passed via registers or separate mechanism
    
    # For now, we'll assume CGRA reads goal_x, goal_y from positions arrays
    # and bounds/g_current are configured separately
    
    # === STEP 2: Configure and start CGRA ===
    
    # Calculate number of iterations needed
    # Each loop iteration processes one edge
    sub t0, s4, s3         # num_edges = edge_end - edge_start
    
    # Assuming II=2 (Initiation Interval) and latency=15 cycles (higher due to sqrt)
    # Total cycles = latency + (num_iterations - 1) * II
    li t1, 15              # latency (increased for sqrt operation)
    addi t2, t0, -1        # num_iterations - 1
    slli t2, t2, 1         # multiply by II=2
    add t0, t1, t2         # total cycles
    
    # Add some margin for safety
    addi t0, t0, 30
    
    # Start CGRA by writing to counter
    li t1, counter
    sw t0, 0(t1)
    
    # === STEP 3: Wait for CGRA completion ===
wait_cgra:
    li t1, counter
    lw t2, 0(t1)
    bnez t2, wait_cgra     # Wait while counter > 0
    
    # === STEP 4: Read results from CGRA memory banks ===
    
    # mem_4: output_neighbors array
    li t0, mem_4
    la t1, output_neighbors
    sub t2, s4, s3         # num_neighbors = edge_end - edge_start
    call store_array_int
    
    # mem_5: output_g_values array (doubles, 8 bytes each)
    li t0, mem_5
    la t1, output_g_values
    sub t2, s4, s3         # num_values
    call store_array_double
    
    # mem_6: output_h_values array (doubles, 8 bytes each) - NEW
    li t0, mem_6
    la t1, output_h_values
    sub t2, s4, s3         # num_values
    call store_array_double
    
    # mem_7: output_f_values array (doubles, 8 bytes each) - NEW
    li t0, mem_7
    la t1, output_f_values
    sub t2, s4, s3         # num_values
    call store_array_double
    
    # === STEP 5: Process results (example: find best f-score) ===
    
    # Find the neighbor with minimum f-score
    sub t0, s4, s3         # num_neighbors
    beqz t0, skip_processing  # Skip if no neighbors
    
    la t1, output_f_values
    fld ft0, 0(t1)         # min_f = f_values[0]
    li t2, 0               # best_idx = 0
    li t3, 1               # i = 1
    
find_min_f:
    bge t3, t0, found_min
    
    slli t4, t3, 3         # offset = i * 8
    add t5, t1, t4
    fld ft1, 0(t5)         # f_values[i]
    
    # Compare: if f_values[i] < min_f
    flt.d t6, ft1, ft0
    beqz t6, not_better
    
    # Update minimum
    fmv.d ft0, ft1
    mv t2, t3              # best_idx = i
    
not_better:
    addi t3, t3, 1
    j find_min_f
    
found_min:
    # t2 now contains the index of the best neighbor
    # In real A*, you would add this to the open set
    
skip_processing:
    # === STEP 6: Update for next iteration ===
    
    # Update current_node (simple: just increment for testing)
    la t0, current_node
    lw t1, 0(t0)
    addi t1, t1, 1
    li t2, 20
    rem t1, t1, t2         # Wrap around to test multiple nodes
    sw t1, 0(t0)
    
    # Update g_score (example: increment)
    la t0, current_g_score
    fld ft0, 0(t0)
    li t1, 100
    fcvt.d.w ft1, t1       # Convert 100 to double
    fadd.d ft0, ft0, ft1
    fsd ft0, 0(t0)
    
    # Increment iteration counter
    addi s1, s1, 1
    j main_loop

done:
    # Restore registers and return
    lw s4, 8(sp)
    lw s3, 12(sp)
    lw s2, 16(sp)
    lw s1, 20(sp)
    lw s0, 24(sp)
    lw ra, 28(sp)
    addi sp, sp, 32
    
    li a0, 0               # Return 0 (success)
    ret

# ============================================
# Helper Functions
# ============================================

# Load integer array from memory to CGRA bank
# Arguments:
#   t0 = CGRA memory bank base address
#   t1 = Source array address
#   t2 = Number of elements
load_array_int:
    addi sp, sp, -16
    sw ra, 12(sp)
    sw t3, 8(sp)
    sw t4, 4(sp)
    
    li t3, 0               # counter
load_loop:
    bge t3, t2, load_done
    
    slli t4, t3, 2         # offset = counter * 4
    add t5, t1, t4         # source address
    lw t6, 0(t5)           # load word
    
    add t5, t0, t4         # dest address
    sw t6, 0(t5)           # store to CGRA memory
    
    addi t3, t3, 1
    j load_loop
    
load_done:
    lw t4, 4(sp)
    lw t3, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
    ret

# Store integer array from CGRA bank to memory
# Arguments:
#   t0 = CGRA memory bank base address
#   t1 = Destination array address
#   t2 = Number of elements
store_array_int:
    addi sp, sp, -16
    sw ra, 12(sp)
    sw t3, 8(sp)
    sw t4, 4(sp)
    
    li t3, 0               # counter
store_loop:
    bge t3, t2, store_done
    
    slli t4, t3, 2         # offset = counter * 4
    add t5, t0, t4         # source address (CGRA)
    lw t6, 0(t5)           # load word from CGRA
    
    add t5, t1, t4         # dest address (main memory)
    sw t6, 0(t5)           # store to memory
    
    addi t3, t3, 1
    j store_loop
    
store_done:
    lw t4, 4(sp)
    lw t3, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
    ret

# Store double array from CGRA bank to memory
# Arguments:
#   t0 = CGRA memory bank base address
#   t1 = Destination array address
#   t2 = Number of elements (doubles are 8 bytes each)
store_array_double:
    addi sp, sp, -16
    sw ra, 12(sp)
    sw t3, 8(sp)
    sw t4, 4(sp)
    
    li t3, 0               # counter
store_double_loop:
    bge t3, t2, store_double_done
    
    slli t4, t3, 3         # offset = counter * 8 (double = 8 bytes)
    add t5, t0, t4         # source address (CGRA)
    fld ft0, 0(t5)         # load double from CGRA
    
    add t5, t1, t4         # dest address (main memory)
    fsd ft0, 0(t5)         # store double to memory
    
    addi t3, t3, 1
    j store_double_loop
    
store_double_done:
    lw t4, 4(sp)
    lw t3, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
    ret

