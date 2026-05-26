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

// 方向数组：下、右、上、左（与 DFS 版本一致，保证路径符合预期）
int dir[4][2] = {
    {1, 0},   // 下
    {0, 1},   // 右
    {-1, 0},  // 上
    {0, -1}   // 左
};

// 记录每个格子的前驱坐标，用于回溯路径
typedef struct {
    int x, y;
} Point;

Point prev[MAX_ROW][MAX_COL];   // 前驱表
bool visited[MAX_ROW][MAX_COL] = {false};

// BFS 队列
Point queue[MAX_ROW * MAX_COL];
int front = 0, rear = 0;

void enqueue(Point p) {
    queue[rear++] = p;
}

Point dequeue() {
    return queue[front++];
}

bool is_empty() {
    return front == rear;
}

// BFS 搜索最短路径
bool bfs(Point start, Point end) {
    // 初始化起点
    enqueue(start);
    visited[start.x][start.y] = true;
    prev[start.x][start.y] = (Point){-1, -1};  // 起点无前驱

    while (!is_empty()) {
        Point cur = dequeue();

        // 到达终点，立即返回
        if (cur.x == end.x && cur.y == end.y) {
            return true;
        }

        // 尝试四个方向
        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dir[i][0];
            int ny = cur.y + dir[i][1];

            // 越界检查
            if (nx < 0 || nx >= MAX_ROW || ny < 0 || ny >= MAX_COL)
                continue;
            // 墙壁或已访问检查
            if (maze[nx][ny] == 1 || visited[nx][ny])
                continue;

            visited[nx][ny] = true;
            prev[nx][ny] = cur;          // 记录前驱
            enqueue((Point){nx, ny});
        }
    }
    return false;  // 无路径
}

// 回溯路径并打印（从终点到起点）
void print_path(Point start, Point end) {
    // 用栈或数组收集路径，然后从终点逆序输出
    Point path[MAX_ROW * MAX_COL];
    int len = 0;

    Point cur = end;
    while (!(cur.x == start.x && cur.y == start.y)) {
        path[len++] = cur;
        cur = prev[cur.x][cur.y];
    }
    path[len++] = start;  // 加入起点

    // 输出路径（path 数组已经是终点→起点顺序）
    for (int i = 0; i < len; i++) {
        printf("(%d, %d)\n", path[i].x, path[i].y);
    }
}

int main(void) {
    Point start = {0, 0};
    Point end = {4, 4};

    if (bfs(start, end)) {
        print_path(start, end);
    } else {
        printf("No path!\n");
    }

    return 0;
}