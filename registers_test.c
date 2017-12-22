#include <stdio.h>
#include "registers.c"

uint8_t get_mode_from_num(int num)
{
	switch (num)
	{
		case 0:
			return USR;
		case 1:
			return FIQ;
		case 2:
			return IRQ;
		case 3:
			return SVC;
		case 4:
			return ABT;
		case 5:
			return UND;
		default:
			return SYS;
	}
}

int main ()
{
	registers r = registers_create();
	for (int i = 0; i < NB_PHYS_REGS; i++)
		r->registers[i] = i;
	for (int i = 0; i < NB_UNBANKED_REGS; i++)
	{
		for (int j = 0; j < NB_MODES; j++)
		{
			uint32_t value;
			r->mode = get_mode_from_num(j);
			value = read_register(r, i);
			for (int k = j+1; k < NB_MODES; k++)
			{
				r->mode = get_mode_from_num(k);
				if (read_register(r, i) != value)
					printf("Erreur!\n");
			}
		}
	}
	for (int i = NB_UNBANKED_REGS; i < NB_UNBANKED_REGS+NB_2BANKED_REGS; i++)
	{
		for (int j = 0; j < NB_MODES; j++)
		{
			uint32_t value;
			r->mode = get_mode_from_num(j);
			value = read_register(r, i);
			for (int k = j+1; k < NB_MODES; k++)
			{
				r->mode = get_mode_from_num(k);
				if (read_register(r, i) != value && get_mode_from_num(j) != FIQ && get_mode_from_num(k) != FIQ)
					printf("Erreur!\n");
			}
		}
	}
	for (int i = NB_UNBANKED_REGS+NB_2BANKED_REGS; i < NB_UNBANKED_REGS+NB_2BANKED_REGS+NB_6BANKED_REGS; i++)
	{
		for (int j = 0; j < NB_MODES; j++)
		{
			uint32_t value;
			r->mode = get_mode_from_num(j);
			value = read_register(r, i);
			for (int k = j+1; k < NB_MODES; k++)
			{
				r->mode = get_mode_from_num(k);
				if (read_register(r, i) == value && (get_mode_from_num(j) != USR || get_mode_from_num(k) != SYS))
					printf("Erreur!\n");
			}
		}
	}
	for (int i = NB_UNBANKED_REGS+NB_2BANKED_REGS+NB_6BANKED_REGS; i < 17; i++)
	{
		for (int j = 0; j < NB_MODES; j++)
		{
			uint32_t value;
			r->mode = get_mode_from_num(j);
			value = read_register(r, i);
			for (int k = j+1; k < NB_MODES; k++)
			{
				r->mode = get_mode_from_num(k);
				if (read_register(r, i) != value)
					printf("Erreur!\n");
			}
		}
	}
	for (int j = 0; j < NB_MODES; j++)
	{
		r->mode = get_mode_from_num(j);
		if (current_mode_has_spsr(r) && (get_mode(r) == USR || get_mode(r) == SYS))
			printf("Erreur!\n");
	}
	for (int j = 1; j <= 5; j++)
	{
		uint32_t value;
		r->mode = get_mode_from_num(j);
		value = read_register(r, 17);
		for (int k = j+1; k <= 5; k++)
		{
			r->mode = get_mode_from_num(k);
			if (read_register(r, 17) == value)
				printf("Erreur!\n");
		}
	}
	registers_destroy(r);
	return 0;
}
