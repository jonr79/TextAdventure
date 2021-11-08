#include "MainMenuSystem.h"

MainMenuSystem::MainMenuSystem(GameDisplay* gameDisplay, ISaveLoadGame* gameSaver, ItemRepository* itemRepository)
{
	Display = gameDisplay;
	GameSaver = gameSaver;
	Items = itemRepository;
}

void MainMenuSystem::HandleMainMenu(Player* player, vector<Magic*>& spells, vector<Item*>& worldItems, vector<Item*>& playerInventory, string& map)
{
	int choice = 0;
	bool escapeWasPressed = false;
	bool selectionWasMade = false;
	COORD cursorPosition;

	while (!escapeWasPressed)
	{
		ClearTextBottomRight(11);
		Display->DisplayPlayerInfo(player);
		Display->ground(worldItems, map, player->GetPositionX(), player->GetPositionY());
		cursorPosition.X = 2;
		cursorPosition.Y = 12;
		selectionWasMade = false;
		Display->text("/---------\\", 1, 11, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Inv.   |", 1, 12, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Status |", 1, 13, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Magic  |", 1, 14, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Options|", 1, 15, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("\\---------/", 1, 16, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("           ", 1, 17, blue);

		DrawCursor(cursorPosition, yellow, 175);
		do
		{
			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 12, 15))
			{
				DrawCursor(cursorPosition, yellow, 175);
			}
			Display->text(" ", 79, 23, white);
		} while (!selectionWasMade);
		if (escapeWasPressed)
			break;

		choice = cursorPosition.Y;
		switch (choice)
		{
		case 12:
			HandleInventory(player, worldItems, playerInventory, map);
			break;
		case 13:
			Display->DisplayPlayerStatus(player);
			break;
		case 14:
			if (player->HasLearnedSpells())
				MagicMenu(player, spells);
			else
			{
				Display->text("You have no magic", 13, 11, white);
				Sleep(player->GetPauseDuration());
				Display->text("                 ", 13, 11, white);
			}
			break;
		case 15:
			Options(player, worldItems, playerInventory, spells, map);
			break;
		}
	}// End of While(escapeWasPressed)


}
void MainMenuSystem::HandleInventory(Player* player, vector<Item*>& worldItems, vector<Item*>& playerInventory, string map)
{
	unsigned int offset;
	int choice = 0;
	int Y = 11;
	bool escapeWasPressed = false;
	bool selectionWasMade = false;
	bool wasUsed = false;
	bool pickup;
	bool selectedUse;
	COORD cursorPosition = { 2,12 };

	while (!escapeWasPressed)
	{
		Display->DisplayPlayerItems(playerInventory);
		cursorPosition.X = 2;
		cursorPosition.Y = 12;
		selectionWasMade = false;
		pickup = false;
		selectedUse = false;
		Display->text("/---------\\", 1, 11, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  PickUp |", 1, 12, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Utilize|", 1, 13, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|         |", 1, 14, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|         |", 1, 15, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("\\---------/", 1, 16, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		DrawCursor(cursorPosition, yellow, 175);
		do
		{
			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 12, 13))
			{
				DrawCursor(cursorPosition, yellow, 175);
			}
			Display->text("", 79, 23, white);
		} while (!selectionWasMade);
		if (escapeWasPressed)
			break;

		choice = cursorPosition.Y;
		switch (choice)
		{
		case 12:
			pickup = true;
			break;
		case 13:
			selectedUse = true;
			break;
		}

		if (pickup)
		{
			Item* temp;
			if (playerInventory.size() < 12)
			{
				for (offset = 0; offset < worldItems.size(); offset++)
				{
					if (worldItems[offset]->GetPositionY() == player->GetPositionY() && worldItems[offset]->GetPositionX() == player->GetPositionX() && worldItems[offset]->GetMapName() == map)
					{
						playerInventory.push_back(worldItems[offset]);
						temp = worldItems[worldItems.size() - 1];
						worldItems[worldItems.size() - 1] = worldItems[offset];
						worldItems[offset] = temp;
						worldItems.pop_back();
						break;
					}
				}
				Display->clrtop(2);
			}
			else
			{
				Display->text("You do not have enough room!", 13, 9, white);
				Sleep(3000);
				Display->text("                            ", 13, 9, white);
			}
		}
		if (selectedUse)
		{
			choice = 0;
			escapeWasPressed = false;
			selectionWasMade = false;
			bool bFight = false;

			while (!escapeWasPressed)
			{
				Display->DisplayPlayerItems(playerInventory);
				Display->ground(worldItems, map, player->GetPositionX(), player->GetPositionY());
				cursorPosition.X = 2;
				cursorPosition.Y = 12;
				selectionWasMade = false;
				Display->text("/---------\\", 1, 11, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				Display->text("|  Use    |", 1, 12, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				Display->text("|  Equip  |", 1, 13, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				Display->text("|  Drop   |", 1, 14, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				Display->text("|  XXXX   |", 1, 15, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				Display->text("\\---------/", 1, 16, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				DrawCursor(cursorPosition, yellow, 175);
				do
				{
					if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 12, 15))
					{
						DrawCursor(cursorPosition, yellow, 175);
					}
					Display->text(" ", 79, 23, white);
				} while (!selectionWasMade);
				if (escapeWasPressed)
					break;
				choice = cursorPosition.Y;
				switch (choice)
				{
				case 12:
					if (playerInventory.size() > 0)
						Use(player, playerInventory, wasUsed, bFight);
					else
					{
						Display->text("You have nothing to use", 13, 11, white);
						Sleep(player->GetPauseDuration());
					}
					break;
				case 13:
					if (playerInventory.size() > 0)
						Equip(player, playerInventory);
					else
					{
						Display->text("You have nothing to equip", 13, 12, white);
						Sleep(player->GetPauseDuration());
					}
					break;
				case 14:
					if (playerInventory.size() > 0)
						Drop(player, worldItems, playerInventory, map);
					else
					{
						Display->text("You have nothing to drop", 13, 12, white);
						Sleep(player->GetPauseDuration());
					}
					break;
				case 15:
					if (playerInventory.size() > 0)
						DeleteItem(playerInventory);
					else
					{
						Display->text("You have nothing to dispose of", 13, 12, white);
						Sleep(player->GetPauseDuration());
					}
					break;
				}//--------End Switch
			}//------------End While escapeWasPressed
			ClearTextBottomRight(12);
		}//----------------End if(Use)
	}//--------------------End While escapeWasPressed
}//------------------------End Inventory function

void MainMenuSystem::Equip(Player* player, vector<Item*>& playerInventory)
{
	unsigned int Offset = 0;
	int choice = 0;
	bool escapeWasPressed = false;
	bool selectionWasMade = false;
	Item* temp;
	COORD cursorPosition;

	while (!selectionWasMade)
	{
		selectionWasMade = false;
		cursorPosition.X = 13;
		cursorPosition.Y = 12;
		Offset = cursorPosition.Y - 12;
		DrawCursor(cursorPosition, yellow, 175);
		playerInventory[Offset]->Display();
		do
		{
			choice = static_cast<int>(11 + playerInventory.size());
			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 12, choice))
			{
				DrawCursor(cursorPosition, yellow, 175);
				Offset = cursorPosition.Y - 12;
				playerInventory[Offset]->Display();
			}
			Display->text(" ", 79, 23, white);
		} while (!selectionWasMade);
		if (escapeWasPressed)
		{
			Display->text("          ", 1, 22, white);
			Display->text("          ", 1, 23, white);
			Display->clrbottom();
			Display->DisplayPlayerItems(playerInventory);
			return;
		}
	}// End While selectionWasMade
	Offset = cursorPosition.Y - 12;
	temp = playerInventory[Offset];
	if (!temp->GetIsArmor() && !temp->GetIsWeapon())
	{
		Display->text("You cannot equip that.", 13, 9, white);
		Sleep(player->GetPauseDuration());
	}
	if (temp->GetIsArmor())
	{
		playerInventory.push_back(player->GetArmor());
		player->SetArmor(Items->GetArmor(temp->GetName()));
		playerInventory[Offset] = playerInventory[playerInventory.size() - 1];
		playerInventory[playerInventory.size() - 1] = temp;
		playerInventory.pop_back();
	}
	if (temp->GetIsWeapon())
	{
		playerInventory.push_back(player->GetWeapon());
		player->SetWeapon(Items->GetWeapon(temp->GetName()));
		playerInventory[Offset] = playerInventory[playerInventory.size() - 1];
		playerInventory[playerInventory.size() - 1] = temp;
		playerInventory.pop_back();
	}
	Display->text("              ", cursorPosition.X, cursorPosition.Y, white);
	Display->DisplayPlayerInfo(player);
	Display->DisplayPlayerItems(playerInventory);
	Display->text("                                           ", 175, 11, white);
	Display->text("           ", 1, 22, white);
	Display->text("           ", 1, 23, white);
}

void MainMenuSystem::Drop(Player* player, vector<Item*>& worldItems, vector<Item*>& playerInventory, string map)
{
	unsigned int Offset = 0;
	int choice = 0;
	bool escapeWasPressed = false;
	bool selectionWasMade = false;
	//item *temp;
	COORD cursorPosition;


	while (!escapeWasPressed)
	{
		Display->ground(worldItems, map, player->GetPositionX(), player->GetPositionY());
		Display->DisplayPlayerItems(playerInventory);
		selectionWasMade = false;
		cursorPosition.X = 13;
		cursorPosition.Y = 12;
		DrawCursor(cursorPosition, yellow, 175);
		do
		{
			choice = static_cast<int>(11 + playerInventory.size());
			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 12, choice))
			{
				DrawCursor(cursorPosition, yellow, 175);
				Offset = cursorPosition.Y - 12;
				playerInventory[Offset]->Display();
			}
			Display->text(" ", 79, 23, white);
		} while (!selectionWasMade);
		if (escapeWasPressed)
		{
			Display->text("          ", 1, 22, white);
			Display->text("          ", 1, 23, white);
			Display->clrbottom();
			Display->DisplayPlayerItems(playerInventory);
			return;
		}


		Offset = cursorPosition.Y - 12;
		playerInventory[Offset]->SetMapName(map);
		playerInventory[Offset]->SetPositionX(player->GetPositionX());
		playerInventory[Offset]->SetPositionY(player->GetPositionY());
		worldItems.push_back(playerInventory[Offset]);
		SlideDown(playerInventory, Offset);
		playerInventory.pop_back();
		Display->text("  ", 13, 12, white);
		Display->text("                    ", 13, 12 + Offset, white);
		Display->text("                    ", 13, static_cast<short>(12 + playerInventory.size()), white);
		if (playerInventory.size() < 1)
			return;
	}//end While escapeWasPressed
}
void MainMenuSystem::DeleteItem(vector<Item*>& playerInventory)
{
	unsigned int offset = 0;
	int choice = 0;
	bool escapeWasPressed = false;
	bool selectionWasMade = false;
	//item *temp;
	COORD cursorPosition;

	while (!escapeWasPressed)
	{
		Display->DisplayPlayerItems(playerInventory);
		selectionWasMade = false;
		cursorPosition.X = 13;
		cursorPosition.Y = 12;
		DrawCursor(cursorPosition, yellow, 175);
		do
		{
			choice = static_cast<int>(11 + playerInventory.size());
			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 12, choice))
			{
				DrawCursor(cursorPosition, yellow, 175);
				offset = cursorPosition.Y - 12;
				playerInventory[offset]->Display();
			}
			Display->text(" ", 79, 23, white);
		} while (!selectionWasMade);
		if (escapeWasPressed)
		{
			Display->text("          ", 1, 22, white);
			Display->text("          ", 1, 23, white);
			Display->clrbottom();
			Display->DisplayPlayerItems(playerInventory);
			return;
		}

		offset = cursorPosition.Y - 12;
		selectionWasMade = false;
		Display->text(playerInventory[offset]->GetName(), 15, cursorPosition.Y, FOREGROUND_RED | FOREGROUND_INTENSITY);
		do
		{

			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, cursorPosition.Y, cursorPosition.Y))
			{
				DrawCursor(cursorPosition, yellow, 175);
			}
			Display->text(" ", 79, 23, white);
		} while (!selectionWasMade);
		if (escapeWasPressed)
			return;
		SlideDown(playerInventory, offset);
		playerInventory.pop_back();
		ClearTextBottomRight(12);
		Display->text("                    ", 13, static_cast<short>(12 + playerInventory.size()), white);
		if (playerInventory.size() < 1)
			return;

	}// End While escapeWasPressed

}

