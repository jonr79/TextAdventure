#include "FightDisplay.h"

void FightDisplay::DisplayAttackAnnouncement(Creature* enemy)
{
	DisplayText("You have been attacked by a ", 13, 11, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	DisplayText(enemy->GetName(), 41, 11, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	DisplayText("", 13, 12, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	system("pause");
	clear();
}

void FightDisplay::DisplayBanter(std::vector<string> speech)
{
	clrtop(1);
	for (unsigned int i = 0; i < speech.size(); i++)
	{
		DisplayText(speech[i], 13, i + 1, white);
	}
	DisplayText(string(), 13, 23, white);
	system("pause");
	clrtop(1);
}

void FightDisplay::DisplayMessage(string message, int pauseDuration)
{
	DisplayText(message, 13, 11, white);
	DisplayText("", 79, 23, white);
	Sleep(pauseDuration);
	DisplayText("                     ", 13, 11, white);
}

void FightDisplay::DisplayMessageWithRedNumber(string message, int damage, int pauseDuration)
{
	DisplayText(message, 13, 11, white);
	DisplayNumber(damage, 45, 11, red);
	Sleep(pauseDuration);
}

void FightDisplay::DisplayCreatureStatus(Creature* creature)
{
	DisplayText(creature->GetName(), 13, 1, white);
	DisplayText("Level: ", 13, 2, white);
	DisplayText("HP:    ", 13, 3, white);
	DisplayText("Ka:    ", 13, 4, white);
	DisplayNumber(creature->GetLevel(), 20, 2, white);
	DisplayNumber(creature->GetHitPoints(), 17, 3, white);
	DisplayNumber(creature->GetKa(), 17, 4, white);
}

void FightDisplay::DisplayFightMenu()
{
	DisplayText("           ", 1, 11, white);
	DisplayText("           ", 1, 12, white);
	DisplayText("           ", 1, 13, white);
	DisplayText("                                                                 ", 13, 11, FOREGROUND_BLUE);
	DisplayText("/---------\\", 1, 14, ftext);
	DisplayText("|  Attack |", 1, 15, ftext);
	DisplayText("|  Magic  |", 1, 16, ftext);
	DisplayText("|  Item   |", 1, 17, ftext);
	DisplayText("|  Run    |", 1, 18, ftext);
	DisplayText("\\---------/", 1, 19, ftext);
}

void FightDisplay::DisplayWinContent(Player* player, Creature* enemy)
{
	DisplayText("The enemy has been slain", 13, 11, white);
	Sleep(player->GetPauseDuration());
	clear();

	DisplayText("After every enemy dies you get stuff!!!", 13, 9, green);
	DisplayText("Gold: ", 13, 11, green);
	DisplayText("Exp : ", 13, 12, green);
	DisplayNumber(enemy->GetGold(), 20, 11, green);
	DisplayNumber(enemy->GetExperience(), 20, 12, green);

	enemy->Win(player, this);
	Sleep(player->GetPauseDuration());
}

void FightDisplay::DisplayLevelUp(Player* player)
{
	clear();
	DisplayText("You have gone up in level!!", 13, 11, FOREGROUND_GREEN);
	DisplayText("", 79, 23, white);
	Sleep(player->GetPauseDuration());
}
