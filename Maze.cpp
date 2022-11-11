#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include <assert.h>

/*
assert���������ȼ�����ʽexpression�������ֵΪ�٣���Ϊ0����
��ô�������׼������stderr��ӡһ��������Ϣ��Ȼ��ͨ������abort����ֹ�������У�����assert()���κ����á�
��assert����һ������ȷ�ϳ�����������������б��ʽ�����޴�ʱ��Ϊ��ֵ��
*/

#define OK 1;
#define FLASE 0;
#define LEN 5

//����һ���ṹ�壬�洢Ԫ�ص��к���
typedef struct {
	int row;	//��
	int col;	//��
}SPosition, *sposition;

//����һ���ṹ�壬ָ�򿪱ٴ洢λ�ýṹ��Ŀռ䣬�Լ�������top������capacity
typedef struct {
	sposition head;
	int top;
	int capacity;
}Stack, *stack;
//�洢·����ջ
Stack Path;

void InitStack(stack S);
void PushStack(stack S, SPosition data);
SPosition GetTop(stack S);
SPosition PopStack(stack S);
int StackSize(stack S);
void ClearStack(stack S);
int EmptyStack(stack S);
int StackDestory(stack S);
void PrintMaze(int** maze, int N, int M);
void PrintPath(stack tack);
bool GetPath(int** maze, int N, int M, SPosition cur, SPosition end);

void InitStack(stack S)//��ʼ��ջ
{
	S->head = (sposition)malloc(LEN * sizeof(SPosition));//�ô������޸ģ�����·�������Ӧ��Ϊ���ӿ�-1���õ�ջ������ʼ����

	if (S->head == NULL)	
		exit(-1);
	S->top = 0;
	S->capacity = LEN;
}

void PushStack(stack S, SPosition data) {//��������Ԫ�ز���ջ�� ��ʼ������ջ��ʼ�����

	assert(S);
	if (S->top == S->capacity)
	{
		sposition sp = (sposition)realloc(S->head, S->capacity * 2 * sizeof(SPosition));
		if (sp == NULL)
		{
			printf("����ռ�ʧ�ܣ�\n");
			exit(-1);
		}
		else
		{
			S->head = sp;
			S->capacity *= 2;
		}
	}
	S->head[S->top] = data;
	S->top++;
}

SPosition GetTop(stack S)//��������ջ����Ԫ����������ǲ���ı�ջ��Ԫ�أ�  ��ʼ������ջ��ʼ��������ջ��Ϊ�գ���Ҫ�����������ͱ�����ַ
{
	assert(S);
	if (S->top > 0)
		return S->head[S->top - 1];
	else {
		printf("ջ�ա�\n");
		exit(-1);
	}
}

SPosition PopStack(stack S)//����������ָ���ͷ��ر���������ʱ����Ҫ����һ���ṹ����ո�ָ�����
{
		assert(S);
	if (S->top > 0)
	{
		S->top--;
		return S->head[S->top];
	}
	else
	{
		printf("ջ�ա�\n");
		exit(-1);
	}

}

int StackSize(stack S)
{
	assert(S);
	return S->top;
}

void ClearStack(stack S)
{
	do {
		S->head[S->top - 1].col = 0;
		S->head[S->top - 1].row = 0;
		S->top--;
	} while (S->top = 0);
}

int EmptyStack(stack S)
{
	//assert(S);
	if (S->top == 0)
	{
		return OK;
	}
	else
	{
		return FLASE;
	}

}

int StackDestory(stack S)
{
	assert(S);
	free(S->head);
	S->head = NULL;
	S->top = S->capacity = 0;
	if (!S->head)
	{
		return OK;
	}
	else 
	{
		return FLASE;
	}
}


