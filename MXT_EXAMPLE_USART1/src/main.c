/**
 * \file
 *
 * \brief Example of usage of the maXTouch component with USART
 *
 * This example shows how to receive touch data from a maXTouch device
 * using the maXTouch component, and display them in a terminal window by using
 * the USART driver.
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage
 *
 * \section intro Introduction
 * This simple example reads data from the maXTouch device and sends it over
 * USART as ASCII formatted text.
 *
 * \section files Main files:
 * - example_usart.c: maXTouch component USART example file
 * - conf_mxt.h: configuration of the maXTouch component
 * - conf_board.h: configuration of board
 * - conf_clock.h: configuration of system clock
 * - conf_example.h: configuration of example
 * - conf_sleepmgr.h: configuration of sleep manager
 * - conf_twim.h: configuration of TWI driver
 * - conf_usart_serial.h: configuration of USART driver
 *
 * \section apiinfo maXTouch low level component API
 * The maXTouch component API can be found \ref mxt_group "here".
 *
 * \section deviceinfo Device Info
 * All UC3 and Xmega devices with a TWI module can be used with this component
 *
 * \section exampledescription Description of the example
 * This example will read data from the connected maXTouch explained board
 * over TWI. This data is then processed and sent over a USART data line
 * to the board controller. The board controller will create a USB CDC class
 * object on the host computer and repeat the incoming USART data from the
 * main controller to the host. On the host this object should appear as a
 * serial port object (COMx on windows, /dev/ttyxxx on your chosen Linux flavour).
 *
 * Connect a terminal application to the serial port object with the settings
 * Baud: 57600
 * Data bits: 8-bit
 * Stop bits: 1 bit
 * Parity: None
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for AVR.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

/**
* - TODO [x] Tela 2 quando clica o botao start
* - TODO [x] Implementar funcao de lock
* - TODO [x] Feedback sonoro dos botoes com Buzzer
* - TODO [x] Implementar funcao da porta aberta
* - TODO [x] Colocar fonte nos textos
* - TODO [ ] Cronometro do fim da lavagem
* - TODO [ ] Animacao do tempo de carregamento
* - TODO [ ] FAZER MAIS DE DOIS BOTOES FUNCIONAREM NA LISTA
* - TODO [ ] BOTOES AINDA ESTAO COM TOQUE DUPLO
* - TODO [ ] DESCOMENTAR A ATIVACAO DO BUZZER(JA FOI IMPLEMENTADO)
*/
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
#include "icones/stop.h"					//author: https://www.flaticon.com/authors/smashicons

/************************************************************************/
/* STRUCTS                                                              */
/************************************************************************/

/*
 typedef struct {
	 const uint8_t *data;
	 uint16_t width;
	 uint16_t height;
	 uint8_t dataSize;
 } tImage;
 */


/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/

//PB2 - BOTAO PORTA
#define PORTA_PIO          PIOA
#define PORTA_PIO_ID        10
#define PORTA_PIO_IDX       11u
#define PORTA_PIO_IDX_MASK  (1u << PORTA_PIO_IDX)

//PB0 - BUZZER
#define BUZZER_PIO           PIOB
#define BUZZER_PIO_ID        11
#define BUZZER_PIO_IDX       0u
#define BUZZER_PIO_IDX_MASK  (1u << BUZZER_PIO_IDX)

//PB1 - LED PORTA
#define LED_PIO           PIOC
#define LED_PIO_ID        12
#define LED_PIO_IDX       8u
#define LED_PIO_IDX_MASK  (1u << LED_PIO_IDX)


/**
 *  Informacoes para o RTC
 *  poderia ser extraida do __DATE__ e __TIME__
 *  ou ser atualizado pelo PC.
 */
#define YEAR        2018
#define MOUNT       3
#define DAY         19
#define WEEK        12
#define HOUR        15
#define MINUTE      45
#define SECOND      0


#define MAX_ENTRIES        3
#define STRING_LENGTH     70
#define USART_TX_MAX_LENGTH     0xff

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/
struct ili9488_opt_t g_ili9488_display_opt;
const uint32_t BUTTON_W = 80;
const uint32_t BUTTON_H = 60;
const uint32_t BUTTON_BORDER = 2;
const uint32_t BUTTON_X = 40; // BUTTON_W/2
const uint32_t BUTTON_Y = 320-50;

