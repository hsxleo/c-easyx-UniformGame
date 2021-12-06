#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <comdef.h>

//菜单窗口的高和宽
#define MENU_HEIGHT 600   
#define MENU_WIDTH 600  

//游戏窗口的高和宽
#define GAME_HEIGHT 400
#define GAME_WIDTH 1000

//图片变量
IMAGE menuBackground;  //菜单背景图
IMAGE gameBackground;  //游戏背景图
IMAGE mjImage[30];  //麻将图片


int mj[20];  //存放17张麻将图片下标的数组
int choose[2];  //选择的两张图的下标 

struct user {
	char name[20];  //玩家名
	int score;  //玩家积分
};
struct user users[100];  //用户们
int len = 0;   //用户个数


void loadResource();//加载资源
void drawMenu();//绘制菜单
void mouseMenu();//菜单鼠标交互
void drawGame();//绘制游戏
void playGame();//玩游戏
bool isWin();  //判断游戏是否胜利
void randMj();   //随机生成麻将
void readFile();  //读取文件
void saveFile();  //写入文件
void rankList();  //排行榜


//加载资源
void loadResource() {
	loadimage(&menuBackground, "image\\menubackground.jpg", MENU_WIDTH, MENU_HEIGHT);
	loadimage(&gameBackground, "image\\gamebackground.jpg", GAME_WIDTH, GAME_HEIGHT);
	int i;
	char str[20];
	for (i = 1; i <= 27; i++) {   //将27张麻将的图片加载进来
		wsprintf(str, "image\\%d.png", i);   //生成图片地址字符串
		loadimage(&mjImage[i],str, 50, 50);
	}
}
//绘制菜单
void drawMenu() {
	//生成画布
	initgraph(MENU_WIDTH,MENU_HEIGHT);
	//菜单背景图片
	putimage(0, 0, &menuBackground);

	//绘制选项   开始游戏  排行榜
	settextcolor(WHITE);          //设置字体颜色
	settextstyle(40, 0, "华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明
	outtextxy(200, 150, "开始游戏");
	outtextxy(220, 250, "排行榜");
}
//菜单鼠标交互
void mouseMenu() {
	drawMenu();      //绘制菜单界面
	MOUSEMSG m;  //鼠标变量   用来获取鼠标信息

	while (true) {
		//获取鼠标位置
		m = GetMouseMsg();

		//判断鼠标位置
		if (m.x >= 200 && m.x <= 340 && m.y >= 150 && m.y <= 190) {  //鼠标在 开始游戏 的位置
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				readFile();  //读取文件
				playGame();  //开始游戏
			}
		}
		else if (m.x >= 220 && m.y <= 340 && m.y >= 250 && m.y <= 290) {  //鼠标在 排行榜 的位置
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				rankList();  //排行榜
			}
		}
	}
}

//绘制游戏
void drawGame() {
	//清空画布
	cleardevice();
	//游戏背景图片
	putimage(0, 0, &gameBackground);

	//重新开始  返回菜单
	settextcolor(BLACK);          //设置字体颜色
	settextstyle(30, 0, "华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明
	outtextxy(800, 300, "重新开始");
	outtextxy(800, 350, "返回菜单");

	//显示麻将
	int i;
	for (i = 0; i < 17; i++) {
		putimage(25 + i * 55, 150, &mjImage[mj[i]]);
	}
}

//随机生成麻将
void randMj() {
	//随机生成17张相同类型的牌
	int i,j, tmp;
	int mj_tmp[30];
	for (i = 0; i < 30; i++) {   //每一张牌最多4张
		mj_tmp[i] = 4;
	}
	srand(unsigned(time(NULL)));  //时间种子函数
	switch (rand() % 3) {   //随机判断是生成条子、筒子还是万子
	case 0:
		for (i = 0; i < 17; i++) {
			tmp = rand() % 9 + 1;   //筒子
			if (mj_tmp[tmp] > 0) {   //最多生成四张
				mj_tmp[tmp]--;  //减少一张牌
				mj[i] = tmp;
			}
			else {
				i--;
			}
		}
		break;
	case 1:
		for (i = 0; i < 17; i++) {
			tmp = rand() % 9 + 10;   //条子
			if (mj_tmp[tmp] > 0) {   //最多生成四张
				mj_tmp[tmp]--;  //减少一张牌
				mj[i] = tmp;
			}
			else {
				i--;
			}
		}
		break;
	case 2:
		for (i = 0; i < 17; i++) {
			tmp = rand() % 9 + 19;   //万子
			if (mj_tmp[tmp] > 0) {   //最多生成四张
				mj_tmp[tmp]--;  //减少一张牌
				mj[i] = tmp;
			}
			else {
				i--;
			}
		}
		break;
	}
}

//玩游戏
void playGame() {
	//随机生成麻将
	randMj();

	//生成画布
	initgraph(GAME_WIDTH, GAME_HEIGHT);
	drawGame();   //绘制游戏画面
	
	//计时
	time_t start, end;
	start = time(NULL);  //计时开始

	MOUSEMSG m;  //鼠标变量   用来获取鼠标信息
	int n = 0;    //每两次交换一次
	while (true) {

		//获取鼠标位置
		m = GetMouseMsg();
		int i;
		for (i = 0; i < 17; i++) {
			if (m.x >= 25 + i * 55 && m.x <= 75 + i * 55 && m.y >= 150 && m.y <= 200) {   //判断鼠标位置
				if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
					choose[n] = i;  //选择一个
					setcolor(RED);
					rectangle(25 + i * 55, 150, 75 + i * 55, 200);   //把选中的图片用红框框起来
					n++;
					break;
				}
			}
		}
		if (m.x >= 800 && m.x <= 920 && m.y >= 300 && m.y <= 330) {   //重新开始
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				playGame();   //重新开始游戏
			}
		}
		else if (m.x >= 800 && m.x <= 920 && m.y >= 350 && m.y <= 380) {  //返回菜单
			if (m.uMsg == WM_LBUTTONDOWN) {   //鼠标按下
				mouseMenu();   //游戏菜单
			}
		}
		if (n > 1) {   //选择了两个
			n = n % 2;
			int t = mj[choose[0]];
			mj[choose[0]] = mj[choose[1]];
			mj[choose[1]] = t;
			drawGame();   //绘制游戏画面
			if (isWin()) {  //判断游戏是否胜利
				settextcolor(RED);          //设置字体颜色
				settextstyle(50, 0, "华文新魏"); //设置文字样式
				setbkmode(TRANSPARENT);       //文字背景透明
				outtextxy(400, 50, "游戏胜利");
				end = time(NULL);  //计时结束
				//根据时间得分
				int score = 0;
				if (end - start < 60) {
					outtextxy(380, 250, "得分：5分");
					score = 5;
				}
				else if (end - start < 180) {
					outtextxy(380, 250, "得分：3分");
					score = 3;
				}
				else {
					outtextxy(380, 250, "得分：1分");
					score = 1;
				}
				// 定义字符串缓冲区，并接收用户输入
				char str[11];
				InputBox(str, 10, "请输入昵称:",0,0,0, false);
				//保存成绩到结构体
				int n = strlen(str);
				str[n] = '\0';
				int flg = 0;
				for (i = 0; i < len; i++) {  //判断该用户是否存在
					if (strcmp(str, users[i].name) == 0) {  //若存在
						flg = 1;
						users[i].score += score;
						break;
					}
				}
				if (flg == 0) {
					strcpy(users[len].name, str);
					users[len].score = score;
					len++;
				}

				saveFile();  //写入文件
			}
			 
		}
	}
}


