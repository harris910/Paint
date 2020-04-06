#include <iostream>
using namespace std;
#include "graphics.h"
#include<math.h>
#include<fstream>

void flood_fill(int X, int Y,int old, int C)
{
	if (X > 0 && Y > 0 && getpixel(X, Y) == old)
	{
		putpixel(X, Y, C);
		flood_fill(X + 1, Y, old, C);
		flood_fill(X, Y + 1, old, C);
		flood_fill(X, Y - 1, old, C);
		flood_fill(X - 1, Y, old, C);
	}


}

struct point
{
	int x;
	int y;
};
class shape
{
protected:
	point *points;
	int colour;
	int numberOfPoints;
public:
	shape()
	{

	}
	virtual void draw()
	{

	}
	virtual bool contains(point p) = 0;

	shape(point *p, int c, int nop)
	{
		points = new point[nop];
		for (int i = 0; i < nop; i++)
			points[i] = p[i];
		colour = c;
		numberOfPoints = nop;
	}
	virtual void fill(int X, int Y, int colour)
	{

	}

};
class openShape:public shape
{
protected:
	bool style;
public:
	openShape(point *p, int c, int nop, bool s) :shape(p, c, nop), style(s)
	{

	}
	virtual void changeColour(int colour) = 0;
};
class polygon :public shape
{
protected:
	int fillColour;
public:
	polygon(point *p, int c, int nop, int fc) :shape(p, c, nop), fillColour(fc)
	{

	}
	virtual void draw()
	{
		setcolor(colour);
		int *P;
		P= new int[numberOfPoints*2+2];
		int j = 0;
		for (int i = 0; i < numberOfPoints; i++)
		{
			P[j] = points[i].x;
			j++;
			P[j] = points[i].y;
			j++;
		}
		P[j] = points[0].x;
		j++;
		P[j] = points[0].y;
		setfillstyle(1,fillColour);
		
		int q;
		for (q = 1; q < numberOfPoints; q++)
		{
			if (q==2)
				line(points[numberOfPoints-1].x, points[numberOfPoints-1].y, points[0].x, points[0].y);
			line(points[q - 1].x, points[q - 1].y, points[q].x, points[q].y);
		}
		
	}
	bool contains(point p)
	{
		int i, j;
		bool c = false;
		for (i = 0, j = numberOfPoints - 1; i < numberOfPoints; j = i++) {
			if ((((points[i].y <= p.y) && (p.y < points[j].y)) ||((points[j].y <= p.y) && (p.y < points[i].y))) &&
				(p.x < (points[j].x - points[i].x) * (p.y - points[i].y) / (points[j].y - points[i].y) + points[i].x))
				c = !c;
		}
		return c;
	}
	void fill(int X,int Y,int colour)
	{
		int old = getpixel(X, Y);
		flood_fill(X, Y,old, colour);
	}
	void erase()
	{
		setcolor(15);
		int *P;
		P = new int[numberOfPoints * 2 + 2];
		int j = 0;
		for (int i = 0; i < numberOfPoints; i++)
		{
			P[j] = points[i].x;
			j++;
			P[j] = points[i].y;
			j++;
		}
		P[j] = points[0].x;
		j++;
		P[j] = points[0].y;
		setfillstyle(1, fillColour);

		int q;
		for (q = 1; q < numberOfPoints; q++)
		{
			if (q == 2)
				line(points[numberOfPoints - 1].x, points[numberOfPoints - 1].y, points[0].x, points[0].y);
			line(points[q - 1].x, points[q - 1].y, points[q].x, points[q].y);
		}
		setcolor(colour);
	}
	
};
class Circle :public shape
{
protected:
	int fillColour;
	double radius;
public:
	Circle(point *p, int c, int nop, int fc, double rad) :shape(p, c, nop), fillColour(fc), radius(rad)
	{

	}
	void draw()
	{
		cout << colour << endl;
		setcolor(colour);
		points[0].x = points[1].x + points[0].x;
		points[0].x /= 2;
		points[0].y = points[1].y + points[0].y;
		points[0].y /= 2;
		circle(points[0].x, points[0].y, radius);
	}
	bool contains(point p)
	{
		if ((p.x - points[0].x) * (p.x - points[0].x) +
			(p.y - points[0].y) * (p.y - points[0].y) <= radius * radius)
			return true;
		else
			return false;
	}
	void fill(int X,int Y,int colour)
	{
		int old = getpixel(X, Y);
		flood_fill(X, Y,old, colour);
	}

};
class Line :public openShape
{
public:
	Line(point *p, int c, int nop, bool s) :openShape( p,  c,  nop, s)
	{

	}
	void draw()
	{	
		setlinestyle(style, 1, 3);
		setcolor(colour);
		line(points[0].x, points[0].y, points[1].x, points[1].y);
		setlinestyle(style, 1, 1);
	}
	void changeColour(int num)
	{
		setcolor(num);
	}
	bool contains(point p)
	{
		float a = (points[1].y - points[0].y) / (points[1].x - points[0].x);
		float b = points[0].y - a * points[0].x;
		if (fabs(p.y - (a*p.x + b)) < 0.000000000000001)
		{
			return true;
		}

		return false;
	}
};
class curve :public openShape
{
public:
	curve(point *p, int c, int nop, bool s) :openShape(p, c, nop, s)
	{

	}
	void draw()
	{
		setcolor(colour);
		int radius;
		point P;
		P.x = (points[1].x + points[0].x) / 2;
		P.y = (points[1].y + points[0].y) / 2;
		radius = (points[1].x - points[0].x)*(points[1].x - points[0].x) + (points[1].y - points[0].y)*(points[1].y - points[0].y);
		radius = sqrt(radius);
		arc(P.x, P.y, 45, 135, radius);
	}
	void changeColour(int num)
	{
		setcolor(num);
	}
	bool contains(point p)
	{
		return true;
	}

};
class Rect :public polygon
{
public:
	Rect(point *p, int c, int nop, int fc) :polygon(p,  c,  nop,  fc)
	{
	}
	void draw()
	{
			int *P;
			P = new int[4];
			int j = 0;
			for (int i = 0; i < numberOfPoints; i++)
			{
				P[j] = points[i].x;
				j++;
				P[j] = points[i].y;
				j++;
			}
			P[j] = points[0].x;
			j++;
			P[j] = points[0].y;
			setcolor(colour);
			line(points[0].x, points[0].y, points[1].x, points[0].y);
			line(points[0].x, points[0].y, points[0].x, points[1].y);
			line(points[1].x, points[0].y, points[1].x, points[1].y);
			line(points[0].x, points[1].y, points[1].x, points[1].y);
	}
	void fill(int X,int Y,int colour)
	{
		int old = getpixel(X, Y);
		flood_fill(X, Y,old, colour);
	}
	bool contains(point p)
	{
		return true;
	}

};
class Text :public shape
{
public:
	Text(point *p, int c, int nop) :shape(p, c, nop)
	{

	}
	void draw()
	{
		setcolor(colour);
		char *ch=new char[2];
		ch[1] = '\0';
		settextstyle(8, HORIZ_DIR, 1);

		int temp = points[0].x;
		while (ch[0]!='#')
		{
			ch[0] = getch();
			outtextxy(points[0].x, points[0].y, ch);
			points[0].x = points[0].x + textwidth(ch);
			
			if (ch[0] == 13)
			{
				points[0].x = temp;
				points[0].y = points[0].y + textheight(ch);
			}
			else if (points[0].x > (getwindowwidth() - 30))
			{
				points[0].x = temp;
				points[0].y = points[0].y + textheight(ch);
			}
		}
	}
	void fill(int colour)
	{

	}
	bool contains(point p)
	{
		return true;
	}
};

