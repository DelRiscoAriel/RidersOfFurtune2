#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <time.h>
#include <fstream>
#include <stack>
#include "Yarra.h"
#include "Reaper.h"
//Wait a Second Function
#include <chrono>
#include <thread>

using namespace std;

//Globals
const int ROWS = 8;
const int COLUMNS = 8;
char gameBoard[ROWS][COLUMNS];

Player Yarra;

struct saveWeapons
{
	string weaponName;
	int damage;
};
struct readSaveGame
{
	int turns;
	int hp;
	int exp;
	int positionX;
	int positionY;
	int alligment;
	vector<saveWeapons> weaponVect;
	int itemsNum;
	string* saveItems = new string[20];
	string weaponName;
	int damage;
};

struct Blacksmith
{
	string weaponName;
	int damage;
	int turn;
};
stack< Blacksmith, vector<Blacksmith> > blacksmithWeapons;

//Funtions Prototyps
void drawBoard();
void showBoard(int, int);
void displeyUI();
void changePosition(int&, int&, int);
void processSpace(Player&, int&);
int randomNumber(int, int);
void leaveDespair();

void populateStruct(readSaveGame&, Player&);
void outputGameSate(const readSaveGame&, Player&);

//Wait for a Second
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;
//---------------------------------

int main()
{
	fstream saveFile;
    int spacesToMove = 0;
    int positionX = 0;
    int positionY = 0;
	string choice;
	string choice2;

	cout << "-----Welcome to Riders Of Furtune 2: Dungeon Escape-------" << endl;
	cout << "1- New Game." << endl
		<< "2- Load Game." << endl;
	cin >> choice;
	while (choice != "1" && choice != "2")
	{
		cout << "Wrong input. Choose again: ";
		cin >> choice;
	}

	drawBoard();

	//Start new game
	if (choice == "1")
	{
		//Add deafult weapon and Item if Inventory is empty
		Yarra.addWeapon("Knife", 0);
		Yarra.addItem("PotionLv1");

		while (spacesToMove != -1)
		{
			if (Yarra.getTurn() == 0)
			{
				displeyUI();
				showBoard(positionX, positionY);
			}
			
			cout << "Press Enter to Roll the Dice: ";
			spacesToMove = randomNumber(1, 6);
			cin.ignore();
			getline(cin, choice2);
			cout << "Rolling Dice.... ";
			sleep_until(system_clock::now() + 1s); //Wait for Roll Dice
			cout << spacesToMove << endl;

			changePosition(positionX, positionY, spacesToMove);

			Yarra.setPosition(positionX, positionY);
			processSpace(Yarra, spacesToMove);
		}
	}

	//Load old game
	if (choice == "2")
	{
		readSaveGame readSave;
		saveFile.open("GameSave.txt", ios::in | ios::out);
		saveFile.read(reinterpret_cast<char*>(&readSave), sizeof(readSave));
		outputGameSate(readSave, Yarra);
		saveFile.close();

		positionX = Yarra.getPositionX();
		positionY = Yarra.getPositionY();

		int z = 0; //Display UI only once
		while (spacesToMove != -1)
		{		
			if (z == 0)
			{
				displeyUI();
				showBoard(Yarra.getPositionX(), Yarra.getPositionY());
				z++;
			}
			
			cout << "Press Enter to Roll the Dice: ";
			int z = 0;
			cin >> z;
			spacesToMove = randomNumber(1, 6);
			cout << "Rolling Dice.... ";
			sleep_until(system_clock::now() + 1s); //Wait for Roll Dice
			cout << spacesToMove << endl;

			changePosition(positionX, positionY, spacesToMove);

			Yarra.setPosition(positionX, positionY);
			processSpace(Yarra, spacesToMove);
		}
	}	
}

void displeyUI()
{
	cout << endl;
	cout << "Legend: C-Cathedral | G-Graveyard | W-Well of Refelction | B-Blacksmith | L-Lake of Despair | E-Dungeon Exit" << endl;
	cout << "Turn: " << Yarra.getTurn() << "| Health: " << Yarra.getHP() << " | EXP: " << Yarra.getEXP()
		<< " | Position: X" << Yarra.getPositionX() << " Y" << Yarra.getPositionY();
	string z;
	if (Yarra.getAlligment() == 0)
		z = "Good";
	else
		z = "Bad";
	cout << " | Alligment: " << z << " | Equiped Weapon: " << Yarra.getWeaponName()
		<< "(+" << Yarra.getWeaponDamage() << ")" << endl;//<< " |Current space: ";

	//Show current level
	//cout << " |Current space: " << gameBoard[Yarra.getPositionX()][Yarra.getPositionY()] << endl;
}

