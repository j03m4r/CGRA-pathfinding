#include "hybrid.h"

    .section .text
    .globl app

app:
    // Print start message
    la      a0, msg_start
    call    print_str
    
    // Record start cycles
    rdcycle s10
    
    // Initialize A* data structures
    // Memory layout:
    //   0x1000: g_scores[10]
    //   0x1028: f_scores[10]
    //   0x1050: parent[10]
    
    li      s0, 0x1000      // g_scores base
    li      s1, 0x1028      // f_scores base
    li      s2, 0x1050      // parent base
    
    // Initialize all g_scores to large value (0x7FFFFFFF)
    li      t0, 10
    li      t1, 0x7FFFFFFF
    mv      t2, s0
init_g:
    sw      t1, 0(t2)
    addi    t2, t2, 4
    addi    t0, t0, -1
    bnez    t0, init_g
    
    // Set start node (0): g=0, f=600
    li      t0, 0
    sw      t0, 0(s0)       // g[0] = 0
    li      t0, 600
    sw      t0, 0(s1)       // f[0] = 600
    
    // Simulate expanding node 0 to node 1
    // new_g = g[0] + edge_weight = 0 + 100 = 100
    li      a1, 100         // edge weight
    li      a2, 0           // current g = 0
    add     a3, a2, a1      // tentative_g = 0 + 100 = 100
    
    // Store in g[1]
    sw      a3, 4(s0)
    
    // Calculate f[1] = g[1] + h[1] = 100 + 500 = 600
    li      t0, 500
    add     t1, a3, t0
    sw      t1, 4(s1)
    
    // Set parent[1] = 0
    li      t0, 0
    sw      t0, 4(s2)
    
    // Print result for node 1
    la      a0, msg_node1
    call    print_str
    lw      a0, 4(s0)       // g[1]
    call    print_dec
    li      a0, '\n'
    call    print_chr
    
    // Record end cycles
    rdcycle s11
    
    // Print cycle count
    la      a0, msg_cycles
    call    print_str
    sub     a0, s11, s10
    call    print_dec
    li      a0, '\n'
    call    print_chr
    
    // Return to caller (start.S will handle app_ret)
    ret

    .section .rodata
msg_start:
    .string "Starting A* PoC...\n"
msg_node1:
    .string "Node 1 g-score: "
msg_cycles:
    .string "Total cycles: "
