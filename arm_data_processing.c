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

// valeur du shifter_carry_out nécessaire pour la mise à jour des flags
int shifter_carry_out = 0;

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
  if (verif_cond(p, ins)) {
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
    }
  }
  return UNDEFINED_INSTRUCTION;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

uint32_t decode_shifter_operand(arm_core p, uint32_t ins) {
  uint32_t shifter_operand;
  if (get_bit(ins, 25)) { // valeur immédiate avec rotaion
    int rotate_imm = get_bits(ins, 11, 8);
    int immed_8 = get_bits(ins, 7, 0);
    shifter_operand = rotate_right(immed_8, rotate_imm*2);
    // mis a jouer shifter shifter_carry_out
    if (rotate_imm == 0) {
      shifter_carry_out = get_bit(arm_read_cpsr(p), 31);
    }else {
      shifter_carry_out = get_bit(shifter_operand, 31);
    }
  }else if (get_bits(ins, 11, 4)==0) { // valeur dans un registre
    shifter_operand = shifter_operand_from_register(p, ins);
  }else if (get_bits(ins, 6, 4)==0) { // LSL immediate left shift
    if(get_bits(ins, 11, 6)==0) { // valeur dans registre
      shifter_operand = shifter_operand_from_register(p, ins);
    }else { // shift
      shifter_operand = shifter_operand_LSL_imm(p, ins);
    }
  }else if (get_bits(ins, 7, 4)==1) { // LSL register shift
    if (get_bits(ins, 7, 0)==0) {
      shifter_operand = shifter_operand_from_register(p, ins);
    }else if (get_bits(ins, 7, 0)<32) {
      shifter_operand = shifter_operand_LSL_imm(p, ins);
    }else {
      shifter_operand = shifter_operand_LSL_register(p, ins);
    }
  }else if (get_bits(ins, 6, 4)==2) { // LSR immediate shift
    shifter_operand = shifter_operand_LSR_imm(p, ins);
  }else if (get_bits(ins, 7, 4)==3) { // LSR register shift
    shifter_operand = shifter_operand_LSR_register(p, ins);
  }else if (get_bits(ins, 6, 4)==4) { // ASR immediate
    shifter_operand = shifter_operand_ASR_imm(p, ins);
  }else if (get_bits(ins, 7, 4)==5) {
    shifter_operand = shifter_operand_ASR_register(p, ins);
  }else if(get_bits(ins, 6, 4) == 6) { //ROR immediate ou RRX si shift_imm = 0
    shifter_operand = shifter_operand_ROR_imm(p,ins);
  } else if(get_bits(ins, 7, 4) == 7){
    shifter_operand = shifter_operand_ROR_register(p,ins);
  }
  return shifter_operand;
}

uint32_t shifter_operand_ASR_register(arm_core p, uint32_t ins) {
  uint32_t rs_register_number = get_bits(ins, 11, 8);
  uint32_t rm_register_number = get_bits(ins, 3, 0);
  uint32_t rs_value_7_0 = get_bits(arm_read_register(p, rs_register_number), 7, 0);
  uint32_t rm_value = arm_read_register(p, rm_register_number);
  if (rs_value_7_0 == 0) {
    shifter_carry_out = get_bit(arm_read_cpsr(p), C);
    return rm_value;
  }else if(rs_value_7_0 < 32) {
    shifter_carry_out = get_bit(rm_value, rs_value_7_0-1);
    return asr(rm_value, rs_value_7_0);
  }else {
    if(get_bit(rm_value, 31)==0) {
      shifter_carry_out=0;
      return 0;
    }else {
      shifter_carry_out = 1;
      return 0xFFFFFFFF;
    }
  }
}

uint32_t shifter_operand_ASR_imm(arm_core p, uint32_t ins) {
  uint32_t register_number = get_bits(ins, 3, 0);
  uint32_t rm_value = arm_read_register(p, register_number);
  uint32_t shift_imm = get_bits(ins, 11, 7);
  if (shift_imm == 0) {
    if (get_bit(rm_value, 31) == 0) {
      shifter_carry_out = 0;
      return 0;
    }else {
      shifter_carry_out = 1;
      return 0xFFFFFFFF;
    }
  }else {
    shifter_carry_out = get_bit(rm_value, shift_imm-1);
    return asr(rm_value, shift_imm);
  }
}

uint32_t shifter_operand_LSR_register(arm_core p, uint32_t ins) {
  int rs_register_number=get_bits(ins, 11, 8);
  int rm_register_number=get_bits(ins, 3, 0);
  uint32_t rm_value = arm_read_register(p, rm_register_number);
  uint32_t rs_value_7_0 = get_bits(arm_read_register(p, rs_register_number), 7, 0);
  if (rs_value_7_0==0) {
    shifter_carry_out = get_bit(arm_read_cpsr(p), C);
    return rm_value;
  }else if (rs_value_7_0<32) {
    shifter_carry_out = get_bit(rm_value, rs_value_7_0-1);
    return rm_value >> rs_value_7_0;
  }else if (rs_value_7_0==32) {
    shifter_carry_out = get_bit(rm_value, 31);
    return 0;
  }else {
    shifter_carry_out = 0;
    return 0;
  }
}

uint32_t shifter_operand_LSR_imm(arm_core p, uint32_t ins) {
  int register_number=get_bits(ins, 3, 0);
  int shift_imm = get_bits(ins, 11, 7);
  if (shift_imm==0) {
    shifter_carry_out = get_bit(arm_read_register(p, register_number), 31);
    return 0;
  }else {
    shifter_carry_out = get_bit(arm_read_register(p, register_number), shift_imm-1);
    return arm_read_register(p, register_number) >> shift_imm;
  }
}