void extendBy1(shape **&temp, int &size)
{
	if (temp == nullptr)
	{
		temp = new shape*[1];
		size = 1;
	}
	else
	{
		shape ** hold = new shape*[size + 1];
		for (int i = 0; i < size; i++)
		{
			hold[i] = temp[i];
		}
		delete temp;
		temp = new shape*[size + 1];
		for (int i = 0; i < size; i++)
		{
			temp[i] = hold[i];
		}
		size++;
	}
}

void shrinkBy1(shape **&temp, int &size,int index)
{
	shape ** hold = new shape*[size - 1];
	int k = 0;
	for (int i = 0; i < size;i++)
	{
		if (i == index)
			i++;
		else
		{
			hold[k] = temp[i];
			k++;
		}
		delete temp;
		temp = hold;
		size--;
	}
}
void palette()
{
	setcolor(15);
	setfillstyle(SOLID_FILL, 15);
	floodfill(11, 267, 15);

	setcolor(5);//colour palete
	setfillstyle(SOLID_FILL, 5);
	rectangle(10, 265, 30, 285);
	floodfill(11, 267, 5);
	setcolor(RED);
	setfillstyle(SOLID_FILL, RED);
	rectangle(30, 265, 50, 285);
	floodfill(31, 267, 4);
	setcolor(YELLOW);
	setfillstyle(SOLID_FILL, YELLOW);
	rectangle(50, 265, 70, 285);
	floodfill(51, 267, 14);
	setcolor(BROWN);
	setfillstyle(SOLID_FILL, BROWN);
	rectangle(70, 265, 90, 285);
	floodfill(71, 267, 6);
	setcolor(BLUE);
	setfillstyle(SOLID_FILL, BLUE);
	rectangle(90, 265, 110, 285);
	floodfill(91, 267, 1);
	setcolor(GREEN);
	setfillstyle(SOLID_FILL, GREEN);
	rectangle(110, 265, 130, 285);
	floodfill(111, 267, 2);

	setcolor(BLACK);

	int *p = new int[10];
	p[0] = 195; p[1] = 270;
	p[2] = 203; p[3] = 268;
	p[4] = 200; p[5] = 280;
	p[6] = 195; p[7] = 283;
	p[8] = 195; p[9] = 270;

	char *text = new char[2];
	text[0] = 'T';
	text[1] = '\0';

	rectangle(150, 265, 170, 285);
	circle(160, 275, 5);
	rectangle(170, 265, 190, 285);
	rectangle(175, 270, 185, 279);
	rectangle(190, 265, 210, 285);
	drawpoly(5, p);
	rectangle(210, 265, 230, 285);
	line(215, 270, 225, 280);
	rectangle(230, 265, 250, 285);
	setcolor(BLACK);
	setbkcolor(15);
	outtextxy(235, 267, text);
	rectangle(250, 265, 270, 285);
	arc(260, 280, 20, 135, 7);

	text[0] = 'S';
	rectangle(300, 265, 320, 285);
	setcolor(BLACK);
	setbkcolor(15);
	outtextxy(305, 267, text);

	text[0] = 'L';
	rectangle(320, 265, 340, 285);
	setcolor(BLACK);
	setbkcolor(15);
	outtextxy(325, 267, text);

	text[0] = 'F';
	rectangle(340, 265, 360, 285);
	setcolor(BLACK);
	setbkcolor(15);
	outtextxy(345, 267, text);

	text[0] = 'E';
	rectangle(360, 265, 380, 285);
	setcolor(BLACK);
	setbkcolor(15);
	outtextxy(365, 267, text);
}