volatile Bool locked = false;
volatile Bool flag_back = false;	
volatile Bool flag_next = false;
volatile Bool isWashing = false;
volatile Bool isOpen = false;
volatile Bool isDrawn = false;
volatile Bool flag_button = false;
volatile int flag_started = 0;

/** \brief Touch event struct */
struct botao {
	uint16_t x;
	uint16_t y;
	uint16_t size;
	tImage *image;
	void (*p_handler)(void);
};	



/************************************************************************/
/* handler / callbacks                                                  */
/************************************************************************/
void back_callback(void){
	printf("NEXT  ");
	printf("%d  ",flag_started );
	flag_back = true;
}
void next_callback(void){
	printf("BACK  ");
	printf("%d  ",flag_started );
	flag_next = true;
}

void lock_callback(void){
	locked = !locked;
	isDrawn = false;
}

void PORTA_callback(void){
	flag_button = true;
}
void start_callback(void){
	flag_started = !flag_started;
	isDrawn = false;
	printf("   artFLAG EH :%d   ", flag_started);
}



struct botao botaoNext  = {.x=384-32,.y=160-32,.size=64,.p_handler = next_callback, .image = &next_colorido};
struct botao botaoBack  = {.x=106-32,.y=160-32,.size=64,.p_handler = back_callback, .image = &back_colorido};
struct botao botaoStart = {.x=240-32,.y=280-32,.size=64,.p_handler = start_callback, .image = &fast2};
struct botao diarioAzul = {.x=240-64,.y=120-64,.size=128,.p_handler = next_callback, .image = &diario_azul};
struct botao centrifuga	= {.x=240-64,.y=120-64,.size=128,.p_handler = next_callback, .image = &centrifuga_colorido};
struct botao enxague	= {.x=240-64,.y=120-64,.size=128,.p_handler = next_callback, .image = &enxague_colorido};
struct botao fast		= {.x=240-64,.y=120-64,.size=128,.p_handler = next_callback, .image = &fast_colorido};
struct botao pesado		= {.x=240-64,.y=120-64,.size=128,.p_handler = next_callback, .image = &pesado_colorido};
struct botao bLocked	= {.x=450-64,.y=305-64,.size=128,.p_handler = lock_callback, .image = &unlocked_colorido};
struct botao bUnlocked	= {.x=450-64,.y=305-64,.size=128,.p_handler = lock_callback, .image = &locked_colorido};
struct botao botaoStop	= {.x=240-32,.y=280-32,.size=64,.p_handler = start_callback, .image = &stop};

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/
int processa_touch(struct botao b[], uint *rtn, uint N ,uint x, uint y ){
	
	for (int i = 0; i< N; i++){
		printf("");
		if(x >= (b[i].x) && x <= (b[i].x) +(b[i].size)){		
			if(y >= (b[i].y) && y<= (b[i].y) + (b[i].size)) {
				*rtn = i;
				printf("foi");
				//pio_set(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
				//delay_ms(60);
				//pio_clear(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
				return 1;
			}
		}
	}
	printf("Nao foi");
	return 0;

}


void font_draw_text(tFont *font, const char *text, int x, int y, int spacing) {
	char *p = text;
	while(*p != NULL) {
		char letter = *p;
		int letter_offset = letter - font->start_char;
		if(letter <= font->end_char) {
			tChar *current_char = font->chars + letter_offset;
			ili9488_draw_pixmap(x, y, current_char->image->width, current_char->image->height, current_char->image->data);
			x += current_char->image->width + spacing;
		}
		p++;
	}
}

