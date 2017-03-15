#include "vdilib.h"

VOID vq_cellarray(P(int16_t) handle, P(int16_t *) xy, P(int16_t) row_len, P(int16_t) num_rows, P(int16_t *) el_used, P(int16_t *) rows_used, P(int16_t *) stat, P(int16_t *) colors)
PP(int16_t handle;)
PP(int16_t *xy;)
PP(int16_t row_len;)
PP(int16_t num_rows;)
PP(int16_t *el_used;)
PP(int16_t *rows_used;)
PP(int16_t *stat;)
PP(int16_t *colors;)
{
	i_ptsin(xy);
	i_intout(colors);

	contrl[0] = 27;
	contrl[1] = 2;
	contrl[3] = 0;
	contrl[6] = handle;
	contrl[7] = row_len;
	contrl[8] = num_rows;
	vdi();

	*el_used = contrl[9];
	*rows_used = contrl[10];
	*stat = contrl[11];
	i_ptsin(ptsin);
	i_intout(intout);
}