void MainMenuSystem::Options(Player* player, vector<Item*>& worldItems, vector<Item*>& playerInventory, vector<Magic*>& spells, string& map)
{
	int choice = 0;
	bool escapeWasPressed = false;
	bool selectionWasMade = false;
	COORD cursorPosition;

	while (!escapeWasPressed)
	{
		//clear();       For smooth look remarked out 2/15/06
		Display->DisplayPlayerInfo(player);
		cursorPosition.X = 2;
		cursorPosition.Y = 12;
		selectionWasMade = false;
		Display->text("/---------\\", 1, 11, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Save   |", 1, 12, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Load   |", 1, 13, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Setup  |", 1, 14, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Quit   |", 1, 15, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("\\---------/", 1, 16, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		DrawCursor(cursorPosition, yellow, 175);
		do
		{
			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 12, 15))
			{
				DrawCursor(cursorPosition, yellow, 175);
			}
			Display->text(" ", 79, 23, white);
		} while (!selectionWasMade);
		if (escapeWasPressed)
			break;

		choice = cursorPosition.Y;

		switch (choice)
		{
		case 12:
			Display->text("Saving game ", 13, 11, yellow);
			GameSaver->SaveGame(player, worldItems, playerInventory, spells, map);
			Sleep(player->GetPauseDuration());
			Display->text("            ", 13, 11, yellow);
			break;
		case 13:
			GameSaver->LoadGame(player, worldItems, playerInventory, spells, map, Display->GetFileName());			
			break;
		case 14:
			OptionsMenu(player, worldItems, playerInventory, spells, map);
			break;
		case 15:
			system("cls");
			Display->text("See you next time.", 13, 11, yellow);
			Sleep(player->GetPauseDuration());
			exit(0);
			break;
		}
	}// End of While(escapeWasPressed)
}