static void configure_lcd(void){
	/* Initialize display parameter */
	g_ili9488_display_opt.ul_width = ILI9488_LCD_WIDTH;
	g_ili9488_display_opt.ul_height = ILI9488_LCD_HEIGHT;
	g_ili9488_display_opt.foreground_color = COLOR_CONVERT(COLOR_WHITE);
	g_ili9488_display_opt.background_color = COLOR_CONVERT(COLOR_WHITE);

	/* Initialize LCD */
	ili9488_init(&g_ili9488_display_opt);
}
static void mxt_init(struct mxt_device *device)
{
	enum status_code status;

	/* T8 configuration object data */
	uint8_t t8_object[] = {
		0x0d, 0x00, 0x05, 0x0a, 0x4b, 0x00, 0x00,
		0x00, 0x32, 0x19
	};

	/* T9 configuration object data */
	uint8_t t9_object[] = {
		0x8B, 0x00, 0x00, 0x0E, 0x08, 0x00, 0x80,
		0x32, 0x05, 0x02, 0x0A, 0x03, 0x03, 0x20,
		0x02, 0x0F, 0x0F, 0x0A, 0x00, 0x00, 0x00,
		0x00, 0x18, 0x18, 0x20, 0x20, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x02,
		0x02
	};

	/* T46 configuration object data */
	uint8_t t46_object[] = {
		0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x03,
		0x00, 0x00
	};
	
	/* T56 configuration object data */
	uint8_t t56_object[] = {
		0x02, 0x00, 0x01, 0x18, 0x1E, 0x1E, 0x1E,
		0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
		0x1E, 0x1E, 0x1E, 0x1E, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00
	};

	/* TWI configuration */
	twihs_master_options_t twi_opt = {
		.speed = MXT_TWI_SPEED,
		.chip  = MAXTOUCH_TWI_ADDRESS,
	};

	status = (enum status_code)twihs_master_setup(MAXTOUCH_TWI_INTERFACE, &twi_opt);
	Assert(status == STATUS_OK);

	/* Initialize the maXTouch device */
	status = mxt_init_device(device, MAXTOUCH_TWI_INTERFACE,
			MAXTOUCH_TWI_ADDRESS, MAXTOUCH_XPRO_CHG_PIO);
	Assert(status == STATUS_OK);

	/* Issue soft reset of maXTouch device by writing a non-zero value to
	 * the reset register */
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_COMMANDPROCESSOR_T6, 0)
			+ MXT_GEN_COMMANDPROCESSOR_RESET, 0x01);

	/* Wait for the reset of the device to complete */
	delay_ms(MXT_RESET_TIME);

	/* Write data to configuration registers in T7 configuration object */
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_POWERCONFIG_T7, 0) + 0, 0x20);
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_POWERCONFIG_T7, 0) + 1, 0x10);
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_POWERCONFIG_T7, 0) + 2, 0x4b);
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_POWERCONFIG_T7, 0) + 3, 0x84);

	/* Write predefined configuration data to configuration objects */
	mxt_write_config_object(device, mxt_get_object_address(device,
			MXT_GEN_ACQUISITIONCONFIG_T8, 0), &t8_object);
	mxt_write_config_object(device, mxt_get_object_address(device,
			MXT_TOUCH_MULTITOUCHSCREEN_T9, 0), &t9_object);
	mxt_write_config_object(device, mxt_get_object_address(device,
			MXT_SPT_CTE_CONFIGURATION_T46, 0), &t46_object);
	mxt_write_config_object(device, mxt_get_object_address(device,
			MXT_PROCI_SHIELDLESS_T56, 0), &t56_object);

	/* Issue recalibration command to maXTouch device by writing a non-zero
	 * value to the calibrate register */
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_COMMANDPROCESSOR_T6, 0)
			+ MXT_GEN_COMMANDPROCESSOR_CALIBRATE, 0x01);
}


