#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include "EnemyShip.h"
#include <cmath>
int nScreenWidth = 120;
int nScreenHeight = 40;

wchar_t *screen = new wchar_t[nScreenHeight*nScreenWidth];

std::vector<EnemyShip> EnemyShips;
std::vector<COORD> PlayerProjectiles;
std::vector<COORD> EnemyProjectiles;
COORD PlayerPos;

char direction = 'a';

//Game options && Values
int ammo = 75;
int score = 1;
int dificulty = 10;
bool hasShield = false;

void DrawProjectiles() {
	for (int i = 0; i < PlayerProjectiles.size(); i++) {
		screen[PlayerProjectiles[i].Y*nScreenWidth + PlayerProjectiles[i].X] = '^';
	}
	for (int i = 0; i < EnemyProjectiles.size(); i++) {
		screen[EnemyProjectiles[i].Y*nScreenWidth + EnemyProjectiles[i].X] = '*';
	}
}

void AddBeginEnemies()
{
	
	for (int i = 0; i < rand() % 4+(int)(dificulty/10+2); i++) {

		EnemyShip ship(rand()%100000);
		EnemyShips.push_back(ship);
	}
}

void DrawEnemy() {
	for (int i = 0; i < EnemyShips.size(); i++) {
		screen[EnemyShips[i].coords.Y*nScreenWidth + EnemyShips[i].coords.X] = 0x2554;
		screen[EnemyShips[i].coords.Y*nScreenWidth + EnemyShips[i].coords.X+1] = 0x2550;
		screen[EnemyShips[i].coords.Y*nScreenWidth + EnemyShips[i].coords.X+2] = 0x2550;
		screen[EnemyShips[i].coords.Y*nScreenWidth + EnemyShips[i].coords.X + 3] = 0x2557;
	}
}

void DrawPlayer(COORD PlayerPos) {

	screen[PlayerPos.Y*nScreenWidth + PlayerPos.X] = 0x255A;
	screen[PlayerPos.Y*nScreenWidth + PlayerPos.X + 1] = 0x2550;
	screen[PlayerPos.Y*nScreenWidth + PlayerPos.X + 2] = 0x2550;
	screen[PlayerPos.Y*nScreenWidth + PlayerPos.X + 3] = 0x2569;
	screen[PlayerPos.Y*nScreenWidth + PlayerPos.X + 4] = 0x2550;
	screen[PlayerPos.Y*nScreenWidth + PlayerPos.X + 5] = 0x2550;
	screen[PlayerPos.Y*nScreenWidth + PlayerPos.X + 6] = 0x255D;
	screen[(PlayerPos.Y - 1)*nScreenWidth + PlayerPos.X + 3] = 0x2551;
	if(hasShield)
		screen[(PlayerPos.Y+1)*nScreenWidth + PlayerPos.X + 3] = 0x0398;
}

void AdvanceProjectiles() {

	//Player Projectiles
		for (int i = 0; i < PlayerProjectiles.size(); i++) {
			
			//Check if reached border
			if (PlayerProjectiles[i].Y == 1) {
				PlayerProjectiles.erase(PlayerProjectiles.begin() + i);
				i--;
				continue;
			}

			//Check if hit Enemy
			for (int x = 0; x < EnemyShips.size(); x++) {
				if ((PlayerProjectiles[i].Y == EnemyShips[x].coords.Y) && (PlayerProjectiles[i].X >= EnemyShips[x].coords.X&&PlayerProjectiles[i].X <= EnemyShips[x].coords.X + 3)) {
					EnemyShips.erase(EnemyShips.begin() + x);
					PlayerProjectiles.erase(PlayerProjectiles.begin() + i);
					if (rand() % 3 == 1||EnemyShips.size()<3) {
						AddBeginEnemies();
					}
					ammo += 7;
					score += dificulty;
					i--;
					Beep(2000, 2);
					break;
				}
			}
		}

		//Push forward remaining projectiles
		for (int i = 0; i < PlayerProjectiles.size(); i++) {
			PlayerProjectiles[i].Y--;
		}

	
	//EnemyProjectiles
		for (int i = 0; i < EnemyProjectiles.size(); i++) {
			if (EnemyProjectiles[i].Y == 38) {
				EnemyProjectiles.erase(EnemyProjectiles.begin() + i);
				i--;
				continue;
			}
			if ((EnemyProjectiles[i].Y == PlayerPos.Y && EnemyProjectiles[i].X >= PlayerPos.X && EnemyProjectiles[i].X <= PlayerPos.X + 4)&&(!hasShield)) {
				EnemyProjectiles.erase(EnemyProjectiles.begin() + i);
				ammo -= (int)(dificulty/10);
				i--;
				Beep(1000, 2);

			}
		}
		for (int i = 0; i < EnemyProjectiles.size(); i++) {
			EnemyProjectiles[i].Y++;
		}

		DrawProjectiles();
}

