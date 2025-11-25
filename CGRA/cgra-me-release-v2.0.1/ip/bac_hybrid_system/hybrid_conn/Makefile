ISA = rv32im
ABI = ilp32
PROJECT = cgrame
COUNTER_DOT_FILE = ../cgra/bitstream/counter/fir4_1.dot
DATA_DOT_FILE = ../cgra/bitstream/data/fir4.dot
FIX_PORT_DATA = ../cgra/bitstream/data/fir4.txt
FIX_PORT_COUNTER = ../cgra/bitstream/counter/fir4.txt
COL = 4
ROW = 4
MEMORY = 22
MAPPER = -m ILPHeuristicMapper
ARCH = 2
PE_CONN = 255
MAPPER_OPT = --mapper-opts "ILPHeuristicMapper.allow_recomputation=true"
VERILOG_PATH = .
HYBRID=on
VERILOG_OPT = --verilog-opts "hybrid=$(HYBRID) memsize=$(MEMORY)"
ASSEM_DATA = firmware/data/fir4.S
ASSEM_COUNTER = firmware/counter/fir4.S
VSIM = vsim
VLOG = vlog
COMPILER = riscv32-unknown-elf

MODELSIM_DIR ?= /opt/intelFPGA/modelsim_ase/
VLOG ?= $(MODELSIM_DIR)/bin/vlog
VSIM ?= $(MODELSIM_DIR)/bin/vsim
VLIB ?= $(MODELSIM_DIR)/bin/vlib

all_counter: riscv bitstream_counter hybrid hex_counter simulation
all_data: riscv bitstream_data hybrid hex_data simulation
#compile the verilog files for riscv processor
riscv:
	echo "Compile hardware of RISC-V processor"
	${VLIB} work
	./riscv_compile.sh

#example of how to generate bitstream
bitstream_counter: 
	echo "Generate bitstream for $(COL)x$(ROW) $(ARCH) CGRA with application $(COUNTER_DOT_FILE)"
	$(PROJECT) -c $(ARCH) -g $(COUNTER_DOT_FILE) $(MAPPER) $(MAPPER_OPT) --arch-opts "pe_conn=$(PE_CONN) cols=$(COL) rows=$(ROW)" --gen-testbench --fixed-ports $(FIX_PORT_COUNTER)

bitstream_data:
	echo "Generate bitstream for $(COL)x$(ROW) $(ARCH) CGRA with application $(DATA_DOT_FILE)"
	$(PROJECT) -c $(ARCH) -g $(DATA_DOT_FILE) $(MAPPER) $(MAPPER_OPT) --arch-opts "pe_conn=$(PE_CONN) cols=$(COL) rows=$(ROW)" --gen-testbench --fixed-ports $(FIX_PORT_DATA)
#generate control file and hybrid header file for the system
hybrid:
	$(PROJECT) -c $(ARCH) $(MAPPER) $(MAPPER_OPT) --arch-opts "pe_conn=$(PE_CONN) cols=$(COL) rows=$(ROW)" --gen-verilog $(VERILOG_PATH) $(VERILOG_OPT) 

#generate hex file for risc-v processor
hex_data:
	echo "Compile software applications for RISC-V processor"
	cp $(ASSEM_DATA) firmware/app.S 
	cp $(VERILOG_PATH)/hybrid.h firmware/
	$(COMPILER)-gcc -c -march=rv32im -mabi=ilp32 -g -o firmware/app.o -DTEST_FUNC_NAME=app -DTEST_FUNC_TXT='"app"' -DTEST_FUNC_RET=app_ret firmware/app.S
	$(COMPILER)-gcc -c -march=rv32im -mabi=ilp32 -g -o firmware/print.o firmware/print.c
	$(COMPILER)-gcc -c -march=rv32im -mabi=ilp32 -g -o firmware/stats.o firmware/stats.c
	$(COMPILER)-gcc -c -march=rv32im -mabi=ilp32 -g -o firmware/start.o firmware/start.S
	$(COMPILER)-gcc -g -Os -march=rv32im -mabi=ilp32 -ffreestanding -nostdlib -o firmware/firmware.elf -Wl,-Bstatic,-T,firmware/link.ld,firmware/start.o firmware/print.o firmware/stats.o firmware/app.o -lgcc
	$(COMPILER)-objcopy -O verilog firmware/firmware.elf firmware/firmware.hex

#generate hex file for risc-v processor
hex_counter:
	echo "Compile software applications for RISC-V processor"
	cp $(ASSEM_COUNTER) firmware/app.S 
	cp $(VERILOG_PATH)/hybrid.h firmware/
	$(COMPILER)-gcc -c -march=rv32im -mabi=ilp32 -g -o firmware/app.o -DTEST_FUNC_NAME=app -DTEST_FUNC_TXT='"app"' -DTEST_FUNC_RET=app_ret firmware/app.S
	$(COMPILER)-gcc -c -march=rv32im -mabi=ilp32 -g -o firmware/print.o firmware/print.c
	$(COMPILER)-gcc -c -march=rv32im -mabi=ilp32 -g -o firmware/stats.o firmware/stats.c
	$(COMPILER)-gcc -c -march=rv32im -mabi=ilp32 -g -o firmware/start.o firmware/start.S
	$(COMPILER)-gcc -g -Os -march=rv32im -mabi=ilp32 -ffreestanding -nostdlib -o firmware/firmware.elf -Wl,-Bstatic,-T,firmware/link.ld,firmware/start.o firmware/print.o firmware/stats.o firmware/app.o -lgcc
	$(COMPILER)-objcopy -O verilog firmware/firmware.elf firmware/firmware.hex

