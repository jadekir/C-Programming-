// I have not used C language code obtained from other students, the Internet, or any other unauthorized sources, either modified or unmodified. If any code in my program was obtained from an authorized source, such as a textbook or course notes, that has been clearly noted as a citation in the comments of the program.
// Jade Kirkpatric
// ja847451@ucf.edu

#include "spimcore.h"

/* ALU function */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
    // Perform ALU operation based on ALUControl
    switch (ALUControl)
    {
        case 0:
            *ALUresult = A + B; // Addition
            break;
        case 1:
            *ALUresult = A - B; // Subtraction
            break;
        case 2:
            *ALUresult = (int)A < (int)B ? 1 : 0; // Less than (signed)
            break;
        case 3:
            *ALUresult = A < B ? 1 : 0; // Less than (unsigned)
            break;
        case 4:
            *ALUresult = A & B; // Bitwise AND
            break;
        case 5:
            *ALUresult = A | B; // Bitwise OR
            break;
        case 6:
            *ALUresult = B << 16; // Shift left by 16 bits
            break;
        case 7:
            *ALUresult = ~A; // Bitwise NOT
            break;
    }

    // Set Zero flag based on ALU result
    *Zero = (*ALUresult == 0) ? 1 : 0;
}

/* Instruction Fetch */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
    *instruction = Mem[PC >> 2];
    return 0;
}

/* Instruction Partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    *op = (instruction >> 26) & 0x3F;
    *r1 = (instruction >> 21) & 0x1F;
    *r2 = (instruction >> 16) & 0x1F;
    *r3 = (instruction >> 11) & 0x1F;
    *funct = instruction & 0x3F;
    *offset = instruction & 0xFFFF;
    *jsec = instruction & 0x3FFFFFF;
}

/* Instruction Decode */
int instruction_decode(unsigned op, struct_controls *controls)
{
    switch (op)
    {
        case 0:
            controls->RegDst = 1;
            controls->ALUSrc = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->Branch = 0;
            controls->ALUOp = 7;
            controls->Jump = 0;
            break;
        case 8:
            controls->RegDst = 0;
            controls->ALUSrc = 1;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            controls->Jump = 0;
            break;
        case 15:
            controls->RegDst = 0;
            controls->ALUSrc = 1;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->Branch = 0;
            controls->ALUOp = 6;
            controls->Jump = 0;
            break;
        case 35:
            controls->RegDst = 0;
            controls->ALUSrc = 1;
            controls->MemtoReg = 1;
            controls->RegWrite = 1;
            controls->MemRead = 1;
            controls->MemWrite = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            controls->Jump = 0;
            break;
        case 43:
            controls->RegDst = 2;
            controls->ALUSrc = 1;
            controls->MemtoReg = 2;
            controls->RegWrite = 0;
            controls->MemRead = 0;
            controls->MemWrite = 1;
            controls->Branch = 0;
            controls->ALUOp = 0;
            controls->Jump = 0;
            break;
        case 4:
            controls->RegDst = 2;
            controls->ALUSrc = 0;
            controls->MemtoReg = 2;
            controls->RegWrite = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->Branch = 1;
            controls->ALUOp = 1;
            controls->Jump = 0;
            break;
        case 10:
            controls->RegDst = 1;
            controls->ALUSrc = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->Branch = 0;
            controls->ALUOp = 2;
            controls->Jump = 0;
            break;
        case 11:
            controls->RegDst = 1;
            controls->ALUSrc = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->Branch = 0;
            controls->ALUOp = 3;
            controls->Jump = 0;
            break;
        case 2:
            controls->RegDst = 0;
            controls->ALUSrc = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            controls->Jump = 1;
            break;
        default:
            return 1;
    }
    return 0;
}

/* Read Register */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
void sign_extend(unsigned offset, unsigned *extended_value)
{
    unsigned signBit = offset >> 15;
    if (signBit == 1)
        *extended_value = offset | 0xFFFF0000;
    else
        *extended_value = offset & 0x0000FFFF;
}

/* ALU Operations */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
    unsigned B = ALUSrc == 1 ? extended_value : data2;

    if (ALUOp == 7)
    {
        switch (funct)
        {
            case 32:
                ALUOp = 0;
                break;
            case 34:
                ALUOp = 1;
                break;
            case 42:
                ALUOp = 2;
                break;
            case 43:
                ALUOp = 3;
                break;
            case 36:
                ALUOp = 4;
                break;
            case 37:
                ALUOp = 5;
                break;
            case 4:
                ALUOp = 6;
                break;
            case 39:
                ALUOp = 7;
                break;
            default:
                return 1;
        }
    }

    ALU(data1, B, ALUOp, ALUresult, Zero);
    return 0;
}

/* Read / Write Memory */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem)
{
    if (MemRead)
    {
        if (ALUresult % 4 == 0)
            *memdata = Mem[ALUresult >> 2];
        else
            return 1;
    }
    if (MemWrite)
    {
        if (ALUresult % 4 == 0)
            Mem[ALUresult >> 2] = data2;
        else
            return 1;
    }
    return 0;
}

/* Write Register */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
    if (RegWrite)
    {
        switch (MemtoReg)
        {
            case 0:
                Reg[RegDst ? r3 : r2] = ALUresult;
                break;
            case 1:
                Reg[RegDst ? r3 : r2] = memdata;
                break;
        }
    }
}

/* PC Update */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
    *PC += 4;
    if (Zero && Branch)
        *PC += (extended_value << 2);
    if (Jump)
        *PC = (jsec << 2) | (*PC & 0xF0000000);
}
