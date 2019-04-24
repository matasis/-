#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>

typedef struct SNAKE //�����һ���ڵ�
{
	int x;
	int y;
	struct SNAKE *next;
}snake;

snake *head, *food;//��ͷָ�룬ʳ��ָ��
snake *q;
int S_W=58,S_H=26,L_W=86,L_H=38;//��ͼ��С����
int W, H;//ȫ�ֵ�ͼ����
int score, food_score;//��������
int move, U = 1, D = 2, L = 3, R = 4;//�ߵ��ƶ�����
int sleeptime = 200;

void pause()//��ͣ
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

void Pos(int x, int y)//���ù��λ��
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//���ر�׼�����롢����������豸�ľ����Ҳ���ǻ�����롢���/�������Ļ�������ľ��
	SetConsoleCursorPosition(hOutput, pos);
}

void switch_map()//ѡ���ͼ��С
{
	while (1)
	{
		char S;
		printf("��ѡ���ͼ��С\n");
		printf("1��С��ͼ�� \n");
		printf("2�����ͼ�� \n");
		scanf_s("%c", &S);
		if (S == '1' || S == '2')
		{
			switch (S)
			{
			case '1':
				printf("��ѡ����С��ͼ");
				W = S_W;
				H = S_H;
				map(W, H);//����С��ͼ
				break;
			case '2':
				printf("��ѡ���˴��ͼ");
				W = L_W;
				H = L_H;
				map(W, H);//�������ͼ
				break;
			}
			break;
		}
		else
			printf("����������������롣");
	}
	
}

int map()//������ͼ
{
	int i;
	system("cls");
	for (i = 0; i < W; i++)//��ӡ���±߿�
	{
		Pos(i, 0);
		printf("-");
		Pos(i, H);
		printf("-");
	}
	for (i = 1; i < H; i++)//��ӡ���ұ߿�
	{
		Pos(0, i);
		printf("|");
		Pos(W, i);
		printf("|");
	}
	return 0;
}

void resnake()//��ʼ������
{
	snake *tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));//����β��ʼ����x,y�趨��ʼ��λ��//
	tail->x = 24;
	tail->y = 8;
	tail->next = NULL;
	for (i = 1; i <= 4; i++)//��ʼ����Ϊ4
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 24 + i;
		head->y = 8;
		tail = head;
	}
	while (tail != NULL)//��ͷ��β���������
	{
		Pos(tail->x, tail->y);
		printf("@");
		tail = tail->next;
	}
}

void create_food()//�������ʳ��
{
	snake *food_1;
	srand((unsigned)time(NULL));//Ϊ�˷�ֹÿ�β������������ͬ����������Ϊtime
	food_1 = (snake*)malloc(sizeof(snake));
	while ((food_1->x % 2) != 0)    //��֤��Ϊż����ʹ��ʳ��������ͷ����
	{
		food_1->x = rand() % 52 + 2;
	}
	food_1->y = rand() % 24 + 1;
	q = head;
	while (q->next == NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y) //�ж������Ƿ���ʳ���غ�
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

void snake_move()//��ǰ��,��U,��D,��L,��R
{
	snake * nexthead;
	wall();

	nexthead = (snake*)malloc(sizeof(snake));
	if (move == U)
	{
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		if (nexthead->x == food->x && nexthead->y == food->y)//�����һ����ʳ��//
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
		else                                               //���û��ʳ��//
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
		if (nexthead->x == food->x && nexthead->y == food->y)  //��ʳ��
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
		else                               //û��ʳ��
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
		if (nexthead->x == food->x && nexthead->y == food->y)//��ʳ��
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
		else                                //û��ʳ��
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
		if (nexthead->x == food->x && nexthead->y == food->y)//��ʳ��
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
		else                                         //û��ʳ��
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
	if (eat_self())       //�ж��Ƿ��ҧ���Լ�
		end_game();
	if (wall())       //�ж��Ƿ��ҧ���Լ�
		end_game();
}

void run_game()//������Ϸ        
{
	move = R;
	while (1)
	{
		Pos(W+10, 10);
		printf("�÷֣�%d  ", score);
		Pos(W+10, 11);
		printf("ÿ��ʳ��÷֣�%d��", food_score);
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
					food_score = 2;//��ֹ����1֮���ټӻ����д�
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
					food_score = 1;  //��֤��ͷ�Ϊ1
				}
			}
		}
		Sleep(sleeptime);
		snake_move();
	}
}

int eat_self()//�ж��Ƿ�ҧ�����Լ�
{
	snake *self;
	self = head->next;
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)
		{
			return 1;//ҧ������ֵ1
		}
		self = self->next;
	}
	return 0;//ûҧ������ֵ0
}

int wall()//�ж��Ƿ�����ǽ
{
	if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)
		return 1;//����ǽ����ֵ1
	else
		return 0;//û��������ֵ0
}

int end_game()
{
	system("cls");
	Pos(W / 2, H / 2);
	printf("��Ϸ����\n");
	printf("           ");
	printf("���ĵ÷���%d\n", score);
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
	printf("ʹ�á�.��.��.�������ߵ��ƶ�.");
	Pos(W + 10, 16);
	printf("ʹ��F1 ���٣�F2 ����\n");
	Pos(W + 10, 17);
	printf("ESC ���˳���Ϸ.space����ͣ��Ϸ.");
	run_game();
	end_game();
	Pos(0,H+2);
	system("pause");
}