#start simulation for the hybrid system	
simulation:
	echo "Start simlation"
	if [ ! -d "work" ]; then vlib work; fi
	vlog $(VERILOG_PATH)/control.sv $(VERILOG_PATH)/cgrame.v testbench.v instruc_ram.sv dp_ram.sv tb_top.sv
	vsim work.tb_top +firmware=firmware/firmware.hex
	do wave.do
	run -all	 

clean: 
	rm -rf work hybrid.h firmware/*.o firmware/*.hex firmware/*.map $(VERILOG_PATH)/control.sv $(VERILOG_PATH)/cgrame.v transcript vsim.wlf trace_core_00* 

# A* targets
COUNTER_DOT_ASTAR = ./cgra/bitstream/astar/neighbor_expand.dot
FIX_PORT_ASTAR = ./cgra/bitstream/astar/neighbor_expand.txt
ASSEM_ASTAR = firmware/astar/neighbor_expand.S

all_astar: bitstream_astar hex_astar simulation

bitstream_astar:
	$(PROJECT) -c $(ARCH) -g $(COUNTER_DOT_ASTAR) $(MAPPER) $(MAPPER_OPT) --arch-opts "pe_conn=$(PE_CONN) cols=$(COL) rows=$(ROW)" --gen-testbench --fixed-ports $(FIX_PORT_ASTAR)
	$(PROJECT) -c $(ARCH) $(MAPPER) $(MAPPER_OPT) --arch-opts "pe_conn=$(PE_CONN) cols=$(COL) rows=$(ROW)" --gen-verilog . --verilog-opts "hybrid=on memsize=$(MEMSIZE)"

hex_astar:
	@echo "Compile A* application for RISC-V processor"
	cp $(ASSEM_ASTAR) firmware/app.S 
	cp ./hybrid.h firmware/
	$(COMPILER)-gcc -c -march=$(ISA) -mabi=$(ABI) -g -o firmware/app.o -DTEST_FUNC_NAME=app -DTEST_FUNC_TXT='"app"' -DTEST_FUNC_RET=app_ret firmware/app.S
	$(COMPILER)-gcc -c -march=$(ISA) -mabi=$(ABI) -g -o firmware/print.o firmware/print.c
	$(COMPILER)-gcc -c -march=$(ISA) -mabi=$(ABI) -g -o firmware/stats.o firmware/stats.c
	$(COMPILER)-gcc -c -march=$(ISA) -mabi=$(ABI) -g -o firmware/start.o firmware/start.S
	$(COMPILER)-gcc -g -Os -march=$(ISA) -mabi=$(ABI) -ffreestanding -nostdlib -o firmware/firmware.elf -Wl,-Bstatic,-T,firmware/link.ld,firmware/start.o firmware/print.o firmware/stats.o firmware/app.o -lgcc
	$(COMPILER)-objcopy -O verilog firmware/firmware.elf firmware/firmware.hex

# === A* Proof of Concept ===
ASTAR_DOT = ./cgra/bitstream/astar/neighbor_expand.dot
ASTAR_PORT = ./cgra/bitstream/astar/neighbor_expand.txt
ASTAR_ASSEM = firmware/astar/simple_astar.S

all_astar: riscv_compile bitstream_astar hex_astar simulation

bitstream_astar:
	@echo "Generating CGRA bitstream for A* neighbor expansion"
	$(PROJECT) -c $(ARCH) -g $(ASTAR_DOT) $(MAPPER) $(MAPPER_OPT) --arch-opts "pe_conn=$(PE_CONN) cols=$(COL) rows=$(ROW)" --gen-testbench 
	$(PROJECT) -c $(ARCH) $(MAPPER) $(MAPPER_OPT) --arch-opts "pe_conn=$(PE_CONN) cols=$(COL) rows=$(ROW)" --gen-verilog . --verilog-opts "hybrid=on memsize=$(MEMSIZE)"

hex_astar:
	@echo "Compiling A* firmware for RISC-V"
	cp $(ASTAR_ASSEM) firmware/app.S
	cp ./hybrid.h firmware/
	$(COMPILER)-gcc -c -march=$(ISA) -mabi=$(ABI) -g -o firmware/app.o -DTEST_FUNC_NAME=app -DTEST_FUNC_TXT='"app"' -DTEST_FUNC_RET=app_ret firmware/app.S
	$(COMPILER)-gcc -c -march=$(ISA) -mabi=$(ABI) -g -o firmware/print.o firmware/print.c
	$(COMPILER)-gcc -c -march=$(ISA) -mabi=$(ABI) -g -o firmware/stats.o firmware/stats.c
	$(COMPILER)-gcc -c -march=$(ISA) -mabi=$(ABI) -g -o firmware/start.o firmware/start.S
	$(COMPILER)-gcc -g -Os -march=$(ISA) -mabi=$(ABI) -ffreestanding -nostdlib -o firmware/firmware.elf -Wl,-Bstatic,-T,firmware/link.ld,firmware/start.o firmware/print.o firmware/stats.o firmware/app.o -lgcc
	$(COMPILER)-objcopy -O verilog firmware/firmware.elf firmware/firmware.hex
	@echo "A* firmware compiled successfully!"
