#makefile
#lib.asm - файл с print_hex и exit
ASM=nasm
ASMFLAGS=-f elf64
LD=ld
FILES = lib.o main.o

%.o: %.asm 
	$(ASM) $(ASMFLAGS) -o $@ $<
#main.asm - файл вызова

program: $(FILES)
	ld -o $@ $+
