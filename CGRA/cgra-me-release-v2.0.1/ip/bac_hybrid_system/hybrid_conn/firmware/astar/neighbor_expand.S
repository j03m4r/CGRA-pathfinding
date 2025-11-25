#include "hybrid.h"

    .section .text
    .globl app
    .globl app_ret

app:
    // Initialize base addresses
    li      a0, CGRA_CONFIG_BASE
    li      a1, CGRA_INPUT_BASE
    li      a2, CGRA_OUTPUT_BASE
    li      a3, MEM_BASE
    
    // Load CGRA bitstream (dummy - will be generated)
    li      t0, 0x12345678
    sw      t0, 0(a0)
    
    // Set up test data in memory
    // Node positions: neighbor at (500, 500), goal at (1000, 1000)
    li      t0, 500
    sw      t0, 0(a3)     // neighbor_x
    sw      t0, 4(a3)     // neighbor_y
    
    // Edge weight = 10
    li      t0, 10
    sw      t0, 8(a3)     // edge_weight
    
    // Current g-score = 100
    li      t0, 100
    sw      t0, 0(a1)     // current_g input
    
    // Start CGRA execution
    li      t0, 1
    sw      t0, CGRA_START(a0)
    
    // Wait for CGRA to finish
wait_cgra:
    lw      t0, CGRA_STATUS(a0)
    andi    t0, t0, 0x1
    bnez    t0, wait_cgra
    
    // Read results
    lw      t1, 0(a2)     // new_g
    lw      t2, 4(a2)     // heuristic (dist_sq)
    
    // Print results
    mv      a0, t1
    call    print_dec
    li      a0, '\n'
    call    print_chr
    
    mv      a0, t2
    call    print_dec
    li      a0, '\n'
    call    print_chr
    
app_ret:
    ret

    .data
    .align 4
test_nodes:
    .word 500, 500    // neighbor position
    .word 10          // edge weight
