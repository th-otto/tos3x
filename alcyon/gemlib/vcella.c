#include "vdilib.h"

VOID v_cellarray(P(int16_t) handle, P(int16_t *) xy, P(int16_t) row_length, P(int16_t) el_per_row, P(int16_t) num_rows, P(int16_t) wr_mode, P(int16_t *) colors)
PP(int16_t handle;)
PP(int16_t *xy;)
PP(int16_t row_length;)
PP(int16_t el_per_row;)
PP(int16_t num_rows;)
PP(int16_t wr_mode;)
PP(int16_t *colors;)
{
	i_intin(colors);
	i_ptsin(xy);

	contrl[0] = 10;
	contrl[1] = 2;
	contrl[3] = row_length * num_rows;
	contrl[6] = handle;
	contrl[7] = row_length;
	contrl[8] = el_per_row;
	contrl[9] = num_rows;
	contrl[10] = wr_mode;
	vdi();

	i_intin(intin);
	i_ptsin(ptsin);
}

