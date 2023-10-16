
//张则雨


#include<iostream>
#include<windows.h>
#include<time.h>
#include<conio.h>
#include<stdlib.h>
#include<fstream>
#include<string.h>

using namespace std;
class CSnake
{
public:
	int x; //横坐标
	int y;	//纵坐标
	CSnake()	//之后会有CSnake类的数组，如果不初始化，会报“引发了异常: 写入访问权限冲突。”
	{
		x = 0;
		y = 0;
	};
};
class CFood
{
public:
	int x;	//横坐标
	int y;	//纵坐标
};
class Hero
{
public:
	char heroname[30];	//用户名
	int heroscore;	//用户成绩

};
class CMap
{
	CSnake s[400];	//贪吃蛇蛇头及身体坐标
	CFood f;	//食物坐标
	Hero h;	//用户信息
	int map[20][20];	//地图坐标
	int score;	//成绩
	int scoretem;	//用来判断是否增加50分
	int level;	//等级
	int length;	//蛇长度
	int speed;	//速度
	char dir;//方向
	char tem;//键盘输入
public:
	void initiate();//初始化数据
	void showmap();//显示地图
	int check();//检查是否吃到食物，是否撞墙，是否撞到蛇身
	int update();//蛇前进时身子和头坐标如何改变
	void showinfor();//显示分数，等级
	void setspeed();//更新速度
	int sethero();//输入用户名，写入文件
	int getscore() { return score; }	//得到成绩

};
void gotoxy(int x, int y)//将光标移动到指定位置
{
	COORD pos = { y ,x };//光标所在位置函数COORD pos。这一句定义了一个结构体变量pos，它的两个成员是y和x，y为列坐标，x为行坐标
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
	SetConsoleCursorPosition(hout, pos);//SetConsoleCursorPosition是API中定位光标位置的函数
	CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
	SetConsoleCursorInfo(hout, &cursor_info);//隐藏光标
}
void CMap::initiate()
{
	int i, j;
	score = 0;
	level = 1;
	length = 2;
	speed = 300;
	s[0].x = 1;
	s[0].y = 3;
	s[1].x = 1;
	s[1].y = 2;
	f.x = 6;
	f.y = 6;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 20; j++)
		{
			map[i][j] = 0;
		}
	}
	map[1][4] = 1;
	map[1][3] = 2;
	map[6][6] = 3;
	tem = 77;
	dir = 77;
	scoretem = 0;
}
void CMap::showmap()
{
	int i, j;
	gotoxy(0, 0);
	for (i = 0; i < 22; i++)//墙的上横边
	{
		cout << "■";
	}
	gotoxy(1, 0);
	for (i = 0; i < 20; i++)
	{
		cout << "■";//墙的左竖边
		for (j = 0; j < 20; j++)
		{
			switch (map[i][j])
			{
			case 0: cout << "  "; break;
			case 1: cout << "●"; break;//head
			case 2: cout << "■"; break;//body
			case 3: cout << "★"; break;//food
			}
		}
		cout << "■" << endl;//墙的右竖边
	}
	for (i = 0; i < 22; i++)
		cout << "■";	//墙的下横边
}

