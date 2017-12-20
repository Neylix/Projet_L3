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
#include <stdlib.h>
#include "memory.h"
#include "util.h"

struct memory_data {
	size_t size;
	int is_big_endian;
	uint8_t *memory;
};

memory memory_create(size_t size, int is_big_endian) {
    memory mem = malloc(sizeof(struct memory_data));
	mem->size = size;
	mem->is_big_endian = is_big_endian;
	mem->memory = malloc(size*sizeof(uint8_t));
    return mem;
}

size_t memory_get_size(memory mem) {
    return mem->size;
}

void memory_destroy(memory mem) {
	free(mem->memory);
	free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
	if (address >= mem->size)
		return -1;
    *value = mem->memory[address];
	return 0;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
	if (address >= mem->size-1)
		return -1;
	if (is_big_endian() == mem->is_big_endian)
		*value = *((uint16_t *)(&(mem->memory[address])));
	else
		*value = reverse_2(*((uint16_t *)(&(mem->memory[address]))));
    return 0;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
	if (address >= mem->size-3)
		return -1;
	if (is_big_endian() == mem->is_big_endian)
		*value = *((uint32_t *)(&(mem->memory[address])));
	else
		*value = reverse_4(*((uint32_t *)(&(mem->memory[address]))));
    return 0;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
	if (address >= mem->size)
		return -1;
	mem->memory[address] = value;
    return 0;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
	if (address >= mem->size-1)
		return -1;
	if (is_big_endian() == mem->is_big_endian)
		*((uint16_t *)(&(mem->memory[address]))) = value;
	else
		*((uint16_t *)(&(mem->memory[address]))) = reverse_2(value);
    return 0;
}

int memory_write_word(memory mem, uint32_t address, uint32_t value) {
	if (address >= mem->size-3)
		return -1;
	if (is_big_endian() == mem->is_big_endian)
		*((uint32_t *)(&(mem->memory[address]))) = value;
	else
		*((uint32_t *)(&(mem->memory[address]))) = reverse_4(value);
    return 0;
}