void AdvanceEnemy() {

	for (int i = 0; i < EnemyShips.size(); i++) {
		EnemyShips[i].AdvanceShip();
		if (rand()%(100-dificulty)==0) {
				EnemyProjectiles.push_back(EnemyShips[i].ShootProjectile());
		}
	}
}
int main()
{
	srand(time(NULL)*ammo*score);
	for (int i = 0; i < nScreenHeight*nScreenWidth - 1; i++) {
		screen[i] = ' ';
	}

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	PlayerPos.X = 53;
	PlayerPos.Y = 35;
	AddBeginEnemies();
	AddBeginEnemies();
	while (true) {
		//cheats
		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
			if (dificulty < 98) 
				dificulty += 2;
		}
		if (GetAsyncKeyState((unsigned short)'E') & 0x8000) {
			if(dificulty>2)
			dificulty -= 2;
		}
		if (GetAsyncKeyState((unsigned short)'X') & 0x8000) {
			ammo += 1000;
		}
		//PowerUps
		if (GetAsyncKeyState((unsigned short)'P') & 0x8000) {
			if (!hasShield) {
				ammo -= 10;
				hasShield = true;
			}
		}
		//Direction of player
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
			if (PlayerPos.Y > 20) {
				PlayerPos.Y--;
			}
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
			if (PlayerPos.Y < 35) {
				PlayerPos.Y++;
			}
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
			direction = 'd';
		}
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
			direction = 'a';
		}
		if (direction == 'a') {
			if (PlayerPos.X > 1) {
				PlayerPos.X--;
			}
			else
			{
				direction = 'd';
			}
		}
		else
		{
			if (PlayerPos.X < 113) {
				PlayerPos.X++;
			}
			else
			{
				direction = 'a';
			}
		}

		if (GetAsyncKeyState((unsigned short)' ') & 0x8000 && PlayerProjectiles.size()<3 && ammo>=0) {
			COORD projectile;
			projectile.X = PlayerPos.X + 3;
			projectile.Y = PlayerPos.Y - 2;
			PlayerProjectiles.push_back(projectile);
			Beep(300, 2);
			ammo--;
			hasShield = false;
		}
			//Advance Enemy
			AdvanceEnemy();
			//Player Draw
			DrawPlayer(PlayerPos);
			//Enemies Draw
			DrawEnemy();
			//Projectiles Draw
			AdvanceProjectiles();

		//Update Frame	
			if (ammo > 0) {
				swprintf_s(screen, 31 + (int)(log10((double)ammo) + 1) + (int)(log10((double)score) + 1)+(int)(log10((double)dificulty)), L"Ammo: %d, Score: %d, Dificulty: %d ", ammo, score,dificulty);
			}
			else
			{
				for (int i = 0; i < nScreenHeight*nScreenWidth - 1; i++) {
					screen[i] = ' ';
				}
				swprintf_s(screen, 11, L"Game over!");
				screen[nScreenHeight*nScreenWidth - 1] = '\0';
				WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight*nScreenWidth, { 0,0 }, &dwBytesWritten);
				Sleep(1000);
				return 0;
			}

		screen[nScreenHeight*nScreenWidth - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight*nScreenWidth, { 0,0 }, &dwBytesWritten);
		
		Sleep(10);
		for (int i = 0; i < nScreenHeight*nScreenWidth - 1; i++) {
			screen[i] = ' ';
		}
	}

    return 0;
}

