#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <fcntl.h>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#define KEYBOARD_INTERRUPT  0x09  // the keyboard interrupt number
#define KEY_BUFFER          0x60  // the port of the keyboard buffer
#define KEY_CONTROL         0x61  // the port of the keyboard controller
#define PIC_PORT            0x20  // the port of the programmable

#define MAKE_ESC              1
#define MAKE_1                2
#define MAKE_2                3
#define MAKE_3                4
#define MAKE_4                5
#define MAKE_5                6
#define MAKE_6                7
#define MAKE_7                8
#define MAKE_8                9
#define MAKE_9                10
#define MAKE_0                11
#define MAKE_MINUS            12
#define MAKE_EQUALS           13
#define MAKE_BKSP             14
#define MAKE_TAB              15
#define MAKE_Q                16
#define MAKE_W                17
#define MAKE_E                18
#define MAKE_R                19
#define MAKE_T                20
#define MAKE_Y                21
#define MAKE_U                22
#define MAKE_I                23
#define MAKE_O                24
#define MAKE_P                25
#define MAKE_LFT_BRACKET      26
#define MAKE_RGT_BRACKET      27
#define MAKE_ENTER            28
#define MAKE_CTRL             29
#define MAKE_A                30
#define MAKE_S                31
#define MAKE_D                32
#define MAKE_F                33
#define MAKE_G                34
#define MAKE_H                35
#define MAKE_J                36
#define MAKE_K                37
#define MAKE_L                38
#define MAKE_SEMI             39
#define MAKE_APOS             40
#define MAKE_TILDE            41
#define MAKE_LEFT_SHIFT       42
#define MAKE_BACK_SLASH       43
#define MAKE_Z                44
#define MAKE_X                45
#define MAKE_C                46
#define MAKE_V                47
#define MAKE_B                48
#define MAKE_N                49
#define MAKE_M                50
#define MAKE_COMMA            51
#define MAKE_PERIOD           52
#define MAKE_FOWARD_SLASH     53
#define MAKE_RIGHT_SHIFT      54
#define MAKE_PRT_SCRN         55
#define MAKE_ALT              56
#define MAKE_SPACE            57
#define MAKE_CAPS_LOCK        58
#define MAKE_F1               59
#define MAKE_F2               60
#define MAKE_F3               61
#define MAKE_F4               62
#define MAKE_F5               63
#define MAKE_F6               64
#define MAKE_F7               65
#define MAKE_F8               66
#define MAKE_F9               67
#define MAKE_F10              68
#define MAKE_F11              87
#define MAKE_F12              88
#define MAKE_NUM_LOCK         69
#define MAKE_SCROLL_LOCK      70
#define MAKE_HOME             71
#define MAKE_UP               72
#define MAKE_PGUP             73
#define MAKE_KEYPAD_MINUS     74
#define MAKE_LEFT             75
#define MAKE_CENTER           76
#define MAKE_RIGHT            77
#define MAKE_KEYPAD_PLUS      78
#define MAKE_END              79
#define MAKE_DOWN             80
#define MAKE_PGDWN            81
#define MAKE_INS              82
#define MAKE_DEL              83

// these are the defines for the break codes, they are computed by adding 128
// to each of the make codes

