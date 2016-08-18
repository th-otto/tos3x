*
*	control array for vdibind
*
	.globl	_ctrl_cnts
	.data
    	.even
_ctrl_cnts:			 	*	Application Manager
	.dc.b	0, 1, 0			* func 010
    .dc.b	2, 1, 1			* func 011
    .dc.b	2, 1, 1 		* func 012
	.dc.b	0, 1, 1			* func 013
	.dc.b	2, 1, 1			* func 014
	.dc.b	1, 1, 1			* func 015
	.dc.b	0, 0, 0			* func 016
	.dc.b	0, 0, 0			* func 017
	.dc.b	0, 0, 0			* func 018
	.dc.b	0, 1, 0			* func 019
*							 Event Manager
	.dc.b	0, 1, 0			* func 020
	.dc.b	3, 5, 0			* func 021
	.dc.b	5, 5, 0			* func 022
	.dc.b	0, 1, 1			* func 023
	.dc.b	2, 1, 0			* func 024
	.dc.b	16, 7, 1 		* func 025
	.dc.b	2, 1, 0			* func 026
	.dc.b	0, 0, 0			* func 027
	.dc.b	0, 0, 0			* func 028
	.dc.b	0, 0, 0			* func 029
*							 Menu Manager
	.dc.b	1, 1, 1			* func 030
	.dc.b	2, 1, 1			* func 031
	.dc.b	2, 1, 1			* func 032
	.dc.b	2, 1, 1			* func 033
	.dc.b	1, 1, 2			* func 034
	.dc.b	1, 1, 1			* func 035
	.dc.b	0, 0, 0			* func 036
	.dc.b	0, 0, 0			* func 037
	.dc.b	0, 0, 0			* func 038
	.dc.b	0, 0, 0			* func 039
*							 Object Manager
	.dc.b	2, 1, 1			* func 040
	.dc.b	1, 1, 1			* func 041
  	.dc.b	6, 1, 1			* func 042
	.dc.b	4, 1, 1			* func 043
	.dc.b	1, 3, 1			* func 044
	.dc.b	2, 1, 1			* func 045
	.dc.b	4, 2, 1			* func 046
	.dc.b	8, 1, 1			* func 047
	.dc.b	0, 0, 0			* func 048
	.dc.b	0, 0, 0			* func 049
*							 Form Manager
	.dc.b	1, 1, 1			* func 050
	.dc.b	9, 1, 1			* func 051
	.dc.b	1, 1, 1			* func 052
	.dc.b	1, 1, 0			* func 053
	.dc.b	0, 5, 1			* func 054
	.dc.b	0, 0, 0			* func 055
	.dc.b	0, 0, 0			* func 056
	.dc.b	0, 0, 0			* func 057
	.dc.b	0, 0, 0			* func 058
	.dc.b	0, 0, 0			* func 059
*							 Dialog Manager
	.dc.b	0, 0, 0			* func 060
	.dc.b	0, 0, 0			* func 061
	.dc.b	0, 0, 0			* func 062
	.dc.b	0, 0, 0			* func 063
	.dc.b	0, 0, 0			* func 064
	.dc.b	0, 0, 0			* func 065
	.dc.b	0, 0, 0			* func 066
	.dc.b	0, 0, 0			* func 067
	.dc.b	0, 0, 0			* func 068
	.dc.b	0, 0, 0			* func 069
*							Graphics Manager
	.dc.b	4, 3, 0			* func 070
	.dc.b	8, 3, 0			* func 071
	.dc.b	6, 1, 0			* func 072
	.dc.b	8, 1, 0			* func 073
	.dc.b	8, 1, 0			* func 074
	.dc.b	4, 1, 1			* func 075
	.dc.b	3, 1, 1			* func 076
	.dc.b	0, 5, 0			* func 077
	.dc.b	1, 1, 1			* func 078
	.dc.b	0, 5, 0			* func 079
*							Scrap Manager
	.dc.b	0, 1, 1			* func 080
	.dc.b	0, 1, 1			* func 081
	.dc.b	0, 0, 0			* func 082
	.dc.b	0, 0, 0			* func 083
	.dc.b	0, 0, 0			* func 084
	.dc.b	0, 0, 0			* func 085
	.dc.b	0, 0, 0			* func 086
	.dc.b	0, 0, 0			* func 087
	.dc.b	0, 0, 0			* func 088
	.dc.b	0, 0, 0			* func 089
*							fseler Manager
	.dc.b	0, 2, 2			* func 090
	.dc.b	0, 0, 0			* func 091
	.dc.b	0, 0, 0			* func 092
	.dc.b	0, 0, 0			* func 093
	.dc.b	0, 0, 0			* func 094
	.dc.b	0, 0, 0			* func 095
	.dc.b	0, 0, 0			* func 096
	.dc.b	0, 0, 0			* func 097
	.dc.b	0, 0, 0			* func 098
	.dc.b	0, 0, 0 		* func 099
*							Window Manager
	.dc.b	5, 1, 0			* func 100
	.dc.b	5, 1, 0			* func 101
	.dc.b	1, 1, 0			* func 102
	.dc.b	1, 1, 0			* func 103
	.dc.b	2, 5, 0			* func 104
	.dc.b	6, 1, 0			* func 105
	.dc.b	2, 1, 0			* func 106
	.dc.b	1, 1, 0			* func 107
	.dc.b	6, 5, 0			* func 108
	.dc.b	0, 0, 0 		* func 109
*							Resource Manger
	.dc.b	0, 1, 1			* func 110
	.dc.b	0, 1, 0			* func 111
	.dc.b	2, 1, 0			* func 112
	.dc.b	2, 1, 1			* func 113
	.dc.b	1, 1, 1			* func 114
	.dc.b	0, 0, 0			* func 115
	.dc.b	0, 0, 0			* func 116
	.dc.b	0, 0, 0			* func 117
	.dc.b	0, 0, 0			* func 118
	.dc.b	0, 0, 0			* func 119
*							Shell Manager
	.dc.b	0, 1, 2			* func 120
	.dc.b	3, 1, 2			* func 121
	.dc.b	1, 1, 1			* func 122
	.dc.b	1, 1, 1			* func 123
	.dc.b	0, 1, 1			* func 124
	.dc.b	0, 1, 2			* func 125
