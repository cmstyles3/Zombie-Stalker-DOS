#include "pcx.h"

void main()
{
	Screen_Transition(SCREEN_DARKNESS);
	system("cls");
	system("cd drivers");
	Set_Graphics_Mode(TEXTMODE);
	system("setd");
	clrscr();
	system("setm");
	clrscr();
	Screen_Transition(SCREEN_DARKNESS);
	system("copy midpak.ad ..");
	system("copy midpak.adv ..");
	system("copy midpak.com ..");
	system("copy soundrv.com ..");
	system("cd..");
	Set_Graphics_Mode(TEXTMODE);
}