#include <iostream>
#include <ctime>
#include <random>
#include "help.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include<fstream>

using namespace std;

int highestScoresList[10];
int score = 0;

struct alien
{
	int x_coord;
	int y_coord;
	char movement = 'r';
	bool hit = false;
} alienIJ[5][10];

void scoreSort() 
{
	for (int i = 0; i < 2; i++) 
	{
		for (int j = 0; j < 2 - i; j++) 
		{
			if (highestScoresList[j] > highestScoresList[j + 1]) 
			{
				int temp = highestScoresList[j];
				highestScoresList[j] = highestScoresList[j + 1];
				highestScoresList[j + 1] = temp;
			}
		}
	}
}

void saveHighScores() 
{
    ofstream outData("highscores.txt");
    if (outData.is_open()) 
    {
        for (int i = 0; i < 3; ++i) 
    		outData << highestScoresList[i] << " ";
        outData.close();
    }
    else 
        cerr << "Unable to open high score file for writing." << endl;
}

void loadHighScores() 
{
    ifstream inData("highscores.txt");
    if (inData.is_open()) 
	{
        for (int i = 0; i < 3; i++) 
            inData >> highestScoresList[i];
        inData.close();
    }
    else
        cerr << "Unable to open high score file for reading." << endl;
}

int randomValue()
{
	srand(time(0));
	return rand();
}

void removeBox(const int &box_x, const int &box_y)
{
	myRect(box_x, box_y, box_x + 70, box_y + 20, 0, 0, 0);
	myRect(box_x, box_y - 10, box_x + 20, box_y, 0, 0, 0);
	myRect(box_x + 50, box_y - 10, box_x + 70, box_y, 0, 0, 0);
}

void playerShip(const int &box_x, const int &box_y)
{
	myRect(box_x, box_y, box_x + 70, box_y + 20, 135, 206, 235, 135, 206, 235);
	myRect(box_x, box_y - 10, box_x + 20, box_y, 135, 206, 235, 135, 206, 235);
	myRect(box_x + 50, box_y - 10, box_x + 70, box_y, 135, 206, 235, 135, 206, 235);
}

bool playerFireCollision(const int& alien_x, const int& alien_y, const int& fireLeft, const int& fireStart)
{
	int shipLeft, shipTop;
	for (int vert = 4; vert >= 0; vert--)
	{
		shipTop = alien_y + (vert * 20);
		for (int horiz = 0; horiz < 10; horiz++)
		{
			shipLeft = alien_x + (horiz * 25);
			if (alienIJ[vert][horiz].hit)
			{
				shipLeft += 10;
				continue;
			}
			if ((shipLeft + 25 > fireLeft) &&
				(shipLeft < fireLeft + 25) &&
				(shipTop + 20 > fireStart - 10) &&
				(shipTop < fireStart)
				)
			{
				alienIJ[vert][horiz].hit = true;
				return true;
			}
			shipLeft += 10;
		}
		shipTop -= 10;
	}
	return false;
}

void playerFires(const int &x, const int &start, const int &end, bool &release)
{
	if (end > 60)
	{
		myRect(x + 32, start, x + 38, end, 255, 180, 0, 255, 180, 0);
		Sleep(15);
		myRect(x + 32, start, x + 38, end, 0, 0, 0);
	}
	else
	{
		myRect(x + 32, start + 10, x + 38, 61, 0, 0, 0);
		release = false;
	}
}

void enemyShip(const int &x, const int &y)
{
	myEllipse(x, y, x + 25, y + 20, 255, 255, 0, 255, 255, 0);
	myLine(x, y + 10, x + 25, y + 10, 255, 0, 0);
	myLine(x + 13, y, x + 12, y + 20, 255, 0, 0);
}

void enemyRemove(const int& x, const int& y)
{
	myEllipse(x, y, x + 25, y + 20, 0, 0, 0, 0, 0, 0);
	myLine(x, y + 10, x + 25, y + 10, 0, 0, 0);
	myLine(x + 13, y, x + 12, y + 20, 0, 0, 0);
}

void alienFires(const int& x, const int& y, bool &fire_release)
{
	if (y + 20 < 360)
	{
		myRect(x + 10, y + 10, x + 15, y + 20, 0, 0, 255, 0, 0, 255);
		Sleep(15);
		myRect(x + 10, y + 10, x + 15, y + 20, 0, 0, 0, 0, 0, 0);
	}
	else
		fire_release = false;
}

