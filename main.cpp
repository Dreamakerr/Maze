#include<time.h>
#include<stdio.h>
#include<stdlib.h>
const int N = 50;
int g[N][N];
int yep = 0, sx1, sy1, sx2, sy2;
int dx[] = {-1, 0, 1, 0}, dy[] = {0, 1, 0, -1};  // �����������Һ��������ƫ����

bool check(int ux, int uy, int pre_d)
{
	for (int i = 0; i < 4; i ++)
	{
		if (abs(pre_d - i) == 2) continue;  // �����жϵ�ǰ�����ߵķ���
		int nx = ux + dx[i], ny = uy + dy[i];
		if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;  // Խ���жϣ������߳���ͼ��Χ
		if (g[nx][ny]) return true;  // ���Ҫ��ȥ�����������Χ�Ѿ��߹��ˣ���ô�߹�ȥ�ͻ��γɻ�
	}
	return false;
}

void dfs(int ux, int uy)  // ux��uy�ֱ�Ϊ��ǰ��ĺ������꣬�������ĸ�������չ
{
	if (ux == N - 1 || uy == N - 1 || !ux || !uy) return;
	int book[4] = {-1, -1, -1, -1}, cnt = 0;  // ��¼�Ѿ������ļ�������
	while (1)
	{
		if (cnt == 4) break;
		int d = rand() % 4;
		bool flag = false;
		for (int i = 0; i < 4; i ++)
			if (book[i] == d) 
			{
				flag = true;
				break;
			}
		if (flag) continue;
		book[cnt ++] = d;
		int nx = ux + dx[d], ny = uy + dy[d];
		if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;  // Խ���жϣ������߳���ͼ��Χ
		if (check(nx, ny, d)) continue;
		if (yep >= 2 && (nx == N - 1 || ny == N - 1 || !nx || !ny)) continue;
		if (nx == N - 1 || ny == N - 1 || !nx || !ny) 
		{
			if (!yep) sx1 = nx, sy1 = ny;
			else if (yep == 1) 
			{
				if (abs(nx - sx1) + abs(ny - sy1) < int(3 * N / 2)) continue;
				
			}
			sx2 = nx, sy2 = ny;
			yep ++;
		}
		g[nx][ny] = 1;
		dfs(nx, ny);
	}
}

int main()
{
	srand(time(0));
	int start_x = rand() % (N - 3) + 1, start_y = rand() % (N - 3) + 1;  // �������һ�����
	printf("%d %d\n", start_x, start_y);
	// start_x = 1, start_y = 1;
	
	yep = 0;
	while (yep < 2)
	{
		yep = 0;
		for (int i = 0; i < N; i ++)
			for (int j = 0; j < N; j ++)
				g[i][j] = 0;
		dfs(start_x, start_y);
	}
	
	FILE* fp = fopen("data.txt", "w");
	fprintf(fp, "%d %d\n", N, N);
	for (int i = 0; i < N; i ++)
	{
		for (int j = 0; j < N; j ++)
			if (g[i][j]) 
			{
				printf("  ");
				fputs("0 ", fp);
			}
			else 
			{
				printf("��");
				fputs("1 ", fp);
			}
		printf("\n");
		fputs("\n", fp);
	}
	fprintf(fp, "\n%d %d\n%d %d", sx1 + 1, sy1 + 1, sx2 + 1, sy2 + 1);
	fclose(fp);
	system("pause");

	return 0;
}