/*=====================================================================================
	Input for the setup of the game. Music, speed, etc.
=====================================================================================*/
void MainMenuSystem::OptionsMenu(Player* player, vector<Item*>& worldItems, vector<Item*>& playerInventory, vector<Magic*>& spells, string& map)
{
	int choice = 0;
	bool escapeWasPressed = false;
	bool selectionWasMade = false;
	COORD cursorPosition;

	while (!escapeWasPressed)
	{
		Display->DisplayPlayerInfo(player);
		cursorPosition.X = 2;
		cursorPosition.Y = 12;
		selectionWasMade = false;
		Display->text("/---------\\", 1, 11, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Music  |", 1, 12, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Speed  |", 1, 13, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Invis  |", 1, 14, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("|  Drive  |", 1, 15, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		Display->text("\\---------/", 1, 16, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		/*=======================================
		Displays the current music status
		========================================*/
		if (cursorPosition.Y == 12 && player->GetIsMusicOn())
			Display->text("Music: On    ", 13, 12, yellow);
		else if (cursorPosition.Y == 12 && !player->GetIsMusicOn())
			Display->text("Music: Off   ", 13, 12, yellow);
		//========================================
		DrawCursor(cursorPosition, yellow, 175);
		do
		{
			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 12, 15))
			{
				//clr(11);
				DrawCursor(cursorPosition, yellow, 175);
				/*=======================================
				Displays the current music status
				========================================*/
				if (cursorPosition.Y == 12 && player->GetIsMusicOn())
					Display->text("Music: On     ", 13, 12, yellow);
				else if (cursorPosition.Y == 12 && !player->GetIsMusicOn())
					Display->text("Music: Off    ", 13, 12, yellow);
				/*========================================
				Displays current player pause duration
				========================================*/
				if (cursorPosition.Y == 13) {
					Display->text("Speed: ", 13, 12, yellow);
					cout << player->GetPauseDuration();
				}
				/*========================================
				Displays the current invisibility status
				=========================================*/
				if (cursorPosition.Y == 14 && player->GetIsInvisible())
					Display->text("Invis: On     ", 13, 12, yellow);
				else if (cursorPosition.Y == 14 && !player->GetIsInvisible())
					Display->text("Invis: Off    ", 13, 12, yellow);
				//========================================
			}
			Display->text(" ", 79, 23, white);
		} while (!selectionWasMade);
		if (escapeWasPressed)
			break;

		choice = cursorPosition.Y;
		switch (choice)
		{
		case 12:
			Display->text("Changing Music.", 13, 11, yellow);
			player->ToggleMusic();
			Sleep(player->GetPauseDuration());
			Display->text("               ", 13, 11, yellow);
			break;
		case 13:
			Display->text("Changing Speed?", 13, 11, yellow);
			Sleep(player->GetPauseDuration());
			Display->text("               ", 13, 11, yellow);
			break;
		case 14:
			Display->text("To be added later.", 13, 11, yellow);
			Sleep(player->GetPauseDuration());
			Display->text("               ", 13, 11, yellow);
			break;
		case 15:
			Display->text("To be added later.", 13, 11, yellow);
			Sleep(player->GetPauseDuration());
			Display->text("               ", 13, 11, yellow);
			break;
		}
	}// End of While(escapeWasPressed)
}
void MainMenuSystem::UseItem(Player* player, vector<Item*>& worldItems, vector<Item*>& playerInventory, bool& isFighting, bool& didLeave, string map)
{
	int choice = 0;
	bool escapeWasPressed = false;
	bool selectionWasMade = false;
	bool Iused = false;
	COORD cursorPosition;

	while (!escapeWasPressed)
	{
		ClearTextBottomRight(12);
		if (isFighting && Iused)
			break;
		Display->DisplayPlayerInfo(player);
		Display->DisplayPlayerItems(playerInventory);
		if (!isFighting)
			Display->ground(worldItems, map, player->GetPositionX(), player->GetPositionY());
		cursorPosition.X = 2;
		cursorPosition.Y = 15;
		selectionWasMade = false;
		Display->text("/---------\\", 1, 14, ftext);
		Display->text("|  Use    |", 1, 15, ftext);
		Display->text("|  Equip  |", 1, 16, ftext);
		Display->text("|  Drop   |", 1, 17, ftext);
		Display->text("|  XXXX   |", 1, 18, ftext);
		Display->text("\\---------/", 1, 19, ftext);

		DrawCursor(cursorPosition, ftext, 175);
		do
		{
			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 15, 18))
			{
				DrawCursor(cursorPosition, ftext, 175);
			}
			Display->text("", 79, 23, ftext);
		} while (!selectionWasMade);
		if (escapeWasPressed)
		{
			didLeave = true;
			break;
		}
		choice = cursorPosition.Y;
		switch (choice)
		{
		case 15:
			if (playerInventory.size() > 0)
				Use(player, playerInventory, Iused, isFighting);
			else
			{
				Display->text("You have no items.", 13, 11, white);
				Sleep(player->GetPauseDuration());
			}
			break;
		case 16:
			if (playerInventory.size() > 0)
				Equip(player, playerInventory);
			else
			{
				Display->text("You have nothing to equip", 13, 12, white);
				Sleep(player->GetPauseDuration());
			}
			break;
		case 17:
			if (playerInventory.size() > 0)
				Drop(player, worldItems, playerInventory, map);
			else
			{
				Display->text("You have nothing to drop", 13, 12, white);
				Sleep(player->GetPauseDuration());
			}
			break;
		case 18:
			if (playerInventory.size() > 0)
				DeleteItem(playerInventory);
			else
			{
				Display->text("You have nothing to dispose of", 13, 12, white);
				Sleep(player->GetPauseDuration());
			}
			break;
		}//--------End Switch
	}//------------End While escapeWasPressed
	ClearTextBottomRight(12);
}
void MainMenuSystem::Use(Player* player, vector<Item*>& playerInventory, bool& itemWasUsed, bool isFighting)
{
	Weapon weapon;
	Armor  armor;
	Item* itemUsed;
	unsigned int offset = 0;
	int choice = 0;
	int Num;
	bool escapeWasPressed = false;
	bool selectionWasMade = false;
	COORD cursorPosition;


	while (!escapeWasPressed && playerInventory.size() > 0)
	{
		Display->clritems();
		Display->DisplayPlayerItems(playerInventory);
		Display->DisplayPlayerInfo(player);

		if (itemWasUsed && isFighting)
			break;
		escapeWasPressed = false;
		selectionWasMade = false;
		cursorPosition.X = 13;
		cursorPosition.Y = 12;
		DrawCursor(cursorPosition, yellow, 175);

		playerInventory[offset]->Display();
		do
		{
			choice = static_cast<int>(11 + playerInventory.size());
			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 12, choice))
			{
				DrawCursor(cursorPosition, yellow, 175);
				offset = cursorPosition.Y - 12;
				playerInventory[offset]->Display();
			}
			Display->text(" ", 79, 23, white);
		} while (!selectionWasMade);

		if (escapeWasPressed)
		{
			Display->text("          ", 1, 22, white);
			Display->text("          ", 1, 23, white);
			Display->clrbottom();
			Display->DisplayPlayerItems(playerInventory);
			return;
		}
		offset = cursorPosition.Y - 12;
		itemUsed = playerInventory[offset];
		if (!escapeWasPressed)
		{
			if (!itemUsed->GetKeep())
			{
				SlideDown(playerInventory, offset);
				playerInventory.pop_back();
				Display->text("                   ", 13, static_cast<short>(12 + playerInventory.size()), white);
			}


			switch (itemUsed->GetType())
			{
			case 0:
				Num = 25;
				player->SetHitPoints(player->GetCurrentHitPoints() + Num);
				Display->text("You were healed: ", 13, 8, white); cout << Num;
				Display->DisplayCure(Num);
				break;
			case 1:
				Num = 15;
				player->SetKa(player->GetCurrentKa() + Num);
				Display->text("Ka recovered: ", 13, 8, white); cout << Num;
				Display->DisplayCure(Num);
				break;
			case 2:
				Num = rand() % 5 + 1;
				player->SetStrength(player->GetStrength() + Num);
				Display->text("Your strength has increased", 13, 8, white);
				Sleep(player->GetPauseDuration());
				break;
			case 3:
				Num = rand() % 5 + 1;
				player->SetMind(player->GetMind() + Num);
				Display->text("Your mind powers have increased", 13, 8, white);
				Sleep(player->GetPauseDuration());
				break;
			case 4:
				Num = 75;
				player->SetHitPoints(player->GetCurrentHitPoints() + Num);
				Display->text("You were healed: ", 13, 8, white); cout << Num;
				Display->DisplayCure(Num);
				break;
			case 5:
				Num = 150;
				player->SetHitPoints(player->GetCurrentHitPoints() + Num);
				Display->text("You were healed: ", 13, 8, white); cout << Num;
				Display->DisplayCure(Num);
				break;
			case 6:
				Num = player->GetMaxHitPoints() - player->GetCurrentHitPoints();
				player->SetHitPoints(player->GetMaxHitPoints());
				Display->text("You were healed: ", 13, 8, white); cout << Num;
				Display->DisplayCure(Num);
			default:
				Display->text("This item does nothing", 13, 8, white);
				Sleep(player->GetPauseDuration());
				break;
			}
			itemWasUsed = true;
			Display->clrtop(2);
			if (isFighting)
				escapeWasPressed = true;
		}

	}// End While escapeWasPressed && playerInventory.size() > 0
	Display->clrbottom();
}
void MainMenuSystem::MagicMenu(Player* player, vector<Magic*>& spells)
{
	int choice = 0;
	int offset = 0;
	unsigned int i;
	bool escapeWasPressed = false;
	bool selectionWasMade = false;
	COORD cursorPosition;

	while (!escapeWasPressed)
	{
		Display->clear();
		Display->DisplayPlayerInfo(player);
		cursorPosition.X = 14;
		cursorPosition.Y = 13;
		selectionWasMade = false;
		Display->text("/----------------\\", 13, 12, green);
		Display->text("|                |", 13, 13, green);
		Display->text("|                |", 13, 14, green);
		Display->text("|                |", 13, 15, green);
		Display->text("|                |", 13, 16, green);
		Display->text("|                |", 13, 17, green);
		Display->text("|                |", 13, 18, green);
		Display->text("|                |", 13, 19, green);
		Display->text("|                |", 13, 20, green);
		Display->text("|                |", 13, 21, green);
		Display->text("|                |", 13, 22, green);
		Display->text("\\----------------/", 13, 23, green);


		for (i = 0; i < spells.size(); i++)
		{
			if (spells[i]->GetIsInFight() || player->GetCurrentKa() < spells[i]->GetCost())
			{
				Display->DisplaySpellName(spells[i]->GetName(), i + 13, dkgreen);
			}
			else
				Display->DisplaySpellName(spells[i]->GetName(), i + 13, green);			
		}
		DrawCursor(cursorPosition, yellow, 175);
		offset = cursorPosition.Y - 13;
		Display->DisplayCastingCost(spells[offset]->GetCost());
		do
		{
			if (MoveCursor(cursorPosition, selectionWasMade, escapeWasPressed, 13, 12 + static_cast<int>(spells.size())))
			{
				DrawCursor(cursorPosition, yellow, 175);
				offset = cursorPosition.Y - 13;
				Display->DisplayCastingCost(spells[offset]->GetCost());
			}
			Display->text(" ", 79, 23, white);
		} while (!selectionWasMade);
		if (escapeWasPressed)
			return;
		offset = cursorPosition.Y - 13;
		if (spells[offset]->GetIsInFight())
		{
			Display->text("That spell can only be used in battle", 13, 9, white);
			Sleep(player->GetPauseDuration());
		}
		else if (player->GetCurrentKa() < spells[offset]->GetCost())
		{
			Display->text("Not enough Ka", 13, 9, white);
			Sleep(player->GetPauseDuration());
		}
		else
			spells[offset]->Cast(player, NULL, Display);
	}//==================================End of While loop==================================
}

