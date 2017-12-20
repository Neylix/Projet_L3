/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

static uint32_t albit(uint32_t x, uint8_t i, int b)
{
	if (b)
		return set_bit(x, i);
	else
		return clr_bit(x, i);
}

void and(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void eor(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void sub(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void rsb(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void add(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void adc(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void sbc(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void rsc(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void tst(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void teq(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void cmp(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void cmn(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void orr(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void mov(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void bic(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);
void mvn(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit);

int shifter_carry_out;

uint32_t decode_so(arm_core p, uint32_t ins)
{
	uint32_t result;
	if (get_bit(ins, 25))
	{
		result = ror(get_bits(ins, 7, 0), get_bits(ins, 11, 8)*2);
		if (get_bits(ins, 11, 8) == 0)
			shifter_carry_out = get_bit(arm_read_cpsr(p), C);
		else
			shifter_carry_out = get_bit(result, 31);
		return result;
	}
	if (!get_bit(ins, 4))
	{
		uint8_t shift_imm = get_bits(ins, 11, 7);
		uint8_t shift = get_bits(ins, 6, 5);
		uint8_t rm = get_bits(ins, 3, 0);
		uint32_t rm_cont = arm_read_register(p, rm);
		if (shift == LSL)
		{
			if (shift_imm == 0)
			{
				shifter_carry_out = get_bit(arm_read_cpsr(p), C);
				return rm_cont;
			}
			shifter_carry_out = get_bit(rm_cont, 32 - shift_imm);
			return rm_cont << shift_imm;
		}
		if (shift == LSR)
		{
			if (shift_imm == 0)
			{
				shifter_carry_out = get_bit(rm_cont, 31);
				return 0;
			}
			shifter_carry_out = get_bit(rm_cont, shift_imm - 1);
			return rm_cont >> shift_imm;
		}
		if (shift == ASR)
		{
			if (shift_imm == 0)
			{
				if (get_bit(rm_cont, 31) == 0)
				{
					shifter_carry_out = 0;
					return 0;
				}
				shifter_carry_out = 1;
				return -1;
			}
			shifter_carry_out = get_bit(rm_cont, shift_imm - 1);
			return asr(rm_cont, shift_imm);
		}
		if (shift_imm == 0)
		{
			shifter_carry_out = get_bit(rm_cont, 0);
			return (get_bit(arm_read_cpsr(p), C) << 31) | (rm_cont >> 1);
		}
		shifter_carry_out = get_bit(rm_cont, shift_imm - 1);
		return ror(rm_cont, shift_imm);
	}
	uint8_t shift_imm = arm_read_register(p, get_bits(ins, 11, 8)) & 0xFF;
	uint8_t shift = get_bits(ins, 6, 5);
	uint8_t rm = get_bits(ins, 3, 0);
	uint32_t rm_cont = arm_read_register(p, rm);
	if (shift == LSL)
	{
		if (shift_imm == 32)
		{
			shifter_carry_out = get_bit(rm_cont, 0);
			return 0;
		}
		if (shift_imm > 32)
		{
			shifter_carry_out = 0;
			return 0;
		}
		if (shift_imm == 0)
		{
			shifter_carry_out = get_bit(arm_read_cpsr(p), C);
			return rm_cont;
		}
		shifter_carry_out = get_bit(rm_cont, 32 - shift_imm);
		return rm_cont << shift_imm;
	}
	if (shift == LSR)
	{
		if (shift_imm == 32)
		{
			shifter_carry_out = get_bit(rm_cont, 31);
			return 0;
		}
		if (shift_imm > 32)
		{
			shifter_carry_out = 0;
			return 0;
		}
		if (shift_imm == 0)
		{
			shifter_carry_out = get_bit(rm_cont, 31);
			return 0;
		}
		shifter_carry_out = get_bit(rm_cont, shift_imm - 1);
		return rm_cont >> shift_imm;
	}
	if (shift == ASR)
	{
		if (shift_imm >= 32)
		{
			if (get_bit(rm_cont, 31) == 0)
			{
				shifter_carry_out = get_bit(rm_cont, 31);
				return 0;
			}
			shifter_carry_out = get_bit(rm_cont, 31);
			return -1;
		}
		if (shift_imm == 0)
		{
			if (get_bit(rm_cont, 31) == 0)
			{
				shifter_carry_out = 0;
				return 0;
			}
			shifter_carry_out = 1;
			return -1;
		}
		shifter_carry_out = get_bit(rm_cont, shift_imm - 1);
		return asr(rm_cont, shift_imm);
	}
	uint8_t shift_imm_5L = shift_imm & 0x1F;
	if (shift_imm == 0)
	{
		shifter_carry_out = get_bit(arm_read_cpsr(p), C);
		return rm_cont;
	}
	if (shift_imm_5L == 0)
	{
		shifter_carry_out = get_bit(rm_cont, 31);
		return rm_cont;
	}
	shifter_carry_out = get_bit(rm_cont, shift_imm_5L - 1);
	return ror(rm_cont, shift_imm_5L);
}

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
	uint32_t rn_cont = arm_read_register(p, get_bits(ins, 19, 16));
	uint8_t rd = get_bits(ins, 15, 12);
	uint32_t so = decode_so(p, ins);
	int s_bit = get_bit(ins, 20);
	switch (get_bits(ins, 24, 21))
	{
		case 0b0000:
			and(p, rd, rn_cont, so, s_bit);
		case 0b0001:
			eor(p, rd, rn_cont, so, s_bit);
		case 0b0010:
			sub(p, rd, rn_cont, so, s_bit);
		case 0b0011:
			rsb(p, rd, rn_cont, so, s_bit);
		case 0b0100:
			add(p, rd, rn_cont, so, s_bit);
		case 0b0101:
			adc(p, rd, rn_cont, so, s_bit);
		case 0b0110:
			sbc(p, rd, rn_cont, so, s_bit);
		case 0b0111:
			rsc(p, rd, rn_cont, so, s_bit);
		case 0b1000:
			tst(p, rd, rn_cont, so, s_bit);
		case 0b1001:
			teq(p, rd, rn_cont, so, s_bit);
		case 0b1010:
			cmp(p, rd, rn_cont, so, s_bit);
		case 0b1011:
			cmn(p, rd, rn_cont, so, s_bit);
		case 0b1100:
			orr(p, rd, rn_cont, so, s_bit);
		case 0b1101:
			mov(p, rd, rn_cont, so, s_bit);
		case 0b1110:
			bic(p, rd, rn_cont, so, s_bit);
		default:
			mvn(p, rd, rn_cont, so, s_bit);
	}
    return 0;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

void and(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
	uint32_t res = rn & so;
	arm_write_register(p, rd, res);
	if (s_bit && rd == 15)
	{
		if (arm_current_mode_has_spsr(p))
			arm_write_cpsr(p, arm_read_spsr(p));
		else
			return;
	}
	else if (s_bit)
	{
		uint32_t cpsr = arm_read_cpsr(p);
		cpsr = albit(cpsr, N, get_bit(res, 31));
		cpsr = albit(cpsr, Z, res == 0);
		cpsr = albit(cpsr, C, shifter_carry_out);
		arm_write_cpsr(p, cpsr);
	}
}

void eor(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void sub(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void rsb(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void add(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void adc(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void sbc(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void rsc(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void tst(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void teq(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void cmp(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void cmn(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void orr(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void mov(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void bic(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}

void mvn(arm_core p, uint8_t rd, uint32_t rn, uint32_t so, int s_bit)
{
}
