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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

int conditionPassed(arm_core p, uint32_t value)
{
	uint32_t cpsr = arm_read_cpsr(p);
	switch (get_bits(value, 31, 28))
	{
		case 0b0000:
			return get_bit(cpsr, Z);
		case 0b0001:
			return !get_bit(cpsr, Z);
		case 0b0010:
			return get_bit(cpsr, C);
		case 0b0011:
			return !get_bit(cpsr, C);
		case 0b0100:
			return get_bit(cpsr, N);
		case 0b0101:
			return !get_bit(cpsr, N);
		case 0b0110:
			return get_bit(cpsr, V);
		case 0b0111:
			return !get_bit(cpsr, V);
		case 0b1000:
			return get_bit(cpsr, C) && !get_bit(cpsr, Z);
		case 0b1001:
			return !get_bit(cpsr, C) || get_bit(cpsr, Z);
		case 0b1010:
			return get_bit(cpsr, V) == get_bit(cpsr, N);
		case 0b1011:
			return get_bit(cpsr, V) != get_bit(cpsr, N);
		case 0b1100:
			return !get_bit(cpsr, Z) && (get_bit(cpsr, V) == get_bit(cpsr, N));
		case 0b1101:
			return get_bit(cpsr, Z) || (get_bit(cpsr, V) != get_bit(cpsr, N));
		case 0b1110:
			return 1;
		default:
			return 0;
	}
}

static int arm_execute_instruction(arm_core p) {
	uint32_t ins;
	int result = 0;
	result = arm_fetch(p, &ins);
	if (result)
		return result;
	if (conditionPassed(p, ins))
	{
		switch (get_bits(ins, 27, 25))
		{
			case 0b000:
				if (get_bit(ins, 7) && get_bit(ins, 4))
					return arm_load_store(p, ins);
				if (get_bit(ins, 24) && !get_bit(ins, 23) && !get_bit(ins, 20) && !get_bit(ins, 4))
					return arm_miscellaneous(p, ins);
				return arm_data_processing_shift(p, ins);
			case 0b001:
				return arm_data_processing_immediate_msr(p, ins);
			case 0b010:
				return arm_load_store(p, ins);
			case 0b011:
				return arm_load_store(p, ins);
			case 0b100:
				return arm_load_store_multiple(p, ins);
			case 0b101:
				return arm_branch(p, ins);
			case 0b111:
				return arm_coprocessor_others_swi(p, ins);
			default:
				return UNDEFINED_INSTRUCTION;
		}
	}
	return 0;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result)
        arm_exception(p, result);
    return result;
}
