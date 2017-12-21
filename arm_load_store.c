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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

int str(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int strb(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int ldr(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int ldrb(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);

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

int arm_load_store(arm_core p, uint32_t ins) {
	if (get_bit(ins, 26))
	{
		uint32_t off = decode_off(p, ins);
		uint8_t rn = get_bits(ins, 19, 16);
		uint8_t rd = get_bits(ins, 15, 12);
		int p_bit = get_bit(ins, 24);
		int u_bit = get_bit(ins, 23);
		int w_bit = get_bit(ins, 21);
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
		return UNDEFINED_INSTRUCTION;	//Rajouter ldrh et strh
	}
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
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
	return result;
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
	return result;
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
		return result;
	if (rd == 15)
		arm_write_register(p, rd, rd_cont & 0xFFFFFFFC);
	else
		arm_write_register(p, rd, rd_cont);
	return result;
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
		return result;
	arm_write_register(p, rd, (uint32_t)rd_cont);
	return result;
}