void processSpace(Player& theplayer, int& exit)
{
	fstream saveFile;
	string optionS;
	int landedX = theplayer.getPositionX();
	int landedY = theplayer.getPositionY();
	theplayer.setTurns(1);
	displeyUI();
	showBoard(landedX, landedY);
	cout << "Choose an option: " << endl
		<< "1- Travel to another space on the board." << endl
		<< "2- Dismount and explore current level." << endl
		<< "3- Save your game and Travel to another space on the board. " << endl;
	cin >> optionS;
	while (optionS != "1" && optionS != "2" && optionS != "3")
	{
		cout << "Wrong input. Choose again: ";
		cin >> optionS;
	}	
	cout << endl;

	if (optionS == "1")
	{
		
	}

	else if (optionS == "2")
	{
		if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'W')
		{
			cout << "You have found a Well of Reflection. Your EXP increases by 2 and yout alligment is change." << endl;
			theplayer.setEXP(2);
			theplayer.changeAlligment();
			displeyUI();
			showBoard(landedX, landedY);
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'C')
		{
			if (theplayer.getAlligment() == 0)
			{
				int i;
				cout << "You have found a Cathedral. Because you Alligment is Good you gain 20% extra life." << endl;
				if (theplayer.getHP() < 84)
				{
					i = theplayer.getHP() * 0.2;
					theplayer.setHealth(theplayer.getHP() + i);
				}
				else if (theplayer.getHP() > 84)
				{
					theplayer.setHealth(100);
				}
			}
			else if (theplayer.getAlligment() == 1)
			{
				int i;
				cout << "You have found a Cathedral. Because you Alligment is Bad you lose 20% of life." << endl;
				i = theplayer.getHP() * 0.2;
				theplayer.setHealth(theplayer.getHP() - i);
			}
			displeyUI();
			showBoard(landedX, landedY);
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'G')
		{
			if (theplayer.getAlligment() == 1)
			{
				int i;
				cout << "You have found a Graveyard. Because you Alligment is Bad you gain 20% extra life." << endl;
				if (theplayer.getHP() < 84)
				{
					i = theplayer.getHP() * 0.2;
					theplayer.setHealth(theplayer.getHP() + i);
				}
				else if (theplayer.getHP() > 84)
				{
					theplayer.setHealth(100);
				}
			}
			else if (theplayer.getAlligment() == 0)
			{
				int i;
				cout << "You have found a Gravehard. Because you Alligment is Good you lose 20% of life." << endl;
				i = theplayer.getHP() * 0.2;
				theplayer.setHealth(theplayer.getHP() - i);
			}
			displeyUI();
			showBoard(landedX, landedY);
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'E')
		{
			cout << "You have found the Dungeon Exit. To exit you need the Magic Relic and a EXP higher then 20." << endl;
			bool found = false;
			bool exp = false;
			for (int abc = 0; abc < theplayer.getItem(); abc++)
			{
				theplayer.getItems(abc);
				if (theplayer.getItems(abc) == "MagicRelic")
				{
					found = true;
				}
			}
			if (theplayer.getEXP() >= 20)
				exp = true;

			if (found == true && exp == true)
			{
				cout << "You posses the Magic Relic and the EXP need to Exit." << endl
					<< "----Congratulations on Wining the Game!!!----";
				exit = -1;
			}
			else if (found == false && exp == false)
			{
				cout << "You do not posses the Magic Relic or the needed EXP." << endl
					<< "----You have to keep playing to be able to win.----" << endl;
				displeyUI();
				showBoard(landedX, landedY);
			}
			else if (found == true && exp == false)
			{
				cout << "You posses the Magic Relic but not the needed EXP. " << endl
					<< "----You have to keep playing to be able to win.----" << endl;
				displeyUI();
				showBoard(landedX, landedY);
			}
			else if (found == false && exp == true)
			{
				cout << "You do not posses the Magic Relic but you have the needed EXP. " << endl
					<< "----You have to keep playing to be able to win.----" << endl;
				displeyUI();
				showBoard(landedX, landedY);
			}

		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'S')
		{
		    cout << "This level is empty. Continue. " << endl;
			drawBoard();
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'R')
		{
			bool found = false;
			cout << "You have found a Magic Relic." << endl;
			for (int abc = 0; abc < theplayer.getItem(); abc++)
			{
				if (theplayer.getItems(abc) == "MagicRelic")
				{
					found = true;
				}
			}

			if (found == true)
			{
				cout << "You already posses a Magic Relic." << endl;
			}
			else if (found == false)
			{
				theplayer.addItem("MagicRelic");
				cout << "The Magic Relic has been added to your Inventory." << endl;
			}
			displeyUI();
			showBoard(landedX, landedY);
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'p')
		{
			int a = randomNumber(1, 2);
			if (a == 1)
			{
				cout << "You have found a Potion lv1. It have been added to your inventory." << endl;
				theplayer.addItem("PotionLv1");
				displeyUI();
				showBoard(landedX, landedY);
			}
			else if (a == 2)
			{
				cout << "You have found a Potion lv2. It have been added to your inventory." << endl;
				theplayer.addItem("PotionLv2");
				displeyUI();
				showBoard(landedX, landedY);
			}
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'c')
		{
		    cout << "You have found the Crossbow." << endl;
			try
			{
				theplayer.findWeapon("Crossbow");

				theplayer.addWeapon("Crossbow", 3);
				cout << "The Crossbow(+3) have been added to your Inventory." << endl;
			}
			catch (Player::WeaponLimit)
			{
				cout << "The Crossbow(+3) was already in your inventory." << endl;
			}
			displeyUI();
			showBoard(landedX, landedY);
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'f')
		{
		    cout << "You have found the Flail." << endl;
			try
			{
				theplayer.findWeapon("Flail");

				theplayer.addWeapon("Flail", 5);
				cout << "The Flail(+5) have been added to your Inventory." << endl;
			}
			catch (Player::WeaponLimit)
			{
				cout << "The Flail(+5) was already in your inventory." << endl;
			}
			displeyUI();
			showBoard(landedX, landedY);
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 's')
		{
		    cout << "You have found the BroadSword." << endl;
		    try
			{
				theplayer.findWeapon("BroadSword");

				theplayer.addWeapon("BroadSword", 8);
				cout << "The BroadSword(+8) have been added to your Inventory." << endl;
			}
			catch (Player::WeaponLimit)
			{
				cout << "The BroadSword(+8) was already in your inventory." << endl;
			}
			displeyUI();
			showBoard(landedX, landedY);
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'M')
		{
		    cout << "You have found the MorningStar." << endl;
		    try
			{
				theplayer.findWeapon("MorningStar");

				theplayer.addWeapon("MorningStar", 13);
				cout << "The MorningStar(+8) have been added to your Inventory." << endl;
			}
			catch (Player::WeaponLimit)
			{
				cout << "The MorningStar(+13) was already in your inventory." << endl;
			}
			displeyUI();
			showBoard(landedX, landedY);
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'h')
		{
		    cout << "You have found the HolyStaff." << endl;
		    try
			{
				theplayer.findWeapon("HolyStaff");

				theplayer.addWeapon("HolyStaff", 21);
				cout << "The HolyStaff(+21) have been added to your Inventory." << endl;
			}
			catch (Player::WeaponLimit)
			{
				cout << "The HolyStaff(+21) was already in your inventory." << endl;
			}

			//Old Code 
			/*bool found = false;
			cout << "You have found the Holy Staff." << endl;
			for (int abc = 0; abc < theplayer.getVector().size(); abc++)
			{
				if (theplayer.getVector()[abc].weaponName == "HolyStaff")
				{
					found = true;
				}
			}

			if (found == false)
			{
				theplayer.addWeapon("HolyStaff", 21);
				cout << "The weapon Holy Staff(+21) have been added to your Inventory." << endl;
			}
			else if (found == true)
			{
				cout << "The Holy Staff(+21) is already in your inventory." << endl;
			}*/                             
			
			displeyUI();
			showBoard(landedX, landedY);
		}

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'L')
		{
		    int remain = theplayer.getHP();
		    cout << "You have found a Lake of Despair. You must roll a 1 or 3 to escape. " << endl;
			cout << "Every roll will take 5 point of life. " << endl;
			leaveDespair();
			remain -= theplayer.getHP();
			if (theplayer.getHP() < 0)
			{
				cout << "\n-----You have lost all of yout life points. You LOSE!!----------" << endl;
				exit = -1;
			}
			else
			{
				cout << "You have ecape! You lost " << remain << " ponts of life." << endl;
				displeyUI();
				showBoard(landedX, landedY);
			}			
        }
		
		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'B')
		{
		    cout << "You have found the Blacksmith. " << endl;
		    cout << "You can leave weapons with him and you can pick them up later with the modifier increase by the amount of turns thwy were stored." << endl;
			
			if (theplayer.getVector().size() == 1 && blacksmithWeapons.size() == 0)
			{
				cout << "You do not have eanogh weapons to leave any. " << endl;
			}

			else if (blacksmithWeapons.size() == 0)
			{
				string choice;
				int choiceW = 0;
				int howMany = 0;

				cout << "The Blacksmith storage is empty, you can only leave weapons. " << endl;
				cout << "Do you wish to leave some weapons with the Blacksmith? " << endl
					<< "1- Yes" << endl
					<< "2- No " << endl;
				cin >> choice;
				while (choice != "1" && choice != "2")
				{
					cout << "Wrong input. Choose again: ";
					cin >> choice;
				}

				if (choice == "1")
				{
					cout << "List of weapons in the Inventory: " << endl;
					for (int cbt = 0; cbt < theplayer.getVector().size(); cbt++)
					{
						cout << (cbt + 1) << "- " << theplayer.getVector()[cbt].weaponName
							<< "(+" << theplayer.getVector()[cbt].damage << ")" << endl;
					}
					cout << "How many weapons do you wish to leave? (Up to 3. You msut stay with at least one)" << endl;
					cin >> howMany;
					while (howMany < 1 || howMany > 3 || howMany > (theplayer.getVector().size() - 1))
					{
						cout << "Wrong Input. Try again: ";
						cin >> howMany;
					}

					int z = 0;
					for (int abc = 0; abc < howMany; abc++)
					{
						cout << "\nList of weapons: " << endl;
						for (int cbt = 0; cbt < theplayer.getVector().size(); cbt++)
						{
							cout << (cbt + 1) << "- " << theplayer.getVector()[cbt].weaponName
								<< "(+" << theplayer.getVector()[cbt].damage << ")" << endl;
						}
						cout << "Select a Weapon to leave: ";
						cin >> z;
						while (z < 0 || z > theplayer.getVector().size())
						{
							cout << "Wrong Input. Try again: ";
							cin >> z;
						}
						Blacksmith weapon;
						weapon.weaponName = theplayer.getVector()[z - 1].weaponName;
						weapon.damage = theplayer.getVector()[z - 1].damage;
						weapon.turn = theplayer.getTurn();
						blacksmithWeapons.push(weapon);
						theplayer.deleteWeapon(z - 1);
					}

					cout << "The are in storage. They will be ready whwn you come back." << endl;

					cout << "\nList of weapons in the Inventory: " << endl;
					for (int cbt = 0; cbt < theplayer.getVector().size(); cbt++)
					{
						cout << (cbt + 1) << "- " << theplayer.getVector()[cbt].weaponName
							<< "(+" << theplayer.getVector()[cbt].damage << ")" << endl;
					}
				}

				else
				{ }
			}
			
			else if ((blacksmithWeapons.size() == 3))
			{
				string choice;
				int howMany = 0;
				int choiceW = 0;
				
				cout << "The Blacksmith storage is full. You can only take weapons." << endl
					<< "Would you like to? " << endl
					<< "1- Yes" << endl
					<< "2- No" << endl;
				cin >> choice;
				while (choice != "1" && choice != "2")
				{
					cout << "Wrong input. Choose again: ";
					cin >> choice;
				}

				if (choice == "1")
				{
					cout << "There are " << blacksmithWeapons.size() << " in Storage." << endl;
					cout << "How many Weapons would you like to take? ";
					cin >> howMany;
					while (howMany < 0 || howMany > blacksmithWeapons.size())
					{
						cout << "Wrong Input. Try again: ";
						cin >> howMany;
					}

					for (int abc = 0; abc < howMany; abc++)
					{
						int incMod = 0;
						
						incMod = theplayer.getTurn() - blacksmithWeapons.top().turn;
						incMod = incMod + blacksmithWeapons.top().damage;

						try
						{
							theplayer.findWeapon(blacksmithWeapons.top().weaponName);

							theplayer.addWeapon(blacksmithWeapons.top().weaponName, incMod);
							blacksmithWeapons.pop();
						}
						catch (Player::WeaponLimit)
						{
							for (int zxc = 0; zxc < theplayer.getVector().size(); zxc++)
							{
								if (theplayer.getVector()[zxc].weaponName == blacksmithWeapons.top().weaponName)
								{
									theplayer.deleteWeapon(zxc);
									theplayer.addWeapon(blacksmithWeapons.top().weaponName, incMod);
									blacksmithWeapons.pop();
								}
							}
						}
					}
				}

				cout << "\nList of weapons in the Inventory: " << endl;
				for (int cbt = 0; cbt < theplayer.getVector().size(); cbt++)
				{
					cout << (cbt + 1) << "- " << theplayer.getVector()[cbt].weaponName
						<< "(+" << theplayer.getVector()[cbt].damage << ")" << endl;
				}

			}
			
			else
			{
			    string choice;

				cout << "List of weapons in the Inventory: " << endl;
				for (int cbt = 0; cbt < theplayer.getVector().size(); cbt++)
				{
					cout << (cbt + 1) << "- " << theplayer.getVector()[cbt].weaponName
						<< "(+" << theplayer.getVector()[cbt].damage << ")" << endl;
				}

			    cout << "\nDo you wish to Drop off weapons or Pick them up? " << endl
					<< "1- Drop off" << endl
					<< "2- Pick up" << endl;
				cin >> choice;
				while (choice != "1" && choice != "2")
				{
					cout << "Wrong Input. Try again: ";
					cin >> choice;
				}

				if (choice == "1")
				{
					int howMany = 0;
					int limit = 3 - blacksmithWeapons.size();

					cout << "How many weapons do you wish to leave? (Up to " << limit << ". You msut stay with at least one)" << endl;
					cin >> howMany;

					while (howMany < 1 || howMany > limit || howMany > (theplayer.getVector().size() - 1))
					{
						cout << "Wrong Input. Try again: ";
						cin >> howMany;
					}

					int z = 0;
					for (int abc = 0; abc < howMany; abc++)
					{
						cout << "\nList of weapons in the Inventory: " << endl;
						for (int cbt = 0; cbt < theplayer.getVector().size(); cbt++)
						{
							cout << (cbt + 1) << "- " << theplayer.getVector()[cbt].weaponName
								<< "(+" << theplayer.getVector()[cbt].damage << ")" << endl;
						}
						cout << "Select a Weapon to leave: ";
						cin >> z;
						while (z < 0 || z > theplayer.getVector().size())
						{
							cout << "Wrong Input. Try again: ";
							cin >> z;
						}
						Blacksmith weapon;
						weapon.weaponName = theplayer.getVector()[z - 1].weaponName;
						weapon.damage = theplayer.getVector()[z - 1].damage;
						weapon.turn = theplayer.getTurn();
						blacksmithWeapons.push(weapon);
						theplayer.deleteWeapon(z - 1);
					}

					cout << "The weapons are in storage. They will be ready whwn you come back." << endl;
				}
				
				else if (choice == "2")
				{
					int howMany = 0;

					cout << "There are " << blacksmithWeapons.size() << " in Storage." << endl;
					cout << "How many Weapons would you like to take? ";
					cin >> howMany;
					while (howMany < 0 || howMany > blacksmithWeapons.size())
					{
						cout << "Wrong Input. Try again: ";
						cin >> howMany;
					}

					for (int abc = 0; abc < howMany; abc++)
					{
						int incMod = 0;

						incMod = theplayer.getTurn() - blacksmithWeapons.top().turn;
						incMod = incMod + blacksmithWeapons.top().damage;

						try
						{
							theplayer.findWeapon(blacksmithWeapons.top().weaponName);

							theplayer.addWeapon(blacksmithWeapons.top().weaponName, incMod);
							blacksmithWeapons.pop();
						}
						catch (Player::WeaponLimit)
						{
							for (int zxc = 0; zxc < theplayer.getVector().size(); zxc++)
							{
								if (theplayer.getVector()[zxc].weaponName == blacksmithWeapons.top().weaponName)
								{
									theplayer.deleteWeapon(zxc);
									theplayer.addWeapon(blacksmithWeapons.top().weaponName, incMod);
									blacksmithWeapons.pop();
								}
							}
						}
					}
				}
				
				cout << "\nList of weapons in the Inventory: " << endl;
				for (int cbt = 0; cbt < theplayer.getVector().size(); cbt++)
				{
					cout << (cbt + 1) << "- " << theplayer.getVector()[cbt].weaponName
						<< "(+" << theplayer.getVector()[cbt].damage << ")" << endl;
				}
			}

			displeyUI();
			showBoard(landedX, landedY);
        }

		else if (gameBoard[theplayer.getPositionX()][theplayer.getPositionY()] == 'm')
		{
		    int optionA = 0;
			Reaper monster(theplayer.getEXP());
			string monsterClass = "";
			int rOm = randomNumber(1, 3);
			if (rOm == 1 || rOm == 3)
				monsterClass = "Monster";
			else
				monsterClass = "Reaging Reaper";
			
			cout << "--A " << monsterClass << " has just appear.You have to defeted!!--" << endl;
			cout << "The " << monsterClass << " HP is " << monster.getHP() << "  ";
			int inmune = randomNumber(1, 5);
			string inmuneS;
			if (inmune == 1)
			{
				cout << "The " << monsterClass << " is inmune to the --Crossbow--.The attack added from this weapon will be 0." << endl;
				inmuneS = "Crossbow";
			}
			else if (inmune == 2)
			{
				cout << "The " << monsterClass << " is inmune to the --Flail--.The attack added from this weapon will be 0." << endl;
				inmuneS = "Flail";
			}
			else if (inmune == 3)
			{
				cout << "The " << monsterClass << " is inmune to --the Broad Sword--.The attack added from this weapon will be 0." << endl;
				inmuneS = "BroadSword";
			}
			else if (inmune == 4)
			{
				cout << "The " << monsterClass << " is inmune to the --Morning Star--.The attack added from this weapon will be 0." << endl;
				inmuneS = "MorningStar";
			}
			else if (inmune == 5)
			{
				cout << "The " << monsterClass << " is inmune to the --Holy Staff--.The attack added from this weapon will be 0." << endl;
				inmuneS = "HolyStaff";
			}
			sleep_until(system_clock::now() + 1s); //Wait for Roll Dice
			cout << "Do you wish to: " << endl
				<< "1- Attack now." << endl
				<< "2- Use an Item first." << endl;
			cin >> optionA;
			while (optionA != 1 && optionA != 2)
			{
				cout << "Wrong input. Choose again: ";
				cin >> optionA;
			}

			if (optionA == 2)
			{
				int choice = 0;
				cout << "List of Items: " << endl;
				if (theplayer.getItem() == 0)
				{
					cout << "Your Inventory is Empty. " << endl;
				}
				else if (theplayer.getItem() == 1 && theplayer.getItems(0) == "MagicRelic")
				{
					cout << "--The only Item in your Inventory is the Magic Relic and it can only be use in the Exit.--" << endl;
				}
				else if (theplayer.getItem() != 0)
				{
					for (int a = 0; a < theplayer.getItem(); a++)
					{
						cout << (a + 1) << "- " << theplayer.getItems(a) << endl;
					}
					cout << "Choose an item: ";
					cin >> choice;
					while (choice < 1 || choice > theplayer.getItem() )
					{
						cout << "Wrong input. Choose again: ";
						cin >> choice;
					}
					while (theplayer.getItems(choice - 1) == "MagicRelic")
					{
						cout << "The Magic Relic can only be use in the Exit. Choose another item: ";
						cin >> choice;
					}
					theplayer.useItem(choice - 1);
				}				
			}

			int choiceW = 0;
			cout << "List of weapons: " << endl;
			for (int cbt = 0; cbt < theplayer.getVector().size(); cbt++)
			{
				cout << (cbt + 1) << "- " << theplayer.getVector()[cbt].weaponName 
					<< "(+" << theplayer.getVector()[cbt].damage << ")" << endl;
			}
			cout << "Choose an Weapon: ";
			cin >> choiceW;
			while (choiceW < 1 || choiceW > theplayer.getVector().size())
			{
				cout << "Wrong input. Choose again: ";
				cin >> choiceW;
			}
			theplayer.setEquipedWeapon(theplayer.getVector()[choiceW - 1].weaponName, theplayer.getVector()[choiceW - 1].damage);

			while (monster.getHP() > 0) 
			{
				string choice;
				int attack = 0;
				int var = theplayer.getWeaponDamage();
				if (theplayer.getWeaponName() == inmuneS)
					var = 0;

				cout << endl << "Yarra HP: " << theplayer.getHP() << " Equipped Weapon: " << theplayer.getWeaponName() << "(+" << var << ") "
					<< "|| "<< monsterClass << " HP: " << monster.getHP() << endl;

				if (rOm == 1 || rOm == 3)
					attack = monster.attackMonster(theplayer.getEXP());
				else
					attack = monster.attack(Yarra);
				

				cout << monsterClass << "'s turn to attack. ";
				sleep_until(system_clock::now() + 1s); //Wait for Roll Dice
				cout << "Attack Damage: " << attack;
				theplayer.setHealth(theplayer.getHP() - attack);
				if (theplayer.getHP() < 0)
				{
					cout << "\n-----The " << monsterClass << " has defeted you.You LOSE!!--------" << endl;
					exit = -1;
					break;
				}

				int attackY = 0;
				cout << endl << "Yarra's turn to attack. Press Enter to Roll dice: ";
				cin.ignore();
				getline(cin, choice);
				sleep_until(system_clock::now() + 1s); //Wait for Roll Dice
				if (theplayer.getWeaponName() == inmuneS)
					attackY = randomNumber(1, 6);
				else
					attackY = (randomNumber(1, 6) + theplayer.getWeaponDamage());
				cout << "Attack Damage: " << attackY << endl;
				monster.setHP(attackY);
				if (monster.getHP() <= 0)
				{
					cout << "You have defeated the " << monsterClass << ". Your EXP increase by 3" << endl;
					theplayer.setEXP(3);
					displeyUI();
					showBoard(landedX, landedY);
				}

			}
			
		}
	}

	else if (optionS == "3")
	{
		fstream saveFile;
		readSaveGame gameSave;
		saveFile.open("GameSave.txt", ios::in | ios::out);

		populateStruct(gameSave, Yarra);
		saveFile.write(reinterpret_cast<char*>(&gameSave), sizeof(gameSave));
		saveFile.close();
		cout << "--Progress has been save -- " << endl;

		displeyUI();
		showBoard(landedX, landedY);
	}
}

