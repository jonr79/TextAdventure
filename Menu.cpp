#include <fstream>
#include <string>
#include <windows.h>
#include <vector>

#include "world.h"
#include "creature.h"
#include "enemies.h"
#include "item.h"
#include "location.h"
#include "player.h"

#define box FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define white FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define yellow FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define brown FOREGROUND_RED | FOREGROUND_GREEN
#define green FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define dkgreen FOREGROUND_GREEN
#define blue FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define ftext BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY

void world::menu(player *p2, vector<magik*> &spells,vector<item*> &stuff, vector<item*> &pstuff,string &Map)
{
	int choice = 0;
	bool bEsc = false;
	bool bSel = false;
	COORD CursPos; 

	while(!bEsc)
	{
		clr(11);
		p2->info();	
		ground(stuff,Map,p2->getX(),p2->getY());
		CursPos.X = 2;
		CursPos.Y = 12;
		bSel = false;
		text("/---------\\",1,11,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Inv.   |",1,12,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Status |",1,13,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Magic  |",1,14,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Options|",1,15,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("\\---------/",1,16,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("           ",1,17,blue);

		DrawCurs(CursPos,yellow,175);
		do
		{
			if(MoveCurs(CursPos,bSel,bEsc,12,15))
			{
				DrawCurs(CursPos,yellow,175);
			}
			text(" ", 79, 23,white);
		}while(!bSel);
		if(bEsc)
			break;

		choice = CursPos.Y;
		switch(choice)
		{
		case 12:
			inventory(p2,stuff,pstuff,Map);
			break;
		case 13:
			p2->status();
			break;
		case 14:
			if(p2->getbspells())
				magic(p2,spells);
			else
			{
				text("You have no magic",13,11,white);
				Sleep(p2->getPause());
				text("                 ",13,11,white);
			}
			break;
		case 15:
			options(p2,stuff,pstuff,spells,Map);
			break;		
		}
	}// End of While(bEsc)


}
void world::inventory(player *p2,vector<item*> &stuff, vector<item*> &pstuff,string Map)
{
	unsigned int Offset;
	int choice = 0;
	int Y = 11;
	bool bEsc = false;
	bool bSel = false;
	bool Iused = false;
	bool Pick;
	bool Use;
	COORD CursPos = {2,12};

	while(!bEsc)
	{		
		ground(stuff,Map,p2->getX(),p2->getY());
		items(pstuff);
		CursPos.X = 2;
		CursPos.Y = 12;
		bSel = false;
		Pick = false;
		Use = false;
		text("/---------\\",1,11,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  PickUp |",1,12,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Utilize|",1,13,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|         |",1,14,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|         |",1,15,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("\\---------/",1,16,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		DrawCurs(CursPos,yellow,175);
		do
		{
			if(MoveCurs(CursPos,bSel,bEsc,12,13))
			{
				DrawCurs(CursPos,yellow,175);
			}
			text("",79,23,white);
		}while(!bSel);
		if(bEsc)
			break;

		choice = CursPos.Y;
		switch(choice)
		{
		case 12:
			Pick = true;
			break;
		case 13:
			Use = true;
			break;
		}

		if(Pick)
		{
			item *temp;
			if(pstuff.size() < 12)
			{
				for(Offset = 0;Offset < stuff.size();Offset++)
				{
					if(stuff[Offset]->getY() == p2->getY() && stuff[Offset]->getX() == p2->getX() && stuff[Offset]->getmap() == Map)
					{
						pstuff.push_back(stuff[Offset]);
						temp = stuff[stuff.size()-1];
						stuff[stuff.size()-1] = stuff[Offset];
						stuff[Offset] = temp;
						stuff.pop_back();
						break;
					}
				}
				clrtop(2);
			}
			else
			{
				text("You do not have enough room!",13,9,white);
				Sleep(3000);
				text("                            ",13,9,white);
			}
		}
		if(Use)
		{
			choice = 0;
			bEsc = false;
			bSel = false;
			bool bFight = false;
			
			while(!bEsc)
			{
				//clr();
				//clrtop();
				items(pstuff);
				ground(stuff,Map,p2->getX(),p2->getY());
				CursPos.X = 2;
				CursPos.Y = 12;
				bSel = false;
				text("/---------\\",1,11,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				text("|  Use    |",1,12,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				text("|  Equip  |",1,13,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				text("|  Drop   |",1,14,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				text("|  XXXX   |",1,15,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				text("\\---------/",1,16,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

				DrawCurs(CursPos,yellow,175);
				do
				{
					if(MoveCurs(CursPos,bSel,bEsc,12,15))
					{
						DrawCurs(CursPos,yellow,175);
					}
					text(" ", 79, 23,white);
				}while(!bSel);
				if(bEsc)
					break;
				choice = CursPos.Y;
				switch(choice)
				{
				case 12:
					if(pstuff.size() > 0)
						use(p2,pstuff,Iused,bFight);
					else
					{
						text("You have nothing to use",13,11,white);
						Sleep(p2->getPause());
					}
					break;
				case 13:
					if(pstuff.size() > 0)
						equip(p2,pstuff);
					else
					{
						text("You have nothing to equip",13,12,white);
						Sleep(p2->getPause());
					}
					break;
				case 14:
					if(pstuff.size() > 0)
						drop(p2,stuff,pstuff,Map);
					else
					{
						text("You have nothing to drop",13,12,white);
						Sleep(p2->getPause());
					}
					break;
				case 15:
					if(pstuff.size() > 0)
						XXX(pstuff);
					else
					{
						text("You have nothing to dispose of",13,12,white);
						Sleep(p2->getPause());
					}
					break;
				}//--------End Switch
			}//------------End While bEsc
			clr(12);
		}//----------------End if(Use)
	}//--------------------End While bEsc
}//------------------------End Inventory function

void world::items(vector<item*> &pstuff)
{
	//clr();
	unsigned int Offset = 0;
	text(" [---Items---] ",13,11,brown);
	while(Offset < pstuff.size())
	{		
		text(pstuff[Offset]->getname(),15,12+Offset,yellow);
		Offset++;
		if(Offset >= 12)
			break;
	}
}

void world::equip(player *p2,vector<item*> &pstuff)
{
	unsigned int Offset = 0;
	int choice = 0;
	bool bEsc = false;
	bool bSel = false;
	item *temp;
	COORD CursPos;

	while(!bSel)
	{
		bSel = false;
		CursPos.X = 13;
		CursPos.Y = 12;
		Offset = CursPos.Y - 12;
		DrawCurs(CursPos,yellow,175);
		pstuff[Offset]->display();
		do
		{
			choice = static_cast<int>(11+pstuff.size());
			if(MoveCurs(CursPos,bSel,bEsc,12,choice))
			{
				DrawCurs(CursPos,yellow,175);
				Offset = CursPos.Y - 12;
				pstuff[Offset]->display();
			}
			text(" ", 79, 23,white);
		}while(!bSel);
		if(bEsc)
		{
			text("          ",1,22,white);
			text("          ",1,23,white);
			clrbottom();
			items(pstuff);
			return;
		}
	}// End While bSel
	Offset = CursPos.Y - 12;
	temp = pstuff[Offset];
	if(!temp->getarm() && !temp->getweap())
	{
		text("You cannot equip that.",13,9,white);
		Sleep(p2->getPause());
	}
	if(temp->getarm())
	{
		pstuff.push_back(p2->getarmor());
		p2->setarmor(loadArmor(temp->getname()));
		pstuff[Offset] = pstuff[pstuff.size()-1];
		pstuff[pstuff.size()-1] = temp;
		pstuff.pop_back();		
	}
	if(temp->getweap())
	{
		pstuff.push_back(p2->getweapon());
		p2->setweapon(loadWeapon(temp->getname()));
		pstuff[Offset] = pstuff[pstuff.size()-1];
		pstuff[pstuff.size()-1] = temp;
		pstuff.pop_back();		
	}
	text("              ",CursPos.X,CursPos.Y,white);
	p2->info();
	items(pstuff);
	text("                                           ",175,11,white);
	text("           ",1,22,white);
	text("           ",1,23,white);
}

void world::drop(player *p2,vector<item*> &stuff,vector<item*> &pstuff,string Map)
{
	unsigned int Offset = 0;
	int choice = 0;
	bool bEsc = false;
	bool bSel = false;
	//item *temp;
	COORD CursPos;

	
	while(!bEsc)
	{
		ground(stuff,Map,p2->getX(),p2->getY());
		items(pstuff);
		bSel = false;
		CursPos.X = 13;
		CursPos.Y = 12;
		DrawCurs(CursPos,yellow,175);
		do
		{
			choice = static_cast<int>(11+pstuff.size());
			if(MoveCurs(CursPos,bSel,bEsc,12,choice))
			{
				DrawCurs(CursPos,yellow,175);
				Offset = CursPos.Y - 12;
				pstuff[Offset]->display();
			}
			text(" ", 79, 23,white);
		}while(!bSel);
		if(bEsc)
		{
			text("          ",1,22,white);
			text("          ",1,23,white);
			clrbottom();
			items(pstuff);
			return;
		}
	

		Offset = CursPos.Y - 12;
		pstuff[Offset]->setmap(Map);
		pstuff[Offset]->setX(p2->getX());
		pstuff[Offset]->setY(p2->getY());
		stuff.push_back(pstuff[Offset]);
		slidedown(pstuff,Offset);
		pstuff.pop_back();	
		text("  ",13,12,white);
		text("                    ",13,12+Offset,white);
		text("                    ",13,static_cast<int>(12+pstuff.size()),white);
		if(pstuff.size() < 1)
			return;
	}//end While bEsc
}
void world::XXX(vector<item*> &pstuff)
{
	unsigned int Offset = 0;
	int choice = 0;
	bool bEsc = false;
	bool bSel = false;
	//item *temp;
	COORD CursPos;

	while(!bEsc)
	{
		items(pstuff);
		bSel = false;
		CursPos.X = 13;
		CursPos.Y = 12;
		DrawCurs(CursPos,yellow,175);
		do
		{
			choice = static_cast<int>(11+pstuff.size());
			if(MoveCurs(CursPos,bSel,bEsc,12,choice))
			{
				DrawCurs(CursPos,yellow,175);
				Offset = CursPos.Y - 12;
				pstuff[Offset]->display();
			}
			text(" ", 79, 23,white);
		}while(!bSel);
		if(bEsc)
		{
			text("          ",1,22,white);
			text("          ",1,23,white);
			clrbottom();
			items(pstuff);
			return;
		}
			
		Offset = CursPos.Y - 12;
		bSel = false;
		text(pstuff[Offset]->getname(),15,CursPos.Y,FOREGROUND_RED | FOREGROUND_INTENSITY);
		do
		{
			
			if(MoveCurs(CursPos,bSel,bEsc,CursPos.Y,CursPos.Y))
			{
				DrawCurs(CursPos,yellow,175);
			}
			text(" ", 79, 23,white);
		}while(!bSel);
		if(bEsc)
			return;
		slidedown(pstuff,Offset);
		pstuff.pop_back();
		clr(12);
		text("                    ",13,static_cast<int>(12+pstuff.size()),white);
		if(pstuff.size() < 1)
			return;

	}// End While bEsc
	
}

void world::options(player *p2, vector<item*> &stuff,vector<item*> &pstuff,vector<magik*> &M, string &Map)
{
	int choice = 0;
	bool bEsc = false;
	bool bSel = false;
	COORD CursPos; 

	while(!bEsc)
	{
		//clear();       For smooth look remarked out 2/15/06
		p2->info();	
		CursPos.X = 2;
		CursPos.Y = 12;
		bSel = false;
		text("/---------\\",1,11,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Save   |",1,12,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Load   |",1,13,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Setup  |",1,14,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Quit   |",1,15,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("\\---------/",1,16,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		DrawCurs(CursPos,yellow,175);
		do
		{
			if(MoveCurs(CursPos,bSel,bEsc,12,15))
			{
				DrawCurs(CursPos,yellow,175);
			}
			text(" ", 79, 23,white);
		}while(!bSel);
		if(bEsc)
			break;

		choice = CursPos.Y;
		switch(choice)
		{
		case 12:
			text("Saving game ",13,11,yellow);
			save(p2,stuff,pstuff,M,Map);
			Sleep(p2->getPause());
			text("            ",13,11,yellow);
			break;
		case 13:
			load(p2,stuff,pstuff,M,Map);
			break;
		case 14:
			setup(p2,stuff,pstuff,M,Map);		
			break;
		case 15:
			system("cls");
			text("See you next time.",13,11,yellow);
			Sleep(p2->getPause());
			exit(0);
			break;		
		}
	}// End of While(bEsc)
}
/*=====================================================================================
	Input for the setup of the game. Music, speed, etc.
=====================================================================================*/
void world::setup(player *p2, vector<item*> &stuff,vector<item*> &pstuff,vector<magik*> &M, string &Map)
{
	int choice = 0;
	bool bEsc = false;
	bool bSel = false;
	COORD CursPos; 

	while(!bEsc)
	{
		p2->info();	
		CursPos.X = 2;
		CursPos.Y = 12;
		bSel = false;
		text("/---------\\",1,11,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Music  |",1,12,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Speed  |",1,13,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Invis  |",1,14,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("|  Drive  |",1,15,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		text("\\---------/",1,16,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		/*=======================================
		Displays the current music status
		========================================*/
		if(CursPos.Y == 12 && p2->getMusic())
			text("Music: On    ",13,12,yellow);
		else if(CursPos.Y == 12 && !p2->getMusic())
			text("Music: Off   ",13,12,yellow);
		//========================================
		DrawCurs(CursPos,yellow,175);
		do
		{
			if(MoveCurs(CursPos,bSel,bEsc,12,15))
			{
				//clr(11);
				DrawCurs(CursPos,yellow,175);
				/*=======================================
				Displays the current music status
				========================================*/
				if(CursPos.Y == 12 && p2->getMusic())
					text("Music: On     ",13,12,yellow);
				else if(CursPos.Y == 12 && !p2->getMusic())
					text("Music: Off    ",13,12,yellow);
				/*========================================
				Displays current player pause duration
				========================================*/
				if(CursPos.Y == 13){
					text("Speed: ",13,12,yellow);
					cout << p2->getPause();}
				/*========================================
				Displays the current invisibility status
				=========================================*/
				if(CursPos.Y == 14 && p2->getInvis())
					text("Invis: On     ",13,12,yellow);
				else if(CursPos.Y == 14 && !p2->getInvis())
					text("Invis: Off    ",13,12,yellow);
				//========================================
			}
			text(" ", 79, 23,white);
		}while(!bSel);
		if(bEsc)
			break;

		choice = CursPos.Y;
		switch(choice)
		{
		case 12:
			text("Changing Music.",13,11,yellow);
			p2->changeMusic();
			Sleep(p2->getPause());
			text("               ",13,11,yellow);
			break;
		case 13:
			text("Changing Speed?",13,11,yellow);
			Sleep(p2->getPause());
			text("               ",13,11,yellow);
			break;
		case 14:
			text("To be added later.",13,11,yellow);
			Sleep(p2->getPause());
			text("               ",13,11,yellow);
			break;
		case 15:
			text("To be added later.",13,11,yellow);
			Sleep(p2->getPause());
			text("               ",13,11,yellow);
			break;		
		}
	}// End of While(bEsc)
}
void world::useItem(player *p2,vector<item*> &stuff,vector<item*> &pstuff, bool &bFight,bool &bLeave,string Map)
{
	int choice = 0;
	bool bEsc = false;
	bool bSel = false;
	bool Iused = false;
	COORD CursPos;
	
	while(!bEsc)
	{		
		clr(12);
		if(bFight && Iused)
			break;		
		p2->info();
		items(pstuff);
		if(!bFight)
			ground(stuff,Map,p2->getX(),p2->getY());
		CursPos.X = 2;
		CursPos.Y = 15;
		bSel = false;
		text("/---------\\",1,14,ftext);
		text("|  Use    |",1,15,ftext);
		text("|  Equip  |",1,16,ftext);
		text("|  Drop   |",1,17,ftext);
		text("|  XXXX   |",1,18,ftext);
		text("\\---------/",1,19,ftext);

		DrawCurs(CursPos,ftext,175);
		do
		{
			if(MoveCurs(CursPos,bSel,bEsc,15,18))
			{
				DrawCurs(CursPos,ftext,175);
			}
			text("", 79, 23,ftext);
		}while(!bSel);
		if(bEsc)
		{
			bLeave = true;
			break;
		}
		choice = CursPos.Y;
		switch(choice)
		{
		case 15:
			if(pstuff.size() > 0)
				use(p2,pstuff,Iused,bFight);			
			else
			{
				text("You have no items.",13,11,white);
				Sleep(p2->getPause());
			}
			break;
		case 16:
			if(pstuff.size() > 0)
				equip(p2,pstuff);
			else
			{
				text("You have nothing to equip",13,12,white);
				Sleep(p2->getPause());
			}
			break;
		case 17:
			if(pstuff.size() > 0)
				drop(p2,stuff,pstuff,Map);
			else
			{
				text("You have nothing to drop",13,12,white);
				Sleep(p2->getPause());
			}
			break;
		case 18:
			if(pstuff.size() > 0)
				XXX(pstuff);
			else
			{
				text("You have nothing to dispose of",13,12,white);
				Sleep(p2->getPause());
			}
			break;
		}//--------End Switch
	}//------------End While bEsc
	clr(12);
}
void world::use(player *p2,vector<item*> &pstuff,bool &Iused,bool bFight)
{
	weapon W;
	armor  A;
	//item *temp;
	item *iUsed;
	unsigned int Offset = 0;
	int choice = 0;
	int Num;
	bool bEsc = false;
	bool bSel = false;
	COORD CursPos;

	
	while(!bEsc && pstuff.size() > 0)
	{
		clritems();
		items(pstuff);
		p2->info();

		if(Iused && bFight)
			break;
		bEsc = false;
		bSel = false;
		CursPos.X = 13;
		CursPos.Y = 12;
		DrawCurs(CursPos,yellow,175);
		
		pstuff[Offset]->display();
		do
		{
			choice = static_cast<int>(11+pstuff.size());
			if(MoveCurs(CursPos,bSel,bEsc,12,choice))
			{
				DrawCurs(CursPos,yellow,175);				
				Offset = CursPos.Y - 12;
				pstuff[Offset]->display();
			}
			text(" ", 79, 23,white);
		}while(!bSel);
	
		if(bEsc)
		{
			text("          ",1,22,white);
			text("          ",1,23,white);
			clrbottom();
			items(pstuff);
			return;
		}
		Offset = CursPos.Y - 12;
		iUsed = pstuff[Offset];
		if(!bEsc)
		{	
			if(!iUsed->getkeep())
			{
				slidedown(pstuff,Offset);
				pstuff.pop_back();
				text("                   ",13,static_cast<int>(12+pstuff.size()),white);
			}


			switch(iUsed->getType())
			{
			case 0:
				Num = 25; 
				p2->setHP(p2->getHP()+Num);
				text("You were healed: ",13,8,white); cout << Num;
				cure(Num);
				break;
			case 1:
				Num = 15;
				p2->setka(p2->getka()+Num);
				text("Ka recovered: ",13,8,white); cout << Num;
				cure(Num);
				break;
			case 2:
				Num = rand()%5 + 1;
				p2->setstr(p2->getstr()+Num);
				text("Your strength has increased",13,8,white);
				Sleep(p2->getPause());
				break;
			case 3:
				Num = rand()%5 + 1;
				p2->setmind(p2->getmind()+Num);
				text("Your mind powers have increased",13,8,white);
				Sleep(p2->getPause());
				break;
			case 4:
				Num = 75; 
				p2->setHP(p2->getHP()+Num);
				text("You were healed: ",13,8,white); cout << Num;
				cure(Num);
				break;
			case 5:
				Num = 150; 
				p2->setHP(p2->getHP()+Num);
				text("You were healed: ",13,8,white); cout << Num;
				cure(Num);
				break;
			case 6:
				Num = p2->getMHP() - p2->getHP();
				p2->setHP(p2->getMHP());
				text("You were healed: ",13,8,white); cout << Num;
				cure(Num);
			default:
				text("This item does nothing",13,8,white);
				Sleep(p2->getPause());
				break;
			}
			Iused = true;
			clrtop(2);
			if(bFight)
				bEsc = true;
		}	
		
	}// End While bEsc && pstuff.size() > 0
	clrbottom();
}
void world::magic(player *p2,vector<magik*> &spells)
{
	int choice = 0;
	int offset = 0;
	unsigned int i;
	bool bEsc = false;
	bool bSel = false;
	COORD CursPos; 
	goblin A;
	creature *guy = &A;

	while(!bEsc)
	{
		clear();
		p2->info();	
		CursPos.X = 14;
		CursPos.Y = 13;
		bSel = false;
		text("/----------------\\",13,12,green);
		text("|                |",13,13,green);
		text("|                |",13,14,green);
		text("|                |",13,15,green);
		text("|                |",13,16,green);
		text("|                |",13,17,green);
		text("|                |",13,18,green);
		text("|                |",13,19,green);
		text("|                |",13,20,green);
		text("|                |",13,21,green);
		text("|                |",13,22,green);
		text("\\----------------/",13,23,green);
		

		for(i = 0;i < spells.size();i++)
		{
			if(spells[i]->getbInfight() || p2->getka() < spells[i]->getcost())
			{
				spells[i]->tout(i+13,dkgreen);
			}
			else
				spells[i]->tout(i+13,green);
		}
		DrawCurs(CursPos,yellow,175);
		offset = CursPos.Y - 13;
		spells[offset]->Disp();
		do
		{
			if(MoveCurs(CursPos,bSel,bEsc,13,12 + static_cast<int>(spells.size())))
			{
				DrawCurs(CursPos,yellow,175);
				offset = CursPos.Y - 13;
				spells[offset]->Disp();
			}
			text(" ", 79, 23,white);
		}while(!bSel);
		if(bEsc)
			return;
		offset = CursPos.Y - 13;
		if(spells[offset]->getbInfight())
		{
			text("That spell can only be used in battle",13,9,white);
			Sleep(p2->getPause());
		}
		else if(p2->getka() < spells[offset]->getcost())
		{
			text("Not enough Ka",13,9,white);
			Sleep(p2->getPause());
		}
		else
			spells[offset]->use(p2,guy);
	}//==================================End of While loop==================================
}

void world::fmagic(player *p2,creature *enemy,vector<magik*> &spells,bool &bEsc)
{
	int choice = 0;
	int offset = 0;
	unsigned int i;
	bool bSel = false;
	COORD CursPos; 
	goblin A;
	creature *guy = &A;

	bEsc = false;
	while(!bEsc)
	{
		CursPos.X = 14;
		CursPos.Y = 13;
		bSel = false;
		text("/----------------\\",13,12,green);
		text("|                |",13,13,green);
		text("|                |",13,14,green);
		text("|                |",13,15,green);
		text("|                |",13,16,green);
		text("|                |",13,17,green);
		text("|                |",13,18,green);
		text("|                |",13,19,green);
		text("|                |",13,20,green);
		text("|                |",13,21,green);
		text("|                |",13,22,green);
		text("\\----------------/",13,23,green);
		
		for(i = 0;i < spells.size();i++)
		{
			if(p2->getka() < spells[i]->getcost())
			{
				spells[i]->tout(i+13,dkgreen);
			}
			else
				spells[i]->tout(i+13,green);
		}
		spells[offset]->Disp();
		DrawCurs(CursPos,yellow,175);
		do
		{
			if(MoveCurs(CursPos,bSel,bEsc,13,12 + static_cast<int>(spells.size())))
			{
				DrawCurs(CursPos,yellow,175);
				offset = CursPos.Y - 13;
				spells[offset]->Disp();
			}
			text(" ", 79, 23,white);
		}while(!bSel);
		if(bEsc)
			return;
		offset = CursPos.Y - 13;
		if(p2->getka() < spells[offset]->getcost())
		{
			text("Not enough Ka",13,9,white);
			Sleep(p2->getPause());
		}
		else
		{
			clr(11);
			spells[offset]->use(p2,enemy);
			return;
		}
	}//==================================End of While loop==================================
}


void world::clr(int Y)
{
	while(Y < 24)
			{
				text("                                                                 ",13,Y,FOREGROUND_BLUE);
				Y++;
			}
}
//=========================================================================================================
//	This function moves all the elements of the pstuff DOWN by one.
//After taking element X and putting it aside. It then puts element X at the end.
//Usually to pop the last element off.
//=========================================================================================================
void world::slidedown(vector<item*> &pstuff,int X)
{
	int Y = X + 1;
	item *temp;

	temp = pstuff[X];
	while(Y <= static_cast<int>(pstuff.size()-1))
	{
		pstuff[X] = pstuff[Y];
		X++;
		Y++;
	}
	pstuff[pstuff.size()-1] = temp;
}


