void startpage(const int& box_x, const int& box_y)
{

	myRect(0, 0, 0, 0, 0, 0, 0);
	myRect(box_x + 50, box_y + 300, box_x + 1000, box_y - 350, 135, 206, 235, 12, 0, 12);
	drawText(28, 340, 60, 255, 255, 255, "SPACE INVADER");


	myRect(box_x + 200, box_y + 100, box_x + 800, box_y - 30, 225, 225, 0, 0, 0, 0);
	drawText(17, 280, 190, 0, 255, 0, "press enter to start new game");
	drawText(17, 280, 240, 0, 255, 0, "press shift to resume the previous game");
	drawText(16.5, 250, 320, 64, 52, 235, "-->use spacebar for firing");
	drawText(16.5, 250, 360, 64, 52, 235, "-->use arrow keys for movement");
	drawText(16.5, 250, 400, 64, 52, 235, "-->use escape key to pause the game");

	drawText(16, 250, 500, 128, 0, 120, "10 points for killing each allien");

}

void point(const int& x, const int& y)
{
	myRect(0, 0, 0, 0, 0, 0, 0);
	drawText(20, 310, 150, 0, 255, 0, "Choose your level");
	drawText(20, 310, 260, 0, 128, 128, "press 0 for easy level ");
	drawText(20, 310, 310, 0, 128, 128, "press 1 for hard level");

}

void saveGameState(const int& box_x, const int& box_y, const char& direction, const bool& release, const bool& playerBullet,const int& randAlien_x, const int& randAlien_y, const int& playerLives, const int& fireEndCoord_y)
{
	ofstream outFile("savegame.txt");
	if (outFile.is_open()) 
	{
		outFile << box_x << " " << box_y << " " << direction << " " << release << " " << playerBullet << " "
			<< randAlien_x << " " << randAlien_y << " " << score << " " << playerLives << " " << fireEndCoord_y;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 10; j++)
				outFile << " " << alienIJ[i][j].hit;
		outFile.close();
	}
	else
		cerr << "Unable to open file for saving game state." << endl;
}

void loadGameState(int& box_x, int& box_y, char& direction, bool& release, bool& playerBullet,int& randAlien_x, int& randAlien_y, int& playerLives, int& fireEndCoord_y)
{
	ifstream inFile("savegame.txt");
	if (inFile.is_open()) 
	{
		inFile >> box_x >> box_y >> direction >> release >> playerBullet >> randAlien_x >> randAlien_y >> score >> playerLives >> fireEndCoord_y;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 10; j++)
				inFile >> alienIJ[i][j].hit;
		inFile.close();
	}
	else
		cerr << "Unable to open file of saved game." << endl;
}

void unloadGameState()
{
	ofstream outFile("savegame.txt");
	if (outFile.is_open())
		outFile.close();
}

void pause(const int& x, const int& y)
{
	myRect(x - 10000, y + 300, x + 1200, y - 350, 135, 206, 235, 12, 0, 12);
	drawText(22, 400, 150, 255, 255, 255, "GAME PAUSE");

	// Display high scores
	for (int i = 0; i < 3; ++i)
	{
		stringstream ss;
		ss << "High Score " << i + 1 << ": " << highestScoresList[i];
		drawText(19, 380, 400 + i * 50, 255, 255, 255, ss.str().c_str());
	}

	drawText(19, 280, 250, 0, 255, 0, "press shift to exit the game");
	drawText(19, 280, 300, 0, 255, 0, "press enter to resume the game");
}

void resume(const int& x, const int& y, const int& left_limit, const int& top_limit, const int& right_limit, const int& bottom_limit)
{
	myRect(x + 50, y + 300, x + 1000, y - 350, 0, 0, 0, 0, 0, 0);
	drawText(28, 400, 250, 0, 0, 0, "GAME PAUSE");
	myRect(left_limit - 2, top_limit - 2, right_limit + 2, bottom_limit + 2, 255, 0, 0);
}

