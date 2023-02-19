// *********************************************************
// Course: TCP1101 PROGRAMMING FUNDAMENTALS
// Year: Trimester 1, 2022/23 (T2215)
// Lab: T14L
// Names: NUR AQILAH MARSYA BINTI ABDUL HALIM | NUR INSYIRAH BINTI ABD JALIN 
// IDs: 1211102048 | 1211103274 
// Emails: 1211102048@student.mmu.edu.my | 1211103274@student.mmu.edu.my
// Phones: 017-969 1843 | 019-773 6284
// *********************************************************

#include<iostream>
#include<vector>
#include<random>
#include<time.h>
#include<string.h>
#include<math.h>
#include <fstream>

using namespace std;

int R = 5;
int C = 5;
int zombieSize = 9;
int defZombieSize = 9;
int pZ = 0;
int rx = 0;
int ry = 0;
string status = "";
string cmd = "";
bool moveNext = true;
bool pA = false;
bool rT = false;
bool aZ = false;
bool restart = false;
bool finish = false;
bool canMove = false;


class Character
{
	private:
		char c;
		int x;
		int y;
		int life;
		int attack;
		int range;
	
	public:
		Character(char c, int x, int y, int l, int a, int r)
		{
			this->c = c;
			this->x = x;
			this->y = y;
			this->life = l;
			this->attack = a;
			this->range = r;
		}
		
		char getChar()
		{
			return c;
		}
		
		int getX()
		{
			return x;
		}
		
		int getY()
		{
			return y;
		}
		
		int getLife()
		{
			return life;
		}
		
		int getAttack()
		{
			return attack;
		}
		
		int getRange()
		{
			return range;
		}
		
		bool checkNext(vector<vector<char>> b, int d, int e)
		{
			char ob = b[x+d][y+e];
			
			if(c != 'A')
			{
				
				if(ob == '1' || ob == '2' || ob == '3' || ob == '4' || ob == '5' || ob == '6' || ob == '7' || ob == '8' || ob == '9' || ob == 'A' || ob == 'r')
				{
					canMove = false;
					return false;
				}
				canMove = true;
				return true;
			}
			
			if(ob == 'h')
			{
				life += 20;
				if(life > 100)
				{
					life = 100;
				}
				status = "Found health, adds 20 life to Alien";
				return true;
			}
			else if(ob == 'p')
			{
				pA = true;
			}
			else if(ob == '^')
			{
				attack += 20;
				status = "Found up arrow, adds 20 attacks to Alien, redirecting to up!";
				cmd = "up";
				return true;
			}
			else if(ob == 'v')
			{
				attack += 20;
				status = "Found down arrow, adds 20 attacks to Alien, redirecting to down!";
				cmd = "down";
				return true;
			}
			else if(ob == '>')
			{
				attack += 20;
				status = "Found right arrow, adds 20 attacks to Alien, redirecting to right!";
				cmd = "right";
				return true;
			}
			else if(ob == '<')
			{
				attack += 20;
				status = "Found left arrow, adds 20 attacks to Alien, redirecting to left!";
				cmd = "left";
				return true;
			}
			else if(ob == 'r')
			{
				attack += 20;
				status = "Found rock, exposing hidden thing below it and end turn.";
				rT = true;
				rx = x+d;
				ry = y+e;
				status += " Cannot move pass through rock!";
				moveNext = false;
				return false;
			}
			else if(ob == '1' || ob == '2' || ob == '3' || ob == '4' || ob == '5' || ob == '6' || ob == '7' || ob == '8' || ob == '9')
			{
//				status = "Found zombie, attacking zombie with Alien's current attack.";
				aZ = true;
				rx = x+d;
				ry = y+e;
				return false;
			}
			return true;
		}
		
		bool moveRight(vector<vector<char>> b)
		{
			if(y + 1 < C)
			{
				if(checkNext(b, 0, 1))
				{
					y++;
					return true;
				}
			}
			else
			{
				//status = "Cannot move outside of the boundary!";
				moveNext = false;
				return false;
			}
		}
		
