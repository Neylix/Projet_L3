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

int load(arm_core p,uint32_t address, uint32_t noReg, uint32_t b,int H){
	uint32_t value;
	uint16_t val_16;
    uint8_t val_b;
	if(H){
		if(arm_read_half(p,address,&val_16) == 0){
            return arm_write_register(p,noReg,value);
        } else 
            return -1;
	}
	if(b){ // Word
        if(arm_read_word(p,address,&value) == 0){
            return arm_write_register(p,noReg,value);
        } else 
            return -1;
    } else { // Byte
        if(arm_read_byte(p,address,&val_b) == 0){
            return arm_write_register(p,noReg,val_b);
         } else 
            return -1;
     }
}

int store(arm_core p,uint32_t rn, uint32_t rd, uint32_t b, int H){
	if(H)
		return arm_write_half(p,rn,rd);
	if(b){ 
        return arm_write_word(p,rn,rd);
    } else {
        return arm_write_byte(p,rn,(uint8_t)rd);
    }
}

int arm_load_store(arm_core p, uint32_t ins) {
	uint32_t address,rd,offset;
	uint8_t noRegRn = get_bits(ins,19,16);
	uint8_t noRegRd = get_bits(ins,15,12);
	uint8_t noRegRm = get_bits(ins,3,0);
	int P = get_bit(ins, 24);
	int U = get_bit(ins, 23);
	int B = get_bit(ins, 22);
	int W = get_bit(ins, 21);
	int L = get_bit(ins, 20);
	int I = get_bit(ins, 25); 
	int H = get_bit(ins, 5);
	rd = arm_read_register(p,noRegRd);

    if((P&&W)|| (!P)){ //post-indexed addressing et pre-indexed addressing
		if(noRegRn == 15)
			return 0;
		address = arm_read_register(p,noRegRn);
		if(I){ //Registre
			if(noRegRm != 15)
				offset = arm_read_register(p,noRegRm);
			else 
				return 0;
    	} else { //Immediat
       		if(get_bit(ins,7) && get_bit(ins,5) && get_bit(ins,4)){
				offset = (get_bits(ins,11,8) << 4) | noRegRm;
			} else 
       			offset = arm_read_register(p,get_bits(ins,11,0));
    	}
		if(U) //U = 1
			address += offset;
		else // U = 0
			address -= offset;
		arm_write_register(p,noRegRn,address);
		if(L) // Load
            return load(p,address,noRegRd,B,H);
		else // Store
			return store(p,address,rd,B,H);
	}else{ //offset addressing
		if(I){ //Registre
       		address = arm_read_register(p,noRegRn);
			if(noRegRm != 15)
        		offset = arm_read_register(p,noRegRm);
			else 
				return 0;
    	} else { //Immediat
			address = noRegRn;
			if(get_bit(ins,7) && get_bit(ins,5) && get_bit(ins,4)){
				offset = (get_bits(ins,11,8) << 4) | noRegRm;
			} else 
       			offset = arm_read_register(p,get_bits(ins,11,0));
    	}
		if(noRegRn == 15) //Rn est le Registre 15
        	address += 8;
		if(U) //U = 1
			address += offset;
		else // U = 0
			address -= offset;
		if(L) // Load
            return load(p,address,noRegRd,B,H);
       	else // Store
            return store(p,address,rd,B,H);
    }
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
