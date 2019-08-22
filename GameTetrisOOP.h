#ifndef __GAME_TETRIS_OOP_H__
#define __GAME_TETRIS_OOP_H__

#include "GameProgram.h"
#include <time.h>

class GameTetrisOOP : public GameProgram
{
private:
	GameObject* shape;
	GameObject* nextShape;
	int** checkTable;
	int current;
	int next;
	int keyPressed;
	int score;

	bool checkCollide(GameObject* shape)
	{
		int x = shape->getCoord().x, y = shape->getCoord().y, shapeWidth = shape->getWidth(), shapeHeight = shape->getHeight();
		for (int i = 0; i < shapeHeight; i++)
		{
			for (int j = 0; j < shapeWidth; j++)
			{
				if (shape->getBody()[i][j] != ' ' && checkTable[i + y][j + x])
					return true;
			}
		}
		return false;
	}
	bool checkFullRow(int row)
	{
		int frameWidth = frame->getWidth();
		for (int j = 1; j < frameWidth - 1; j++)
		{
			if (checkTable[row][j] == 0)
				return false;
		}
		return true;
	}
	bool checkGameOver()
	{
		if (shape->getCoord().y <= 0)
		{
			system("cls");
			GameBaseFunction::gotoXY(30, 15);
			std::cout << "GAME OVER :)";
			return true;
		}
		return false;
	}
	void drawCheckTable()
	{
		char c;
		int color, frameHeight = frame->getHeight(), frameWidth = frame->getWidth();
		for (int i = 0; i < frameHeight - 1; i++)
		{
			for (int j = 1; j < frameWidth - 1; j++)
			{
				c = '.';
				switch (checkTable[i][j])
				{
				case 0:
					color = 0;
					c = ' ';
					break;
				case 1:
					color = 100;
					break;
				case 2:
					color = 120;
					break;
				case 3:
					color = 140;
					break;
				case 4:
					color = 160;
					break;
				case 5:
					color = 180;
					break;
				case 6:
					color = 200;
					break;
				case 7:
					color = 220;
					break;
				}
				GameBaseFunction::drawChar(j, i, c, color);
			}
		}
	}
	void randomShape()
	{
		if (nextShape)
		{
			if (shape)
				delete shape;
			shape = new GameObject(*nextShape);
			shape->setCoord(Coord{ 10, 0 });
		}
		next = rand() % 7;
		if (nextShape)
		{
			nextShape->erase();
			delete nextShape;
		}
		nextShape = createShape(next);
		nextShape->setVisible(true);
		nextShape->draw();
	}
	void guide()
	{
		GameBaseFunction::gotoXY(frame->getWidth() + 3, 4);
		std::cout << "A: move left";
		GameBaseFunction::gotoXY(frame->getWidth() + 3, 5);
		std::cout << "D: move right";
		GameBaseFunction::gotoXY(frame->getWidth() + 3, 6);
		std::cout << "S: move down";
		GameBaseFunction::gotoXY(frame->getWidth() + 3, 7);
		std::cout << "Z: show check table";
		GameBaseFunction::gotoXY(frame->getWidth() + 3, 8);
		std::cout << "Enter: rotate";
		GameBaseFunction::gotoXY(frame->getWidth() + 3, 9);
		std::cout << "+: speed up";
		GameBaseFunction::gotoXY(frame->getWidth() + 3, 10);
		std::cout << "-: speed down";
	}
	GameObject* createShape(int randomShape)
	{
		switch (randomShape)
		{
		case 0:
			return new GameObject(
			{ '.', '.', '.',
			' ', '.', ' ' }, 3, 2, 100, INIT_COORD_OF_SHAPE, Direction::NO);
			break;
		case 1:
			return new GameObject(
			{ '.', '.', '.', '.', '.' }, 5, 1, 120, INIT_COORD_OF_SHAPE, Direction::NO);
			break;
		case 2:
			return new GameObject(
			{ '.', '.',
			'.', '.' }, 2, 2, 140, INIT_COORD_OF_SHAPE, Direction::NO);
		case 3:
			return new GameObject(
			{ '.', ' ', ' ', ' ',
			'.', '.', '.', '.' }, 4, 2, 160, INIT_COORD_OF_SHAPE, Direction::NO);
		case 4:
			return  new GameObject(
			{ ' ', ' ', ' ', '.',
			'.', '.', '.', '.' }, 4, 2, 180, INIT_COORD_OF_SHAPE, Direction::NO);
		case 5:
			return new GameObject(
			{ '.', '.', ' ',
			' ', '.', '.' }, 3, 2, 200, INIT_COORD_OF_SHAPE, Direction::NO);
		case 6:
			return new GameObject(
			{ ' ', '.', '.',
			'.', '.', ' ' }, 3, 2, 220, INIT_COORD_OF_SHAPE, Direction::NO);
		default:
			break;
		}
		return NULL;
	}
	DWORD WINAPI dropDown()
	{
		bool flag = false;
		int frameHeight = frame->getHeight(), frameWidth = frame->getWidth();
		for (int i = frameHeight - 2; i >= 0; i--)
		{
			if (checkFullRow(i))
			{
				flag = true;
				for (int j = i; j > 0; j--)
				{
					for (int k = 1; k < frameWidth - 1; k++)
						checkTable[j][k] = checkTable[j - 1][k];
				}
				i++;
				score += 100;
			}
		}
		if (flag)
		{
			drawCheckTable();
			GameBaseFunction::gotoXY(frame->getWidth() + 3, 0);
			std::cout << "SCORE = " << score << "\t";
		}
		return 0;
	}
	static DWORD WINAPI keyEventThreadStart(LPVOID lp)
	{
		GameTetrisOOP* ptr = (GameTetrisOOP*)lp;
		return ptr->keyboardEvent();
	}
	static DWORD WINAPI dropDownThreadStart(LPVOID lp)
	{
		GameTetrisOOP* ptr = (GameTetrisOOP*)lp;
		return ptr->dropDown();
	}
public:
	static const Coord INIT_COORD_OF_SHAPE;

