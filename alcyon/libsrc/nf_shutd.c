#include "lib.h"
#include <mint/arch/nf_ops.h>


VOID nf_shutdown(NOTHING)
{
	struct nf_ops *nf_ops;

	if ((nf_ops = nf_init()) != NULL)
	{
		long shutdown_id = NF_GET_ID(nf_ops, NF_ID_SHUTDOWN);
		
		if (shutdown_id)
        	(*nf_ops->call)(shutdown_id | 0);
	}
}