void draw_time(int enxagueQnt, int enxagueTempo, int centrifugacaoTempo){
		char b[512];
		int Tempo_ciclo = enxagueQnt * enxagueTempo + centrifugacaoTempo; // Minutos
		sprintf(b, "Tempo: 00 : %02d", Tempo_ciclo);
		font_draw_text(&calibri_24, b, 12, 12, 1);
}
void draw_button(uint32_t clicked) {
	static uint32_t last_state = 255; // undefined
	if(clicked == last_state) return;
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	ili9488_draw_filled_rectangle(BUTTON_X-BUTTON_W/2, BUTTON_Y-BUTTON_H/2, BUTTON_X+BUTTON_W/2, BUTTON_Y+BUTTON_H/2);
	if(clicked) {
		ili9488_set_foreground_color(COLOR_CONVERT(COLOR_TOMATO));
		ili9488_draw_filled_rectangle(BUTTON_X-BUTTON_W/2+BUTTON_BORDER, BUTTON_Y+BUTTON_BORDER, BUTTON_X+BUTTON_W/2-BUTTON_BORDER, BUTTON_Y+BUTTON_H/2-BUTTON_BORDER);
	} else {
		ili9488_set_foreground_color(COLOR_CONVERT(COLOR_GREEN));
		ili9488_draw_filled_rectangle(BUTTON_X-BUTTON_W/2+BUTTON_BORDER, BUTTON_Y-BUTTON_H/2+BUTTON_BORDER, BUTTON_X+BUTTON_W/2-BUTTON_BORDER, BUTTON_Y-BUTTON_BORDER);
	}
	last_state = clicked;
}

uint32_t convert_axis_system_x(uint32_t touch_y) {
	// entrada: 4096 - 0 (sistema de coordenadas atual)
	// saida: 0 - 320
	return ILI9488_LCD_HEIGHT - ILI9488_LCD_HEIGHT*touch_y/4096;
}
uint32_t convert_axis_system_y(uint32_t touch_x) {
	// entrada: 0 - 4096 (sistema de coordenadas atual)
	// saida: 0 - 320
	return ILI9488_LCD_WIDTH - ILI9488_LCD_WIDTH*touch_x/4096;
}

void update_screen(uint32_t tx, uint32_t ty) {
	if(tx >= BUTTON_X-BUTTON_W/2 && tx <= BUTTON_X + BUTTON_W/2) {
		if(ty > BUTTON_Y && ty <= BUTTON_Y+BUTTON_H/2) {
			printf("Entrou 1");
			draw_button(1);
		} else if(ty > BUTTON_Y && ty < BUTTON_Y + BUTTON_H/2) {
			printf("Entrou 0");
			draw_button(0);
		}
	}
}

void mxt_handler(struct mxt_device *device, struct botao botoes[], uint Nbotoes)
{
	/* USART tx buffer initialized to 0 */
	char tx_buf[STRING_LENGTH * MAX_ENTRIES] = {0};
	uint8_t i = 0; /* Iterator */
	int last_status;

	/* Temporary touch event data struct */
	struct mxt_touch_event touch_event;

	/* Collect touch events and put the data in a string,
	 * maximum 2 events at the time */
	do {
		/* Temporary buffer for each new touch event line */
		char buf[STRING_LENGTH];
	
		/* Read next next touch event in the queue, discard if read fails */
		if (mxt_read_touch_event(device, &touch_event) != STATUS_OK) {
			continue;
		}
		
		 // eixos trocados (quando na vertical LCD)
		uint32_t conv_y = convert_axis_system_x(touch_event.y);
		uint32_t conv_x = convert_axis_system_y(touch_event.x);
		//conv_y = ILI9488_LCD_HEIGHT - conv_y;
		//uint32_t conv_y = touch_event.y;
		//uint32_t conv_x = touch_event.x;
		
		
		/* Format a new entry in the data string that will be sent over USART */
		sprintf(buf, "X:%3d Y:%3d \n", conv_x, conv_y);
		last_status = touch_event.status;
		
		if(last_status<60){
			//update_screen(conv_x, conv_y);
			uint i;
			if(processa_touch(botoes, &i, Nbotoes, conv_x, conv_y))
			botoes[i].p_handler();
		}

		/* Add the new string to the string buffer */
		strcat(tx_buf, buf);
		i++;
		
		break;

		/* Check if there is still messages in the queue and
		 * if we have reached the maximum numbers of events */
	} while ((mxt_is_message_pending(device)) & (i < MAX_ENTRIES));

	/* If there is any entries in the buffer, send them over USART */
	if (i > 0) {
		usart_serial_write_packet(USART_SERIAL_EXAMPLE, (uint8_t *)tx_buf, strlen(tx_buf));
	}
}