		bool moveLeft(vector<vector<char>> b)
		{
			if(y - 1 >= 0)
			{
				if(checkNext(b, 0, -1))
				{
					y--;
					return true;
				}
			}
			else
			{
				//status = "Cannot move outside of the boundary!";
				moveNext = false;
				return false;
			}
		}
		
		bool moveUp(vector<vector<char>> b)
		{
			if(x - 1 >= 0)
			{
				if(checkNext(b, -1, 0))
				{
					x--;
					return true;
				}
			}
			else
			{
				//status = "Cannot move outside of the boundary!";
				moveNext = false;
				return false;
			}
		}
		
		bool moveDown(vector<vector<char>> b)
		{
			if(x + 1 < R)
			{
				if(checkNext(b, 1, 0))
				{
					x++;
					return true;
				}
			}
			else
			{
				//status = "Cannot move outside of the boundary!";
				moveNext = false;
				return false;
			}
		}
		
		void podAttacked()
		{
			life -= 10;
		}
		
		void attacked(int dmg)
		{
			life -= dmg;
		}
};

class Object
{
	private:
		char c;
		int p;
	
	public:
		Object(char c, int p)
		{
			this->c = c;
			this->p = p;
		}
		
		char getChar()
		{
			if(c == '^')
			{
				int z = rand() % 4;
				
				if(z == 1)
					return 'v';
				else if(z == 2)
					return '<';
				else if(z == 3)
					return '>';
			}
			return c; 
		}
};


class Board
{
	private:
		vector<vector<char>> board;
	
	public:
		Board()
		{
			board = vector<vector<char>>(R, vector<char>(C, '.'));
		}
		
		vector<vector<char>> getBoard()
		{
			return board;
		}
		
		void setBoard(vector<vector<char>> b)
		{
			this->board = b;
		}
		
		void display()
		{
			cout << "      ALIEN VS ZOMBIE" << endl << endl;
			cout << "  +";
			for(int j = 0; j < C; j++)
			{
				cout << "---";
			}
			for(int j = 0; j < C-1; j++)
			{
				cout << "-";
			}
			cout << "+";
			for(int i = 0; i < R; i++)
			{
				if(i < 9)
					cout << endl << (i+1) << " | ";
				else
					cout << endl << (i+1) << "| ";
				for(int j = 0; j < C; j++)
				{
					cout << board[i][j] << " | ";
				}
				cout << endl;
				
				cout << "  +";
				for(int j = 0; j < C; j++)
				{
					cout << "---";
				}
				for(int j = 0; j < C-1; j++)
				{
					cout << "-";
				}
				
				cout << "+";	
			}
			cout << endl << "   ";
			for(int j = 0; j < C; j++)
			{
				if(j < 9)
					cout << " " << j+1 << "  ";
				else
					cout << " " << 1 << "  ";
			}
			
			if(C > 9)
			{
				cout << endl << "\t\t\t\t       ";
				for(int j = 10; j <= C; j++)
				{
					cout << " " << j%10 << "  ";
				}
			}
		}
		
		void update(int i, int j, char c)
		{
			board[i][j] = c;
		}
		
		void update(Character c)
		{
			board[c.getX()][c.getY()] = c.getChar();
		}
		
		void update(Object c, int i, int j)
		{
			board[i][j] = c.getChar();
		}
		
		char getChar(int x, int y)
		{
			return board[x][y];
		}
		
		void fill()
		{
			vector<Object> t;
			t.push_back(Object('^', 0));
			t.push_back(Object('h', 20));
			t.push_back(Object('p', 10));
			t.push_back(Object('r', 0));
			t.push_back(Object(' ', 0));
			for(int i = 0; i < R; i++)
			{
				for(int j = 0; j < C; j++)
				{
					if(board[i][j] == '.')
					{
						int z = rand() % t.size();
						board[i][j] = t[z].getChar();
					}
				}
			}
		}
		