void changePosition(int& positionX, int& positionY, int spacesToMove)
{
	while (spacesToMove > 0)
	{
		//position is in the first row and moving right
		if (spacesToMove > 0 && positionX == 0 && positionY != 7) 
		{
			if (positionY + spacesToMove >= COLUMNS) 
			{
				spacesToMove = spacesToMove - (COLUMNS - (positionY + 1));
				positionY = 7;
			}
			else 
			{
				positionY = positionY + spacesToMove;
				spacesToMove = 0;
			}
		}
		//position is last column and moving down 
		if (spacesToMove > 0 && positionY == 7 && positionX != 7) 
		{
			if (positionX + spacesToMove >= ROWS) 
			{
				spacesToMove = spacesToMove - (ROWS - (positionX + 1));
				positionX = 7;
			}
			else 
			{
				positionX = positionX + spacesToMove;
				spacesToMove = 0;
			}
		}

		//position is on last row and moving left
		if (spacesToMove > 0 && positionX == 7 && positionY != 0) 
		{
			if ((COLUMNS - (positionY + 1)) + spacesToMove >= COLUMNS) 
			{
				spacesToMove = spacesToMove - (positionY);
				positionY = 0;
			}
			else 
			{
				positionY = positionY - spacesToMove;
				spacesToMove = 0;
			}
		}

		//position is on first column and moving up
		if (spacesToMove > 0 && positionY == 0 && positionX != 0) {
			if ((ROWS - (positionX + 1)) + spacesToMove >= ROWS) {
				spacesToMove = spacesToMove - (positionX);
				positionX = 0;
			}
			else {
				positionX = positionX - spacesToMove;
				spacesToMove = 0;
			}
		}
	}
}

