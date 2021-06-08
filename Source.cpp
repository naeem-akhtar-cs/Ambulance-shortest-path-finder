#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <conio.h>
#include <queue>
#include <math.h>
#include <list>

using namespace std;

class ambulance;
class routePlanner;
class Hospital;
class point;
class tree;

class myList {
private:
	int size;
	point** list;

public:
	myList() {
		list = new point * [100000];
		size = 0;
	}

	void insert(point* p) {
		list[size] = p;
		size++;
		/*cout <<size;
		cout <<"Node inserted"<<endl;*/
	}

	bool check(point* p) {
		for (int i = 0; i < size; i++) {
			if (p == list[i])
				return true;
		}
		return false;
	}
};

class node {
private:
	node* parent;
	point* mapPosition;
	float g; //g for saving travelled distance by any node
	float f; //f to set priority in the queue

public:
	node(float f,int g,node* parent, point* mapPosition) {
		this->parent = parent;
		this->mapPosition = mapPosition;
		this->g = g;
		this->f = f;
	}


	node* getParent() {
		return parent;
	}

	point* getMapPos()
	{
		return mapPosition;
	}

	float getG() {
		return g;
	}

	float getF() {
		return f;
	}
};

class point {
private:
	int xCordinate;
	int yCordinate;

	char data; //WHether this position in map has road/hurdle etc

public:
	point(int,int,char);

	void updatexU();
	void updatexD();

	void updateyL();
	void updateyR();

	int getX();
	int getY();
	char getData();
	void setData(char);

	void print();
	void removePath();
};

void point::removePath()
{
	if (data == '-')
		data = ' ';
}

void point::setData(char data) {
	this->data = data;
}

char point::getData() {
	return data;
}

void point::print() {
	cout << this->data;
}

point::point(int x = NULL, int y = NULL,char data=NULL) {
	xCordinate = x;
	yCordinate = y;

	this->data = data;
}

int point::getX() {
	return xCordinate;
}

int point::getY() {
	return yCordinate;
}

void point::updatexU()
{
	xCordinate = xCordinate - 1;
}

void point::updatexD()
{
	xCordinate = xCordinate + 1;
}

void point::updateyL()
{
	yCordinate = yCordinate - 1;
}

void point::updateyR()
{
	yCordinate = yCordinate + 1;
}

class Hospital
{
private:
	point* hospitalPos;

public:
	Hospital(point*);
	point* getPos();
	void print();
};

void Hospital::print()
{
	cout << "Hello!! I am Hospital and my Position on the map is ", hospitalPos->print();
}

Hospital::Hospital(point* Pos) {
	hospitalPos = Pos;
}

point* Hospital::getPos()
{
	return hospitalPos;
}

class ambulance
{
private:
	point* ambulancePos;

public:
	ambulance(point*);
	point* getPos();
	void print();

	void upDatePosition(point*);

};

void ambulance::upDatePosition(point* pos)
{
	this->ambulancePos = pos;
}

void ambulance::print()
{
	cout << "Hello!! I am Ambulance and my Position on the map is ",ambulancePos->print();
}

ambulance::ambulance(point* pos) {
	ambulancePos = pos;
}

point* ambulance::getPos()
{
	return ambulancePos;
}

class routePlanner {
private:
	int rows;
	int cols;
	point*** map;

	ambulance* amb;
	Hospital* hos;
public:
	routePlanner();
	~routePlanner();

	void readFile();
	void display();
	void drive();
	void menu();
	void findPath();
	void printPath(node*);
	void removePath();
};

routePlanner::routePlanner()
{
	map = NULL;
	rows = NULL;
	cols = NULL;
	amb = NULL;
	hos = NULL;
}

void routePlanner::display() {

	system("CLS");

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			map[i][j]->print();
		}
		cout << "\n";
	}
}

routePlanner::~routePlanner()
{

}

void routePlanner::readFile() {

	ifstream fin;

	fin.open("map.txt");

	if (fin) {

		fin >> rows;
		fin >> cols;

		map = new point** [rows];

		for (int i = 0; i < rows; i++) {
			map[i] = new point*[cols];
		}

		//Reading the map of the city with roads Infrastructure

		char val;

		for (int i = 0; i < rows; i++)
		{
			fin.get(val);
			for (int j = 0; j < cols; j++)
			{
				fin.get(val);

				map[i][j] = new point(i, j, val);

				if (val == 'A')
				{
					//Saving Ambulance Co-Ordinates
					amb = new ambulance(map[i][j]);
				}
				else if (val == 'H') {
					hos = new Hospital(map[i][j]);
				}
			}
		}
	}
	else
		cout << "SORRY!! File could not be opened\n\n";

	fin.close();
}

void routePlanner::printPath(node* pointFound) {

	while (pointFound->getParent() != NULL) {
		pointFound = pointFound->getParent();
		pointFound->getMapPos()->setData('-');
	}
}

