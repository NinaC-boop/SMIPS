/* Nina Chen z5209365 SMIPS.C Assignment for COMP1521 28/07/2020
A program which converts smips code, encoded into 32 binary, into c.*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define STRLEN 10

#define INS_OP 0
#define INS_MUL 1
#define INS_BEQ 2
#define INS_BNE 3
#define INS_ADDI 4
#define INS_SLTI 5
#define INS_ANDI 6
#define INS_ORI 7
#define INS_LUI 8
#define NUM_INS 9

#define SYSCALL 9

#define OP_ADD 0
#define OP_SUB 1
#define OP_AND 2
#define OP_OR 3
#define OP_SLT 4
#define OP_MUL 5
#define NUM_OP 6

#define BITS_TOT 32
#define BITS_INS 6
#define BITS_S 5
#define BITS_T 5
#define BITS_D 5
#define BITS_OP 11
#define BITS_IMM 16

#define NUM_REG 32
#define NUM_INPUT 1000

#define S 0
#define T 1
#define D 2
#define IMM 3

#define PRINT_INTEGER 1
#define EXIT 10
#define PRINT_CHARACTER 11

#define CHAR_MASK 0b00000000000000000000000011111111

void print_unknown_syscall_err(int *reg);
void print_invalid_ins_err(char *file_name, int line_num, unsigned int hex);
void print_reg(int *reg);

void perform_add (int *reg, short int *stdi);
void perform_sub (int *reg, short int *stdi);
void perform_and (int *reg, short int *stdi);
void perform_or (int *reg, short int *stdi);
void perform_slt (int *reg, short int *stdi);
void perform_mul (int *reg, short int *stdi);
int perform_beq (int *reg, short int *stdi);
int perform_bne (int *reg, short int *stdi);
void perform_addi (int *reg, short int *stdi);
void perform_slti (int *reg, short int *stdi);
void perform_andi (int *reg, short int *stdi);
void perform_ori (int *reg, short int *stdi);
void perform_lui (int *reg, short int *stdi);
void perform_syscall (int *reg);

unsigned int atoh (char *str);
unsigned int chtoh (char ch);

int main(int argc, char *argv[]) {
    // Execution Setup
    // instruction identifiers
    unsigned int ins_op   = 0b000000;
    unsigned int ins_mul  = 0b011100;
    unsigned int ins_beq  = 0b000100;
    unsigned int ins_bne  = 0b000101;
    unsigned int ins_addi = 0b001000;
    unsigned int ins_slti = 0b001010;
    unsigned int ins_andi = 0b001100;
    unsigned int ins_ori  = 0b001101;
    unsigned int ins_lui  = 0b001111;
    unsigned int s_lui    = 0;

    unsigned int ins[NUM_INS] = {ins_op, ins_mul, ins_beq, ins_bne, 
    ins_addi, ins_slti, ins_andi, ins_ori, ins_lui};

    char *str_ins[NUM_INS] = {"op  ", "mul ", "beq ", "bne ", 
    "addi", "slti", "andi", "ori ", "lui "};

    // operation identifiers
    unsigned int op_add = 0b00000100000;
    unsigned int op_sub = 0b00000100010;
    unsigned int op_and = 0b00000100100;
    unsigned int op_or  = 0b00000100101;
    unsigned int op_slt = 0b00000101010;
    unsigned int op_mul = 0b00000000010;

    unsigned int op[NUM_OP] = {op_add, op_sub, op_and, op_or, op_slt, op_mul};

    char *str_op[NUM_OP] = {"add ", "sub ", "and ", "or  ", "slt ", "mul "};

    // calculation masks
    unsigned int syscall   = 0b00000000000000000000000000001100;
    unsigned int s_mask    = 0b00000011111000000000000000000000;
    unsigned int t_mask    = 0b00000000000111110000000000000000;
    unsigned int d_mask    = 0b00000000000000001111100000000000;
    unsigned int op_mask   = 0b00000000000000000000011111111111;
    unsigned int imm_mask  = 0b00000000000000001111111111111111;

    // execution parameters
    // there are 1000 instructions per execution and 32 total registers
    int method_ins[NUM_INPUT];
    int method_op[NUM_INPUT];
    short int method_stdi[NUM_INPUT][4];
    int reg[NUM_REG];

    // initialise instructions and register arrays
    int i = 0;
    while (i < NUM_INPUT) {
        method_ins[i] = -1;
        i++;
    }

    i = 0;
    while (i < NUM_REG) {
        reg[i] = 0;
        i++;
    }


    // Store Start
    i = 0;
    int j, k;
    unsigned int hex, hex_temp, s, t, d;
    short int imm;
    int line_num = 1;
    char str[STRLEN];

    // read a file
    assert(argc == 2);
    FILE *file = fopen(argv[1], "r");

    while (fgets(str, STRLEN, file) != NULL) {
        // converts ascii to hex
        hex = atoh(str);

        // call syscall if possible, else find all other instructions
        if (hex == syscall) {
            // store instruction
            method_ins[i] = SYSCALL;
        } else {
            // find s, t, d, i
            s = ((hex & s_mask) >> (32 - 6 - 5));
            t = ((hex & t_mask) >> (32 - 6 - 5 - 5));
            d = ((hex & d_mask) >> (32 - 6 - 5 - 5 - 5));
            imm = hex & imm_mask;
            
            // instruction bits
            j = 0;
            hex_temp = hex;
            hex_temp >>= (32 - 6);

            // find instruction
            while (j < NUM_INS) {
                if (hex_temp == ins[j]) {
                    break;
                }

                j++;
            }

            // store instruction
            method_ins[i] = j;

            // sorts by operations/non-operations respectively
            if (j == INS_OP || j == INS_MUL) {
                k = 0;
                hex_temp = hex & op_mask;

                // find operation
                while (k < NUM_OP) {
                    if (hex_temp == op[k]) {
                        break;
                    }

                    k++;
                }

                if (k == NUM_OP) {
                    print_invalid_ins_err(argv[1], line_num, hex);
                }

                // store operation
                method_op[i] = k;
            } else {
                if (j == INS_BEQ || j == INS_BNE) {
                    // valid BEQ or BNE instructions
                } else if ((j == INS_LUI) && ((s_mask & s_lui) == 0)) {
                    // valid LUI instructions
                } else if (j == INS_BEQ || j == INS_BNE || j == INS_ADDI || 
                            j == INS_SLTI || j == INS_ANDI || j == INS_ORI) {
                    // valid all other instructions 
                } else {
                    print_invalid_ins_err(argv[1], line_num, hex);
                }
            }


            method_stdi[i][S] = s;
            method_stdi[i][T] = t;
            method_stdi[i][D] = d;
            method_stdi[i][IMM] = imm;
        }
        line_num++;
        i++;
    }

    // Program Start
    printf("Program\n");
    i = 0;
    while (i < 1000) {
        if (method_ins[i] == -1) {
            break;
        }
        printf("%3d: ", i);

        if (method_ins[i] == SYSCALL) {
            printf("syscall\n");
        } else {
            // reads and prints instructions, 
            // sorting by operations/non-operations respectively
            if (method_ins[i]  == INS_OP || method_ins[i]  == INS_MUL) {
                // print operation
                printf("%s $%d, $%d, $%d\n", str_op[method_op[i]], 
                method_stdi[i][D], method_stdi[i][S], method_stdi[i][T]); 
            } else {
                if (method_ins[i]  == INS_BEQ || method_ins[i]  == INS_BNE) {
                    // print BEQ or BNE instructions
                    printf("%s $%d, $%d, %d\n", str_ins[method_ins[i]], 
                    method_stdi[i][S], method_stdi[i][T], method_stdi[i][IMM]); 
                } else if (method_ins[i] == INS_LUI) {
                    // print LUI instructions
                    printf("%s $%d, %d\n", str_ins[method_ins[i]], 
                    method_stdi[i][T], method_stdi[i][IMM]);  
                } else {
                    // print all other instructions
                    printf("%s $%d, $%d, %d\n", str_ins[method_ins[i]], 
                    method_stdi[i][T], method_stdi[i][S], method_stdi[i][IMM]);   
                }
            }
        }
        i++;
    }

    // Output Start
    printf("Output\n");
    
    i = 0;
    while (i < 1000) { 
        if (i < 0) {
            // invalid execution
            break;
        }

        if (method_ins[i] == -1) {
            // end of method
            break;
        }

        switch (method_ins[i]) {
            // matches instruction to operation
            case INS_OP:
                // make sure not storing in $0
                if (method_stdi[i][D] != 0) {
                    switch (method_op[i]) {
                        case OP_ADD:
                            perform_add(reg, method_stdi[i]);
                            break;
                        case OP_SUB:
                            perform_sub(reg, method_stdi[i]);
                            break;
                        case OP_AND:
                            perform_and(reg, method_stdi[i]);
                            break;
                        case OP_OR:
                            perform_or(reg, method_stdi[i]);
                            break;
                        case OP_SLT:
                            perform_slt(reg, method_stdi[i]);
                            break;
                        case OP_MUL:
                            perform_mul(reg, method_stdi[i]);
                            break;
                    }                    
                }
                break;
            case INS_MUL:
                if (method_stdi[i][D] != 0) {
                    perform_mul(reg, method_stdi[i]);
                }
                break;
            case INS_BEQ:
                i += perform_beq(reg, method_stdi[i]);
                break;
            case INS_BNE:
                i += perform_bne(reg, method_stdi[i]);
                break;
            case INS_ADDI:
                if (method_stdi[i][T] != 0) {
                    perform_addi(reg, method_stdi[i]);
                }
                break;
            case INS_SLTI:
                if (method_stdi[i][T] != 0) {
                    perform_slti(reg, method_stdi[i]);
                }
                break;
            case INS_ANDI:
                if (method_stdi[i][T] != 0) {
                    perform_andi(reg, method_stdi[i]);
                }
                break;
            case INS_ORI:
                if (method_stdi[i][T] != 0) {
                   perform_ori(reg, method_stdi[i]);
                }
                break;
            case INS_LUI:
                if (method_stdi[i][T] != 0) {
                   perform_lui(reg, method_stdi[i]);
                }
                break;
            case SYSCALL:
                perform_syscall(reg);
                break;
        }
        i++;
    }

    print_reg(reg);

    fclose(file);

    return 0;
}

// print unknown system calls
void print_unknown_syscall_err(int *reg) {
    printf("Unknown system call: %d\n", reg[2]);
    print_reg(reg);
    exit(0);
}

// print an error message with file name, 
// line number and invalid instructions and exits
void print_invalid_ins_err(char *file_name, int line_num, unsigned int hex) {
    printf("%s:%d: invalid instruction code: %08X\n", 
           file_name, line_num, hex);
    exit(0);
}

// prints registers after execution
void print_reg(int *reg) {
    printf("Registers After Execution\n");

    int i = 0;
    while (i < NUM_REG) {
        if (reg[i] != 0) {
            printf("$%-2d = %d\n", i, reg[i]);
        }
        i++;
    }
}

// Operations
// add $d, $s, $t	d = s + t
void perform_add (int *reg, short int *stdi) {
    reg[stdi[D]] = reg[stdi[S]] + reg[stdi[T]];
}

// sub $d, $s, $t	d = s - t
void perform_sub (int *reg, short int *stdi) {
    reg[stdi[D]] = reg[stdi[S]] - reg[stdi[T]];
}

// and $d, $s, $t	d = s & t
void perform_and (int *reg, short int *stdi) {
    reg[stdi[D]] = reg[stdi[S]] & reg[stdi[T]];
}

// or $d, $s, $t	d = s | t
void perform_or (int *reg, short int *stdi) {
    reg[stdi[D]] = reg[stdi[S]] | reg[stdi[T]];
}

// slt $d, $s, $t	d = 1 if s < t else 0
void perform_slt (int *reg, short int *stdi) {
    if ((reg[stdi[S]] < reg[stdi[T]]) == 1) {
        reg[stdi[D]] = 1;
    } else {
        reg[stdi[D]] = 0;
    }
}

// mul $d, $s, $t	d = s * t
void perform_mul (int *reg, short int *stdi) {
    reg[stdi[D]] = reg[stdi[S]] * reg[stdi[T]];
}

// Instructions
// beq $s, $t, I	if (s == t) PC += I
int perform_beq (int *reg, short int *stdi) {
    if (reg[stdi[S]] == reg[stdi[T]]) {
        return stdi[IMM] - 1;
    }
    return 0;
}

// bne $s, $t, I	if (s != t) PC += I
int perform_bne (int *reg, short int *stdi) {
    if (reg[stdi[S]] != reg[stdi[T]]) {
        return stdi[IMM] - 1;
    }
    return 0;
}

// addi $t, $s, I	t = s + I
void perform_addi (int *reg, short int *stdi) {
    reg[stdi[T]] = reg[stdi[S]] + stdi[IMM];
}

// slti $t, $s, I	t = (s < I)
void perform_slti (int *reg, short int *stdi) {
    reg[stdi[T]] = (reg[stdi[S]] < stdi[IMM]);
}

// andi $t, $s, I	t = s & I
void perform_andi (int *reg, short int *stdi) {
    reg[stdi[T]] = reg[stdi[S]] & stdi[IMM];
}

// ori $t, $s, I	t = s | I
void perform_ori (int *reg, short int *stdi) {
    reg[stdi[T]] = reg[stdi[S]] | stdi[IMM];

    if ((stdi[T] == 2) && (reg[stdi[T]] != PRINT_INTEGER) &&
    (reg[stdi[T]] != EXIT) && (reg[stdi[T]] != PRINT_CHARACTER)) {
        print_unknown_syscall_err(reg);
    }
}

// lui $t, I	t = I << 16
void perform_lui (int *reg, short int *stdi) {
    unsigned int temp = stdi[IMM];
    temp <<= 16;
    if (stdi[IMM] < 0) {
        reg[stdi[T]] = -temp;
    } else {
        reg[stdi[T]] = temp;
    }
    
}

// syscall	syscall
void perform_syscall (int *reg) {
    switch (reg[2]) {
        case PRINT_CHARACTER:
            printf("%c", reg[4] & CHAR_MASK);
            break;
        case PRINT_INTEGER:
            printf("%d", reg[4]);
            break;
        case EXIT:
            print_reg(reg);
            exit(0);
            break;
        default:
            print_unknown_syscall_err(reg);
    }
}

// changes ascii to hex
unsigned int atoh (char *str) {
    char ch;
    unsigned int hex = 0;
    unsigned int ch_hex = 0;
    unsigned int shift = 0;
    

    int i = strlen(str) - 2;

    // adds each 4 bits in a loop
    while (i >= 0) {
        ch = str[i];
        ch_hex = chtoh(ch);
        ch_hex <<= shift;
        shift += 4;
        hex |= ch_hex;
        i--;
    }

    return hex;
}

// changes char to hex
unsigned int chtoh (char ch) {
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }

    if (ch >= 'a' && ch <= 'z') {
        return ch - 'a' + 10;
    } else {
        return ch - 'A' + 10;
    }
}

