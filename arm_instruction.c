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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

int verif_cond(uint32_t value)
{
	switch (value)
	{
		case 0b0000:
			return get_bit(value, Z);
		case 0b0001:
			return !get_bit(value, Z);
		case 0b0010:
			return get_bit(value, C);
		case 0b0011:
			return !get_bit(value, C);
		case 0b0100:
			return get_bit(value, N);
		case 0b0101:
			return !get_bit(value, N);
		case 0b0110:
			return get_bit(value, V);
		case 0b0111:
			return !get_bit(value, V);
		case 0b1000:
			return get_bit(value, C) && !get_bit(value, Z);
		case 0b1001:
			return !get_bit(value, C) && get_bit(value, Z);
		case 0b1010:
			return get_bit(value, N) == get_bit(value, V);
		case 0b1011:
			return get_bit(value, N) != get_bit(value, V);
		case 0b1100:
			return !get_bit(value, Z) && (get_bit(value, N) == get_bit(value, V));
		case 0b1101:
			return get_bit(value, Z) || (get_bit(value, N) != get_bit(value, V));
		default:
			return 1;
	}
}

static int arm_execute_instruction(arm_core p) {
    uint32_t value;
    int result;
    result = arm_fetch(p, &value);
    if (result)
	return result;
    else
    {
	if (verif_cond(get_bits(value, 31, 28)))
	{
		switch (get_bits(value, 27, 26))
		{
			case 0b00:
				if (get_bit(value, 7) && get_bit(value, 4))
					result = arm_load_store(p, value);
				else
					/* Je ne sais pas ce que signifie arm_data_processing_immediate_msr dans arm_data_processing.h */
					result = arm_data_processing_shift(p, value);
				break;
			case 0b01:		//LDR, LDRB, STR, STRB
				result = arm_load_store(p, value);
				break;
			case 0b10:		//STM, LDM, B/BL
				if (get_bit(value, 25))
					result = arm_branch(p, value);
				else
					result = arm_load_store_multiple(p, value);
				break;
			default:
				result = arm_coprocessor_others_swi(p, value);
		}
	}
	else
		result = 0;
    }
    return result;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result)
        arm_exception(p, result);
    return result;
}