		bool canMove(char cc)
		{
			if(cc == ' ' || cc == '.' || cc == '^' || cc == 'v' || cc == '<' || cc == '>' || cc == 'p' || cc == 'h')
			{
				cout << endl << "Can move!" << endl;
				return true;
			}
			cout << endl << "Cannot move!" << endl;
			return false;
		}
		
		bool checkRight(Character c)
		{
			if(c.getY()+1 > C-1)
				return false;
			char cc = board[c.getX()][c.getY()+1];
			
			if(canMove(cc))
			{
				cout << "Zombie " << c.getChar() << " moves right." << endl;
				return true;
			}
			
			return false;
		}
		
		bool checkLeft(Character c)
		{
			if(c.getY()-1 < 1)
				return false;
			char cc = board[c.getX()][c.getY()-1];
			
			if(canMove(cc))
			{
				cout << "Zombie " <<  c.getChar() << " moves left." << endl;
				return true;
			}
			
			return false;
			
		}
		
		bool checkUp(Character c)
		{
			if(c.getX()-1 < 0)
				return false;
			char cc = board[c.getX()-1][c.getY()];
			
			if(canMove(cc))
			{
				cout << "Zombie " <<  c.getChar() << " moves up." << endl;
				return true;
			}
			
			return false;
			
		}
		
		bool checkDown(Character c)
		{
			if(c.getX()+1 > R-1)
				return false;
			char cc = board[c.getX()+1][c.getY()];
			
			if(canMove(cc))
			{
				cout << "Zombie " <<  c.getChar() << " moves down." << endl;
				return true;
			}
			
			return false;
			
		}
};

char nearest(Character c, vector<Character> z)
{
	char cc;
	double min = 100;
	
	for(auto zz : z)
	{
		double a = (double)c.getX();
		double b = (double)c.getY();
		double d = (double)zz.getX();
		double f = (double)zz.getY();
		
		double dist = sqrt(pow(a-d, 2) + pow(b-f, 2));
		
		if(min > dist)
		{
			min = dist;
			cc = zz.getChar();
		}
	}
	
	return cc;
}

void attackAlien(Character z, Character &c)
{
	char cc = z.getChar();
	
	double a = (double)z.getX();
	double b = (double)z.getY();
	double d = (double)c.getX();
	double f = (double)c.getY();
		
	double dist = sqrt(pow(a-d, 2) + pow(b-f, 2));
	
	if(z.getRange() >= dist)
	{
		c.attacked(z.getAttack());
		status += " Alien is in range of Zombie ";
		status += cc;
		status += " and being attacked with ";
		status += to_string(z.getAttack());
		status += " damage.";
	}
	else
	{
		status += " Alien is not in range of Zombie ";
		status += cc;
		status += " and cannot be attacked.";
	}
	
	cout << endl << "Status : " << status << endl;
}

void setting()
{
	zombieSize = defZombieSize;
	cout << endl << endl << "Default Setting :-";
	cout << endl << endl << "\tNumber of ROWS (odd number) : " << R;
	cout << endl << "\tNumber of COLUMNS (odd number) : " << C;
	cout << endl << "\tNumber of ZOMBIES (min = 1, max = 9) : " << zombieSize;
	
	cout << endl << endl << endl << "Do you want to keep the setting? (y/n) : ";
	char ans;
	cin >> ans;
	
	if(ans != 'y')
	{
		cout << "\n\nEnter the number of ROWS (odd number) : ";
		cin >> R;
		cout << "Enter the number of COLUMNS (odd number) : ";
		cin >> C;
		cout << "Enter the number of ZOMBIES (min = 1, max = 9) : ";
		cin >> zombieSize;
		defZombieSize = zombieSize;
	}
	
	
	cout << "\n\n\nENJOY THE GAME!\n\n\n";
	system("pause");
	system("cls");
}

