#include "aeslib.h"

int16_t wind_calc(
	P(int16_t) wctype, P(int16_t) kind,
	P(int16_t) x, P(int16_t) y, P(int16_t) w, P(int16_t) h,
	P(int16_t *) px, P(int16_t *) py, P(int16_t *) pw, P(int16_t *) ph)
PP(int16_t wctype;)
PP(int16_t kind;)
PP(int16_t x;)
PP(int16_t y;)
PP(int16_t w;)
PP(int16_t h;)

PP(int16_t *px;)
PP(int16_t *py;)
PP(int16_t *pw;)
PP(int16_t *ph;)
{
	WM_WCTYPE = wctype;
	WM_WCKIND = kind;
	WM_WCIX = x;
	WM_WCIY = y;
	WM_WCIW = w;
	WM_WCIH = h;
	crys_if(WIND_CALC);
	*px = WM_WCOX;
	*py = WM_WCOY;
	*pw = WM_WCOW;
	*ph = WM_WCOH;
	return RET_CODE;
}
