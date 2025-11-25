PROJECT = cgrame
ASTAR_DOT_FILE = graph_loop.dot
FIX_PORT_ASTAR = astar_fixed_ports.txt
COL = 8
ROW = 8
MEMORY = 22
MAPPER = -m ILPHeuristicMapper
ARCH = 2
PE_CONN = 255
MAPPER_OPT = --mapper-opts "ILPHeuristicMapper.allow_recomputation=true"
VERILOG_PATH = .
HYBRID = on
VERILOG_OPT = --verilog-opts "hybrid=$(HYBRID) memsize=$(MEMORY)"
ASSEM_ASTAR = firmware/astar_firmware.S
COMPILER = /opt/riscv-sifive/bin/riscv64-unknown-elf
MODELSIM_DIR ?= /opt/questaFPGA/questa_fse
VLOG = $(MODELSIM_DIR)/bin/vlog
VSIM = $(MODELSIM_DIR)/bin/vsim
VLIB = $(MODELSIM_DIR)/bin/vlib

all: riscv bitstream hybrid hex simulation

# riscv:
# 	echo "Compile hardware of RISC-V processor"
# 	${VLIB} work
# 	VLOG=${VLOG} ./riscv_compile.sh
riscv:
	echo "Compile hardware of RISC-V processor"
	${VLIB} work
	export VLOG=${VLOG} && ./riscv_compile.sh

bitstream: 
	echo "Generate bitstream for $(COL)x$(ROW) CGRA with A* pathfinding"
	$(PROJECT) -c $(ARCH) -g $(ASTAR_DOT_FILE) $(MAPPER) $(MAPPER_OPT) --arch-opts "pe_conn=$(PE_CONN) cols=$(COL) rows=$(ROW)" --gen-testbench --fixed-ports $(FIX_PORT_ASTAR)

hybrid:
	$(PROJECT) -c $(ARCH) $(MAPPER) $(MAPPER_OPT) --arch-opts "pe_conn=$(PE_CONN) cols=$(COL) rows=$(ROW)" --gen-verilog $(VERILOG_PATH) $(VERILOG_OPT) 

hex:
	echo "Compile A* firmware for RISC-V processor"
	cp $(ASSEM_ASTAR) firmware/app.S 
	cp $(VERILOG_PATH)/hybrid.h firmware/
	$(COMPILER)-gcc -c -march=rv32imd -mabi=ilp32d -g -o firmware/app.o -DTEST_FUNC_NAME=app -DTEST_FUNC_TXT='"app"' -DTEST_FUNC_RET=app_ret firmware/app.S
	$(COMPILER)-gcc -c -march=rv32imd -mabi=ilp32d -g -o firmware/print.o firmware/print.c
	$(COMPILER)-gcc -c -march=rv32imd -mabi=ilp32d -g -o firmware/stats.o firmware/stats.c
	$(COMPILER)-gcc -c -march=rv32imd -mabi=ilp32d -g -o firmware/start.o firmware/start.S
	$(COMPILER)-gcc -g -Os -march=rv32imd -mabi=ilp32d -ffreestanding -nostdlib -o firmware/firmware.elf -Wl,-Bstatic,-T,firmware/link.ld,firmware/start.o firmware/print.o firmware/stats.o firmware/app.o -lgcc
	$(COMPILER)-objcopy -O verilog firmware/firmware.elf firmware/firmware.hex

simulation:
	echo "Start simulation"
	if [ ! -d "work" ]; then ${VLIB} work; fi
	# ${VLOG} $(VERILOG_PATH)/control.sv $(VERILOG_PATH)/cgrame.v testbench.v instruc_ram.sv dp_ram.sv riscv_tracer_defines.sv riscv_tracer.sv tb_top.sv
	# ${VLOG} riscv/riscv_defines.sv riscv/riscv_config.sv $(VERILOG_PATH)/control.sv $(VERILOG_PATH)/cgrame.v testbench.v instruc_ram.sv dp_ram.sv riscv_tracer_defines.sv riscv_tracer.sv tb_top.sv
	${VLOG} $(VERILOG_PATH)/control.sv $(VERILOG_PATH)/cgrame.v testbench.v instruc_ram.sv dp_ram.sv riscv_tracer_defines.sv riscv_tracer.sv tb_top.sv
	# ${VSIM} work.tb_top +firmware=firmware/firmware.hex
	${VSIM} -c -do "run -all; quit" work.tb_top +firmware=firmware/firmware.hex +verbose
	# do wave.do
	# run 1ms	 

clean: 
	rm -rf work hybrid.h firmware/*.o firmware/*.hex firmware/*.map $(VERILOG_PATH)/control.sv $(VERILOG_PATH)/cgrame.v transcript vsim.wlf trace_core_00*