void MainMenuSystem::InFightMagicMenu(Player* player, Creature* enemy, vector<Magic*>& spells, bool& pressedEscape)
{
	int choice = 0;
	int offset = 0;
	unsigned int i;
	bool selectionWasMade = false;
	COORD cursorPosition;

	pressedEscape = false;
	while (!pressedEscape)
	{
		cursorPosition.X = 14;
		cursorPosition.Y = 13;
		selectionWasMade = false;
		Display->text("/----------------\\", 13, 12, green);
		Display->text("|                |", 13, 13, green);
		Display->text("|                |", 13, 14, green);
		Display->text("|                |", 13, 15, green);
		Display->text("|                |", 13, 16, green);
		Display->text("|                |", 13, 17, green);
		Display->text("|                |", 13, 18, green);
		Display->text("|                |", 13, 19, green);
		Display->text("|                |", 13, 20, green);
		Display->text("|                |", 13, 21, green);
		Display->text("|                |", 13, 22, green);
		Display->text("\\----------------/", 13, 23, green);

		for (i = 0; i < spells.size(); i++)
		{
			if (spells[i]->GetIsInFight() || player->GetCurrentKa() < spells[i]->GetCost())
			{
				Display->DisplaySpellName(spells[i]->GetName(), i + 13, dkgreen);
			}
			else
				Display->DisplaySpellName(spells[i]->GetName(), i + 13, green);
		}
		Display->DisplayCastingCost(spells[offset]->GetCost());
		DrawCursor(cursorPosition, yellow, 175);
		do
		{
			if (MoveCursor(cursorPosition, selectionWasMade, pressedEscape, 13, 12 + static_cast<int>(spells.size())))
			{
				DrawCursor(cursorPosition, yellow, 175);
				offset = cursorPosition.Y - 13;
				Display->DisplayCastingCost(spells[offset]->GetCost());
			}
			Display->text(" ", 79, 23, white);
		} while (!selectionWasMade);
		if (pressedEscape)
			return;
		offset = cursorPosition.Y - 13;
		if (player->GetCurrentKa() < spells[offset]->GetCost())
		{
			Display->text("Not enough Ka", 13, 9, white);
			Sleep(player->GetPauseDuration());
		}
		else
		{
			ClearTextBottomRight(11);
			Magic* castedSpell = spells[offset];
			Display->DisplayIncantation(castedSpell->GetDescription(), castedSpell->GetIncantation());
			castedSpell->Cast(player, enemy, Display);
			return;
		}
	}//==================================End of While loop==================================
}