void showBoard(int positionX, int positionY)
{
	char board[ROWS][COLUMNS];

	for (int x = 0; x < ROWS; x++)
	{
		for (int y = 0; y < COLUMNS; y++)
		{
			if (y == 0 || y == 7 || x == 0 || x == 7)
			{
				if (positionX == x && positionY == y)
				{
					board[x][y] = 'P';
				}
				else if (0 == x && 4 == y)
				{
					board[x][y] = 'W';
				}
				else if (0 == x && 7 == y)
				{
					board[x][y] = 'C';
				}
				else if (3 == x && 7 == y)
				{
					board[x][y] = 'G';
				}
				else if (7 == x && 7 == y)
				{
					board[x][y] = 'C';
				}
				else if (7 == x && 4 == y)
				{
					board[x][y] = 'W';
				}
				else if (7 == x && 0 == y)
				{
					board[x][y] = 'G';
				}
				else if (3 == x && 0 == y)
				{
					board[x][y] = 'E';
				}
				else if (0 == x && 0 == y)
				{
					board[x][y] = 'S';
				}
				else if (0 == x && 1 == y)
				{
					board[x][y] = 'L';
				}
				else if (1 == x && 7 == y)
				{
					board[x][y] = 'L';
				}
				else if (7 == x && 6 == y)
				{
					board[x][y] = 'L';
				}
				else if (6 == x && 0 == y)
				{
					board[x][y] = 'L';
				}
				else if (0 == x && 6 == y)
				{
					board[x][y] = 'B';
				}
				else if (6 == x && 7 == y)
				{
					board[x][y] = 'B';
				}
				else if (7 == x && 1 == y)
				{
					board[x][y] = 'B';
				}
				else if (1 == x && 0 == y)
				{
					board[x][y] = 'B';
				}
				else
				{
					board[x][y] = '*';
				}
				cout << setw(2) << board[x][y];
			}
			else
			{
				cout << setw(2) << " ";
			}
		}
		cout << endl;
	}
}

