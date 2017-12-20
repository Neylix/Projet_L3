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
    return UNDEFINED_INSTRUCTION;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}
