#include <iostream>
#include <vector>
#include <deque>
#include <conio.h>
#include <set>
#include <windows.h>
using namespace std;
struct coordinates
{
	int x;
	int y;
};
const int leftButton = 75;
const int rightButton = 77;
const int downButton = 80;
const int upButton = 72;
const int spaceButton = 32;
const int gameSpeed = 500;
const string gameLevel = "hard+";
const set<int> arrows{72, 75, 77, 80};//коды стрелок
const coordinates fieldSize{ 8, 4 };
const char snakeUnit = '0';
const char appleUnit = '@';
const char fieldUnit = '.';
const char snakeHeadUnit = '$';

class Game
{
public:
	deque<coordinates> snake;
	vector<vector<char>> field;
	string gameStatus;
	string level;
	int direction;
	int speed;
	bool lunch;
	int width;
	int height;
	void initField()
	{
		for (int i{ 0 }; i < height; i++)//инициализация поля
		{
			vector<char> row;
			for (int j{ 0 }; j < width; j++)
			{
				row.push_back(fieldUnit);
			}
			field.push_back(row);
		}
	}
	void printField()
	{
		for (int i{ 0 }; i < height; i++)//вывод поля
		{
			for (int j{ 0 }; j < width; j++)
			{
				cout << field[i][j] << ' ';
			}
			cout << endl;
		}
	}
	void plantApple()
	{
		bool applePlanted = false;
		coordinates apple;
		vector<coordinates> freeCells = getFreeCells();
		while (applePlanted != true && !freeCells.empty())
		{
			apple.x = 0 + rand() % height;
			apple.y = 0 + rand() % width;
			for (coordinates n : freeCells)
			{
				if (n.x == apple.x && n.y == apple.y)
				{
					applePlanted = true;
				}
			}
		}
		if (applePlanted)
		{
			field[apple.x][apple.y] = appleUnit;
		}
	}
	void initSnake()
	{
		int x = height / 2;
		int y = width / 2;
		coordinates head_position{ x, y };
		snake.push_back(head_position);
		y--;
		coordinates tale{ x,y };
		snake.push_back(tale);
	}
	void placingSnake()
	{
		for (coordinates n : snake) //отображение змейки
		{
			field[n.x][n.y] = snakeUnit;
		}
		coordinates snakeHead{ snake.front() };
		field[snakeHead.x][snakeHead.y] = snakeHeadUnit;
	}
	void moveSnakeCheckLunch()
	{
		deque<coordinates> newSnakePosition;
		coordinates newHeadPosition{ snake.front() };
		field[newHeadPosition.x][newHeadPosition.y] = snakeUnit;
		switch (direction)
		{
		case leftButton:
		{
			if (newHeadPosition.y >= 1)
			{
				newHeadPosition.y--;
			}
			else {
				if (level == "easy")
				{
					gameStatus = "Game over. Snake met the wall";
				}
				else if (level == "normal") {
					newHeadPosition.y = width - 1;
				}
				else if (level == "hard" || level == "hard+")
				{
					if (snake.size() == 2) {
						gameStatus = "Game over. Snake is too weak to survive hit the wall";
					}
					else if (level == "hard")
					{
						direction = downButton;
						eraseTail();
						if (newHeadPosition.x + 1 >= height)
						{
							gameStatus = "Game over. No way to move";
						}
						else {
							newHeadPosition.x++;
						};
					}
					else if (level == "hard+")
					{
						newHeadPosition = checkFreeWayAndMove(newHeadPosition);
					}
				}
			}
			break;
		}
		case rightButton:
		{
			if (newHeadPosition.y + 1 < width)
			{
				newHeadPosition.y++;
			}
			else {
				if (level == "easy")
				{
					gameStatus = "Game over. Snake met the wall";
				}
				else if (level == "normal") {
					newHeadPosition.y = 0;
				}
				else if (level == "hard" || level == "hard+")
				{
					if (snake.size() == 2) {
						gameStatus = "Game over. Snake is too weak to survive hit the wall";
					}
					else if (level == "hard")
					{
						direction = upButton;
						eraseTail();
						if (newHeadPosition.x - 1 < 0)
						{
							gameStatus = "Game over. No way to move";
						}
						else {
							newHeadPosition.x--;
						}; 
					}
					else if (level == "hard+")
					{
						newHeadPosition = checkFreeWayAndMove(newHeadPosition);
					}
				}
			} 
			break;
		}
		case upButton:
		{
			if (newHeadPosition.x >= 1)
			{
				newHeadPosition.x--;
			}
			else {
				if (level == "easy")
				{
					gameStatus = "Game over. Snake met the wall";
				}
				else if (level == "normal") {
					newHeadPosition.x = height - 1;
				}
				else if (level == "hard" || level == "hard+")
				{
					if (snake.size() == 2) {
						gameStatus = "Game over. Snake is too weak to survive hit the wall";
					}
					else if (level == "hard")
					{
						direction = leftButton;
						eraseTail();
						if (newHeadPosition.y - 1 < 0)
						{
							gameStatus = "Game over. No way to move";
						}
						else {
							newHeadPosition.y--;
						}; 
					}
					else if (level == "hard+")
					{
						newHeadPosition = checkFreeWayAndMove(newHeadPosition);
					}
				}
			} 
			break;
		}
		case downButton:
		{
			if (newHeadPosition.x + 1 < height)
			{
				newHeadPosition.x++;
			}
			else {
				if (level == "easy")
				{
					gameStatus = "Game over. Snake met the wall";
				}
				else if (level == "normal") {	
					newHeadPosition.x = 0;
				}
				else if (level == "hard" || level == "hard+")
				{
					if (snake.size() == 2) {
						gameStatus = "Game over. Snake is too weak to survive hit the wall";
					}
					else if (level == "hard")
					{
						direction = rightButton;
						eraseTail();
						if (newHeadPosition.y + 1 >= width)
						{
							gameStatus = "Game over. No way to move";
						}
						else {
							newHeadPosition.y++;
						}; 
					}
					else if (level == "hard+")
					{
						newHeadPosition = checkFreeWayAndMove(newHeadPosition);
					}
				}
			} 
			break;
		}
		}
		newSnakePosition.push_back(newHeadPosition);
		for (coordinates n : snake)
		{
			newSnakePosition.push_back(n);
		}
		snake = newSnakePosition;
		if (!(field[newHeadPosition.x][newHeadPosition.y] == appleUnit))
		{
			eraseTail();
		}
		else { lunch = true; };
		field[newHeadPosition.x][newHeadPosition.y] = snakeHeadUnit;
		if (gameStatus == "run")
		{
			checkEatHimself();
		};
	}
	bool correctButton(int button)
	{
		bool correct = true;
		switch (button)
		{
		case leftButton:
		{
			if (direction == rightButton)
			{
				correct = false;
			}
			break;
		}
		case rightButton:
		{
			if (direction == leftButton)
			{
				correct = false;
			}
			break;
		}
		case upButton:
		{
			if (direction == downButton)
			{
				correct = false;
			}
			break;
		}
		case downButton:
		{
			if (direction == upButton)
			{
				correct = false;
			}
			break;
		}
		}
		return correct;
	};
	private:
		void eraseTail()
		{
			coordinates tail = snake.back();
			field[tail.x][tail.y] = fieldUnit;
			snake.pop_back();
		}
		void checkEatHimself()
		{
			bool eatHimself = true;
			coordinates head = snake.front();
			for (coordinates n : snake)
			{
				if (n.x == head.x && n.y == head.y)
				{
					if (eatHimself)
					{
						eatHimself = false;
					}
					else
					{
						eatHimself = true;
					}
				}
			}
			if (eatHimself)
			{
				gameStatus = "Game over. Snake eat himself";
			}
		}
		vector<coordinates> getFreeCells()
		{
			vector<coordinates> freeCells;
			for (int i{ 0 }; i < height; i++)
			{
				for (int j{ 0 }; j < width; j++)
				{
					if (field[i][j] == fieldUnit)
					{
						coordinates free{ i,j };
						freeCells.push_back(free);
					};
				}
			}
			if (freeCells.empty())
			{
				gameStatus = "You win";
			}
			return freeCells;
		}
		coordinates checkFreeWayAndMove(coordinates head)
		{
			coordinates newHead = head;
			eraseTail();
			if (head.x == 0)
			{
				if (head.y == 0)
				{
					if (field[head.x+1][head.y] != snakeUnit)
					{
						direction = downButton;
						newHead = { head.x + 1, head.y };
					}
					else if (field[head.x][head.y + 1] != snakeUnit)
					{
						direction = rightButton;
						newHead = { head.x, head.y + 1 };
					}
					else 
					{
						gameStatus = "Game over. No way to move";
					}
				}
				else if (head.y == width - 1)
				{
					if (field[head.x + 1][head.y] != snakeUnit)
					{
						direction = downButton;
						newHead = { head.x + 1, head.y };
					}
					else if (field[head.x][head.y - 1] != snakeUnit)
					{
						direction = leftButton;
						newHead = { head.x, head.y - 1 };
					}
					else
					{
						gameStatus = "Game over. No way to move";
					}
				}
				else
				{
					if (field[head.x][head.y - 1] != snakeUnit)
					{
						direction = leftButton;
						newHead = { head.x, head.y - 1 };
					}
					else if (field[head.x][head.y + 1] != snakeUnit)
					{
						direction = rightButton;
						newHead = { head.x, head.y + 1 };
					}
					else if (field[head.x + 1][head.y] != snakeUnit)
					{
						direction = downButton;
						newHead = { head.x + 1, head.y };
					}
					else
					{
						gameStatus = "Game over. No way to move";
					}
				}
			}
			else if (head.x == height - 1)
			{
				if (head.y == 0)
				{
					if (field[head.x - 1][head.y] != snakeUnit)
					{
						direction = upButton;
						newHead = { head.x - 1, head.y };
					}
					else if (field[head.x][head.y + 1] != snakeUnit)
					{
						direction = rightButton;
						newHead = { head.x, head.y + 1 };
					}
					else
					{
						gameStatus = "Game over. No way to move";
					}
				}
				else if (head.y == width - 1)
				{
					if (field[head.x - 1][head.y] != snakeUnit)
					{
						direction = upButton;
						newHead = { head.x - 1, head.y };
					}
					else if (field[head.x][head.y - 1] != snakeUnit)
					{
						direction = leftButton;
						newHead = { head.x, head.y - 1 };
					}
					else
					{
						gameStatus = "Game over. No way to move";
					}
				}
				else
				{
					if (field[head.x][head.y + 1] != snakeUnit)
					{
						direction = rightButton;
						newHead = { head.x, head.y + 1 };
					}
					else if (field[head.x][head.y - 1] != snakeUnit)
					{
						direction = leftButton;
						newHead = { head.x, head.y - 1 };
					}
					else if (field[head.x + 1][head.y] != snakeUnit)
					{
						direction = downButton;
						newHead = { head.x + 1, head.y };
					}
					else
					{
						gameStatus = "Game over. No way to move";
					}
				}
			}
			else
			{
				if (head.y == 0)
				{
					if (field[head.x + 1][head.y] != snakeUnit)
					{
						direction = downButton;
						newHead = { head.x + 1, head.y};
					}
					else if (field[head.x - 1][head.y] != snakeUnit)
					{
						direction = upButton;
						newHead = { head.x - 1, head.y };
					}
					else if (field[head.x][head.y + 1] != snakeUnit)
					{
						direction = rightButton;
						newHead = { head.x, head.y + 1 };
					}
					else
					{
						gameStatus = "Game over. No way to move";
					}
				}
				else if (head.y == width - 1)
				{
					if (field[head.x - 1][head.y] != snakeUnit)
					{
						direction = upButton;
						newHead = { head.x - 1, head.y };
					}
					else if (field[head.x + 1][head.y] != snakeUnit)
					{
						direction = downButton;
						newHead = { head.x + 1, head.y };
					}
					else if (field[head.x][head.y - 1] != snakeUnit)
					{
						direction = leftButton;
						newHead = { head.x, head.y - 1 };
					}
					else
					{
						gameStatus = "Game over. No way to move";
					}
				}
			}
			return newHead;
		}
};

int main()
{
	Game game;
	game.height = fieldSize.y;
	game.width = fieldSize.x;
	game.initField();
	game.initSnake();
	game.placingSnake();
	game.plantApple();
	game.gameStatus = "run";
	game.direction = rightButton;
	game.speed = gameSpeed;
	game.level = gameLevel;
	while (game.gameStatus == "run")
	{
		system("cls");
		game.printField();
		Sleep(game.speed);
		if (_kbhit())
		{
			int button = _getch();
			if (button == 0xE0 || button == 0)//0xE0 = 244(код перед кодом стрелки)
			{
				button = _getch();
			}
			if (button == spaceButton)
			{
				cout << "space " << endl;
				game.gameStatus = "end";
			}
			else if (arrows.count(button))
			{
				game.lunch = false;
				if (game.correctButton(button))
				{
					game.direction = button;
				}
				game.moveSnakeCheckLunch();
				if (game.lunch) { game.plantApple(); };
			}
		}
		else
		{
			game.lunch = false;
			game.moveSnakeCheckLunch();
			if (game.lunch) { game.plantApple(); };
		}
		if (game.gameStatus != "run")
		{
			system("cls");
			game.printField();
			cout << game.gameStatus << endl;
		}
	}
}