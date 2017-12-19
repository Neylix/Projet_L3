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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
  if (get_bits(ins, 31, 28) != 0b1111) {
    switch (get_bits(ins, 24, 21)) {
      case 0b0100:
        return add(p, ins);
        break;
      case 0b1101:
        return mov(p, ins);
        break;
      default:
        return UNDEFINED_INSTRUCTION;
    }
  }else {

  }
  return UNDEFINED_INSTRUCTION;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int decode_shifter_operand(arm_core p, uint32_t ins) {
  if (get_bit(ins, 25)) { // valeur immédiate avec rotaion
    int rotate_imm = get_bits(ins, 11, 8)*2;
    int immed_8 = get_bits(ins, 7, 0);
    return rotate_right(immed_8, rotate_imm);
  }else if (!get_bit(ins, 25) && (get_bits(ins, 11, 4)==0)) { // valeur immédiate avec shift
    int register_number=get_bits(ins, 3, 0);
    return arm_read_register(p, register_number);
  }
  return 0;
}

int mov(arm_core p, uint32_t ins) {
  int operand_value = decode_shifter_operand(p, ins);
  int dest_register = get_bits(ins, 15, 12);
  return arm_write_register(p, dest_register, operand_value);
}

int add(arm_core p, uint32_t ins) {
  int source_register = get_bits(ins, 19, 16);
  int source_value = arm_read_register(p, source_register);

  // récupération de la valeur de l'opérande en fonction de I
  int operand_value = decode_shifter_operand(p, ins);

  int operation_result = source_value+operand_value;

  //écriture du résultat dans le registre destination
  int dest_register = get_bits(ins, 15, 12);
  arm_write_register(p, dest_register, operation_result);

  // mise a jour des flags si nécessaire
  if (get_bit(ins, 20) && dest_register==15) {
    if (arm_current_mode_has_spsr(p)) {
      arm_write_cpsr(p, arm_read_spsr(p));
    }else {
      return UNDEFINED_INSTRUCTION;
    }
  }else if (get_bit(ins, 20)) {
    int new_cpsr = arm_read_cpsr(p);

    if (get_bit(operation_result, 31)) {
      new_cpsr = set_bit(new_cpsr, N);
    }else {
      new_cpsr = clr_bit(new_cpsr, N);
    }

    if(operation_result) {
      new_cpsr = clr_bit(new_cpsr, N);
    }else {
      new_cpsr = set_bit(new_cpsr, N);
    }

    arm_write_cpsr(p, new_cpsr);
  }
  return 0;
}