	GameTetrisOOP() : GameProgram() 
	{
		shape = NULL;
		nextShape = NULL;
		checkTable = NULL;
		current = 0;
		next = 0;
		keyPressed = 0;
		score = 0;
	}
	~GameTetrisOOP()
	{
		if (shape)
			delete shape;
		if (nextShape)
			delete nextShape;
		if (checkTable)
		{
			for (int i = 0; i < frame->getHeight(); i++)
				delete[] checkTable[i];
			delete[] checkTable;
		}
	}

	void init()
	{
		GameBaseFunction::resizeCMD(560, 560);

		setFrameSize(20, 30);
		setFrameColor(230);
		setMiliSecondForSleep(150);

		int frameWidth = frame->getWidth(), frameHeight = frame->getHeight();
		checkTable = new int*[frameHeight];
		for (int i = 0; i < frameHeight; i++)
			checkTable[i] = new int[frameWidth];
		for (int i = 0; i < frameHeight; i++)
		{
			for (int j = 0; j < frameWidth; j++)
			{
				if (i == frameHeight - 1 || j == 0 || j == frameWidth - 1)
					checkTable[i][j] = -1;
				else
					checkTable[i][j] = 0;
			}
		}
	}
	DWORD WINAPI keyboardEvent()
	{
		if (_kbhit())
		{
			int frameHeight = frame->getHeight(), frameWidth = frame->getWidth();
			keyPressed = _getch();
			switch (keyPressed)
			{
			case 'A': case 'a':
				shape->setVisible(false);
				shape->moveLeft(1);
				if (checkCollide(shape))
					shape->moveRight(1);
				else
				{
					shape->moveRight(1);
					shape->setVisible(true);
					shape->moveLeft(1);
				}
				break;
			case 'D': case 'd':
				shape->setVisible(false);
				shape->moveRight(1);
				if (checkCollide(shape))
					shape->moveLeft(1);
				else
				{
					shape->moveLeft(1);
					shape->setVisible(true);
					shape->moveRight(1);
				}
				break;
			case 'S': case 's':
				shape->setVisible(false);
				shape->moveDown(1);
				if (checkCollide(shape))
					shape->moveUp(1);
				else
				{
					shape->moveUp(1);
					shape->setVisible(true);
					shape->moveDown(1);
				}
				break;
			case 'Z': case 'z':
				for (int i = 0; i < frameHeight; i++)
				{
					for (int j = 0; j < frameWidth; j++)
						GameBaseFunction::drawChar(frameWidth + j + 25, i, checkTable[i][j] + 48, 15);
				}
				break;
			case 13:
				shape->setVisible(false);
				shape->rotateLeft();
				if (checkCollide(shape))
					shape->rotateRight();
				else
				{
					shape->rotateRight();
					shape->setVisible(true);
					shape->rotateLeft();
				}
				break;
			case '+':
				if (miliSecondForSleep)
				{
					miliSecondForSleep -= 5;
					GameBaseFunction::gotoXY(frame->getWidth() + 3, 1);
					std::cout << "SPEED = " << miliSecondForSleep << "\t";
				}
				break;
			case '-':
				miliSecondForSleep += 5;
				GameBaseFunction::gotoXY(frame->getWidth() + 3, 1);
				std::cout << "SPEED = " << miliSecondForSleep << "\t";
				break;
			}
		}
		return 0;
	}
	void run()
	{
		this->init();

		frame->setVisible(true);
		frame->draw();

		srand(time(0));
		randomShape();
		current = next;
		randomShape();

		DWORD dwKeyboardEventThreadId;
		DWORD dwDropDownThreadId;
		HANDLE hKeyboardEventThread;
		HANDLE hDropDownThread;

		GameBaseFunction::gotoXY(frame->getWidth() + 3, 0);
		std::cout << "SCORE = " << score << "\t";
		GameBaseFunction::gotoXY(frame->getWidth() + 3, 1);
		std::cout << "SPEED = " << miliSecondForSleep << "\t";
		guide();

		while (keyPressed != 27)
		{
			hKeyboardEventThread = CreateThread(NULL, 0, keyEventThreadStart, (LPVOID)this, 0, &dwKeyboardEventThreadId);
			Sleep(miliSecondForSleep);
			shape->setVisible(false);
			shape->moveDown(1);
			if (checkCollide(shape))
			{
				shape->moveUp(1);
				if (checkGameOver())
					return;
				for (int i = 0; i < shape->getHeight(); i++)
				{
					for (int j = 0; j < shape->getWidth(); j++)
					{
						if (shape->getBody()[i][j] != ' ')
							checkTable[i + shape->getCoord().y][j + shape->getCoord().x] = current + 1;
					}
				}
				current = next;
				randomShape();
			}
			else
			{
				shape->moveUp(1);
				shape->setVisible(true);
				shape->moveDown(1);
			}
			hDropDownThread = CreateThread(NULL, 0, dropDownThreadStart, (LPVOID)this, 0, &dwDropDownThreadId);
		}
	}
};
const Coord GameTetrisOOP::INIT_COORD_OF_SHAPE = Coord{ 26, 2 };

#endif