void save(Character c, vector<Character> z, vector<vector<char>> b)
{
	cout << "\n\n\nEnter the file name that you want to save into : ";
	string fname;
	getline(cin, fname);
	fname += ".txt";
	
	ofstream f(fname);
	
	f << R << endl;
	f << C << endl;
	f << zombieSize << endl;
	
	for(int i = 0; i < R; i++)
	{
		for(int j = 0; j < C; j++)
		{
			if(b[i][j] != ' ')
				f << b[i][j] << endl;
			else
				f << '-' << endl;
		}
	}
	
	f << c.getChar() << endl;
	f << c.getX() << endl;
	f << c.getY() << endl;
	f << c.getLife() << endl;
	f << c.getAttack() << endl;
	f << c.getRange() << endl;
	
	for(int i = 0; i < zombieSize; i++)
	{
		f << z[i].getChar() << endl;
		f << z[i].getX() << endl;
		f << z[i].getY() << endl;
		f << z[i].getLife() << endl;
		f << z[i].getAttack() << endl;
		f << z[i].getRange() << endl;
	}
	
	f.close();
	
	cout << "\n\nGame saved in file : " << fname << " sucessfully!\n\n";
}

void load(Character &c, vector<Character> &z, Board &bb)
{
	cout << "\n\n\nEnter the file name that you want to load from (without .txt) : ";
	string fname;
	getline(cin, fname);
	fname += ".txt";
	
	ifstream f(fname);
	string line;
	f >> line;
	R = stoi(line);
	f >> line;
	C = stoi(line);
	f >> line;
	zombieSize = stoi(line);
	
	vector<vector<char>> b = vector<vector<char>>(R, vector<char>(C, '.'));
	
	for(int i = 0; i < R; i++)
	{
		for(int j = 0; j < C; j++)
		{
			f >> line;
			
			if(line[0] != '-')
				b[i][j] = line[0];
			else
				b[i][j] = ' ';
		}
	}
	
	bb.setBoard(b);
	
	f >> line;
	char c1 = line[0];
	f >> line;
	int c2 = stoi(line);
	f >> line;
	int c3 = stoi(line);
	f >> line;
	int c4 = stoi(line);
	f >> line;
	int c5 = stoi(line);
	f >> line;
	int c6 = stoi(line);
	
	c = Character(c1, c2, c3, c4, c5, c6);
	
	z.clear();
	
	for(int i = 0; i < zombieSize; i++)
	{
		f >> line;
		char z1 = line[0];
		f >> line;
		int z2 = stoi(line);
		f >> line;
		int z3 = stoi(line);
		f >> line;
		int z4 = stoi(line);
		f >> line;
		int z5 = stoi(line);
		f >> line;
		int z6 = stoi(line);
		
		z.push_back(Character(z1, z2, z3, z4, z5, z6));
	}
	
	f.close();
	
	cout << "\n\nGame loaded from file : " << fname << " sucessfully!\n\n";
}

void quit()
{
	system("cls");
	cout << "\n\n\nThanks for playing Alien Path! See you again. Goodbye!\n\n\n";
	system("pause");
	finish = true;
}