void end_game(const int& box_x, const int& box_y)
{
	myRect(box_x - 500, box_y + 500, box_x + 1500, box_y - 350, 135, 206, 235, 12, 0, 12);
	drawText(30, 400, 250, 255, 255, 255, "Game Over");

	stringstream scoreText;
	scoreText << "My Score: " << score;
	drawText(20, 400, 380, 255, 255, 0, scoreText.str().c_str());

	for (int i = 0; i < 3; i++)
	{
		if (highestScoresList[i] < score)
		{
			highestScoresList[0] = score;
			scoreSort();
			saveHighScores();
			break;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		stringstream ss;
		ss << "High Score " << i + 1 << ": " << highestScoresList[i];
		drawText(19, 380, 70 + i * 50, 255, 255, 255, ss.str().c_str());
	}

	while (!GetAsyncKeyState(VK_RETURN))
		continue;
}

bool alienCheck()
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 10; j++)
			if (!alienIJ[i][j].hit)
				return false;

	stringstream scoreText;
	scoreText << "My Score: " << score;
	drawText(20, 400, 380, 255, 255, 0, scoreText.str().c_str());

	for (int i = 0; i < 3; i++)
	{
		if (highestScoresList[i] < score)
		{
			highestScoresList[0] = score;
			scoreSort();
			saveHighScores();
			break;
		}
	}
	myRect(30 - 500, 340 + 500, 30 + 1500, 340 - 350, 135, 206, 235, 12, 0, 12);
	drawText(30, 400, 250, 255, 255, 255, "You Win!");

	for (int i = 0; i < 3; ++i)
	{
		stringstream ss;
		ss << "High Score " << i + 1 << ": " << highestScoresList[i];
		drawText(19, 380, 70 + i * 50, 255, 255, 255, ss.str().c_str());
	}
	while (!GetAsyncKeyState(VK_RETURN));
	return true;
}

void spaceshipAlien_collision(const int& alien_x, const int& alien_y, const int& box_x, const int& box_y, const int& row, const int& col)
{
	if (
		(alienIJ[row][col].hit == false) &&
		(box_x + 70 > alien_x) &&
		(box_x < alien_x + 25) &&
		(box_y + 20 > alien_y) &&
		(box_y < alien_y + 20)
		)
	{
		unloadGameState();
		end_game(30, 340);
		exit(0);
	}
}

void display_point()
{
	myRect(830, 150, 1070, 100, 0, 255, 0);

	stringstream ss;

	ss << "Score: " << setw(4) << setfill('0') << score;

	drawText(20, 835, 100, 0, 255, 0, ss.str().c_str());

	for (int i = 0; i < 3; ++i)
	{
		stringstream highScoreText;
		highScoreText << "High Score " << i + 1 << ": " << highestScoresList[i];
		drawText(17, 750, 400 + i * 30, 0, 255, 0, highScoreText.str().c_str());
	}
}

void display_lives(const int& lives)
{
	myRect(90, 50, 300, 10, 0, 255, 0);

	stringstream ss;
	ss << "Lives: " << setw(2) << setfill('0') << lives;

	drawText(20, 100, 10, 0, 255, 0, ss.str().c_str());
}


