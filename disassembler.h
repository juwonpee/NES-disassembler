#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <iterator>


#include "types.h"

using namespace std;

class disassembler {
    public:
        disassembler(string directory, int bankNum, bool disassemble, bool hexDump);
        ~disassembler();
    private:
        vector<uint8_t> buffer;
        vector<uint8_t> ROM;

        // instructions & enum addressingMode {
        enum addressingMode_t{
            acc, abl, abx, aby, imm, imp, ind, xin, yin, rel, zpg, zpx, zpy, axx
        };
        enum instructions_t {
            adc, adm, asl, bcc, bcs, beq, bit, bmi, bne, bpl, brk, bvc, bvs, clc, cld, cli, clv, cmp, cpx, cpy, dec, dex, dey, eor, inc, inx, iny, jmp, jsr, lda, ldx, ldy, lsr, nop, ora, pha, php, pla, plp, rol, ror, rti, rts, sbc, sec, sed, sei, sta, stx, sty, tax, tay, tsx, txa, txs, tya, ixx
        };
        struct Instruction {
            string Name;
            addressingMode_t addressingMode;
            instructions_t instructions;
            uint8_t InstructionClocks;
        };
        vector<Instruction> lookup =
        {
            //                          0xXX01                      0xXX02                      0xXX03                      0xXX04                      0xXX05                       0xXX06                     0xXX07                      0xXX08                      0xXX09                      0xXX0a                      0xXX0b                      0xXX0c                      0xXX0d                      0xXX0e                      0xXX0f
            {"BRK: 0x00", imp, brk, 7}, {"ORA: 0x01", xin, ora, 6}, {"XXX: 0x02", axx, ixx, 2}, {"XXX: 0x03", axx, ixx, 2}, {"XXX: 0x04", axx, ixx, 2}, {"ORA: 0x05", zpg, ora, 3}, {"ASL: 0x06", zpg, asl, 5}, {"XXX: 0x07", axx, ixx, 2}, {"PHP: 0x08", imp, php, 3}, {"ORA: 0x09", imm, ora, 2}, {"ASL: 0x0A", acc, asl, 2}, {"XXX: 0x0B", axx, ixx, 2}, {"XXX: 0x0C", axx, ixx, 2}, {"ORA: 0x0D", abs, ora, 4}, {"ASL: 0x0E", abs, asl, 6}, {"XXX: 0x0F", axx, ixx, 2}, 
            {"BPL: 0x10", rel, bpl, 2}, {"ORA: 0x11", yin, ora, 5}, {"XXX: 0x12", axx, ixx, 2}, {"XXX: 0x13", axx, ixx, 2}, {"XXX: 0x14", axx, ixx, 2}, {"ORA: 0x15", zpx, ora, 4}, {"ASL: 0x16", zpx, asl, 6}, {"XXX: 0x17", axx, ixx, 2}, {"CLC: 0x18", imp, clc, 2}, {"ORA: 0x19", aby, ora, 3}, {"XXX: 0x1A", axx, ixx, 2}, {"XXX: 0x1B", axx, ixx, 2}, {"XXX: 0x1C", axx, ixx, 2}, {"ORA: 0x1D", abx, ora, 4}, {"ASL: 0x1E", abx, asl, 7}, {"XXX: 0x1F", axx, ixx, 2}, 
            {"JSR: 0x20", rel, jsr, 2}, {"AND: 0x21", xin, adm, 6}, {"XXX: 0x22", axx, ixx, 2}, {"XXX: 0x23", axx, ixx, 2}, {"BIT: 0x24", zpg, bit, 3}, {"AND: 0x25", zpg, adm, 3}, {"ROL: 0x26", zpg, rol, 5}, {"XXX: 0x27", axx, ixx, 2}, {"PLP: 0x28", imp, plp, 4}, {"AND: 0x29", imm, adm, 2}, {"ROL: 0x2A", acc, rol, 2}, {"XXX: 0x2B", axx, ixx, 2}, {"BIT: 0x2C", abs, bit, 4}, {"AND: 0x2D", abx, adm, 4}, {"ROL: 0x2E", abx, rol, 6}, {"XXX: 0x2F", axx, ixx, 2}, 
            {"BMI: 0x30", rel, bmi, 2}, {"AND: 0x31", yin, adm, 5}, {"XXX: 0x32", axx, ixx, 2}, {"XXX: 0x33", axx, ixx, 2}, {"XXX: 0x34", axx, ixx, 2}, {"AND: 0x35", zpx, adm, 4}, {"ROL: 0x36", zpg, rol, 6}, {"XXX: 0x37", axx, ixx, 2}, {"SEC: 0x38", imp, sec, 2}, {"AND: 0x39", aby, adm, 4}, {"XXX: 0x3A", axx, ixx, 2}, {"XXX: 0x3B", axx, ixx, 2}, {"XXX: 0x3C", axx, ixx, 2}, {"AND: 0x3D", abx, adm, 4}, {"ROL: 0x3E", abx, rol, 7}, {"XXX: 0x3F", axx, ixx, 2}, 
            {"RTI: 0x40", imp, rti, 2}, {"EOR: 0x41", xin, eor, 6}, {"XXX: 0x42", axx, ixx, 2}, {"XXX: 0x43", axx, ixx, 2}, {"XXX: 0x44", axx, ixx, 2}, {"EOR: 0x45", zpg, eor, 3}, {"LSR: 0x46", zpg, lsr, 5}, {"XXX: 0x47", axx, ixx, 2}, {"PHA: 0x48", imp, pha, 3}, {"EOR: 0x49", imm, eor, 2}, {"LSR: 0x4A", acc, lsr, 2}, {"XXX: 0x4B", axx, ixx, 2}, {"JMP: 0x4C", abs, jmp, 3}, {"EOR: 0x4D", abs, eor, 4}, {"LSR: 0x4E", abs, lsr, 6}, {"XXX: 0x4F", axx, ixx, 2}, 
            {"BVC: 0x50", rel, bvc, 2}, {"EOR: 0x51", xin, eor, 5}, {"XXX: 0x52", axx, ixx, 2}, {"XXX: 0x53", axx, ixx, 2}, {"XXX: 0x54", axx, ixx, 2}, {"EOR: 0x55", zpx, eor, 4}, {"LSR: 0x56", zpx, lsr, 6}, {"XXX: 0x57", axx, ixx, 2}, {"CLI: 0x58", imp, cli, 2}, {"EOR: 0x59", aby, eor, 4}, {"XXX: 0x5A", axx, ixx, 2}, {"XXX: 0x5B", axx, ixx, 2}, {"XXX: 0x5C", axx, ixx, 2}, {"EOR: 0x5D", abx, eor, 4}, {"LSR: 0x5E", abx, lsr, 7}, {"XXX: 0x5F", axx, ixx, 2}, 
            {"RTS: 0x60", imp, rts, 6}, {"ADC: 0x61", xin, adc, 6}, {"XXX: 0x62", axx, ixx, 2}, {"XXX: 0x63", axx, ixx, 2}, {"XXX: 0x64", axx, ixx, 2}, {"ADC: 0x65", zpg, adc, 3}, {"ROR: 0x66", zpg, ror, 5}, {"XXX: 0x67", axx, ixx, 2}, {"PLA: 0x68", imp, pla, 4}, {"ADC: 0x69", imm, adc, 2}, {"ROR: 0x6A", acc, ror, 2}, {"XXX: 0x6B", axx, ixx, 2}, {"JMP: 0x6C", ind, jmp, 5}, {"ADC: 0x6D", abs, adc, 4}, {"ROR: 0x6E", abs, ror, 6}, {"XXX: 0x6F", axx, ixx, 2}, 
            {"BVS: 0x70", rel, bvs, 2}, {"ADC: 0x71", yin, adc, 5}, {"XXX: 0x72", axx, ixx, 2}, {"XXX: 0x73", axx, ixx, 2}, {"XXX: 0x74", axx, ixx, 2}, {"ADC: 0x75", zpx, adc, 4}, {"ROR: 0x76", zpx, ror, 6}, {"XXX: 0x77", axx, ixx, 2}, {"SEI: 0x78", imp, sei, 2}, {"ADC: 0x79", aby, adc, 4}, {"XXX: 0x7A", axx, ixx, 2}, {"XXX: 0x7B", axx, ixx, 2}, {"XXX: 0x7C", axx, ixx, 2}, {"ADC: 0x7D", abx, adc, 4}, {"ROR: 0x7E", abx, ror, 7}, {"XXX: 0x7F", axx, ixx, 2}, 
            {"XXX: 0x80", axx, ixx, 2}, {"STA: 0x81", xin, sta, 6}, {"XXX: 0x82", axx, ixx, 2}, {"XXX: 0x83", axx, ixx, 2}, {"STY: 0x84", zpg, sty, 3}, {"STA: 0x85", zpg, sta, 3}, {"STX: 0x86", zpg, stx, 3}, {"XXX: 0x87", axx, ixx, 2}, {"DEY: 0x88", imp, dey, 2}, {"XXX: 0x89", axx, ixx, 2}, {"TXA: 0x8A", imp, txa, 2}, {"XXX: 0x8B", axx, ixx, 2}, {"STY: 0x8C", abs, sty, 4}, {"STA: 0x8D", abs, sta, 4}, {"STX: 0x8E", abs, stx, 4}, {"XXX: 0x8F", axx, ixx, 2}, 
            {"BCC: 0x90", rel, bcc, 2}, {"STA: 0x91", yin, sta, 6}, {"XXX: 0x92", axx, ixx, 2}, {"XXX: 0x93", axx, ixx, 2}, {"STY: 0x94", zpx, sty, 4}, {"STA: 0x95", zpx, sta, 4}, {"STX: 0x96", zpx, stx, 4}, {"XXX: 0x97", axx, ixx, 2}, {"TYA: 0x98", imp, tya, 2}, {"STA: 0x99", aby, sta ,5}, {"TXS: 0x9A", imp, txs, 2}, {"XXX: 0x9B", axx, ixx, 2}, {"XXX: 0x9C", axx, ixx, 2}, {"STA: 0x9D", abx, sta, 5}, {"XXX: 0x9E", axx, ixx, 2}, {"XXX: 0x9F", axx, ixx, 2}, 
            {"LDY: 0xA0", imm, ldy, 2}, {"LDA: 0xA1", yin, lda, 6}, {"LDX: 0xA2", imm, ldx, 2}, {"XXX: 0xA3", axx, ixx, 2}, {"LDY: 0xA4", zpg, ldy, 3}, {"LDA: 0xA5", zpg, lda, 3}, {"LDX: 0xA6", zpg, ldx, 3}, {"XXX: 0xA7", axx, ixx, 2}, {"TAY: 0xA8", imp, tay, 2}, {"LDA: 0xA9", imm, lda, 2}, {"TAX: 0xAA", imp, tax, 2}, {"XXX: 0xAB", axx, ixx, 2}, {"LDY: 0xAC", abs, ldy, 4}, {"LDA: 0xAD", abs, lda, 4}, {"LDX: 0xAE", abs, ldx, 4}, {"XXX: 0xAF", axx, ixx, 2}, 
            {"BCS: 0xB0", rel, bcs, 2}, {"LDA: 0xB1", yin, lda, 5}, {"XXX: 0xB2", axx, ixx, 2}, {"XXX: 0xB3", axx, ixx, 2}, {"LDY: 0xB4", zpx, ldy, 4}, {"LDA: 0xB5", zpx, lda, 4}, {"LDX: 0xB6", zpx, ldx, 4}, {"XXX: 0xB7", axx, ixx, 2}, {"CLV: 0xB8", imp, clv, 2}, {"LDA: 0xB9", aby, lda, 4}, {"TSX: 0xBA", imp, tsx, 2}, {"XXX: 0xBB", axx, ixx, 2}, {"LDY: 0xBC", abx, ldy, 4}, {"LDA: 0xBD", abx, lda, 4}, {"LDX: 0xBE", abx, ldx, 4}, {"XXX: 0xBF", axx, ixx, 2}, 
            {"CPY: 0xC0", rel, cpy, 2}, {"CMP: 0xC1", xin, cmp, 6}, {"XXX: 0xC2", axx, ixx, 2}, {"XXX: 0xC3", axx, ixx, 2}, {"CPY: 0xC4", zpg, cpy, 3}, {"CMP: 0xC5", zpg, cmp, 3}, {"DEC: 0xC6", zpg, dec, 5}, {"XXX: 0xC7", axx, ixx, 2}, {"INY: 0xC8", imp, iny, 2}, {"CMP: 0xC9", imm, cmp, 2}, {"DEX: 0xCA", imp, dex, 2}, {"XXX: 0xCB", axx, ixx, 2}, {"CPY: 0xCC", abs, cpy, 4}, {"CMP: 0xCD", abs, cmp, 4}, {"DEC: 0xCE", abs, dec, 6}, {"XXX: 0xCF", axx, ixx, 2}, 
            {"BNE: 0xD0", rel, bne, 2}, {"CMP: 0xD1", xin, cmp, 5}, {"XXX: 0xD2", axx, ixx, 2}, {"XXX: 0xD3", axx, ixx, 2}, {"XXX: 0xD4", axx, ixx, 2}, {"CMP: 0xD5", zpx, cmp, 4}, {"DEC: 0xD6", zpx, dec, 6}, {"XXX: 0xD7", axx, ixx, 2}, {"CLD: 0xD8", imp, cld, 2}, {"CMP: 0xD9", aby, cmp, 4}, {"XXX: 0xDA", axx, ixx, 2}, {"XXX: 0xDB", axx, ixx, 2}, {"XXX: 0xDC", axx, ixx, 2}, {"CMP: 0xDD", abx, cmp, 4}, {"DEC: 0xDE", abx, dec, 7}, {"XXX: 0xDF", axx, ixx, 2}, 
            {"CPX: 0xE0", imm, cpx, 2}, {"SBC: 0xE1", xin, sbc, 6}, {"XXX: 0xE2", axx, ixx, 2}, {"XXX: 0xE3", axx, ixx, 2}, {"CPX: 0xE4", zpg, cpx, 3}, {"SBC: 0xE5", zpg, sbc, 3}, {"INC: 0xE6", zpg, inc, 5}, {"XXX: 0xE7", axx, ixx, 2}, {"INX: 0xE8", imp, inx, 2}, {"SBC: 0xE9", imm, sbc, 2}, {"NOP: 0xEA", imp, nop, 2}, {"XXX: 0xEB", axx, ixx, 2}, {"CPX: 0xEC", abs, cpx, 4}, {"SBC: 0xED", abs, sbc, 4}, {"INC: 0xEE", abs, inc, 6}, {"XXX: 0xEF", axx, ixx, 2}, 
            {"BEQ: 0xF0", rel, beq, 2}, {"SBC: 0xF1", yin, sbc, 5}, {"XXX: 0xF2", axx, ixx, 2}, {"XXX: 0xF3", axx, ixx, 2}, {"XXX: 0xF4", axx, ixx, 2}, {"SBC: 0xF5", zpx, sbc, 4}, {"INC: 0xF6", zpx, inc, 6}, {"XXX: 0xF7", axx, ixx, 2}, {"SED: 0xF8", imp, sed, 2}, {"SBC: 0xF9", aby, sbc, 4}, {"XXX: 0xFA", axx, ixx, 2}, {"XXX: 0xFB", axx, ixx, 2}, {"XXX: 0xFC", axx, ixx, 2}, {"SBC: 0xFD", abx, sbc, 4}, {"INC: 0xFE", abx, inc, 7}, {"XXX: 0xFF", axx, ixx, 2}, 
        };
};