//��ӡ�Թ�
//����Ĳ���Ϊ�Թ�����ά���飩������������
void PrintMaze(int** maze, int N, int M)
{
	int i, j;
	char** tmpmaze = (char**)malloc(sizeof(char*) * N);
	for (int i = 0; i < N; i++)
	{
		tmpmaze[i] = (char*)malloc(sizeof(char) * M);
	}
	for ( i = 0; i < N; ++i)
	{
		for ( j = 0; j < M; ++j)
		{
			if (maze[i][j] == 2)
				tmpmaze[i][j] = ' ';
			else if (maze[i][j] == 1)
				tmpmaze[i][j] = '#';
			else if (maze[i][j] == 3)
				tmpmaze[i][j] = '@';
			else
				tmpmaze[i][j] = '*';
		}
	}
	printf("\n--------------------------------------------------------------\n");
	printf("�Թ�·�����£�\n�հ״�������ͨ·��@�����߹�����·��#����ǽ�ڣ�*����δ������·��\n");
	//�Թ�������������һ��ѭ���ı���
	for ( i = 0; i < N; ++i)
	{   //�Թ�����������һ��ѭ���ı���
		for ( j = 0; j < M; ++j)
		{
			printf(" %c", tmpmaze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

//���ջ���������·��
//����Ĳ�����ջ
void PrintPath(stack tack)
{
	int count = 0;
	printf("һ����Ҫ��%d����\n", StackSize(tack));
	printf("·������Ϊ��\n");
	//Path���ݵ���rPath
	//��ʼ��һ��ջ�����н��洢Path �е�Ԫ��
	Stack rPath;
	InitStack(&rPath);
	//�ж�ջ�Ƿ�Ϊ��
	//ѭ���ľ��幦�ܿ�ע��
	while (!EmptyStack(tack))
	{   //��ջtack��ջ��Ԫ�ؽ���ջrPath����
		PushStack(&rPath, GetTop(tack));
		//��tackջ��ջ��Ԫ�س�ջ
		PopStack(tack);
	}
	//��Ҫʵ�ֽ�ջ�е��λ��������ǰ����Թ��н���˳�����������λ��
	while (!EmptyStack(&rPath))
	{
		//��ջrPath�е�ջ��Ԫ�ظ�ֵ��position
		SPosition positon = GetTop(&rPath);
		//���position��λ��
		if (count == 8)
		{
			printf("\n");
			count = 0;
		}
		printf("(%2d,%2d) ", positon.row+1, positon.col+1);
		count++;
		//��ջ����Ԫ�ؽ��г�ջ�������Ѿ������λ�õĵ���г�ջ
		PopStack(&rPath);
	}
	printf("\n");
	//����ջ
	StackDestory(&rPath);
}

int correct = 0;
bool GetPath(int** maze, int N, int M, SPosition cur,SPosition end) {
	SPosition top;
	//���õ�·������ջ�в����Ϊ�Ѿ���
	PushStack(&Path, cur);
	maze[cur.row][cur.col] = 2;
	//�������ھͷ���
	if (cur.row == end.row && cur.col == end.col) {
		correct = 1;
		return true;
	}
	//�����������ʾ��ǰ��λ�õ�x,y��
	//���������ҽ��еݹ�
	SPosition next;
	next = cur;
	next.row += 1;
	if (next.row >= 0 && next.row < N
		&& next.col >= 0 && next.col < M
		&& maze[next.row][next.col] == 0) {
		if (GetPath(maze, N, M, next, end))
			return true;
	}
	
	next = cur;
	next.row -= 1;
	if (next.row >= 0 && next.row < N
		&& next.col >= 0 && next.col < M
		&& maze[next.row][next.col] == 0) {
		if (GetPath(maze, N, M, next, end))
			return true;
	}
	next = cur;
	next.col += 1;
	if (next.row >= 0 && next.row < N
		&& next.col >= 0 && next.col < M
		&& maze[next.row][next.col] == 0) {
		if (GetPath(maze, N, M, next, end))
			return true;
	}
	next = cur;
	next.col -= 1;
	if (next.row >= 0 && next.row < N
		&& next.col >= 0 && next.col < M
		&& maze[next.row][next.col] == 0) {
		if (GetPath(maze, N, M, next, end))
			return true;
	}
	top = PopStack(&Path);
	maze[top.row][top.col] = 3;
	return false;
}



int main()
{
	int N = 0, M = 0;
	int x1, y1, x2 ,y2;
	FILE* fp = fopen("data.txt", "r");
	//printf("�����Թ���С��");
	if (fscanf(fp,"%d%d", &N, &M) != EOF)
	{
		//printf("�밴���Թ���С�����Թ���0����ͨ·��1����ǽ�ڣ�\n");
		//�����Թ�����ȡ�Թ���ͼ
		int** maze = (int**)malloc(sizeof(int*) * N);
		for (int i = 0; i < N; i++)
		{
			maze[i] = (int*)malloc(sizeof(int) * M);
		}
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				fscanf(fp,"%d", &maze[i][j]);
			}
		}
		InitStack(&Path);
		//printf("����������꣺");
		fscanf(fp,"%d%d", &x1, &y1);
		SPosition entry = { x1-1, y1-1 };		//�������
		//printf("�����յ����꣺");
		fscanf(fp,"%d%d", &x2, &y2);
		SPosition end = { x2-1, y2-1 };		//�յ�����
		GetPath(maze, N, M, entry, end);
		if (correct == 1)
		{
			PrintPath(&Path);
			PrintMaze(maze, N, M);
		}
		else
		{
			printf("û���ҵ�ͨ·��\n");
		}
		//�����Թ�
		StackDestory(&Path);
		for (int i = 0; i < N; ++i)
		{
			free(maze[i]);
		}
		free(maze);
		maze = NULL;
	}
	return 0;
}