bool MainMenuSystem::TalkTo(Greeting* greeting, int pauseDuration)
{
	int choice;
	int X = 13;
	bool bSel;
	bool bEsc;
	COORD CursPos;

	Display->clr();
	CursPos.X = 2;
	CursPos.Y = 12;
	bSel = false;
	Display->text(greeting->GetIntroduction(), 13, 11, white);
	Display->text("/---------\\", 1, 11, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	Display->text("|  Talk   |", 1, 12, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	Display->text("|  Fight  |", 1, 13, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	Display->text("|         |", 1, 14, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	Display->text("|         |", 1, 15, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	Display->text("\\---------/", 1, 16, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	Display->text("           ", 1, 17, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	DrawCursor(CursPos, yellow, 175);
	do
	{
		if (MoveCursor(CursPos, bSel, bEsc, 12, X))
		{
			DrawCursor(CursPos, yellow, 175);
		}
		Display->text(" ", 79, 23, white);
	} while (!bSel);
	choice = CursPos.Y;
	switch (choice)
	{
	case 12:
		Display->text(greeting->GetDetails(), 13, 9, white);
		Sleep(pauseDuration);
		return greeting->GetIsHostile();
		break;
	case 13:
		return true;
		break;
	}
}

void MainMenuSystem::ClearTextBottomRight(int Y)
{
	while (Y < 24)
	{
		Display->text("                                                                 ", 13, Y, FOREGROUND_BLUE);
		Y++;
	}
}

//=========================================================================================================
//	This function moves all the elements of the playerInventory DOWN by one.
//After taking element X and putting it aside. It then puts element X at the end.
//Usually to pop the last element off.
//=========================================================================================================
void MainMenuSystem::SlideDown(vector<Item*>& playerInventory, int X)
{
	int Y = X + 1;
	Item* temp;

	temp = playerInventory[X];
	while (Y <= static_cast<int>(playerInventory.size() - 1))
	{
		playerInventory[X] = playerInventory[Y];
		X++;
		Y++;
	}
	playerInventory[playerInventory.size() - 1] = temp;
}

void MainMenuSystem::DrawCursor(COORD pos, WORD color, unsigned char curs)
{
	HANDLE OutputH;
	OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(OutputH, color);
	SetConsoleCursorPosition(OutputH, pos);

	cout << curs;
}

bool MainMenuSystem::MoveCursor(COORD& CursPos, bool& bSelect, bool& bEsc, int Ymin, int Ymax)
{
	INPUT_RECORD InputRecord;
	COORD OldCursPos = CursPos;
	DWORD Events = 0;
	HANDLE hInput;
	bool bCursMov = false;
	int bKeyDown = 0;
	hInput = GetStdHandle(STD_INPUT_HANDLE);
	ReadConsoleInput(hInput, &InputRecord, 1, &Events);
	bKeyDown = InputRecord.Event.KeyEvent.bKeyDown;
	if (InputRecord.EventType == KEY_EVENT && bKeyDown)
	{
		if (InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
		{
			if (CursPos.Y < Ymax)
			{
				CursPos.Y++;
				bCursMov = true;
			}
			else
			{
				CursPos.Y = Ymin;
				bCursMov = true;
			}
		}
		else if (InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_UP)
		{
			if (CursPos.Y > Ymin)
			{
				CursPos.Y--;
				bCursMov = true;
			}
			else
			{
				CursPos.Y = Ymax;
				bCursMov = true;
			}
		}
		else if (InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
		{
			bCursMov = false;
			//cout << "-";
			bSelect = true;
		}
		else if (InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_SPACE)
		{
			bCursMov = false;
			bSelect = true;
		}
		else if (InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
		{
			bCursMov = false;
			bSelect = true;
			bEsc = true;
		}
	}
	if (bCursMov)
	{
		HANDLE	OutputH;
		OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(OutputH, OldCursPos);
		cout << "  ";
		return true;
	}
	return false;
}