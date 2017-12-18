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
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

#define nb_unbanked_regs (8)
#define nb_2banked_regs (5)
#define nb_6banked_regs (2)
#define nb_pc_regs (1)
#define nb_gp_regs (nb_unbanked_regs + 2 * nb_2banked_regs + 6 * nb_6banked_regs + nb_pc_regs)
#define cpsr_num (nb_gp_regs)
#define spsr_num (nb_gp_regs+1)

struct registers_data {
    uint32_t *regs_values;
    uint8_t mode;
};

registers registers_create() {
    registers r = malloc(sizeof(struct registers_data));
    r->regs_values = malloc((nb_gp_regs+2)*sizeof(uint32_t));
    r->mode = SYS;
    return r;
}

void registers_destroy(registers r) {
    free(r->regs_values);
    free(r);
}

uint8_t get_mode(registers r) {
    return r->mode;
} 

int current_mode_has_spsr(registers r) {
    return (r->mode == FIQ || r->mode == IRQ || r->mode == SVC || r->mode == ABT || r->mode == UND);
}

int in_a_privileged_mode(registers r) {
    return (r->mode == FIQ || r->mode == IRQ || r->mode == SVC || r->mode == ABT || r->mode == UND || r->mode == SYS);
}

uint32_t read_register(registers r, uint8_t reg) {
    uint32_t value;
    if (reg >= nb_unbanked_regs)
    {
        if (reg < nb_unbanked_regs + nb_2banked_regs)
        {
            if (r->mode == FIQ)
                value = r->regs_values[reg + nb_2banked_regs];
            else
                value = r->regs_values[reg];
        }
        else {
            if (reg < nb_unbanked_regs + nb_2banked_regs + nb_6banked_regs)
            {
                switch(r->mode)
                {
                    case FIQ:
                        value = r->regs_values[reg + nb_2banked_regs + 1*nb_6banked_regs];
                        break;
                    case IRQ:
                        value = r->regs_values[reg + nb_2banked_regs + 2*nb_6banked_regs];
                        break;
                    case SVC:
                        value = r->regs_values[reg + nb_2banked_regs + 3*nb_6banked_regs];
                        break;
                    case ABT:
                        value = r->regs_values[reg + nb_2banked_regs + 4*nb_6banked_regs];
                        break;
                    case UND:
                        value = r->regs_values[reg + nb_2banked_regs + 5*nb_6banked_regs];
                        break;
                    default:
                        value = r->regs_values[reg + nb_2banked_regs + 0*nb_6banked_regs];
                }
            }
            else
            {
                value = r->regs_values[reg + nb_2banked_regs + 5*nb_6banked_regs];
            }
        }
    }
    else
    {
        value = r->regs_values[reg];
    }
    return value;
}

uint32_t read_usr_register(registers r, uint8_t reg) {
    uint8_t pmode = r->mode;
    r->mode = USR;
    uint32_t value=read_register(r, reg);
    r->mode = pmode;
    return value;
}

uint32_t read_cpsr(registers r) {
    uint32_t value=r->regs_values[cpsr_num];
    return value;
}

uint32_t read_spsr(registers r) {
    uint32_t value=r->regs_values[spsr_num];
    return value;
}

void write_register(registers r, uint8_t reg, uint32_t value) {
    if (reg >= nb_unbanked_regs)
    {
        if (reg < nb_unbanked_regs + nb_2banked_regs)
        {
            if (r->mode == FIQ)
                r->regs_values[reg + nb_2banked_regs] = value;
            else
                r->regs_values[reg] = value;
        }
        else {
            if (reg < nb_unbanked_regs + nb_2banked_regs + nb_6banked_regs)
            {
                switch(r->mode)
                {
                    case FIQ:
                        r->regs_values[reg + nb_2banked_regs + 1*nb_6banked_regs] = value;
                        break;
                    case IRQ:
                        r->regs_values[reg + nb_2banked_regs + 2*nb_6banked_regs] = value;
                        break;
                    case SVC:
                        r->regs_values[reg + nb_2banked_regs + 3*nb_6banked_regs] = value;
                        break;
                    case ABT:
                        r->regs_values[reg + nb_2banked_regs + 4*nb_6banked_regs] = value;
                        break;
                    case UND:
                        r->regs_values[reg + nb_2banked_regs + 5*nb_6banked_regs] = value;
                        break;
                    default:
                        r->regs_values[reg + nb_2banked_regs + 0*nb_6banked_regs] = value;
                }
            }
            else
            {
                r->regs_values[reg + nb_2banked_regs + 5*nb_6banked_regs] = value;
            }
        }
    }
    else
    {
        r->regs_values[reg] = value;
    }
}

void write_usr_register(registers r, uint8_t reg, uint32_t value) {
    uint8_t pmode = r->mode;
    r->mode = USR;
    write_register(r, reg, value);
    r->mode = pmode;
}

void write_cpsr(registers r, uint32_t value) {
    r->regs_values[cpsr_num] = value;
}

void write_spsr(registers r, uint32_t value) {
    r->regs_values[spsr_num] = value;
}