void drawBoard()
{	
    for (int x = 0; x < ROWS; x++)
    {
        for (int y = 0; y < COLUMNS; y++)
        {
            if (y == 0 || y == 7 || x == 0 || x == 7)
            {
				//Well of Reflection
				if (0 == x && 4 == y)
				{
					gameBoard[x][y] = 'W';
				}
				//Cathedral
				else if (0 == x && 7 == y) 
				{
					gameBoard[x][y] = 'C';
				}
				//Graveyard
				else if (3 == x && 7 == y)
				{
					gameBoard[x][y] = 'G';
				}
				//Cathedral
				else if (7 == x && 7 == y)
				{
					gameBoard[x][y] = 'C';
				}
				//Well of Reflection
				else if (7 == x && 4 == y)
				{
					gameBoard[x][y] = 'W';
				}
				//Graveyard
				else if (7 == x && 0 == y)
				{
					gameBoard[x][y] = 'G';
				}
				//Dungeon Exit
				else if (3 == x && 0 == y)
				{
					gameBoard[x][y] = 'E';
				}
				//Player Start
				else if (0 == x && 0 == y)
				{
					gameBoard[x][y] = 'S';
				}
				//Lake of Despair
				else if (0 == x && 1 == y)
				{
					gameBoard[x][y] = 'L';
				}
				else if (1 == x && 7 == y)
				{
					gameBoard[x][y] = 'L';
				}
				else if (7 == x && 6 == y)
				{
					gameBoard[x][y] = 'L';
				}
				else if (6 == x && 0 == y)
				{
					gameBoard[x][y] = 'L';
				}
				//Blacksimth
				else if (0 == x && 6 == y)
				{
					gameBoard[x][y] = 'B';
				}
				else if (6 == x && 7 == y)
				{
					gameBoard[x][y] = 'B';
				}
				else if (7 == x && 1 == y)
				{
					gameBoard[x][y] = 'B';
				}
				else if (1 == x && 0 == y)
				{
					gameBoard[x][y] = 'B';
				}

				else
				{
					gameBoard[x][y] = '*';
				}

				/* --Legend--
				R - Magic Relic
				m - monster
				p - item
				c - Crosbow
				f - Flail
				s - Sword
				M - Morning Star
				h - Holy staff*/
				int a = randomNumber(1, 3);
				if (a == 1)
				{
					//gameBoard[0][1] = 'm';
					gameBoard[0][2] = 'p';
					gameBoard[0][3] = 'm';
					gameBoard[0][5] = 'c';
					//gameBoard[0][6] = 'm';
				}
				else if (a == 2)
				{
					//gameBoard[0][1] = 'p';
					gameBoard[0][2] = 'c';
					gameBoard[0][3] = 'p';
					gameBoard[0][5] = 'm';
					//gameBoard[0][6] = 'm';
				}
				else if (a == 3)
				{
					//gameBoard[0][1] = 'm';
					gameBoard[0][2] = 'm';
					gameBoard[0][3] = 'c';
					gameBoard[0][5] = 'p';
					//gameBoard[0][6] = 'p';
				}
				
				int b = randomNumber(1, 3);
				if (b == 1)
				{
					//gameBoard[1][7] = 'm';
					gameBoard[2][7] = 'm';
					gameBoard[4][7] = 'f';
					gameBoard[5][7] = 'p';
					//gameBoard[6][7] = 'm';
				}
				else if (b == 2)
				{
					//gameBoard[1][7] = 'm';
					gameBoard[2][7] = 'f';
					gameBoard[4][7] = 'm';
					gameBoard[5][7] = 'p';
					//gameBoard[6][7] = 'm';
				}
				else if (b == 3)
				{
					//gameBoard[1][7] = 'm';
					gameBoard[2][7] = 'm';
					gameBoard[4][7] = 'p';
					gameBoard[5][7] = 'f';
					//gameBoard[6][7] = 'm';
				}

				int c = randomNumber(1, 4);
				if (c == 1)
				{
					//gameBoard[7][1] = 'm';
					gameBoard[7][2] = 'p';
					gameBoard[7][3] = 'R';
					gameBoard[7][5] = 's';
					//gameBoard[7][6] = 's';
				}
				else if (c == 2)
				{
					//gameBoard[7][1] = 'p';
					gameBoard[7][2] = 'm';
					gameBoard[7][3] = 's';
					gameBoard[7][5] = 'R';
					//gameBoard[7][6] = 'm';
				}
				else if (c == 3)
				{
					//gameBoard[7][1] = 'p';
					gameBoard[7][2] = 's';
					gameBoard[7][3] = 'm';
					gameBoard[7][5] = 'R';
					//gameBoard[7][6] = 'm';
				}

				int d = randomNumber(1, 3);
				if (d == 1)
				{
					//gameBoard[1][0] = 'm';
					gameBoard[2][0] = 'h';
					gameBoard[4][0] = 'R';
					gameBoard[5][0] = 'M';
					//gameBoard[6][0] = 'p';
				}
				if (d == 2)
				{
					//gameBoard[1][0] = 'm';
					gameBoard[2][0] = 'h';
					gameBoard[4][0] = 'M';
					gameBoard[5][0] = 'R';
					//gameBoard[6][0] = 'p';
				}
				if (d == 3)
				{
					//gameBoard[1][0] = 'm';
					gameBoard[2][0] = 'R';
					gameBoard[4][0] = 'M';
					gameBoard[5][0] = 'h';
					//gameBoard[6][0] = 'p';
				}
            }
			else
			{
				
			}				
        } 				
    }

	//Show the actual Map
	/*for (int x = 0; x < ROWS; x++)
	{
		for (int y = 0; y < COLUMNS; y++)
		{
			if (y == 0 || y == 7 || x == 0 || x == 7)
			{
				
				cout << setw(2) << gameBoard[x][y];
			}
			else
			{
				cout << setw(2) << " ";
			}
		}
		cout << endl;
	}*/
} 

