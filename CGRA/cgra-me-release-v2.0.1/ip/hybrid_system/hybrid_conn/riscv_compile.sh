#!/bin/bash
${VLOG} ../riscv/fpnew_pkg.sv
${VLOG} ../riscv/apu_core_package.sv
${VLOG} ../riscv/apu_macros.sv
${VLOG} ../riscv/riscv_config.sv
${VLOG} ../riscv/riscv_defines.sv
${VLOG} ../riscv/cv32e40p_sim_clock_gating.sv -suppress 2583
${VLOG} ../riscv/riscv_prefetch_buffer.sv -suppress 2583
${VLOG} ../riscv/riscv_prefetch_L0_buffer.sv -suppress 2583
${VLOG} ../riscv/riscv_L0_buffer.sv -suppress 2583
${VLOG} ../riscv/riscv_fetch_fifo.sv -suppress 2583
${VLOG} ../riscv/riscv_if_stage.sv -suppress 2583
${VLOG} ../riscv/register_file_test_wrap.sv -suppress 2583
${VLOG} ../riscv/riscv_register_file.sv -suppress 2583
${VLOG} ../riscv/riscv_decoder.sv -suppress 2583
${VLOG} ../riscv/riscv_compressed_decoder.sv -suppress 2583
${VLOG} ../riscv/riscv_id_stage.sv -suppress 2583
${VLOG} ../riscv/amo_shim.sv -suppress 2583
${VLOG} ../riscv/riscv_hwloop_regs.sv -suppress 2583
${VLOG} ../riscv/riscv_cs_registers.sv  -suppress 2583
${VLOG} ../riscv/riscv_pmp.sv -suppress 2583
${VLOG} ../riscv/riscv_controller.sv -suppress 2583
${VLOG} ../riscv/riscv_int_controller.sv  -suppress 2583
${VLOG} ../riscv/riscv_hwloop_controller.sv -suppress 2583
${VLOG} ../riscv/riscv_alu_basic.sv -suppress 2583
${VLOG} ../riscv/riscv_mult.sv -suppress 2583
${VLOG} ../riscv/riscv_alu_div.sv  -suppress 2583
${VLOG} ../riscv/riscv_alu.sv -suppress 2583
${VLOG} ../riscv/riscv_ex_stage.sv -suppress 2583
${VLOG} ../riscv/riscv_apu_disp.sv -suppress 2583
${VLOG} ../riscv/riscv_load_store_unit.sv -suppress 2583
${VLOG} ../riscv/riscv_core.sv -suppress 2583
