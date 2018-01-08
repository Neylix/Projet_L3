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
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

#define NB_PHYS_REGS (37)
#define NB_VIRT_REGS (18)
#define NB_UNBANKED_REGS (8)
#define NB_2BANKED_REGS (5)
#define NB_6BANKED_REGS (2)
#define NB_MODES (7)

struct registers_data {
	uint8_t mode;
	uint32_t registers[NB_PHYS_REGS];
	uint32_t *virtual_regs[NB_MODES*NB_VIRT_REGS];
};

int get_mode_num(registers r)
{
	switch (r->mode)
	{
		case USR:
			return 0;
		case FIQ:
			return 1;
		case IRQ:
			return 2;
		case SVC:
			return 3;
		case ABT:
			return 4;
		case UND:
			return 5;
		default:
			return 6;
	}
}

registers registers_create() {
    registers r = malloc(sizeof(struct registers_data));
	r->mode = SYS;
	//USR
	for (int i = 0; i <= 7; i++)
		r->virtual_regs[0*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 8; i <= 12; i++)
		r->virtual_regs[0*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 13; i <= 14; i++)
		r->virtual_regs[0*NB_VIRT_REGS+i] = &(r->registers[i+NB_2BANKED_REGS]);
	r->virtual_regs[0*NB_VIRT_REGS+15] = &(r->registers[15+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[0*NB_VIRT_REGS+16] = &(r->registers[16+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[0*NB_VIRT_REGS+17] = NULL;
	//FIQ
	for (int i = 0; i <= 7; i++)
		r->virtual_regs[1*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 8; i <= 12; i++)
		r->virtual_regs[1*NB_VIRT_REGS+i] = &(r->registers[i+NB_2BANKED_REGS]);
	for (int i = 13; i <= 14; i++)
		r->virtual_regs[1*NB_VIRT_REGS+i] = &(r->registers[i+NB_2BANKED_REGS+NB_6BANKED_REGS]);
	r->virtual_regs[1*NB_VIRT_REGS+15] = &(r->registers[15+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[1*NB_VIRT_REGS+16] = &(r->registers[16+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[1*NB_VIRT_REGS+17] = &(r->registers[17+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	//IRQ
	for (int i = 0; i <= 7; i++)
		r->virtual_regs[2*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 8; i <= 12; i++)
		r->virtual_regs[2*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 13; i <= 14; i++)
		r->virtual_regs[2*NB_VIRT_REGS+i] = &(r->registers[i+NB_2BANKED_REGS+2*NB_6BANKED_REGS]);
	r->virtual_regs[2*NB_VIRT_REGS+15] = &(r->registers[15+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[2*NB_VIRT_REGS+16] = &(r->registers[16+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[2*NB_VIRT_REGS+17] = &(r->registers[17+NB_2BANKED_REGS+5*NB_6BANKED_REGS+1]);
	//SVC
	for (int i = 0; i <= 7; i++)
		r->virtual_regs[3*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 8; i <= 12; i++)
		r->virtual_regs[3*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 13; i <= 14; i++)
		r->virtual_regs[3*NB_VIRT_REGS+i] = &(r->registers[i+NB_2BANKED_REGS+3*NB_6BANKED_REGS]);
	r->virtual_regs[3*NB_VIRT_REGS+15] = &(r->registers[15+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[3*NB_VIRT_REGS+16] = &(r->registers[16+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[3*NB_VIRT_REGS+17] = &(r->registers[17+NB_2BANKED_REGS+5*NB_6BANKED_REGS+2]);
	//ABT
	for (int i = 0; i <= 7; i++)
		r->virtual_regs[4*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 8; i <= 12; i++)
		r->virtual_regs[4*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 13; i <= 14; i++)
		r->virtual_regs[4*NB_VIRT_REGS+i] = &(r->registers[i+NB_2BANKED_REGS+4*NB_6BANKED_REGS]);
	r->virtual_regs[4*NB_VIRT_REGS+15] = &(r->registers[15+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[4*NB_VIRT_REGS+16] = &(r->registers[16+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[4*NB_VIRT_REGS+17] = &(r->registers[17+NB_2BANKED_REGS+5*NB_6BANKED_REGS+3]);
	//UND
	for (int i = 0; i <= 7; i++)
		r->virtual_regs[5*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 8; i <= 12; i++)
		r->virtual_regs[5*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 13; i <= 14; i++)
		r->virtual_regs[5*NB_VIRT_REGS+i] = &(r->registers[i+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[5*NB_VIRT_REGS+15] = &(r->registers[15+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[5*NB_VIRT_REGS+16] = &(r->registers[16+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[5*NB_VIRT_REGS+17] = &(r->registers[17+NB_2BANKED_REGS+5*NB_6BANKED_REGS+4]);
	//SYS
	for (int i = 0; i <= 7; i++)
		r->virtual_regs[6*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 8; i <= 12; i++)
		r->virtual_regs[6*NB_VIRT_REGS+i] = &(r->registers[i]);
	for (int i = 13; i <= 14; i++)
		r->virtual_regs[6*NB_VIRT_REGS+i] = &(r->registers[i+NB_2BANKED_REGS]);
	r->virtual_regs[6*NB_VIRT_REGS+15] = &(r->registers[15+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[6*NB_VIRT_REGS+16] = &(r->registers[16+NB_2BANKED_REGS+5*NB_6BANKED_REGS]);
	r->virtual_regs[6*NB_VIRT_REGS+17] = NULL;
	for (int i = 0; i < NB_PHYS_REGS; i++)
		r->registers[i] = 0;
  return r;
}

void registers_destroy(registers r) {
	free(r);
}

uint8_t get_mode(registers r) {
    return r->mode;
}

int current_mode_has_spsr(registers r) {
    return (r->virtual_regs[get_mode_num(r)*NB_VIRT_REGS+17] != NULL);
}

int in_a_privileged_mode(registers r) {
    return (get_mode_num(r) != 0);
}

uint32_t read_register(registers r, uint8_t reg) {
    uint32_t value = *(r->virtual_regs[get_mode_num(r)*NB_VIRT_REGS+reg]);
    return value;
}

uint32_t read_usr_register(registers r, uint8_t reg) {
    uint32_t value = *(r->virtual_regs[reg]);
    return value;
}

uint32_t read_cpsr(registers r) {
    uint32_t value = *(r->virtual_regs[get_mode_num(r)*NB_VIRT_REGS+16]);
    return value;
}

uint32_t read_spsr(registers r) {
    uint32_t value = *(r->virtual_regs[get_mode_num(r)*NB_VIRT_REGS+17]);
    return value;
}

void write_register(registers r, uint8_t reg, uint32_t value) {
	*(r->virtual_regs[get_mode_num(r)*NB_VIRT_REGS+reg]) = value;
}

void write_usr_register(registers r, uint8_t reg, uint32_t value) {
	*(r->virtual_regs[reg]) = value;
}

void write_cpsr(registers r, uint32_t value) {
	*(r->virtual_regs[get_mode_num(r)*NB_VIRT_REGS+16]) = value;
}

void write_spsr(registers r, uint32_t value) {
	*(r->virtual_regs[get_mode_num(r)*NB_VIRT_REGS+17]) = value;
}