void populateStruct(readSaveGame& mySaveGame, Player& p)
{
	saveWeapons weapons;
	mySaveGame.turns = p.getTurn();
	mySaveGame.hp = p.getHP();
	mySaveGame.exp = p.getEXP();
	mySaveGame.positionX = p.getPositionX();
	mySaveGame.positionY = p.getPositionY();
	mySaveGame.alligment = p.getAlligment();

	//Write the Weapons and Items to the file Struct(Could not mahe it work)
	/*for (int a = 0; a <= (p.getVector().size() - 1); a++)
	{
		weapons.weaponName = p.getVector()[a].weaponName;
		weapons.damage = p.getVector()[a].damage;
		mySaveGame.weaponVect.push_back(weapons);
	}

	mySaveGame.itemsNum = numItem;	
	for (int a = 0; a < numItem; a++)
		mySaveGame.saveItems[a] = p.getItems(a);*/

	mySaveGame.weaponName = p.getWeaponName();
	mySaveGame.damage = p.getWeaponDamage();

	//Write Weapons to a different file(I could make it work with the struct)
	fstream weaponSave;
	weaponSave.open("WeaponSave.txt", ios::out | ios::trunc);
	for (int a = 0; a <= (p.getVector().size() - 1); a++)
	{
		weaponSave << "\n" << p.getVector()[a].weaponName << "    " << p.getVector()[a].damage;
	}
	weaponSave.close();

	//Write Items to a different file(I could make it work with the struct)
	fstream itemSave;
	itemSave.open("ItemSave.txt", ios::out | ios::trunc);
	for (int a = 0; a <= (p.getItem() - 1); a++)
	{
		itemSave << "\n" << p.getItems(a);
	}
	itemSave.close();
}

