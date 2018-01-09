/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"
#include <stdio.h>

int str(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int strb(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int ldr(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int ldrb(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int strh(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int ldrh(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);

int ldm(arm_core p, uint32_t ins);
int stm(arm_core p, uint32_t ins);

uint32_t decode_off(arm_core p, uint32_t ins)
{
	if (!get_bit(ins, 25))
	{
		return get_bits(ins, 11, 0);
	}
	uint32_t rm = arm_read_register(p, get_bits(ins, 3, 0));
	uint8_t shift_imm = get_bits(ins, 11, 7);
	uint8_t shift = get_bits(ins, 6, 5);
	if (get_bits(ins, 11, 4) == 0)
		return rm;
	switch (shift)
	{
		case LSL:
			return rm << shift_imm;
		case LSR:
			if (shift_imm == 0)
				return 0;
			return rm >> shift_imm;
		case ASR:
			if (shift_imm == 0)
			{
				if (get_bit(rm, 31))
					return -1;
				return 0;
			}
			return asr(rm, shift_imm);
		default:
			if (shift_imm == 0)
				return (get_bit(arm_read_cpsr(p), C) << 31) | (rm >> 1);
			return ror(rm, shift_imm);
	}
}

uint32_t decode_off_h(arm_core p, uint32_t ins)
{
	if (!get_bit(ins, 22))
	{
		return (get_bits(ins, 11, 8) << 4) | get_bits(ins, 3, 0);
	}
	return arm_read_register(p, get_bits(ins, 3, 0));
}

int arm_load_store(arm_core p, uint32_t ins) {
	uint8_t rn = get_bits(ins, 19, 16);
	uint8_t rd = get_bits(ins, 15, 12);
	int p_bit = get_bit(ins, 24);
	int u_bit = get_bit(ins, 23);
	int w_bit = get_bit(ins, 21);
	if (get_bit(ins, 26))
	{
		uint32_t off = decode_off(p, ins);
		switch (get_bit(ins, 22) << 1 | get_bit(ins, 20))
		{
			case 0b00:
				return str(p, rd, rn, off, p_bit, u_bit, w_bit);
			case 0b01:
				return ldr(p, rd, rn, off, p_bit, u_bit, w_bit);
			case 0b10:
				return strb(p, rd, rn, off, p_bit, u_bit, w_bit);
			default:
				return ldrb(p, rd, rn, off, p_bit, u_bit, w_bit);
		}
	}
	else
	{
		uint32_t off = decode_off_h(p, ins);
		if (get_bit(ins, 20))
			return ldrh(p, rd, rn, off, p_bit, u_bit, w_bit);
		return strh(p, rd, rn, off, p_bit, u_bit, w_bit);
	}
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    if (get_bit(ins, 20))
		return ldm(p, ins);
	return stm(p, ins);
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}

/* On utilise ici le BASE UPDATED ABORT MODEL */
int str(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit)
{
	uint32_t rn_cont = arm_read_register(p, rn);
	uint32_t rd_cont = arm_read_register(p, rd);
	uint32_t address;
	int result;
	if (u_bit)
	{
		address = rn_cont + off;
		if (p_bit)
		{
			result = arm_write_word(p, address, rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_write_word(p, rn_cont, rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	else
	{
		address = rn_cont - off;
		if (p_bit)
		{
			result = arm_write_word(p, address, rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_write_word(p, rn_cont, rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	if (result)
		return DATA_ABORT;
	return 0;
}

int strb(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit)
{
	uint32_t rn_cont = arm_read_register(p, rn);
	uint8_t rd_cont = (uint8_t)(arm_read_register(p, rd));
	uint32_t address;
	int result;
	if (u_bit)
	{
		address = rn_cont + off;
		if (p_bit)
		{
			result = arm_write_byte(p, address, rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_write_byte(p, rn_cont, rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	else
	{
		address = rn_cont - off;
		if (p_bit)
		{
			result = arm_write_byte(p, address, rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_write_byte(p, rn_cont, rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	if (result)
		return DATA_ABORT;
	return 0;
}

int ldr(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit)
{
	uint32_t rn_cont = arm_read_register(p, rn);
	uint32_t rd_cont;
	uint32_t address;
	int result;
	if (u_bit)
	{
		address = rn_cont + off;
		if (p_bit)
		{
			result = arm_read_word(p, address, &rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_read_word(p, rn_cont, &rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	else
	{
		address = rn_cont - off;
		if (p_bit)
		{
			result = arm_read_word(p, address, &rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_read_word(p, rn_cont, &rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	if (result)
		return DATA_ABORT;
	if (rd == 15)
		arm_write_register(p, rd, rd_cont & 0xFFFFFFFC);
	else
		arm_write_register(p, rd, rd_cont);
	return 0;
}

int ldrb(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit)
{
	uint32_t rn_cont = arm_read_register(p, rn);
	uint8_t rd_cont;
	uint32_t address;
	int result;
	if (u_bit)
	{
		address = rn_cont + off;
		if (p_bit)
		{
			result = arm_read_byte(p, address, &rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_read_byte(p, rn_cont, &rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	else
	{
		address = rn_cont - off;
		if (p_bit)
		{
			result = arm_read_byte(p, address, &rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_read_byte(p, rn_cont, &rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	if (result)
		return DATA_ABORT;
	arm_write_register(p, rd, (uint32_t)rd_cont);
	return 0;
}

int strh(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit)
{
	uint32_t rn_cont = arm_read_register(p, rn);
	uint16_t rd_cont = (uint16_t)(arm_read_register(p, rd));
	uint32_t address;
	int result;
	if (u_bit)
	{
		address = rn_cont + off;
		if (p_bit)
		{
			result = arm_write_half(p, address, rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_write_half(p, rn_cont, rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	else
	{
		address = rn_cont - off;
		if (p_bit)
		{
			result = arm_write_half(p, address, rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_write_half(p, rn_cont, rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	if (result)
		return DATA_ABORT;
	return 0;
}

int ldrh(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit)
{
	uint32_t rn_cont = arm_read_register(p, rn);
	uint16_t rd_cont;
	uint32_t address;
	int result;
	if (u_bit)
	{
		address = rn_cont + off;
		if (p_bit)
		{
			result = arm_read_half(p, address, &rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_read_half(p, rn_cont, &rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	else
	{
		address = rn_cont - off;
		if (p_bit)
		{
			result = arm_read_half(p, address, &rd_cont);
			if (w_bit)
				arm_write_register(p, rn, address);
		}
		else
		{
			result = arm_read_half(p, rn_cont, &rd_cont);
			arm_write_register(p, rn, address);
		}
	}
	if (result)
		return DATA_ABORT;
	arm_write_register(p, rd, (uint32_t)rd_cont);
	return 0;
}

int ldm(arm_core p, uint32_t ins)
{
	uint32_t address = arm_read_register(p, get_bits(ins, 19, 16));
	uint32_t value = 0;
	int result = 0;
	uint32_t decale = 0;
	if (get_bit(ins, 23)) {
		decale += 4;
	} else {
		decale -= -4;
	}
	if (get_bit(ins, 24))
		address += decale;
	if (get_bit(ins, 22) && get_bit(ins, 15))
		arm_write_cpsr(p, arm_read_spsr(p));
	if (get_bit(ins, 22) && !get_bit(ins, 15))
	{
		for (int i = 0; i < 15; i++)
		{
			if (get_bit(ins, i))
			{
				result |= arm_read_word(p, address, &value);
				arm_write_usr_register(p, i, value);
				address += decale;
			}
		}
		if (get_bit(ins, 15))
		{
			result |= arm_read_word(p, address, &value);
			arm_write_usr_register(p, 15, value & 0xFFFFFFFC);
			address += decale;
		}

	}
	if (!get_bit(ins, 22))
	{
		for (int i = 0; i < 15; i++)
		{
			if (get_bit(ins, i))
			{
				result |= arm_read_word(p, address, &value);
				arm_write_register(p, i, value);
				address += decale;
			}
		}
		if (get_bit(ins, 15))
		{
			result |= arm_read_word(p, address, &value);
			arm_write_register(p, 15, value & 0xFFFFFFFC);
			address += decale;
		}
	}
	if (get_bit(ins, 21))
	{
		switch (get_bits(ins, 24, 23))
		{
			case 0b00:
				arm_write_register(p, get_bits(ins, 19, 16), address);
				break;
			case 0b01:
				arm_write_register(p, get_bits(ins, 19, 16), address);
				break;
			case 0b10:
				arm_write_register(p, get_bits(ins, 19, 16), address + 4);
				break;
			default:
				arm_write_register(p, get_bits(ins, 19, 16), address - 4);
		}
	}
	if (result)
		return DATA_ABORT;
	return 0;
}

int stm(arm_core p, uint32_t ins)
{
	uint32_t address = arm_read_register(p, get_bits(ins, 19, 16));
	uint32_t value;
	int result = 0;
	uint32_t decale = 0;
	if (get_bit(ins, 23)) {
		decale += 4;
	}else {
		decale -= 4;
	}
	if (get_bit(ins, 24))
		address += 4;
	if (get_bit(ins, 22))
	{
		for (int i = 0; i <= 15; i++)
		{
			if (get_bit(ins, i))
			{
				value = arm_read_usr_register(p, i);
				result |= arm_write_word(p, address, value);
				address += 4;
			}
		}
	}
	else
	{
		for (int i = 0; i <= 15; i++)
		{
			if (get_bit(ins, i))
			{
				value = arm_read_register(p, i);
				result |= arm_write_word(p, address, value);
				address += 4;
			}
		}
	}


	if (get_bit(ins, 21))
	{
		switch (get_bits(ins, 24, 23))
		{
			case 0b00:
				arm_write_register(p, get_bits(ins, 19, 16), address);
				break;
			case 0b01:
				arm_write_register(p, get_bits(ins, 19, 16), address);
				break;
			case 0b10:
				arm_write_register(p, get_bits(ins, 19, 16), address + 4);
				break;
			default:
				arm_write_register(p, get_bits(ins, 19, 16), address - 4);
		}
	}
	if (result)
		return DATA_ABORT;
	return 0;
}
