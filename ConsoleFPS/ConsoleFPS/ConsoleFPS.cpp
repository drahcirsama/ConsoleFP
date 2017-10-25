// ConsoleFPS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <chrono>
#include <iostream>
#include <Windows.h>
#include "vector"
#include <algorithm> 

using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerXPos = 8.0f;
float fPlayerYPos = 8.0f;
float fPlayerViewAngle = 0.0f;
float fPlayerLookSpeed = 1.5f;
float fPlayerMoveSpeed = 7.5f;

int nMapHeight = 16;
int nMapWidth = 16;


float rightAngleRadians = 1.570796f;

float fFOV = 3.13159 / 4;// Field of View

float GetDrawDistance() {
	if (nMapHeight > nMapWidth)
		return (float)nMapHeight;
	else
		return (float) nMapWidth;
}

wstring GenerateMap() {
	
	wstring map;
	map += L"################"; //L means line
	map += L"#..............#"; 
	map += L"#..............#"; 
	map += L"#..............#"; 
	map += L"#..........#...#"; 
	map += L"#..........#...#"; 
	map += L"#..............#"; 
	map += L"#..............#"; 
	map += L"#..............#"; 
	map += L"#..............#"; 
	map += L"#..............#"; 
	map += L"#..............#"; 
	map += L"#.......########"; 
	map += L"#..............#"; 
	map += L"#..............#"; 
	map += L"################"; 

	return map;
}

bool GetKey(short key) {
	return (GetAsyncKeyState((unsigned short)key) & 0x8000);
}

short get_map_avatar_direction() {

		if (fPlayerViewAngle >= -1 && fPlayerViewAngle < 1) {
			return 'v';
		}

		if (fPlayerViewAngle >= 1 && fPlayerViewAngle < 2) {
			return '>';
		}

		if (fPlayerViewAngle >= -2 && fPlayerViewAngle < -1) {
			return '<';
		}

		if ((fPlayerViewAngle >= 2 && fPlayerViewAngle < 3) || (fPlayerViewAngle >= -2 && fPlayerViewAngle < -3)) {
			return '^';
		}

		return '^';
}

class Player {

	private:
		float fXPos = 8.0f;
		float fYPos = 8.0f;
		float fViewAngle = 0.0f;
		float fLookSpeed = 1.5f;
		float fMoveSpeed = 7.5f;

	public:
		float getXPos() { return fXPos; };
		float getYPos() { return fYPos; };
		float getViewAngle() { return fViewAngle; };
		float getLookSpeed() { return fLookSpeed; }
		float getMoveSpeed() { return fMoveSpeed; }

		float setXPos(float value) { fXPos = value; };
		float setYPos(float value) { fYPos = value; };
		float setViewAngle(float value) { fViewAngle = value; };
		float setLookSpeed(float value) { fLookSpeed = value; }
		float setMoveSpeed(float value) { fMoveSpeed = value; }

		Player() {}
		Player(float xPos, float yPos) {
			setXPos(xPos);
			setYPos(yPos);
		}
};