//判断是否胜利
bool isWin() {
	int i = 0;
	bool flg = false;
	while (i < 16) {
		if (mj[i] == mj[i + 1] && mj[i] == mj[i + 2]) {  //刻子
			i = i + 3;
		}
		else if ((mj[i+1] == mj[i] + 1 && mj[i+2] == mj[i] + 2) || (mj[i + 1] == mj[i] - 1 && mj[i + 2] == mj[i] - 2)) {  //顺子
			i = i + 3;
		}
		else if (mj[i] == mj[i + 1] && flg == false) {  //将   唯一
			i = i + 2;
			flg = true;
		}
		else {
			return false;
		}
	}
	return true;
}

//读取文件
void readFile() {
	FILE* fp;
	if ((fp = fopen("score.txt", "r+")) == NULL) {   //打开文件   如果没用自动创建
		fp = fopen("score.txt", "w+");
	}
	struct user tmp;
	//将文件内容全部读入用户结构体数组中
	while (fread(&users[len++], sizeof(struct user), 1, fp));
	len--;
	fclose(fp);  //关闭文件
}

//得分写入文件
void saveFile() {
	FILE* fp;
	fp = fopen("score.txt", "w");
	int i;
	for (i = 0; i < len; i++) {
		fwrite(&users[i], sizeof(struct user), 1, fp);
	}
	fclose(fp);
}

//排行榜
void rankList() {
	int i,j;
	for (i = 0; i < len-1; i++) {
		for (j = 0; j < len - i - 1; j++) {
			if (users[j].score < users[j + 1].score) {
				struct user t = users[j];
				users[j] = users[j + 1];
				users[j + 1] = t;
			}
		}
	}
	cleardevice();
	putimage(0, 0, &menuBackground);
	//显示前三名
	settextcolor(RED);          //设置字体颜色
	settextstyle(50, 0, "华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明
	outtextxy(230, 20, "排行榜");
	settextcolor(BLACK);
	settextstyle(30, 0, "华文新魏"); //设置文字样式
	outtextxy(100, 100, "名次");
	outtextxy(250, 100, "昵称");
	outtextxy(400, 100, "积分");
	outtextxy(100, 200, "状元");
	outtextxy(100, 300, "榜眼");
	outtextxy(100, 400, "探花");
	char s[10];
	if (len >= 3) {
		for (i = 0; i < 3; i++) {
			wsprintf(s, "%s", users[i].name);
			outtextxy(250, 100 * (i + 2), s);
			wsprintf(s, "%d分", users[i].score);
			outtextxy(400, 100 * (i + 2), s);
		}
	}
	else {
		for (i = 0; i < len; i++) {
			wsprintf(s, "%s", users[i].name);
			outtextxy(250, 100 * (i + 2), s);
			wsprintf(s, "%d分", users[i].score);
			outtextxy(400, 100 * (i + 2), s);
		}
	}
}

int main() {
	loadResource();
	mouseMenu();
	_getch();
}