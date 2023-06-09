/************************************************************************
 * FILE: 	SUPER.C                                               
 * DATE:        March 12, 1990                                        
 * DESCRIPTION:                                   
 ************************************************************************/

/************************************************************************/
/* INCLUDE FILES							*/
/************************************************************************/
#include "gemskel.h"
#include <tos.h>


/************************************************************************/
/* PROTOTYPES								*/
/************************************************************************/
void Enter_Super(void);
void Exit_Super(void);


/************************************************************************/
/* DEFINES								*/
/************************************************************************/


/************************************************************************/
/* EXTERNALS								*/
/************************************************************************/



/************************************************************************/
/* GLOBALS								*/
/************************************************************************/
long oldssp;



void Enter_Super(void)
{
	if (Super((void *) 1L) == 0L)
		oldssp = Super((void *) 0L);
	else
		oldssp = 0L;
}



void Exit_Super(void)
{
	if (oldssp != 0L)
		Super((void *) oldssp);
}
