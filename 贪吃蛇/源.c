#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>

typedef struct SNAKE //蛇身的一个节点
{
	int x;
	int y;
	struct SNAKE *next;
}snake;

snake *head, *food;//蛇头指针，食物指针
snake *q;
int S_W=58,S_H=26,L_W=86,L_H=38;//地图大小参数
int W, H;//全局地图参数
int score, food_score;//分数参数
int move, U = 1, D = 2, L = 3, R = 4;//蛇的移动数据
int sleeptime = 200;

void pause()//暂停
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
		{
			break;
		}

	}
}

void Pos(int x, int y)//设置光标位置
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//返回标准的输入、输出或错误的设备的句柄，也就是获得输入、输出/错误的屏幕缓冲区的句柄
	SetConsoleCursorPosition(hOutput, pos);
}

void switch_map()//选择地图大小
{
	while (1)
	{
		char S;
		printf("请选择地图大小\n");
		printf("1：小地图： \n");
		printf("2：大地图： \n");
		scanf_s("%c", &S);
		if (S == '1' || S == '2')
		{
			switch (S)
			{
			case '1':
				printf("你选择了小地图");
				W = S_W;
				H = S_H;
				map(W, H);//创建小地图
				break;
			case '2':
				printf("你选择了大地图");
				W = L_W;
				H = L_H;
				map(W, H);//创建大地图
				break;
			}
			break;
		}
		else
			printf("输入错误请重新输入。");
	}
	
}

int map()//创建地图
{
	int i;
	system("cls");
	for (i = 0; i < W; i++)//打印上下边框
	{
		Pos(i, 0);
		printf("-");
		Pos(i, H);
		printf("-");
	}
	for (i = 1; i < H; i++)//打印左右边框
	{
		Pos(0, i);
		printf("|");
		Pos(W, i);
		printf("|");
	}
	return 0;
}

void resnake()//初始化蛇身
{
	snake *tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));//从蛇尾开始，以x,y设定开始的位置//
	tail->x = 24;
	tail->y = 8;
	tail->next = NULL;
	for (i = 1; i <= 4; i++)//初始长度为4
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 24 + i;
		head->y = 8;
		tail = head;
	}
	while (tail != NULL)//从头到尾，输出蛇身
	{
		Pos(tail->x, tail->y);
		printf("@");
		tail = tail->next;
	}
}

void create_food()//随机出现食物
{
	snake *food_1;
	srand((unsigned)time(NULL));//为了防止每次产生的随机数相同，种子设置为time
	food_1 = (snake*)malloc(sizeof(snake));
	while ((food_1->x % 2) != 0)    //保证其为偶数，使得食物能与蛇头对其
	{
		food_1->x = rand() % 52 + 2;
	}
	food_1->y = rand() % 24 + 1;
	q = head;
	while (q->next == NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y) //判断蛇身是否与食物重合
		{
			free(food_1);
			create_food();
		}
		q = q->next;
	}
	Pos(food_1->x, food_1->y);
	food = food_1;
	printf("$");
}

void snake_move()//蛇前进,上U,下D,左L,右R
{
	snake * nexthead;
	wall();

	nexthead = (snake*)malloc(sizeof(snake));
	if (move == U)
	{
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		if (nexthead->x == food->x && nexthead->y == food->y)//如果下一个有食物//
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("@");
				q = q->next;
			}
			score = score + food_score;
			create_food();
		}
		else                                               //如果没有食物//
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("@");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (move == D)
	{
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
		if (nexthead->x == food->x && nexthead->y == food->y)  //有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("@");
				q = q->next;
			}
			score = score + food_score;
			create_food();
		}
		else                               //没有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("@");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (move == L)
	{
		nexthead->x = head->x - 1;
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y)//有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("@");
				q = q->next;
			}
			score = score + food_score;
			create_food();
		}
		else                                //没有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("@");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (move == R)
	{
		nexthead->x = head->x + 1;
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y)//有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("@");
				q = q->next;
			}
			score = score + food_score;
			create_food();
		}
		else                                         //没有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("@");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (eat_self())       //判断是否会咬到自己
		end_game();
	if (wall())       //判断是否会咬到自己
		end_game();
}

void run_game()//控制游戏        
{
	move = R;
	while (1)
	{
		Pos(W+10, 10);
		printf("得分：%d  ", score);
		Pos(W+10, 11);
		printf("每个食物得分：%d分", food_score);
		if (GetAsyncKeyState(VK_UP) && move != D)
		{
			move = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && move != U)
		{
			move = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && move != R)
		{
			move = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && move != L)
		{
			move = R;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{
			pause();
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			end_game();
			break;
		}
		else if (GetAsyncKeyState(VK_F1))
		{
			if (sleeptime >= 50)
			{
				sleeptime = sleeptime - 30;
				food_score = food_score + 2;
				if (sleeptime == 320)
				{
					food_score = 2;//防止减到1之后再加回来有错
				}
			}
		}
		else if (GetAsyncKeyState(VK_F2))
		{
			if (sleeptime < 350)
			{
				sleeptime = sleeptime + 30;
				food_score = food_score - 2;
				if (sleeptime == 350)
				{
					food_score = 1;  //保证最低分为1
				}
			}
		}
		Sleep(sleeptime);
		snake_move();
	}
}

int eat_self()//判断是否咬到了自己
{
	snake *self;
	self = head->next;
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)
		{
			return 1;//咬到返回值1
		}
		self = self->next;
	}
	return 0;//没咬到返回值0
}

int wall()//判断是否碰到墙
{
	if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)
		return 1;//碰到墙返回值1
	else
		return 0;//没碰到返回值0
}

int end_game()
{
	system("cls");
	Pos(W / 2, H / 2);
	printf("游戏结束\n");
	printf("           ");
	printf("您的得分是%d\n", score);
	printf("           ");
	system("pause");
	exit(0);
	return 0;
}

void main()
{	system("mode con cols=150 lines=50");
	switch_map();
	resnake();
	create_food();
	pause();
	Pos(W + 10, 15);
	printf("使用↑.↓.←.→控制蛇的移动.");
	Pos(W + 10, 16);
	printf("使用F1 加速，F2 减速\n");
	Pos(W + 10, 17);
	printf("ESC ：退出游戏.space：暂停游戏.");
	run_game();
	end_game();
	Pos(0,H+2);
	system("pause");
}