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
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include <debug.h>
#include <stdlib.h>


int arm_branch(arm_core p, uint32_t ins) {
    uint32_t signed_immed_24 = get_bits(ins,23,0);
    uint32_t address;
    int sign = get_bit(signed_immed_24,23);
    clr_bit(signed_immed_24, 23);
    if(sign)
      set_bit(signed_immed_24, 29);
    if(get_bit(ins,24))
      {
        address = arm_read_register(p, 15);
        arm_write_register(p,14,address);
      }
    arm_write_register(p,15,arm_read_register(p,15)+(signed_immed_24<<2));
    return 0;
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins) {
    if (get_bit(ins, 24)) {
        /* Here we implement the end of the simulation as swi 0x123456 */
        if ((ins & 0xFFFFFF) == 0x123456)
            exit(0);
        return SOFTWARE_INTERRUPT;
    }
    return UNDEFINED_INSTRUCTION;
}

int arm_miscellaneous(arm_core p, uint32_t ins) {
    switch (get_bits(ins,22,21))) {
      case 0b01:
        switch (get_bits(4,7)) {
          case 0b0011: //BLX(2)
            if(get_bits(ins,1,0) == 0b10)
              return UNDEFINED_INSTRUCTION;
            uint32_t target,address,cpsr;
            target = arm_read_register(p,get_bits(ins,3,0));
            address = arm_read_register(p, 15);
            arm_write_register(p,14,address);
            cpsr = arm_read_cpsr(p);
            if (get_bit(target,0)) {
              set_bit(cpsr,5);  //PAS SUR SI BIT T est 5.PAGE 92 dans doc
            }
            else
              clr_bit(cpsr,5);
            arm_write_cpsr(p, cpsr);
            arm_write_register(p,15,target & 0xFFFFFFFE);
            break;
          case 0b0001: //BX
            if(get_bits(ins,1,0) == 0b10)
              return UNDEFINED_INSTRUCTION;//not allowed
            uint32_t target,cpsr;
            target = arm_read_register(p,get_bits(ins,3,0));
            cpsr = arm_read_cpsr(p);
            if (get_bit(ins,0)) {  //ou get_bit(target,0)
              set_bit(cpsr,5);
            }
            else
              clr_bit(cpsr,5);
            arm_write_cpsr(p, cpsr);
            arm_write_register(p,15,target & 0xFFFFFFFE);
            break;
          case 0b0001: //BXJ
            return UNDEFINED_INSTRUCTION;//PAS DEFINI POUR Version5
          }
      case 0b11:
        if(get_bits(ins,7,4) == 0b0001) //CLZ
        {
          uint32_t Rm,Rd;
          Rm = arm_read_register(p,get_bits(ins,3,0));
          //Rd = arm_read_register(p,get_bits(ins,15,12));
          if(Rm == 0)
            arm_write_register(p,get_bits(ins,15,12),32);
          else
          {
            int i = -1;
            while(Rm>>i)
              i++;
            arm_write_register(p,get_bits(ins,15,12),31-i);
          }
        }
    }
    return 0;
}