int main()
{
	initwindow(450, 300, "First Sample");
	settextstyle(8, HORIZ_DIR, 1);

	int size = 1;
	shape **sh = new shape*[size];
	int index = 0;
	palette();
	int col = BLACK;

	while (true)
	{
		cout << "\t\t\t\t\t" << size << endl;
		point click;
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
		}
		getmouseclick(WM_LBUTTONDOWN, click.x, click.y);
		cout << click.x << "  " << click.y << endl;

		cout << col << endl;


		if (click.x >= 150 && click.x <= 170 && click.y >= 265 && click.y <= 285)
		{

			point *s = new point[2];
			for (int i = 0; i < 2; i++)
			{
				while (!ismouseclick(WM_LBUTTONDOWN))
				{
				}
				getmouseclick(WM_LBUTTONDOWN, s[i].x, s[i].y);
				cout << s[i].x << "  " << s[i].y << endl;
			}

			double xr, yr, radius;
			xr = (s[1].x - s[0].x)*(s[1].x - s[0].x);
			yr = (s[1].y - s[0].y)*(s[1].y - s[0].y);
			radius = sqrt(xr - yr) / 2;

			if (index < size)
			{
				sh[index] = new Circle(s, col, 2, 12, radius);
				sh[index]->draw();
			}
			else
			{
				extendBy1(sh, size);
				sh[size - 1] = new Circle(s, col, 2, 12, radius);
				sh[size - 1]->draw();
			}
			index++;
			
		}
		else if (click.x >= 170 && click.x <= 190 && click.y >= 265 && click.y <= 285)
		{
			point *s = new point[2];
			for (int i = 0; i < 2; i++)
			{
				while (!ismouseclick(WM_LBUTTONDOWN))
				{
				}
				getmouseclick(WM_LBUTTONDOWN, s[i].x, s[i].y);
				cout << s[i].x << "  " << s[i].y << endl;
			}

			if (index < size)
			{
				sh[index] =new Rect(s, col, 2, 12);
				sh[index]->draw();
			}
			else
			{
				extendBy1(sh, size);
				sh[size - 1] = new Rect (s, col, 2, 12);
				sh[size - 1]->draw();
			}
			index++;
		}
		else if (click.x >= 190 && click.x <= 210 && click.y >= 265 && click.y <= 285)
		{
			int size = 3;
			point *s = new point[size];
			int a = 0;
			while (!ismouseclick(WM_RBUTTONDOWN))
			{
				if (a < size)
				{
					while (!ismouseclick(WM_LBUTTONDOWN))
					{
					}
					getmouseclick(WM_LBUTTONDOWN, s[a].x, s[a].y);
					cout << s[a].x << "  " << s[a].y << endl;
					a++;
				}
				else
				{
					point *temp = new point[size + 1];
					for (int j = 0; j < size; j++)
					{
						temp[j].x = s[j].x;
						temp[j].y = s[j].y;
					}
					while (!ismouseclick(WM_LBUTTONDOWN))
					{
					}
					getmouseclick(WM_LBUTTONDOWN, temp[size].x, temp[size].y);
					cout << temp[size].x << "  " << temp[size].y << endl;
					a++;

					delete[]s;
					size++;
					s = new point[size];
					for (int j = 0; j < size; j++)
					{
						s[j].x = temp[j].x;
						s[j].y = temp[j].y;
					}
				}
			}
			

			if (index < size)
			{
				sh[index] = new polygon (s, col, size, 12);
				sh[index]->draw();
			}
			else
			{
				extendBy1(sh, size);
				sh[size - 1] = new polygon (s, col, size, 12);
				sh[size - 1]->draw();
			}
			index++;
		}
		else if (click.x >= 210 && click.x <= 230 && click.y >= 265 && click.y <= 285)
		{
			point *s = new point[2];
			for (int i = 0; i < 2; i++)
			{
				while (!ismouseclick(WM_LBUTTONDOWN))
				{
				}
				getmouseclick(WM_LBUTTONDOWN, s[i].x, s[i].y);
				cout << s[i].x << "  " << s[i].y << endl;
			}
		
			if (index < size)
			{
				sh[index] = new Line (s, col, 2, 0);;
				sh[index]->draw();
			}
			else
			{
				extendBy1(sh, size);
				sh[size - 1] =new Line (s, col, 2, 0);
				sh[size - 1]->draw();
			}
			index++;
		}
		else if (click.x >= 230 && click.x <= 250 && click.y >= 265 && click.y <= 285)
		{
			point *s = new point[1];
			for (int i = 0; i < 1; i++)
			{
				while (!ismouseclick(WM_LBUTTONDOWN))
				{
				}
				getmouseclick(WM_LBUTTONDOWN, s[i].x, s[i].y);
				cout << s[i].x << "  " << s[i].y << endl;
			}

			if (index < size)
			{
				sh[index] = new Text (s, col, 1);
				sh[index]->draw();
			}
			else
			{
				extendBy1(sh, size);
				sh[size - 1] =new Text (s, col, 1);
				sh[size - 1]->draw();
			}
			index++;
		}
		else if (click.x >= 250 && click.x <= 270 && click.y >= 265 && click.y <= 285)
		{
			point *s = new point[2];
			for (int i = 0; i < 2; i++)
			{
				while (!ismouseclick(WM_LBUTTONDOWN))
				{
				}
				getmouseclick(WM_LBUTTONDOWN, s[i].x, s[i].y);
				cout << s[i].x << "  " << s[i].y << endl;
			}
			

			if (index < size)
			{
				sh[index] = new curve (s, col, 2, 0);
				sh[index]->draw();
			}
			else
			{
				extendBy1(sh, size);
				sh[size - 1] = new curve (s, col, 2, 0);
				sh[size - 1]->draw();
			}
			index++;
		}
		else if (click.x >= 10 && click.x <= 30 && click.y >= 265 && click.y <= 285)
		{
			col = 5;
		}
		else if (click.x >= 30 && click.x <= 50 && click.y >= 265 && click.y <= 285)
		{
			col = 4;
		}
		else if (click.x >= 50 && click.x <= 70 && click.y >= 265 && click.y <= 285)
		{
			col = 14;
		}
		else if (click.x >= 70 && click.x <= 90 && click.y >= 265 && click.y <= 285)
		{
			col = 6;
		}
		else if (click.x >= 90 && click.x <= 110 && click.y >= 265 && click.y <= 285)
		{
			col = 1;
		}
		else if (click.x >= 110 && click.x <= 130 && click.y >= 265 && click.y <= 285)
		{
			col = 2;
		}
		else if (click.x >= 300 && click.x <= 320 && click.y >= 265 && click.y <= 285)
		{
			string str;
			cout << "please enter the name of file :" << endl;
			cin >> str;

			ofstream out(str);
			out << "P2" << endl;
			out << "# Created by IrfanView" << endl;
			out << "1000 264" << endl;
			out << "15" << endl;
			
			for (int i = 0; i < 1000; i+=1)
			{
				for (int j = 0; j < 1000; j+=1)
				{
					out << getpixel(i, j)<<" ";
				}
			}
		}
		else if (click.x >= 320 && click.x <= 340 && click.y >= 265 && click.y <= 285)
		{
			cleardevice();
			string str,str1;
			cout << "please enter the name of file :" << endl;
			cin >> str;
			ifstream in(str);
			
			for (int k = 0; k < 4; k++)
			{
				getline(in, str1);
			}

			int num;
			for (int i = 0; i < 1000; i += 1)
			{
				for (int j = 0; j < 1000; j += 1)
				{
					in >> num;
					putpixel(i, j, num);
				}
			}
		}
		else if (click.x >= 340 && click.x <= 360 && click.y >= 265 && click.y <= 285)
		{
			point P;
			while (!ismouseclick(WM_LBUTTONDOWN))
			{
			}
			getmouseclick(WM_LBUTTONDOWN, P.x,P.y);
			cout << P.x << "  " << P.y << endl;

			for (int i = 0; i < size; i++)
			{
				if (sh[i]->contains(P) == true)
				{
					sh[i]->fill(P.x,P.y,col);
				}
			}
			
		}
		else if (click.x >= 340 && click.x <= 360 && click.y >= 265 && click.y <= 285)
		{
			point P;
			while (!ismouseclick(WM_LBUTTONDOWN))
			{
			}
			getmouseclick(WM_LBUTTONDOWN, P.x, P.y);
			cout << P.x << "  " << P.y << endl;

			for (int i = 0; i < size; i++)
			{
				if (sh[i]->contains(P) == true)
				{
					sh[i]->fill(P.x, P.y, col);
				}
			}
		}
		else if (click.x >= 360 && click.x <= 380 && click.y >= 265 && click.y <= 285)
		{
			point P;
			while (!ismouseclick(WM_LBUTTONDOWN))
			{
			}
			getmouseclick(WM_LBUTTONDOWN, P.x, P.y);
			cout << P.x << "  " << P.y << endl;
			int temp=getpixel(P.x, P.y);
			flood_fill(P.x, P.y,temp,15);
			for (int i = 0; i < size; i++)
			{
				if (sh[i]->contains(P) == true)
				{
					shrinkBy1(sh, size, i);
				}
			}
			cleardevice();
			palette();
			for (int i = 0; i < size; i++)
			{
				sh[i]->draw();
			}

		}
		
	}
		system("pause");
	
}
