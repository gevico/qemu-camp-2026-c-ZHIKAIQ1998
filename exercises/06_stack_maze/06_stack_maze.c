#include <stdio.h>
#include <stdbool.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
    {0, 1, 0, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 1, 0}
};

// 记录访问过的点，防止重复走
bool visited[MAX_ROW][MAX_COL] = {false};

// 存储最终路径（最多 MAX_ROW * MAX_COL 个点）
int path[MAX_ROW * MAX_COL][2];
int path_len = 0;

// 方向数组：上、下、左、右
int dir[4][2] = {
    {-1, 0}, // 上
    {1, 0},  // 下
    {0, -1}, // 左
    {0, 1}   // 右
};

// 深度优先搜索
bool dfs(int x, int y) {
    // 如果越界、撞墙或已经访问过，返回 false
    if (x < 0 || x >= MAX_ROW || y < 0 || y >= MAX_COL)
        return false;
    if (maze[x][y] == 1 || visited[x][y])
        return false;

    // 标记当前点为已访问，并加入路径
    visited[x][y] = true;
    path[path_len][0] = x;
    path[path_len][1] = y;
    path_len++;

    // 到达终点
    if (x == MAX_ROW - 1 && y == MAX_COL - 1)
        return true;

    // 依次尝试四个方向
    for (int i = 0; i < 4; i++) {
        int nx = x + dir[i][0];
        int ny = y + dir[i][1];
        if (dfs(nx, ny))
            return true;   // 找到通路，直接返回
    }

    // 四个方向均不通，回溯：从路径中移除当前点
    path_len--;
    return false;
}

int main(void) {
    // 执行深度优先搜索
    if (dfs(0, 0)) {
        // 从终点到起点逐行打印，不输出任何额外文字
        for (int i = path_len - 1; i >= 0; i--) {
            printf("(%d, %d)\n", path[i][0], path[i][1]);
        }
    } else {
        // 若迷宫无解，输出"No path!"（但本题一定有解）
        printf("No path!\n");
    }

    return 0;
}