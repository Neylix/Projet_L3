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
#include "util.h"
#include <math.h>
#include "arm_constants.h"

/* We implement asr because shifting a signed is non portable in ANSI C */
uint32_t asr(uint32_t value, uint8_t shift) {
    return (value >> shift) | (get_bit(value, 31) ? ~0<<(32-shift) : 0);
}

uint32_t ror(uint32_t value, uint8_t rotation) {
    return (value >> rotation) | (value << (32-rotation));
}

int is_big_endian() {
    static uint32_t one = 1;
    return ((* (uint8_t *) &one) == 0);
}

uint32_t rotate_right(uint32_t value, uint32_t rotation) {
  uint32_t y = value >> rotation;
  uint32_t z = value << (32 - rotation);
  return y | z;
}

uint32_t carry_from(uint32_t x, uint32_t y) {
  if ((x+y)>pow(2, 32)-1) {
    return 1;
  }else {
    return 0;
  }
}

uint32_t overflow_from(uint32_t x, uint32_t y, char op) {
  if (op==ADD) {
    uint32_t res = x+y;
    if ((get_bit(x, 31) == get_bit(y, 31)) && (get_bit(x, 31) != get_bit(res, 31))) {
      return 1;
    }else {
      return 0;
    }
  }else {
    uint32_t res = x-y;
    if ((get_bit(x, 31) != get_bit(y, 31)) && (get_bit(x, 31) != get_bit(res, 31))) {
      return 1;
    }else {
      return 0;
    }
  }
}

int verif_cond(uint32_t value) {
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