void outputGameSate(const readSaveGame& s, Player& player)
{
	player.setTurns(s.turns);
	player.setHealth(s.hp);
	player.setEXP(s.exp);
	player.setPosition(s.positionX, s.positionY);
	player.setAlligmnet(s.alligment);
	
	//Read the Weapons and Items from the file Struct(Could not mahe it work)
	/*for (int cnt = 0; cnt <= (s.weaponVect.size() - 1); cnt++)
	{
		player.addWeapon(s.weaponVect[cnt].weaponName, s.weaponVect[cnt].damage);
	}*/

	/*for (int cnt = 0; cnt < s.itemsNum; cnt++)
	{
		player.addItem(s.saveItems[cnt]);
	}	*/

	player.setEquipedWeapon(s.weaponName, s.damage);

	//Read Weapons from a different file(I could make it work with the struct)
	fstream weaponSave;
	string name;
	int d;
	weaponSave.open("WeaponSave.txt", ios::in | ios::out);
	while (!weaponSave.eof())
	{
		weaponSave >> name >> d;
	    player.addWeapon(name, d);
	}
	weaponSave.close();		

	//Read Items from a different file(I could make it work with the struct)
	fstream itemSave;
	string i;
	itemSave.open("ItemSave.txt", ios::in | ios::out);
	while (!itemSave.eof())
	{
		itemSave >> i;
		player.addItem(i);
	}
	itemSave.close();
}

void leaveDespair()
{
	string a;
	int roll;
	
	cout << "Type Enter to Roll the Dice: ";
	roll = randomNumber(1, 6);
	cin.ignore();
	getline(cin, a);
	cout << roll << endl;
	if (roll == 1 || roll == 3)
	{
		
	}
	else
	{
		Yarra.setHealth(Yarra.getHP() - 5);
		cout << "Yarra has lost 5 points of Life. " << endl;
		leaveDespair();
	}
}

int randomNumber(int low, int high)
{
	srand((unsigned)time(0));
	return (rand() % (high - low + 1)) + low;
}