void routePlanner::removePath() {
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			map[i][j]->removePath();
		}
	}
}

void routePlanner::drive() {

	char input;
	bool updateCheck=true;

	while (1) {

		//removePath();
		display();
		//findPath();
		//display();

		/*char test;
		cin >> test;
		cout << test;*/


		input= _getch();
		updateCheck = false;
		if (input == 'q') {
			menu();
		}
		else {
			if (input == 'w') {

				point* pos = amb->getPos();

				int x = pos->getX();
				int y = pos->getY();

				if (map[x-1][y]->getData() != '1') {
					pos->updatexU();

					amb->upDatePosition(map[x - 1][y]);

					map[x][y]->setData(' ');
					map[x-1][y]->setData('A');

					updateCheck = true;
				}
			}
			else if (input == 's') {
				point* pos = amb->getPos();

				int x = pos->getX();
				int y = pos->getY();

				if (map[x+1][y]->getData() != '1') {
					pos->updatexD();

					amb->upDatePosition(map[x + 1][y]);

					map[x][y]->setData(' ');
					map[x+1][y]->setData('A');

					updateCheck = true;
				}
			}
			else if (input == 'a') {
				point* pos = amb->getPos();

				int x = pos->getX();
				int y = pos->getY();

				if (map[x][y-1]->getData() != '1') {
					pos->updateyL();
					
					amb->upDatePosition(map[x][y - 1]);

					map[x][y]->setData(' ');
					map[x][y-1]->setData('A');

					updateCheck = true;
				}
			}
			else if (input == 'd') {
				point* pos = amb->getPos();

				int x = pos->getX();
				int y = pos->getY();

				if (map[x][y+1]->getData() != '1') {
					pos->updateyR();

					amb->upDatePosition(map[x][y + 1]);

					map[x][y]->setData(' ');
					map[x][y+1]->setData('A');

					updateCheck = true;
				}
			}
		}
	}

}

void routePlanner::menu() {
	int input;

	cout << "\n\n1- Drive the Ambulance\n2- Find Shortest Path To Hospital\nInput: ";
	cin >> input;

	if (input == 1) {
		drive();
	}
	else if (input == 2) {
		findPath();
		display();
	}
}

void routePlanner::findPath() {
	point* sPosition = amb->getPos();
	point* gPosition = hos->getPos();

	int ax = sPosition->getX(); //Ambulance Position
	int ay = sPosition->getY();

	int hx = gPosition->getX(); //Hospital Position
	int hy = gPosition->getY();

	priority_queue <node*>openList;

	float f = (sqrt(pow(hx, 2) + (pow(hy, 2))))+0; //f at the starting point

	node* parentNode = new node(f,0,NULL, sPosition);

	openList.push(parentNode);

	float g;
	float h;

	myList closedList;
	node* currNode=NULL;

	int count = 0;

	while (openList.empty() == false) {

		currNode = openList.top();
		openList.pop();

		if (currNode->getMapPos()->getData() == 'H')
			break;

		int x = currNode->getMapPos()->getX();
		int y = currNode->getMapPos()->getY();
		g = currNode->getG();

		g++;

		if ((map[x][y + 1]->getData() != '1') && (closedList.check(map[x][y + 1]) == false)) {
			h = sqrt(pow(hx - x, 2) + (pow(hy - (y + 1), 2)));
			node* newNode = new node((g + h), g, currNode, map[x][y + 1]);
			openList.push(newNode);
		}

		if ((map[x][y - 1]->getData() != '1') && (closedList.check(map[x][y - 1]) == false)) {
			h = sqrt(pow(hx - x, 2) + (pow(hy - (y - 1), 2)));
			node* newNode = new node((g+h),g, currNode, map[x][y - 1]);
			openList.push(newNode);
		}

		if ((map[x + 1][y]->getData() != '1') && (closedList.check(map[x+1][y]) == false)) {
			h = sqrt(pow(hx - (x+1), 2) + (pow(hy - y, 2)));
			node* newNode = new node((g+h),g, currNode, map[x + 1][y]);
			openList.push(newNode);
		}

		if ((map[x - 1][y]->getData() != '1') && (closedList.check(map[x-1][y]) == false)) {
			h = sqrt(pow(hx - (x - 1), 2) + (pow(hy - y, 2)));
			node* newNode = new node((g+h),g, currNode, map[x - 1][y]);
			openList.push(newNode);
		}

		closedList.insert(map[x][y]);
		count++;
	}

	if (currNode != NULL) {
		printPath(currNode);
	}
	else
		cout << "Leaf Node to print path is NULL";
}

int main() {

	routePlanner* ambulanceSystem=new routePlanner();

	ambulanceSystem->readFile();
	ambulanceSystem->display();

	ambulanceSystem->menu();
    
	return 0;
}