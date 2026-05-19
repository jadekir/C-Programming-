#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMSIZE (65536 >> 2)
#define REGSIZE 32
#define BUFSIZE 256

#define PCINIT 0x4000
#define SPINIT 0xFFFC
#define GPINIT 0xC000

static unsigned Mem[MEMSIZE];
static unsigned Reg[REGSIZE + 4];

#define MEM(addr) (Mem[addr >> 2])

#define PC (Reg[REGSIZE + 0])
#define Status (Reg[REGSIZE + 1])
#define LO (Reg[REGSIZE + 2])
#define HI (Reg[REGSIZE + 3])

const char RegName[REGSIZE + 4][6] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra",
    "$pc", "$stat", "$lo", "$hi" };

#define NREG(name) (*Nreg(name))

const char RedirNull[] = "";
const char RedirPrefix[] = ">";

static char Buf[BUFSIZE];
static int Halt = 0;
static FILE *FP;
static char *Redir = (char *) RedirNull;

// DATAPATH Signals
unsigned instruction;
unsigned op, r1, r2, r3, funct, offset, jsec;
struct_controls controls;
unsigned data1, data2;
unsigned extended_value;
unsigned ALUresult;
char Zero;

unsigned *Nreg(char *name)
{
    int i;

    for (i = 0; i < REGSIZE + 4; i++)
    {
        if (strcmp(name, RegName[i]) == 0)
            return &Reg[i];
        if (strcmp(name, RegName[i] + 1) == 0)
            return &Reg[i];
    }
    return NULL;
}

void Init(void)
{
    memset(Reg, 0, (REGSIZE + 4) * sizeof(unsigned));
    NREG("pc") = PCINIT;
    NREG("sp") = SPINIT;
    NREG("gp") = GPINIT;
}

void DisplayControlSignals(void)
{
    fprintf(stdout, "\tControl Signals: %0x %0x %0x %0x %0x %03x %0x %0x %0x\n",
            controls.RegDst,
            controls.Jump,
            controls.Branch,
            controls.MemRead,
            controls.MemtoReg,
            controls.ALUOp,
            controls.MemWrite,
            controls.ALUSrc,
            controls.RegWrite);
}

int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
    if (PC < 0 || PC >= (MEMSIZE << 2))
    {
        Halt = 1;
        return 1; // Halt condition occurred
    }

    *instruction = Mem[PC >> 2];

    if (*instruction == 0x00000000)
    {
        Halt = 1;
        return 1; // Halt condition occurred
    }

    return 0; // No halt condition
}

void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    *op = (instruction & 0xFC000000) >> 26;
    *r1 = (instruction & 0x03E00000) >> 21;
    *r2 = (instruction & 0x001F0000) >> 16;
    *r3 = (instruction & 0x0000F800) >> 11;
    *funct = instruction & 0x0000003F;
    *offset = instruction & 0x0000FFFF;
    *jsec = instruction & 0x03FFFFFF;
}

int instruction_decode(unsigned op, struct_controls *controls)
{
    switch (op)
    {
        case 0x00: // R-type instruction
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0b110; // ALU will do the operation based on funct field
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;
        case 0x02: // J-type instruction (Jump)
            controls->RegDst = 0; // Not used for Jump instruction
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0; // Not used for Jump instruction
            controls->ALUOp = 0b000; // ALU will not perform any operation
            controls->MemWrite = 0;
            controls->ALUSrc = 0; // Not used for Jump instruction
            controls->RegWrite = 0; // Not used for Jump instruction
            break;
        case 0x04: // I-type instruction (Branch Equal)
            controls->RegDst = 0; // Not used for Branch instruction
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 0; // Not used for Branch instruction
            controls->ALUOp = 0b010; // ALU will do the "set less than" operation
            controls->MemWrite = 0;
            controls->ALUSrc = 0; // ALU will use data1 from register file
            controls->RegWrite = 0; // Not used for Branch instruction
            break;
        case 0x23: // I-type instruction (Load Word)
            controls->RegDst = 0; // Not used for Load instruction
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0b000; // ALU will do addition
            controls->MemWrite = 0;
            controls->ALUSrc = 1; // ALU will use extended value from sign-extend
            controls->RegWrite = 1;
            break;
        case 0x2B: // I-type instruction (Store Word)
            controls->RegDst = 0; // Not used for Store instruction
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0; // Not used for Store instruction
            controls->ALUOp = 0b000; // ALU will do addition
            controls->MemWrite = 1;
            controls->ALUSrc = 1; // ALU will use extended value from sign-extend
            controls->RegWrite = 0; // Not used for Store instruction
            break;
        default:
            Halt = 1;
            return 1; // Halt condition occurred for unknown opcode
    }

    return 0; // No halt condition
}

