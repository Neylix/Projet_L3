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

int arm_load_store(arm_core p, uint32_t ins) {
/*
    uint32_t rn;
    uint32_t rm;
    uint32_t value;
    switch(get_bits(ins,24,20)){ //méthode offset
        case 0b00001 : //LDR avec U = 0
            rn = arm_read_register(p,get_bits(ins,19,16));
            rm = arm_read_register(p,get_bits(ins,3,0));
            if(arm_read_word(p,rn-rm,&value) == 0){
                arm_write_register(p,get_bits(ins,15,12),value);
                return 1;
            } else 
                return 0;
        break;
        case 0b01001 : //LDR avec U = 1
            rn = arm_read_register(p,get_bits(ins,19,16));
            rm = arm_read_register(p,get_bits(ins,3,0));
            if(arm_read_word(p,rn+rm,&value) == 0){
                arm_write_register(p,get_bits(ins,15,12),value);
                return 1;
            } else 
                return 0;
        break;
        case 0b00101 : //LDRB avec U = 0
            rn = arm_read_register(p,get_bits(ins,19,16));
            rm = arm_read_register(p,get_bits(ins,3,0));
            if(arm_read_byte(p,rn-rm,&value) == 0){
                arm_write_register(p,get_bits(ins,15,12),value);
                return 1;
            } else 
                return 0;
        break;
        case 0b01101 : //LDRB avec U = 1
            rn = arm_read_register(p,get_bits(ins,19,16));
            rm = arm_read_register(p,get_bits(ins,3,0));
            if(arm_read_byte(p,rn+rm,&value) == 0){
                arm_write_register(p,get_bits(ins,15,12),value);
                return 1;
            } else 
                return 0;
        break;
        case 0b????? : //LDRH avec U = 0
            rn = arm_read_register(p,get_bits(ins,19,16));
            rm = arm_read_register(p,get_bits(ins,3,0));
            if(arm_read_half(p,rn+rm,&value) == 0){
                arm_write_register(p,get_bits(ins,15,12),value);
                return 1;
            } else 
                return 0;
        break;
        case 0b????? : //LDRH avec U = 1
            rn = arm_read_register(p,get_bits(ins,19,16));
            rm = arm_read_register(p,get_bits(ins,3,0));
            if(arm_read_half(p,rn+rm,&value) == 0){
                arm_write_register(p,get_bits(ins,15,12),value);
                return 1;
            } else 
                return 0;
        break;
        case 0b00000 : //STR avec U = 0
            rn = arm_read_register(p,get_bits(ins,19,16));
            rm = arm_read_register(p,get_bits(ins,3,0));
            if(arm_write_word(p,get_bits(ins,15,12)-rm,rn) == 0)
                return 1;
            else 
                return 0;
        break;
        case 0b01000 : //STR avec U = 1
            rn = arm_read_register(p,get_bits(ins,19,16));
            rm = arm_read_register(p,get_bits(ins,3,0));
            if(arm_write_word(p,get_bits(ins,15,12)+rm,rn) == 0)
                return 1;
            else 
                return 0;
        break;
        case 0b00100 : //STRB avec U = 0
            rn = arm_read_register(p,get_bits(ins,19,16));
            rm = arm_read_register(p,get_bits(ins,3,0));
            if(arm_write_byte(p,get_bits(ins,15,12)-rm,rn) == 0)
                return 1;
            else 
                return 0;
        break;
        case 0b01100 : //STRB avec U = 1
            rn = arm_read_register(p,get_bits(ins,19,16));
            rm = arm_read_register(p,get_bits(ins,3,0));
            if(arm_write_byte(p,get_bits(ins,15,12)+rm,rn) == 0)
                return 1;
            else 
                return 0;
        break;
    }
*/
    return UNDEFINED_INSTRUCTION;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
	uint32_t adress;
	int P = get_bit(ins, 24);
	int U = get_bit(ins, 23);
	int S = get_bit(ins, 22);
	int W = get_bit(ins, 21);
	int L = get_bit(ins, 20);
	int Rn = get_bits(ins, 19, 16);
	uint32_t value;
	int result = 0;
	adress = arm_read_register(p, Rn);
	if (L && S && get_bit(ins, 15))
		result |= arm_write_cpsr(p, arm_read_spsr(p));
	if (P)
	{
		if (U)
			adress += 4;
		else
			adress -= 4;
	}
	for (int i = 0; i < 15; i++)
	{
		if (get_bit(ins, i))
		{
			if (L)
			{
				result |= arm_read_word(p, adress, &value);
				if ((S && !L) || (S && L && !get_bit(ins, 15)))
					result |= arm_write_usr_register(p, i, value);
				else
					result |= arm_write_register(p, i, value);
			}
			else
			{
				if ((S && !L) || (S && L && !get_bit(ins, 15)))
					value = arm_read_usr_register(p, i);
				else
					value = arm_read_register(p, i);
				result |= arm_write_word(p, adress, value);
			}
			if (U)
				adress += 4;
			else
				adress -= 4;
		}
	}
	if (get_bit(ins, 15))
	{
		result |= arm_read_word(p, adress, &value);
		result |= arm_write_register(p, 15, value & 0xFFFFFFFE);
		if (get_bit(value, 0))
		{
			value = arm_read_cpsr(p);
			result |= arm_write_cpsr(p, value | 0x1);
		}
		else
		{
			value = arm_read_cpsr(p);
			result |= arm_write_cpsr(p, value & ~(0x1));
		}
	}
	if (U)
		adress += 4;
	else
		adress -= 4;
	if (W)
		result |= arm_write_register(p, Rn, adress);
    	return result;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