void io_init(void){
	// Inicializa clock do perif�rico PIO responsavel pelo botao
	pmc_enable_periph_clk(PORTA_PIO_ID);
	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(BUZZER_PIO_ID);
	
	// Configura PIO para lidar com o pino do bot�o como entrada
	// com pull-up
	pio_configure(PORTA_PIO, PIO_INPUT, PORTA_PIO_IDX_MASK, PIO_PULLUP);
	pio_set_output(BUZZER_PIO, BUZZER_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	
	// Configura interrup��o no pino referente ao botao e associa
	// fun��o de callback caso uma interrup��o for gerada
	// a fun��o de callback � a: but_callback()
	pio_handler_set(PORTA_PIO, PORTA_PIO_ID, PORTA_PIO_IDX_MASK, PIO_IT_RISE_EDGE, PORTA_callback);
	
	// Ativa interrup��o
	pio_enable_interrupt(PORTA_PIO, PORTA_PIO_IDX_MASK);

	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais pr�ximo de 0 maior)
	NVIC_EnableIRQ(PORTA_PIO_ID);
	NVIC_SetPriority(PORTA_PIO_ID, 4); // Prioridade 4
}

void draw_menu_screen(void) {
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
	ili9488_draw_filled_rectangle(0, 0, ILI9488_LCD_WIDTH-1, ILI9488_LCD_HEIGHT-1);
	
	ili9488_draw_pixmap(botaoNext.x, botaoNext.y, botaoNext.image->width, botaoNext.image->height, botaoNext.image->data);
	ili9488_draw_pixmap(botaoBack.x, botaoBack.y, botaoBack.image->width, botaoBack.image->height, botaoBack.image->data);
	ili9488_draw_pixmap(botaoStart.x, botaoStart.y, botaoStart.image->width, botaoStart.image->height, botaoStart.image->data);
	ili9488_draw_pixmap(diarioAzul.x, diarioAzul.y,diarioAzul.image->width, diarioAzul.image->height, diarioAzul.image->data);
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
	ili9488_draw_filled_rectangle(diarioAzul.x,diarioAzul.y + diarioAzul.size + 20,diarioAzul.x + diarioAzul.size+40,diarioAzul.y+diarioAzul.size+40);
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	//ili9488_draw_string(diarioAzul.x,diarioAzul.y + diarioAzul.size + 20,"Ciclo Diario" );
	font_draw_text(&calibri_24, "Diario", diarioAzul.x, diarioAzul.y + diarioAzul.size + 20, 1);
	ili9488_draw_pixmap(bLocked.x, bLocked.y, bLocked.image->width, bLocked.image->height, bLocked.image->data);
	

	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	draw_time(c_diario.enxagueTempo, c_diario.enxagueQnt, c_diario.centrifugacaoTempo);
	isDrawn = true;
	
}

void draw_started_screen(void){
	
	char b[512];
	sprintf(b, "Tempo Restante: 00 : %02d", 20);
	font_draw_text(&calibri_36, b, 50, 145, 3);
	ili9488_draw_pixmap(bLocked.x, bLocked.y, bLocked.image->width, bLocked.image->height, bLocked.image->data);
	ili9488_draw_pixmap(botaoStop.x, botaoStop.y,botaoStop.image->width, botaoStop.image->height, botaoStop.image->data);
	printf("desenhou");
	flag_started = 2;
	isDrawn = true;
}

void draw_lock_screen(void){
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
	ili9488_draw_filled_rectangle(0, 0, 500, 500);
	ili9488_draw_pixmap(bUnlocked.x, bUnlocked.y, bUnlocked.image->width, bUnlocked.image->height, bUnlocked.image->data);
	isDrawn = true;
}

void draw_door_screen(void){
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
	ili9488_draw_filled_rectangle(0, 0, 500, 500);
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_RED));
	font_draw_text(&calibri_36, "FECHE A PORTA", diarioAzul.x, diarioAzul.y + diarioAzul.size + 20, 1);
	ili9488_draw_pixmap(botaoStop.x, botaoStop.y,botaoStop.image->width, botaoStop.image->height, botaoStop.image->data);
	//pio_set(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
	//delay_ms(60);
	//pio_clear(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
	isDrawn = true;
}