int main()
{
	while(!finish)
	{
		cout << "Welcome to Alien Path!";
		
		setting();
		
		Board b;
		Character a = Character('A', R/2, C/2, 100, 0, 0);
		b.update(a);
		vector<Character> z;
		srand(time(NULL));
		cin.ignore();
		
		
		for(int i = 1; i <= zombieSize; i++)
		{
			int x, y;
			while(true)
			{
				x = rand() % R;
				y = rand() % C;
				
				if(b.getChar(x, y) == '.')
					break;
			}
			int h = ((rand() % 15) + 5) * 10;
			int att = ((rand() % 20) + 10);
			int r = ((rand() % 3) + 1);
			Character zz = Character(((char)i)+48, x, y, h, att, r);
			z.push_back(zz);
		}
		
		for(auto zz : z)
		{
			b.update(zz);
		}
		
		bool turnA = true;
		int turn[zombieSize] = {0};
		bool finishTurn;
		
		while(true)
		{
			finishTurn = false;
			b.fill();
			
			cmd = "";
				
			while(!finishTurn)
			{
				if(finish)
					break; 
					
				b.display();
				
				cout << endl << endl << "Turn :-\n";
				if(turnA)
				{
					cout << "-> Alien\tLife : " << a.getLife() << ",\tAttack : " << a.getAttack() << "\t\t  (" << a.getX()+1 << ", " << a.getY()+1 << ")" << endl;
				}
				else
				{
					cout << "   Alien\tLife : " << a.getLife() << ",\tAttack : " << a.getAttack() << "\t\t  (" << a.getX()+1 << ", " << a.getY()+1 << ")" << endl;
				}
				for(int i = 0; i < zombieSize; i++)
				{
					if(turn[i] == 1)
					{
						cout << "->";
					}
					else
					{
						cout << "  ";
					}
					cout << " Zombie " << z[i].getChar() << "\tLife : " << z[i].getLife() << ",\tAttack : " << z[i].getAttack() << ",\tRange : " << z[i].getRange() <<  " (" << z[i].getX()+1 << ", " << z[i].getY()+1 << ")" << endl;
				}
				
				if(rT)
				{
					char ob[7] = {'^', 'v', '<', '>', ' ', 'p', 'h'};
					int zz =  rand() % 7;
					b.update(rx, ry, ob[zz]);
					status = "Found rock, revealing things under it and ending turn.";
					
					rx = 0;
					ry = 0;
					rT = false;
				}
				
				
				
				if(z.size() < 1)
				{
					cout << endl << endl << "CONGRATULATIONS!! ALL ZOMBIES ARE DEAD, ALIEN WINS!";
					restart = true;
					break;
				}
							
				if(a.getLife() <= 0)
				{
					cout << "\n\nAlien dies, you lose!";
					restart = true;
					break;
				}
				
				
				if(aZ)
				{
					bool die = false;
					char ob = b.getChar(rx, ry);
					for(auto& zz : z)
					{
						char cc = zz.getChar();
						
						if(cc == ob)
						{
							zz.attacked(a.getAttack());
							if(a.getAttack() > zz.getLife())
							{
								turnA = true;
								moveNext = true;
								die = true;
							}
							break;
						}
					}
					
					if(die)
					{
						for(vector<Character>::iterator it = z.begin(); it != z.end(); it++)
						{
							Character &c = *it;
							char cc = c.getChar();
							if(cc == ob)
							{
								b.update(c.getX(), c.getY(), ' ');
								z.erase(it);
								zombieSize--;
								break;
							}
						}
						moveNext = true;
					}
					else
					{
						status = "Found Zombie ";
						status += ob;
						status += " and attacking it with ";
						status += to_string(a.getAttack());
						status += " attack. Zombie did not die, end turn.";
						moveNext = false;
					}
					
					aZ = false;
				}
				
				if(status != "")
				{
					cout << endl << endl << "Status : " << status << endl;
					status = "";
				}
					
				int x = a.getX();
				int y = a.getY();
				
				
				if(turnA)
				{
					if(cmd == "")
					{
						cout << endl << endl << "Command : ";
					
						getline(cin, cmd);
					}
					
					if(cmd == "right")
					{
						if(a.moveRight(b.getBoard()))
						{
							b.update(x, y, '.');
							b.update(a);
						}
					}
					else if(cmd == "left")
					{
						if(a.moveLeft(b.getBoard()))
						{
							b.update(x, y, '.');
							b.update(a);
						}
					}
					else if(cmd == "up")
					{
						if(a.moveUp(b.getBoard()))
						{
							b.update(x, y, '.');
							b.update(a);
						}
					}
					else if(cmd == "down")
					{
						if(a.moveDown(b.getBoard()))
						{
							b.update(x, y, '.');
							b.update(a);
						}
					}
					else if(cmd == "quit")
					{
						quit();
						finish = true;
						break;
					}
					else if(cmd == "arrow")
					{
						cout << endl << "Arrow > ";
						string arrow;
						getline(cin, arrow);
						
						if(arrow.size() < 5)
						{
							cout << endl << "Invalid command, try again.";
							cmd = "";
						}
						else
						{
							int ax = int(char(arrow[0])-48);
							int ay = int(char(arrow[2])-48);
							char ac = arrow[4];
							
							b.update(ax-1, ay-1, ac);
						}
						cmd = "";
					}
					else if(cmd == "save")
					{
						cout << "\n\nAre you sure you want to save the game? (y/n) : ";
						char ans;
						cin >> ans;
						cin.ignore();
						
						if(ans == 'y')
						{
							save(a, z, b.getBoard());
						}
						
						cmd = "";
					}
					else if(cmd == "load")
					{
						cout << "\n\nAre you sure you want to load a game? (y/n) : ";
						char ans;
						cin >> ans;
						cin.ignore();
						
						if(ans == 'y')
						{
							load(a, z, b);
						}
						
						cmd = "";
					}
					else
					{
						cout << endl << "Invalid command, try again.";
						cmd = "";
					}
					
					if(pA)
					{
						pZ = nearest(a, z);
						
						for(auto& zz : z)
						{
							char cc = zz.getChar();
							
							if(cc == pZ)
							{
								zz.podAttacked();
								status = "Found attack pod, attacking the nearest zombie : Zombie ";
								status += cc;
								status += " with 10 damage.";
								break;
							}
						}
						
						pZ = 0;
						pA = false;
					}
				}
				else
				{
					for(int i = 0; i < zombieSize; i++)
					{
						if(turn[i] == 1)
						{
							for(auto& zz : z)
							{
								x = zz.getX();
								y = zz.getY();
								char cc = (char)zz.getChar();
								char tc = (char)i+49;
								if(cc == tc)
								{
									cout << endl;
									for(int i = 0; i < 4; i++)
									{
										int rd = rand() % 4;
										
										if(rd == 0)
										{
											if(zz.moveUp(b.getBoard()))
											{
												if(canMove)
												{
													cout << "Zombie " << cc << " moves up." << endl;
													b.update(x, y, ' ');
													b.update(zz);
													break;
												}
											}
										} 
										else if(rd == 1)
										{
											if(zz.moveDown(b.getBoard()))
											{
												if(canMove)
												{
													cout << "Zombie " << cc << " moves down." << endl;
													b.update(x, y, ' ');
													b.update(zz);
													break;
												}
											}
										} 
										else if(rd == 2)
										{
											if(zz.moveRight(b.getBoard()))
											{
												if(canMove)
												{
													cout << "Zombie " << cc << " moves right." << endl;
													b.update(x, y, ' ');
													b.update(zz);
													break;
												}
											}
										} 
										else
										{
											if(zz.moveLeft(b.getBoard()))
											{
												if(canMove)
												{
													cout << "Zombie " << cc << " moves left." << endl;
													b.update(x, y, ' ');
													b.update(zz);
													break;
												}
											}
										}
									}
										
									attackAlien(zz, a);
									
									moveNext = false;
									break;
								}
							}
							moveNext = false;
							break;
						}
					}
				}
				
				
				if(!moveNext)
				{
					if(turnA != 1)
					{
						for(int i = 0; i < zombieSize; i++)
						{
							if(turn[i] == 1)
							{
								turn[i] = 0;
								
								if(i < zombieSize-1)
								{
									turn[i+1] = 1;
								}
								else
								{
									turnA = true;
								}
								break;
							}
						}
					}
					else
					{
						turnA = false;
						turn[0] = 1;
					}
						
					finishTurn = true;
					moveNext = true;
					
					cout << endl << "Cannot move anymore, turn finished." << endl;
				}
				
				system("pause");
				system("cls");
			}
			status = "";
						
			if(restart)
			{
				cout << "\n\nDo you wish to play again? (y/n) : ";
				char ans;
				cin >> ans;
				
				if(ans == 'n')
				{
					quit();
					finish = true;
					break;
				}
				
				restart = false;
				system("cls");
				break;
			}
			
			if(finish)
			{
				restart = false;
				system("cls");
				break;
			}
		}
	}
}
