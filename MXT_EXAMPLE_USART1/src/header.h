
/************************************************************************/
/* includes                                                             */
/************************************************************************/
#include "tfont.h"
#include "calibri_36.h"
#include "calibri_24.h"
#include "arial_72.h"
#include <asf.h>
#include <maquina1.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conf_board.h"
#include "conf_example.h"
#include "conf_uart_serial.h"

//ICONS

#include "icones/next_colorido.h"			//author: https://www.flaticon.com/authors/smashicons
#include "icones/back_colorido.h"			//author: https://www.flaticon.com/authors/smashicons
#include "icones/diario_azul.h"				//author: https://www.flaticon.com/authors/smashicons
#include "icones/centrifuga_colorido.h"		//author: https://www.flaticon.com/authors/pixelmeetup
#include "icones/enxague_colorido.h"		//author: https://www.flaticon.com/authors/smalllikeart
#include "icones/fast_colorido.h"			//author: https://www.flaticon.com/authors/freepik
#include "icones/locked_colorido.h"			//author: https://www.flaticon.com/authors/smashicons
#include "icones/unlocked_colorido.h"		//author: https://www.flaticon.com/authors/smashicons
#include "icones/pesado_colorido.h"			//author: https://www.flaticon.com/authors/freepik
#include "icones/fast2.h"					//author: https://www.flaticon.com/authors/smashicons
#include "icones/load1.h"					//author: https://www.flaticon.com/authors/roundicons
#include "icones/load2.h"					//author: https://www.flaticon.com/authors/roundicons
#include "icones/load3.h"					//author: https://www.flaticon.com/authors/roundicons
#include "icones/load4.h"					//author: https://www.flaticon.com/authors/roundicons
#include "icones/stop.h"					//author: https://www.flaticon.com/authors/smashicons

void back_callback(void);
void next_callback(void);

void lock_callback(void);

void PORTA_callback(void);
void start_callback(void);