void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

void sign_extend(unsigned offset, unsigned *extended_value)
{
    *extended_value = (offset & 0x00008000) ? (offset | 0xFFFF0000) : offset;
}

int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, unsigned ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
    char ALUControl;
    if (ALUSrc)
        ALUControl = ALUOp;
    else
        ALUControl = funct;

    ALU(data1, (ALUSrc) ? extended_value : data2, ALUControl, ALUresult, Zero);

    return 0; // No halt condition
}

void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
    unsigned dest_reg = (RegDst) ? r3 : r2;
    unsigned write_value = (MemtoReg) ? memdata : ALUresult;

    if (RegWrite && dest_reg != 0)
        Reg[dest_reg] = write_value;
}

int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem)
{
    if (MemWrite)
        MEM(ALUresult) = data2;
    if (MemRead)
        *memdata = MEM(ALUresult);

    return 0; // No halt condition
}

void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
    *PC += 4;
    if (Branch && Zero)
        *PC += extended_value << 2;
    else if (Jump)
        *PC = (jsec << 2) | (*PC & 0xF0000000);
}

void print_registers(unsigned *Reg)
{
    int i;
    for (i = 0; i < REGSIZE; i++)
    {
        if (i % 4 == 0)
            printf("\n");
        printf("$%-3d = %10d\t", i, Reg[i]);
    }
}

void print_memory(unsigned *Mem, unsigned start, unsigned end)
{
    unsigned i;
    for (i = start; i <= end; i += 4)
    {
        if (i % 16 == 0)
            printf("\n%08X: ", i);
        printf("%08X ", Mem[i >> 2]);
    }
    printf("\n");
}

void DumpRegisters(void)
{
    printf("\nRegisters content:\n");
    print_registers(Reg);
}

void DumpMemory(void)
{
    printf("\nMemory content:\n");
    print_memory(Mem, 0, MEMSIZE << 2);
}

void Step(void)
{
    int halted;
    unsigned pc, instruction;
    unsigned op, r1, r2, r3, funct, offset, jsec;
    unsigned data1, data2, extended_value;
    unsigned ALUresult, memdata;
    char Zero;

    halted = instruction_fetch(PC, Mem, &instruction);
    if (halted)
    {
        printf("Halt condition occurred.\n");
        return;
    }

    instruction_partition(instruction, &op, &r1, &r2, &r3, &funct, &offset, &jsec);

    halted = instruction_decode(op, &controls);
    if (halted)
    {
        printf("Halt condition occurred.\n");
        return;
    }

    read_register(r1, r2, Reg, &data1, &data2);
    sign_extend(offset, &extended_value);

    ALU_operations(data1, data2, extended_value, funct, controls.ALUOp, controls.ALUSrc, &ALUresult, &Zero);
    rw_memory(ALUresult, data2, controls.MemWrite, controls.MemRead, &memdata, Mem);
    write_register(r2, r3, memdata, ALUresult, controls.RegWrite, controls.RegDst, controls.MemtoReg, Reg);
    PC_update(jsec, extended_value, controls.Branch, controls.Jump, Zero, &PC);

    printf("\nInstruction: %08X\n", instruction);
    printf("PC: %08X\n", PC);
    printf("ALUresult: %08X\n", ALUresult);
    printf("Memory: %08X\n", memdata);
    DisplayControlSignals();
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input_file.asc>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    FP = fopen(filename, "r");

    if (FP == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", filename);
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int i = 0;
    while ((read = getline(&line, &len, FP)) != -1)
    {
        unsigned instr;
        sscanf(line, "%X", &instr);
        Mem[i++] = instr;
    }

    fclose(FP);
    if (line)
        free(line);

    printf("\nSimulation begins.\n");
    Init();
    DumpRegisters();
    DumpMemory();

    while (!Halt)
    {
        printf("\nSimulating instruction at PC = %08X\n", PC);
        Step();
        DumpRegisters();
        DumpMemory();
    }

    printf("\nSimulation ended.\n");

    return 0;
}
