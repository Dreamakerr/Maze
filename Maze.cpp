#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include <assert.h>

/*
assert的作用是先计算表达式expression，如果其值为假（即为0），
那么它先向标准错误流stderr打印一条出错信息，然后通过调用abort来终止程序运行；否则，assert()无任何作用。
宏assert（）一般用于确认程序的正常操作，其中表达式构造无错时才为真值。
*/

#define OK 1;
#define FLASE 0;
#define LEN 5

//创建一个结构体，存储元素的行和列
typedef struct {
	int row;	//行
	int col;	//列
}SPosition, *sposition;

//定义一个结构体，指向开辟存储位置结构体的空间，以及计数器top，容量capacity
typedef struct {
	sposition head;
	int top;
	int capacity;
}Stack, *stack;
//存储路径的栈
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

void InitStack(stack S)//初始化栈
{
	S->head = (sposition)malloc(LEN * sizeof(SPosition));//该处可以修改，根据路径的最长，应该为长加宽-1，得到栈的最大初始化量

	if (S->head == NULL)	
		exit(-1);
	S->top = 0;
	S->capacity = LEN;
}

void PushStack(stack S, SPosition data) {//操作：将元素插入栈； 初始条件：栈初始化完成

	assert(S);
	if (S->top == S->capacity)
	{
		sposition sp = (sposition)realloc(S->head, S->capacity * 2 * sizeof(SPosition));
		if (sp == NULL)
		{
			printf("申请空间失败！\n");
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

SPosition GetTop(stack S)//操作：将栈顶的元素输出，但是不会改变栈的元素；  初始条件：栈初始化，并且栈不为空，需要传递两个整型变量地址
{
	assert(S);
	if (S->top > 0)
		return S->head[S->top - 1];
	else {
		printf("栈空。\n");
		exit(-1);
	}
}

SPosition PopStack(stack S)//本函数采用指针型返回变量，操作时候，需要定义一个结构体接收该指针变量
{
		assert(S);
	if (S->top > 0)
	{
		S->top--;
		return S->head[S->top];
	}
	else
	{
		printf("栈空。\n");
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


//打印迷宫
//输入的参数为迷宫（二维数组），行数，列数
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
	printf("迷宫路径如下：\n空白代表所在通路，@代表走过的死路，#代表墙壁，*代表未经过的路。\n");
	//迷宫的行数，进行一个循环的遍历
	for ( i = 0; i < N; ++i)
	{   //迷宫的列数进行一个循环的遍历
		for ( j = 0; j < M; ++j)
		{
			printf(" %c", tmpmaze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

//输出栈里面的坐标路径
//代码的参数是栈
void PrintPath(stack tack)
{
	int count = 0;
	printf("一共需要走%d步。\n", StackSize(tack));
	printf("路径坐标为：\n");
	//Path数据倒到rPath
	//初始化一个栈，进行将存储Path 中的元素
	Stack rPath;
	InitStack(&rPath);
	//判断栈是否为空
	//循环的具体功能看注释
	while (!EmptyStack(tack))
	{   //将栈tack的栈顶元素进入栈rPath当中
		PushStack(&rPath, GetTop(tack));
		//将tack栈的栈顶元素出栈
		PopStack(tack);
	}
	//主要实现将栈中点的位置输出，是按照迷宫行进的顺序进行输出点的位置
	while (!EmptyStack(&rPath))
	{
		//将栈rPath中的栈顶元素赋值给position
		SPosition positon = GetTop(&rPath);
		//输出position的位置
		if (count == 8)
		{
			printf("\n");
			count = 0;
		}
		printf("(%2d,%2d) ", positon.row+1, positon.col+1);
		count++;
		//将栈顶的元素进行出栈处理，将已经输出的位置的点进行出栈
		PopStack(&rPath);
	}
	printf("\n");
	//销毁栈
	StackDestory(&rPath);
}

int correct = 0;
bool GetPath(int** maze, int N, int M, SPosition cur,SPosition end) {
	SPosition top;
	//将该点路径放入栈中并标记为已经过
	PushStack(&Path, cur);
	maze[cur.row][cur.col] = 2;
	//遇到出口就返回
	if (cur.row == end.row && cur.col == end.col) {
		correct = 1;
		return true;
	}
	//引入变量，表示当前的位置的x,y；
	//对上下左右进行递归
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
	//printf("输入迷宫大小：");
	if (fscanf(fp,"%d%d", &N, &M) != EOF)
	{
		//printf("请按照迷宫大小创建迷宫，0代表通路，1代表墙壁：\n");
		//创建迷宫，获取迷宫地图
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
		//printf("输入起点坐标：");
		fscanf(fp,"%d%d", &x1, &y1);
		SPosition entry = { x1-1, y1-1 };		//起点坐标
		//printf("输入终点坐标：");
		fscanf(fp,"%d%d", &x2, &y2);
		SPosition end = { x2-1, y2-1 };		//终点坐标
		GetPath(maze, N, M, entry, end);
		if (correct == 1)
		{
			PrintPath(&Path);
			PrintMaze(maze, N, M);
		}
		else
		{
			printf("没有找到通路。\n");
		}
		//销毁迷宫
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