void draw_cicle(struct botao atual, t_ciclo selecionado){
	ili9488_draw_pixmap(atual.x,
	atual.y,
	atual.image->width,
	atual.image->height,
	atual.image->data);
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
	ili9488_draw_filled_rectangle(atual.x, atual.y + atual.size + 20, atual.x + atual.size + 40, atual.y + atual.size+40);
	ili9488_draw_filled_rectangle(0, 0, 200, 40);
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	font_draw_text(&calibri_24, selecionado.nome, atual.x, atual.y + atual.size + 20, 1);
	draw_time(selecionado.enxagueTempo, selecionado.enxagueQnt, selecionado.centrifugacaoTempo);
}
/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void)
{
	struct mxt_device device; /* Device data container */

	/* Initialize the USART configuration struct */
	const usart_serial_options_t usart_serial_options = {
		.baudrate     = USART_SERIAL_EXAMPLE_BAUDRATE,
		.charlength   = USART_SERIAL_CHAR_LENGTH,
		.paritytype   = USART_SERIAL_PARITY,
		.stopbits     = USART_SERIAL_STOP_BIT
	};
	int tipo_lavagem = 5;

	sysclk_init(); /* Initialize system clocks */
	board_init();  /* Initialize board */
	configure_lcd();
	//draw_menu_screen();
	
	//draw_button(0);
	/* Initialize the mXT touch device */
	mxt_init(&device);
	
	io_init();
	
	/* Initialize stdio on USART */
	stdio_serial_init(USART_SERIAL_EXAMPLE, &usart_serial_options);

	printf("\n\rmaXTouch data USART transmitter\n\r");

	
	struct botao botoes[] = {bLocked, botaoStart, botaoBack, botaoNext};

	while (true) {
		//printf(" FLAGe :%d   ", flag_started);
		/* Check for any pending messages and run message handler if any
		 * message is found in the queue */
		if (mxt_is_message_pending(&device)) {
			mxt_handler(&device, botoes,  sizeof(botoes) / sizeof(struct botao));
		}
		if(locked){
			printf("LOCKED");
			if(!isDrawn){
				draw_lock_screen();
			}
		}
		
		if(flag_button){
			if(isOpen){
				isOpen = false;
				
				pio_clear(LED_PIO, LED_PIO_IDX_MASK);
				printf("PORTA FECHADA");
			}
			else{
				isOpen = true;
				pio_set(LED_PIO, LED_PIO_IDX_MASK);
				printf("PORTA ABERTA");
			}
			flag_button = false;
			isDrawn = false;
		}
		
		if(flag_started == 0){
			if(!isDrawn){
				ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
				ili9488_draw_filled_rectangle(0, 0, 500, 500);
				draw_menu_screen();
			}
			if(flag_next == true || flag_back == true){
				
				if(flag_next == true){
					tipo_lavagem += 1;
				}
				else if(flag_back == true){
					tipo_lavagem -= 1;
				}
				if (tipo_lavagem%5 == 0 ){
					draw_cicle(diarioAzul, c_diario);
				}
				if (tipo_lavagem%5 == 1 ){
					draw_cicle(pesado, c_pesado);
				}
				if (tipo_lavagem%5 == 2 ){
					draw_cicle(enxague, c_enxague);
				}
				if (tipo_lavagem%5 == 3 ){
					draw_cicle(centrifuga, c_centrifuga);
				}
				if (tipo_lavagem%5 == 4 ){
					draw_cicle(fast, c_rapido);
				}
				flag_next = false;
				flag_back = false;
			}
		}
		if(flag_started == 1 && isOpen == false){
			printf("COMECOU");
			if(!isDrawn){
				ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
				ili9488_draw_filled_rectangle(0, 0, 500, 500);
				draw_started_screen();
			}
		}
		else if(flag_started == 1 && isOpen == true){
			if(!isDrawn){
				draw_door_screen();
			}
		}
	}

	return 0;
}