#define BREAK_ESC              (1 +128)
#define BREAK_1                (2 +128)
#define BREAK_2                (3 +128)
#define BREAK_3                (4 +128)
#define BREAK_4                (5 +128)
#define BREAK_5                (6 +128)
#define BREAK_6                (7 +128)
#define BREAK_7                (8 +128)
#define BREAK_8                (9 +128)
#define BREAK_9                (10+128)
#define BREAK_0                (11+128)
#define BREAK_MINUS            (12+128)
#define BREAK_EQUALS           (13+128)
#define BREAK_BKSP             (14+128)
#define BREAK_TAB              (15+128)
#define BREAK_Q                (16+128)
#define BREAK_W                (17+128)
#define BREAK_E                (18+128)
#define BREAK_R                (19+128)
#define BREAK_T                (20+128)
#define BREAK_Y                (21+128)
#define BREAK_U                (22+128)
#define BREAK_I                (23+128)
#define BREAK_O                (24+128)
#define BREAK_P                (25+128)
#define BREAK_LFT_BRACKET      (26+128)
#define BREAK_RGT_BRACKET      (27+128)
#define BREAK_ENTER            (28+128)
#define BREAK_CTRL             (29+128)
#define BREAK_A                (30+128)
#define BREAK_S                (31+128)
#define BREAK_D                (32+128)
#define BREAK_F                (33+128)
#define BREAK_G                (34+128)
#define BREAK_H                (35+128)
#define BREAK_J                (36+128)
#define BREAK_K                (37+128)
#define BREAK_L                (38+128)
#define BREAK_SEMI             (39+128)
#define BREAK_APOS             (40+128)
#define BREAK_TILDE            (41+128)
#define BREAK_LEFT_SHIFT       (42+128)
#define BREAK_BACK_SLASH       (43+128)
#define BREAK_Z                (44+128)
#define BREAK_X                (45+128)
#define BREAK_C                (46+128)
#define BREAK_V                (47+128)
#define BREAK_B                (48+128)
#define BREAK_N                (49+128)
#define BREAK_M                (50+128)
#define BREAK_COMMA            (51+128)
#define BREAK_PERIOD           (52+128)
#define BREAK_FOWARD_SLASH     (53+128)
#define BREAK_RIGHT_SHIFT      (54+128)
#define BREAK_PRT_SCRN         (55+128)
#define BREAK_ALT              (56+128)
#define BREAK_SPACE            (57+128)
#define BREAK_CAPS_LOCK        (58+128)
#define BREAK_F1               (59+128)
#define BREAK_F2               (60+128)
#define BREAK_F3               (61+128)
#define BREAK_F4               (62+128)
#define BREAK_F5               (63+128)
#define BREAK_F6               (64+128)
#define BREAK_F7               (65+128)
#define BREAK_F8               (66+128)
#define BREAK_F9               (67+128)
#define BREAK_F10              (68+128)
#define BREAK_F11              (87+128)
#define BREAK_F12              (88+128)
#define BREAK_NUM_LOCK         (69+128)
#define BREAK_SCROLL_LOCK      (70+128)
#define BREAK_HOME             (71+128)
#define BREAK_UP               (72+128)
#define BREAK_PGUP             (73+128)
#define BREAK_KEYPAD_MINUS     (74+128)
#define BREAK_LEFT             (75+128)
#define BREAK_CENTER           (76+128)
#define BREAK_RIGHT            (77+128)
#define BREAK_KEYPAD_PLUS      (78+128)
#define BREAK_END              (79+128)
#define BREAK_DOWN             (80+128)
#define BREAK_PGDWN            (81+128)
#define BREAK_INS              (82+128)
#define BREAK_DEL              (83+128)

#define KEY_UP                 0
#define KEY_DOWN               1

void _interrupt (_far *Old_Keyboard_ISR)(...);  // holds old keyboard interrupt handler
void _interrupt _far Keyboard_Driver();
int raw_scan_code=0;
int keyboard_state[128];
int keys_active = 0;

void _interrupt _far Keyboard_Driver(...)
{
_asm   {
	sti                    // re-enable interrupts
	in al, KEY_BUFFER      // get the key that was pressed from the keyboard
	xor ah,ah              // zero out upper 8 bits of AX
	mov raw_scan_code, ax  // store the key in global variable
	in al, KEY_CONTROL     // set the control register to reflect key was read
	or al, 82h             // set the proper bits to reset the keyboard flip flop
	out KEY_CONTROL,al     // send the new data back to the control register
	and al,7fh
	out KEY_CONTROL,al     // complete the reset
	mov al,20h             // reset command
	out PIC_PORT,al        // tell PIC to re-enable interrupts

	} // end inline assembly
if (raw_scan_code <128)
   {
   if (keyboard_state[raw_scan_code]==KEY_UP)
	 {
	 keys_active++;
	 keyboard_state[raw_scan_code] = KEY_DOWN;
	 } // end if key wasn't already pressed
   } // end if a make code
else
   {
   if (keyboard_state[raw_scan_code-128]==KEY_DOWN)
	 {
	 keys_active--;
		keyboard_state[raw_scan_code-128] = KEY_UP;
	 } // end if key wasn't already pressed
   } // end else
} // end Keyboard_Driver

///////////////////////////////////////////////////////////////////////////////

void Keyboard_Install_Driver(void)
{
int index;
for (index=0; index<128; index++)
    keyboard_state[index]=0;
Old_Keyboard_ISR = _dos_getvect(KEYBOARD_INTERRUPT);
_dos_setvect(KEYBOARD_INTERRUPT, Keyboard_Driver);
}

///////////////////////////////////////////////////////////////////////////////

void Keyboard_Remove_Driver(void)
{
_dos_setvect(KEYBOARD_INTERRUPT, Old_Keyboard_ISR);
}