uint32_t shifter_operand_from_register(arm_core p, uint32_t ins) {
  int register_number=get_bits(ins, 3, 0);
  shifter_carry_out = get_bit(arm_read_cpsr(p), 31);
  return arm_read_register(p, register_number);
}

uint32_t shifter_operand_LSL_imm(arm_core p, uint32_t ins) {
  int register_number=get_bits(ins, 3, 0);
  int shift_imm = get_bits(ins, 11, 7);
  shifter_carry_out = get_bit(arm_read_register(p, register_number), 32-shift_imm);
  return arm_read_register(p, register_number) << shift_imm;
}

uint32_t shifter_operand_LSL_register(arm_core p, uint32_t ins) {
  if (get_bits(ins, 7, 0)==32) {
    int register_number=get_bits(ins, 3, 0);
    shifter_carry_out = get_bit(arm_read_register(p, register_number), 0);
  }else {
    shifter_carry_out = 0;
  }
  return 0;
}

uint32_t shifter_operand_ROR_imm(arm_core p,uint32_t ins){
  uint8_t shift_imm = get_bits(ins,11,7);
  uint8_t RegRm = arm_read_register(p,get_bits(ins,3,0));
  uint32_t shifter_operand;
  if(shift_imm == 0)
    return shifter_operand_RRX(p,ins);
  else {
    shifter_operand = ror(RegRm,shift_imm);
    shifter_carry_out = get_bit(RegRm,shift_imm - 1);
    return shifter_operand;
  }
}

uint32_t shifter_operand_ROR_register(arm_core p,uint32_t ins){
  uint32_t RegRs = arm_read_register(p,get_bits(ins,11,8));
  uint32_t RegRm = arm_read_register(p,get_bits(ins,3,0));
  uint32_t shifter_operand;
  if(get_bits(RegRs,7,0) == 0){
    shifter_operand = RegRm;
    shifter_carry_out = get_bit(arm_read_cpsr(p),C);
  } else if(get_bits(RegRs,4,0) == 0){
    shifter_operand = RegRm;
    shifter_carry_out = get_bit(RegRm,31);
  } else {
    shifter_operand = ror(RegRm,get_bits(RegRs,4,0));
    shifter_carry_out = get_bit(RegRm,get_bits(RegRs,4,0)-1);
  }
  return shifter_operand;
}

uint32_t shifter_operand_RRX(arm_core p,uint32_t ins){
  uint32_t shifter_operand = (get_bit(arm_read_cpsr(p),C) << 31) | (arm_read_register(p,get_bits(ins,3,0)) >> 1);
  shifter_carry_out = get_bit(arm_read_register(p,get_bits(ins,3,0)),0);
  return shifter_operand;
}

int mov(arm_core p, uint32_t ins) {
  int operand_value = decode_shifter_operand(p, ins);
  int dest_register = get_bits(ins, 15, 12);
  arm_write_register(p, dest_register, operand_value);

  // mise a jour des flags si nécessaire
  if (get_bit(ins, 20) && dest_register==15) {
    if (arm_current_mode_has_spsr(p)) {
      arm_write_cpsr(p, arm_read_spsr(p));
    }else {
      return UNDEFINED_INSTRUCTION;
    }
  }else if (get_bit(ins, 20)) {
    uint32_t new_cpsr = arm_read_cpsr(p);

    if (get_bit(operand_value, 31)) {
      new_cpsr = set_bit(new_cpsr, N);
    }else {
      new_cpsr = clr_bit(new_cpsr, N);
    }

    if (operand_value) {
      new_cpsr = clr_bit(new_cpsr, Z);
    }else {
      new_cpsr = set_bit(new_cpsr, Z);
    }

    if (shifter_carry_out) {
      new_cpsr = set_bit(new_cpsr, C);
    }else {
      new_cpsr = clr_bit(new_cpsr, C);
    }

    arm_write_cpsr(p, new_cpsr);
  }
  return 0;

}

int add(arm_core p, uint32_t ins) {
  int source_register = get_bits(ins, 19, 16);
  uint32_t source_value = arm_read_register(p, source_register);

  // récupération de la valeur de l'opérande en fonction de I
  uint32_t operand_value = decode_shifter_operand(p, ins);

  uint32_t operation_result = source_value+operand_value;
  printf("%u + %u = %u\n", source_value, operand_value, operation_result);

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
    uint32_t new_cpsr = arm_read_cpsr(p);

    if (get_bit(operation_result, 31)) {
      new_cpsr = set_bit(new_cpsr, N);
    }else {
      new_cpsr = clr_bit(new_cpsr, N);
    }

    if (operation_result) {
      new_cpsr = clr_bit(new_cpsr, Z);
    }else {
      new_cpsr = set_bit(new_cpsr, Z);
    }

    if (carry_from(source_value, operand_value)) {
      new_cpsr = set_bit(new_cpsr, C);
    }else {
      new_cpsr = clr_bit(new_cpsr, C);
    }

    if (overflow_from(source_value, operand_value, ADD)) {
      new_cpsr = set_bit(new_cpsr, V);
    }else {
      new_cpsr = clr_bit(new_cpsr, V);
    }

    arm_write_cpsr(p, new_cpsr);
  }
  return 0;
}