int CMap::check()
{
	if (s[0].x == f.x && s[0].y == f.y)//如果吃到食物
	{
		score += 10;
		length += 1;
		if ((score - scoretem) == 50)
		{
			level += 1;
			scoretem = score;
		}
		srand((unsigned)time(NULL));	//随机产生食物
		do {
			f.x = rand() % 19;//0-19内任意数
			f.y = rand() % 19;
		} while (map[f.x][f.y] != 0);//直到产生食物的坐标不在蛇的坐标上
		map[f.x][f.y] = 3;	//令新食物坐标处值为3		
	}
	if (s[0].x < 0 || s[0].x>19 || s[0].y < 0 || s[0].y>19)//如果撞墙
	{
		gotoxy(8, 46);
		cout << " Game Over!" << endl;
		gotoxy(25, 0);
		return 0;
	}
	for (int i = 1; i < length; i++)//如果蛇头撞到蛇身
	{
		if (s[0].x == s[i].x && s[0].y == s[i].y)
		{
			gotoxy(8, 46);
			cout << "Game Over!" << endl;
			gotoxy(25, 0);
			return 0;
		}
	}
	gotoxy(25, 0);
	return 1;
}
int CMap::update()
{
	if (_kbhit())//检查当前是否有键盘输入, 若有则返回一个非0值, 否则返回0 。最开始，tem初始值为77。
		tem = _getch();//当用户按下某个字符时，函数自动读取，无需按回车

	if (tem == 72 || tem == 80 || tem == 75 || tem == 77)
	{
		map[s[length - 1].x][s[length - 1].y] = 0;//蛇最后一节坐标的值为0
		for (int i = length - 1; i > 0; i--)//蛇后一节的坐标等于前一节的坐标，直到第二节
		{
			s[i].x = s[i - 1].x;
			s[i].y = s[i - 1].y;
			map[s[i].x][s[i].y] = 2;	//蛇身坐标值为2
		}
	}

	switch (tem)//蛇头方向
	{
	case 72://up
		if (dir != 80)//判断目前不在向下走
		{
			s[0].x -= 1;
			dir = tem;
		}
		else if (dir == 80)//若目前方向为向下，则继续向下
		{
			s[0].x += 1;
		}
		break;
	case 80://down
		if (dir != 72)
		{
			s[0].x += 1;
			dir = tem;
		}
		else if (dir == 72)
		{
			s[0].x -= 1;
		}
		break;
	case 75://left
		if (dir != 77)
		{
			s[0].y -= 1;
			dir = tem;
		}
		else if (dir == 77)
		{
			s[0].y += 1;
		}
		break;
	case 77://right
		if (dir != 75)
		{
			s[0].y += 1;
			dir = tem;
		}
		else if (dir == 75)
		{
			s[0].y -= 1;
		}
		break;
	case 32://pause
		_getch();//直到重新输入上下左右再继续移动
		break;
	}
	map[s[0].x][s[0].y] = 1;//蛇头坐标

	return 0;
}
void CMap::showinfor()
{
	gotoxy(1, 46);
	cout << "HELP：" << endl;
	gotoxy(2, 46);
	cout << "UP（↑）、DOWN（↓）、LEFT（←）、RIGHT（→）";
	gotoxy(3, 46);
	cout << "STOP（spacebar）、CONTINUE（↑/↓/←/→）";
	gotoxy(5, 46);
	cout << "SCORE：" << score;
	gotoxy(6, 46);
	cout << "LEVEL：" << level;
	gotoxy(7, 46);
	cout << "SPEED：" << speed;
}
void CMap::setspeed()
{
	speed = 300 - 30 * level;	//等级最高为十级，成绩最高为500分，蛇最长50节
	Sleep(speed);	//在Windows操作系统中，sleep()函数需要一个以毫秒为单位的参数代表程序挂起时长
}
int CMap::sethero()
{

	gotoxy(10, 46);
	cout << "恭喜，你的成绩达到30分！输入用户名即可加入英雄榜。";
	gotoxy(11, 46);

	cin >> h.heroname;
	h.heroscore = score;

	fstream fout("heroinfor1.dat", ios::out | ios::app | ios::binary);//ios::out文件不存在则创建，若文件已存在则清空原内容
														//ios::app文件不存在则创建，若文件已存在则在原文件内容后写入新的内容，指针位置总在最后
	fout.write((char*)&h, sizeof(Hero));//把该对象写入文件
	fout.close();

	gotoxy(30, 0);
	return 0;

}

int game()
{
	CMap m;
	int a = 1;
	m.initiate();
	do
	{
		m.showinfor();
		m.showmap();
		m.setspeed();
		m.update();
		a = m.check();
	} while (a != 0);
	if (m.getscore() > 30)
		m.sethero();
	system("pause");   //暂停界面 
	system("cls");     //刷新
	return 0;
}

int welcome(int& ops)
{
	int i;
	gotoxy(0, 0);
	for (i = 0; i < 21; i++)
		cout << "■";
	for (i = 1; i < 21; i++)
	{
		gotoxy(i, 0);
		cout << "■";
		gotoxy(i, 40);
		cout << "■";
	}
	gotoxy(21, 0);
	for (i = 0; i < 21; i++)
		cout << "■";
	gotoxy(5, 16);
	cout << "贪吃蛇游戏";
	gotoxy(8, 16);
	cout << "1.开始游戏";
	gotoxy(10, 16);
	cout << "2.英雄榜";
	gotoxy(12, 16);
	cout << "3.退出游戏";
	gotoxy(17, 14);
	cout << "制作人：张则雨";
	gotoxy(19, 16);
	cout << "2020/5/26";
	gotoxy(22, 0);

	cout << "please enter your choice(1-3)：";
	cin >> ops;  //输入一个整数 
	system("pause");   //暂停界面 
	system("cls");     //刷新
	return 0;
}

void showhero()
{

	int i = 0;
	int num = 0;
	int j;
	int k;
	char ks[30] = "a";
	Hero hs[10];

	fstream file("heroinfor1.dat", ios::binary | ios::in);
	cout << "\t" << "用户名" << "\t" << "成绩" << endl;
	while (file.peek() != EOF)
	{

		file.read((char*)&hs[num], sizeof(Hero));//从文件读出所有信息		

		num++;
	}
	file.close();
	for (i = 0; i < num - 1; i++)
	{
		for (j = i + 1; j < num; j++)
		{
			if (hs[j].heroscore > hs[i].heroscore)
			{

				k = hs[j].heroscore;
				hs[j].heroscore = hs[i].heroscore;
				hs[i].heroscore = k;

				strcpy_s(ks, hs[i].heroname);
				strcpy_s(hs[i].heroname, hs[j].heroname);
				strcpy_s(hs[j].heroname, ks);
			}
		}
	}
	for (i = 0; i < num; i++)
	{
		cout << "\t" << hs[i].heroname << "\t" << hs[i].heroscore << endl;
	}

	system("pause");   //暂停界面 
	system("cls");     //刷新


}

int main()
{
	int op;

	do
	{
		welcome(op);
		switch (op)
		{
		case 1:
			game();
			break;
		case 2:
			showhero();
			break;
		}

	} while (op != 3);
	cout << "------------感谢使用！-------------" << endl;
	return 0;
}