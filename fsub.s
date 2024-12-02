.data
ARRAY_A_ADDR:   .word 0x0400      # Base address for Array A
ARRAY_B_ADDR:   .word 0x0800      # Base address for Array B
ARRAY_D_ADDR:   .word 0x1000      # Base address for Array D (where results are stored)

.text
.globl main

main:
    la a0, ARRAY_A_ADDR    # Load base address of ARRAY_A
    la a1, ARRAY_B_ADDR    # Load base address of ARRAY_B
    la a2, ARRAY_D_ADDR    # Load base address of ARRAY_D
    li t0, 0               # Initialize loop index
    li t1, 1024            # Set loop limit (256 elements * 4 bytes per element)

loop_start:
    flw f0, 0(a0)          # Load floating-point number from ARRAY_A + t0
    flw f1, 0(a1)          # Load floating-point number from ARRAY_B + t0
    fsub.s f2, f0, f1      # Perform floating-point subtraction
    fsw f2, 0(a2)          # Store result into ARRAY_D + t0

    addi t0, t0, 4         # Increment index by size of float (4 bytes)
    blt t0, t1, loop_start # Loop if t0 < 1024

    # Exit the program
    li a7, 93              # Service number for exit in RISC-V
    li a0, 0               # Status code
    ecall                  # Make system call to terminate the program