int main()
{
	//Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map = GenerateMap();
	float fDrawDistance = GetDrawDistance();

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();
	bool refreshScreen = true;
	Player player1;

	//Game Loop
	while (true) {
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		// Controlsjjj
		// Handle CCW Rotation
		if (GetKey('A'))
		{
			refreshScreen = true;
			fPlayerViewAngle -= fPlayerLookSpeed * fElapsedTime;
			if (fPlayerViewAngle < -3) {
				fPlayerViewAngle = fPlayerViewAngle + 6;
			}
		}
		if (GetKey('D')) {
			refreshScreen = true;
			fPlayerViewAngle += fPlayerLookSpeed * fElapsedTime;
			if (fPlayerViewAngle > 3) {
				fPlayerViewAngle = fPlayerViewAngle - 6;
			}
		}
		if (GetKey('W')) {
			refreshScreen = true;
			fPlayerXPos += sinf(fPlayerViewAngle) * fPlayerMoveSpeed * fElapsedTime;
			fPlayerYPos += cosf(fPlayerViewAngle) * fPlayerMoveSpeed * fElapsedTime;

			if (map[(int)fPlayerYPos * nMapWidth + (int)fPlayerXPos] == '#') {
				fPlayerXPos -= sinf(fPlayerViewAngle) * fPlayerMoveSpeed * fElapsedTime;
				fPlayerYPos -= cosf(fPlayerViewAngle) * fPlayerMoveSpeed * fElapsedTime;
			}
		}
		if (GetKey('S')) {
			refreshScreen = true;
			fPlayerXPos -= sinf(fPlayerViewAngle) * fPlayerMoveSpeed * fElapsedTime;
			fPlayerYPos -= cosf(fPlayerViewAngle) * fPlayerMoveSpeed * fElapsedTime;

			if (map[(int)fPlayerYPos * nMapWidth + (int)fPlayerXPos] == '#') {
				fPlayerXPos += sinf(fPlayerViewAngle) * fPlayerMoveSpeed * fElapsedTime;
				fPlayerYPos += cosf(fPlayerViewAngle) * fPlayerMoveSpeed * fElapsedTime;
			}
		}
		if (GetKey('Q')) {
			refreshScreen = true;
			float directionRadians = fPlayerViewAngle - rightAngleRadians;
			fPlayerXPos += sinf(directionRadians) * fPlayerMoveSpeed * fElapsedTime;
			fPlayerYPos += cosf(directionRadians) * fPlayerMoveSpeed * fElapsedTime;

			if (map[(int)fPlayerYPos * nMapWidth + (int)fPlayerXPos] == '#') {
				fPlayerXPos -= sinf(directionRadians) * fPlayerMoveSpeed * fElapsedTime;
				fPlayerYPos -= cosf(directionRadians) * fPlayerMoveSpeed * fElapsedTime;
			}
		}
		if (GetKey('E')) {
			refreshScreen = true;
			float directionRadians = fPlayerViewAngle + rightAngleRadians;
			fPlayerXPos += sinf(directionRadians) * fPlayerMoveSpeed * fElapsedTime;
			fPlayerYPos += cosf(directionRadians) * fPlayerMoveSpeed * fElapsedTime;

			if (map[(int)fPlayerYPos * nMapWidth + (int)fPlayerXPos] == '#') {
				fPlayerXPos -= sinf(directionRadians) * fPlayerMoveSpeed * fElapsedTime;
				fPlayerYPos -= cosf(directionRadians) * fPlayerMoveSpeed * fElapsedTime;
			}
		}

		if (!refreshScreen) {
			continue;
		}

		for (int x = 0; x < nScreenWidth; x++) {
			// for each column, calculate the projected ray angle into world space
			float fRayAngle = (fPlayerViewAngle - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) *fFOV;
			float fDistanceToWall = 0;// Distance from Player to wall
			bool bHitWall = false;

			float fRayX = sinf(fRayAngle);
			float fRayY = cosf(fRayAngle);
			bool bLeftBoundary = false;
			bool bRightBoundary = false;

			while (!bHitWall && fDistanceToWall < fDrawDistance) {
				fDistanceToWall += 0.1f;

				int nTestRaySegmentX = (int)(fPlayerXPos + fRayX * fDistanceToWall);
				int nTestRaySegmentY = (int)(fPlayerYPos + fRayY * fDistanceToWall);

				//Test if ray is out of bounds
				if (nTestRaySegmentX < 0 || nTestRaySegmentX >= nMapWidth || nTestRaySegmentY < 0 || nTestRaySegmentY >= nMapHeight) {
					bHitWall = true;
					fDistanceToWall = fDrawDistance;
				}
				else {
					if (map[nTestRaySegmentY * nMapWidth + nTestRaySegmentX] == '#') {
						bHitWall = true;

						//Draw wall edges
						vector<pair<float, float>> p; // distance, dot

						for (int tx = 0; tx < 2; tx++) {
							for (int ty = 0; ty < 2; ty++) {
								float vy = (float)nTestRaySegmentY + ty - fPlayerYPos;
								float vx = (float)nTestRaySegmentX + tx - fPlayerXPos;
								float d = sqrt(vx*vx + vy*vy);
								float dot = (fRayX * vx / d) + (fRayY * vy / d);
								p.push_back(make_pair(d, dot));
							}
						}
						//Sort pairs from closes to farthest
						sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right) {return left.first < right.first; });

						float fBound = 0.01f;
						if (acos(p.at(0).second) < fBound) bLeftBoundary = true;
						if (acos(p.at(1).second) < fBound) bRightBoundary  = true;

					}
				}
			}

			//Calculate distance to ceiling and floor
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;// Mirror Ceiling

			short nBlock = ' ';


			for (int y = 0; y < nScreenHeight; y++) {
				if (y < nCeiling) {
					//The it is a ceiling
					nBlock = ' ';
				}
				else if (y > nCeiling && y <= nFloor) {
					//Then it is a wall
					if (fDistanceToWall <= fDrawDistance / 4.0f)		nBlock = 'I';//0x2588; // Closest
					else if (fDistanceToWall < fDrawDistance / 3.0f)	nBlock = 'l'; 
					else if (fDistanceToWall < fDrawDistance / 2.0f)	nBlock = 'i';//0x2592; 
					else if (fDistanceToWall < fDrawDistance)			nBlock = ':';//0x2591; // Farthest

					if(bLeftBoundary)									nBlock = '[';
					if(bRightBoundary)									nBlock = ']';
				}
				else {
					//Then it is a floor
					float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25)		nBlock = '=';
					else if (b < 0.5)	nBlock = '~';
					else if (b < 0.75)	nBlock = '-';
					else if (b < 0.9)	nBlock = '.';
			
				}

				screen[y*nScreenWidth + x] = nBlock;
			}

		}
		//Display Stats
		//swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f, FPS=%3.2f ", fPlayerXPos, fPlayerYPos, fPlayerViewAngle, 1.0f / fElapsedTime);
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f ", fPlayerXPos, fPlayerYPos, fPlayerViewAngle);

		//Display Map
		for (int nx = 0; nx < nMapWidth; nx++) {
			for (int ny = 0; ny < nMapWidth; ny++) {
				screen[(ny + 1)*nScreenWidth + nx] = map[ny * nMapWidth + nx];
			}
		}

		short sMapAvatar = get_map_avatar_direction();

		screen[((int)fPlayerYPos + 1) * nScreenWidth + (int)fPlayerXPos] = sMapAvatar; //'P';
		

		screen[nScreenWidth * nScreenHeight] = '\0';
		SetConsoleTextAttribute(hConsole, 200);

		//WORD attribute = FOREGROUND_RED;// | FOREGROUND_INTENSITY;
		//WriteConsoleOutputAttribute(hConsole, &attribute, 1, { 0, 0 }, &dwBytesWritten);
		WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight * nScreenWidth, { 0, 0 }, &dwBytesWritten);
		refreshScreen = false;
	
	}// End Game Loop
		return 0;
}

void write_symbol_in_color(HANDLE h, SHORT x, SHORT y, LPCSTR symbol, WORD color)
{
	COORD here;
	here.X = x;
	here.Y = y;

	WORD attribute = color;
	DWORD written;
	::WriteConsoleOutputAttribute(h, &attribute, 1, here, &written);
	::WriteConsoleOutputCharacterA(h, symbol, 1, here, &written);
}

