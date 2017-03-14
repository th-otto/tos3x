#include "aeslib.h"

int16_t graf_mouse(P(int16_t) m_number, P(const MFORM *) m_addr)
PP(int16_t m_number;)
PP(const MFORM *m_addr;)
{
	GR_MNUMBER = m_number;
	GR_MADDR = NO_CONST(m_addr);
	return crys_if(GRAF_MOUSE);
}