int main()
{
	int left_limit = 20, right_limit = 820, top_limit = 60, bottom_limit = 360;
	int box_x = 30, box_y = 340, step_size = 5, x = 30, fireEndCoord_y, fireStartCoord_y;
	char direction = ' ';
	bool release = false, playerBullet = false, level = false;
	int randAlien_x = randomValue() % 10;
	int randAlien_y = randomValue() % 5;
	int playerLives = 3;
	int livesTimerDuration = 15;
	int scoreTimerDuration = 20;
	auto startTime = chrono::high_resolution_clock::now();

	loadHighScores();

	startpage(box_x, box_y);
	while (true)
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			myRect(30 + 50, 340 + 300, 30 + 1000, 340 - 350, 0, 0, 0, 0, 0, 0);
			myRect(30 + 10, 340 + 200, 30 + 1000, 340 - 300, 0, 0, 0, 0, 0, 0);
			drawText(30, 250, 80, 0, 0, 0, "Welcome to the Game");
			drawText(30, 310, 150, 0, 0, 0, "SPACE INVADER");
			point(30, 340);
			while (true)
			{
				if (GetAsyncKeyState(0x30))    // 0 Key for easy level
				{
					level = true;
					break;
				}
				if (GetAsyncKeyState(0x31))   // 1 Key for hard level
				{
					livesTimerDuration += 10;
					scoreTimerDuration -= 5;
					break;
				}
			}
			break;
		}
		if (GetAsyncKeyState(VK_RSHIFT))
		{
			myRect(30 + 50, 340 + 300, 30 + 1000, 340 - 350, 0, 0, 0, 0, 0, 0);
			myRect(30 + 10, 340 + 200, 30 + 1000, 340 - 300, 0, 0, 0, 0, 0, 0);
			drawText(30, 250, 80, 0, 0, 0, "Welcome to the Game");
			drawText(30, 310, 150, 0, 0, 0, "SPACE INVADER");
			loadGameState(box_x, box_y, direction, release, playerBullet, randAlien_x, randAlien_y, playerLives, fireEndCoord_y);
			level = false;
			break;
		}
	}

	
	display_lives(playerLives);
	display_point();

	auto livesIncrementTimer = startTime + chrono::seconds(livesTimerDuration);
	auto scoreDecrementTimer = startTime + chrono::seconds(scoreTimerDuration);

	myRect(30 + 40, 340 + 190, 30 + 370, 340 + 0, 0, 0, 0, 0, 0, 0);
	drawText(16, 90, 430, 0, 0, 0, "Choose your level");


	myRect(left_limit - 2, top_limit - 2, right_limit + 2, bottom_limit + 2, 255, 0, 0);
	drawText(20, 10, 365, 0, 255, 0, "Use Cursor keys to move the box!");
	playerShip(box_x, box_y);

	alienIJ[0][0].x_coord = 30;
	alienIJ[0][0].y_coord = 65;

	for (int i = 0; i < 150; i += 30)
		for (int j = 0; j < 350; j += 35)
			enemyShip(alienIJ[0][0].x_coord + j, alienIJ[0][0].y_coord + i);

	int whichKey = 0;

	while (true)
	{
		while (whichKey != 7)
		{
			if (chrono::high_resolution_clock::now() >= livesIncrementTimer)
			{
				if (playerLives < 5)
					playerLives++;
				livesIncrementTimer = chrono::high_resolution_clock::now() + chrono::seconds(livesTimerDuration);
			}
			if (chrono::high_resolution_clock::now() >= scoreDecrementTimer)
			{
				if (score <= 5)
					score = 0;
				else
					score -= 5;
				scoreDecrementTimer = chrono::high_resolution_clock::now() + chrono::seconds(scoreTimerDuration);
			}
			saveGameState(box_x, box_y, direction, release, playerBullet, randAlien_x, randAlien_y, playerLives, fireEndCoord_y);
			if (alienCheck())
			{
				unloadGameState();
				return 0;
			}
			if (isKeyPressed(whichKey))
			{
				switch (whichKey)
				{
				case 1:
					direction = 'L';
					break;
				case 2:

					direction = 'U';
					break;
				case 3:
					direction = 'R';
					break;
				case 4:
					direction = 'D';
					break;
				}
			}

			Sleep(10);

			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					int alien_x = alienIJ[0][0].x_coord + j * 35;
					int alien_y = alienIJ[0][0].y_coord + i * 30;
					enemyRemove(alien_x, alien_y);
					spaceshipAlien_collision(alien_x, alien_y, box_x, box_y, i, j);
				}
			}

			switch (alienIJ[0][0].movement)
			{
			case 'r':
			{
				int grid_width, grid_height;
				for (grid_height = 0; grid_height < 5; grid_height++)
					for (int l = 0; l < 10; l++)
						if (!alienIJ[grid_height][l].hit)
							break;
				grid_height *= 30;
				for (grid_width = 0; grid_width < 10; grid_width++)
					for (int l = 0; l < 5; l++)
						if (!alienIJ[l][grid_width].hit)
							break;
				grid_width *= 35;
				if ((alienIJ[0][0].x_coord + grid_width + 2) < right_limit)
				{
					for (int i = 0, row = 0; i < 150; i += 30, row++)
						for (int j = 0, col = 0; j < 350; j += 35, col++)
							if (!alienIJ[row][col].hit)
								enemyShip(alienIJ[0][0].x_coord + j + 2, alienIJ[0][0].y_coord + i);
					alienIJ[0][0].x_coord += 2;
				}
				else if ((alienIJ[0][0].y_coord + grid_height + 20) < bottom_limit)
				{
					alienIJ[0][0].y_coord += 20;
					for (int i = 0, row = 0; i < 150; i += 30, row++)
						for (int j = 0, col = 0; j < 350; j += 35, col++)
							if (!alienIJ[row][col].hit)
								enemyShip(alienIJ[0][0].x_coord + j, alienIJ[0][0].y_coord + i);
					alienIJ[0][0].movement = 'l';
				}
				else 
				{
					unloadGameState();
					end_game(box_x, box_y);
					exit(0);
				}
				break;
			}
			case 'l':
			{
				int grid_width, grid_height;
				for (grid_height = 0; grid_height < 5; grid_height++)
					for (int l = 0; l < 10; l++)
						if (!alienIJ[grid_height][l].hit)
							break;
				grid_height *= 30;
				for (grid_width = 0; grid_width < 10; grid_width++)
					for (int l = 0; l < 5; l++)
						if (!alienIJ[l][grid_width].hit)
							break;
				grid_width *= 35;
				if ((alienIJ[0][0].x_coord - 2) > left_limit)
				{
					for (int i = 0, row = 0; i < 150; i += 30, row++)
						for (int j = 0, col = 0; j < 350; j += 35, col++)
							if (!alienIJ[row][col].hit)
								enemyShip(alienIJ[0][0].x_coord + j - 2, alienIJ[0][0].y_coord + i);
					alienIJ[0][0].x_coord -= 2;
				}
				else if ((alienIJ[0][0].y_coord + grid_height + 20) < bottom_limit)
				{
					alienIJ[0][0].y_coord += 20;
					for (int i = 0, row = 0; i < 150; i += 30, row++)
						for (int j = 0, col = 0; j < 350; j += 35, col++)
							if (!alienIJ[row][col].hit)
								enemyShip(alienIJ[0][0].x_coord + j, alienIJ[0][0].y_coord + i);
					alienIJ[0][0].movement = 'r';
				}
				else
				{
					unloadGameState();
					end_game(30, 340);
					exit(0);
				}
				break;
			}
			}

			switch (direction)
			{
			case 'L':
				removeBox(box_x, box_y);
				if (box_x > left_limit)
					box_x -= step_size;
				playerShip(box_x, box_y);
				break;
			case 'U':
				removeBox(box_x, box_y);
				if (box_y - 10 > top_limit)
					box_y -= step_size;
				playerShip(box_x, box_y);
				break;
			case 'R':
				removeBox(box_x, box_y);
				if (box_x + 70 < right_limit)
					box_x += step_size;
				playerShip(box_x, box_y);
				break;
			case 'D':
				removeBox(box_x, box_y);
				if (box_y + 20 < bottom_limit)
					box_y += step_size;
				playerShip(box_x, box_y);
				break;
			}

			if (playerBullet == true)
			{
				fireStartCoord_y = fireEndCoord_y - 1;
				fireEndCoord_y = fireStartCoord_y - 10;
				if (playerFireCollision(alienIJ[0][0].x_coord, alienIJ[0][0].y_coord, x, fireStartCoord_y))
				{
					playerBullet = false;
					score += 10;
					display_point();
					continue;
				}
				playerFires(x, fireStartCoord_y, fireEndCoord_y, playerBullet);
			}
			else if (GetAsyncKeyState(32))
			{
				x = box_x;
				fireEndCoord_y = box_y - 10;
				fireStartCoord_y = box_y;
				if (playerFireCollision(alienIJ[0][0].x_coord, alienIJ[0][0].y_coord, x, fireStartCoord_y))
				{
					playerBullet = false;
					score += 10;

					continue;
				}
				playerBullet = true;
				playerFires(x, fireStartCoord_y, fireEndCoord_y, playerBullet);
			}

			if (release == false)
			{
				int x = randomValue() % 10, y = randomValue() % 5, loop_count = 1;
				bool found = true;
				while (alienIJ[y][x].hit == true && loop_count <= 5) // Keep in loop until not-hit ship found but max iterations = 5
				{
					x = randomValue() % 10;
					y = randomValue() % 5;
					loop_count++;
					if (loop_count == 5)
					{
						found = false;
						break;
					}
				}
				if (found)
				{
					randAlien_x = (x * 35) + alienIJ[0][0].x_coord;
					randAlien_y = (y * 30) + alienIJ[0][0].y_coord;
					alienFires(randAlien_x, randAlien_y, release);
					release = true;
					if (
						(box_x + 70 > randAlien_x) &&
						(box_x < randAlien_x + 5) &&
						(box_y + 20 > randAlien_y) &&
						(box_y < randAlien_y + 10)
						)
					{
						release = false;
						playerLives--;
						display_lives(playerLives);
						if (playerLives == 0)
						{
							unloadGameState();
							end_game(30, 340);
							return 0;
						}
					}
				}
			}
			else
			{
				randAlien_y += 11;
				alienFires(randAlien_x, randAlien_y, release);
				if (
					(box_x + 70 > randAlien_x) &&
					(box_x < randAlien_x + 5) &&
					(box_y + 20 > randAlien_y) &&
					(box_y < randAlien_y + 10)
					)
				{
					release = false;
					playerLives--;
					display_lives(playerLives);
					if (playerLives == 0)
					{
						unloadGameState();
						end_game(30, 340);
						return 0;
					}
				}
			}
		}
		pause(30, 340);
		while (true)
		{
			if (GetAsyncKeyState(VK_RSHIFT))
			{
				saveGameState(box_x, box_y, direction, release, playerBullet, randAlien_x, randAlien_y, playerLives, fireEndCoord_y);
				end_game(30, 340);
				exit(0);
			}
			else if (GetAsyncKeyState(VK_RETURN))
			{
				resume(30, 340, left_limit, top_limit, right_limit, bottom_limit);
				display_lives(playerLives);
				display_point();
				break;
			}
		}
		whichKey = 0;
	}
	return 0;
}