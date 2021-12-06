#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <comdef.h>

//�˵����ڵĸߺͿ�
#define MENU_HEIGHT 600   
#define MENU_WIDTH 600  

//��Ϸ���ڵĸߺͿ�
#define GAME_HEIGHT 400
#define GAME_WIDTH 1000

//ͼƬ����
IMAGE menuBackground;  //�˵�����ͼ
IMAGE gameBackground;  //��Ϸ����ͼ
IMAGE mjImage[30];  //�齫ͼƬ


int mj[20];  //���17���齫ͼƬ�±������
int choose[2];  //ѡ�������ͼ���±� 

struct user {
	char name[20];  //�����
	int score;  //��һ���
};
struct user users[100];  //�û���
int len = 0;   //�û�����


void loadResource();//������Դ
void drawMenu();//���Ʋ˵�
void mouseMenu();//�˵���꽻��
void drawGame();//������Ϸ
void playGame();//����Ϸ
bool isWin();  //�ж���Ϸ�Ƿ�ʤ��
void randMj();   //��������齫
void readFile();  //��ȡ�ļ�
void saveFile();  //д���ļ�
void rankList();  //���а�


//������Դ
void loadResource() {
	loadimage(&menuBackground, "image\\menubackground.jpg", MENU_WIDTH, MENU_HEIGHT);
	loadimage(&gameBackground, "image\\gamebackground.jpg", GAME_WIDTH, GAME_HEIGHT);
	int i;
	char str[20];
	for (i = 1; i <= 27; i++) {   //��27���齫��ͼƬ���ؽ���
		wsprintf(str, "image\\%d.png", i);   //����ͼƬ��ַ�ַ���
		loadimage(&mjImage[i],str, 50, 50);
	}
}
//���Ʋ˵�
void drawMenu() {
	//���ɻ���
	initgraph(MENU_WIDTH,MENU_HEIGHT);
	//�˵�����ͼƬ
	putimage(0, 0, &menuBackground);

	//����ѡ��   ��ʼ��Ϸ  ���а�
	settextcolor(WHITE);          //����������ɫ
	settextstyle(40, 0, "������κ"); //����������ʽ
	setbkmode(TRANSPARENT);       //���ֱ���͸��
	outtextxy(200, 150, "��ʼ��Ϸ");
	outtextxy(220, 250, "���а�");
}
//�˵���꽻��
void mouseMenu() {
	drawMenu();      //���Ʋ˵�����
	MOUSEMSG m;  //������   ������ȡ�����Ϣ

	while (true) {
		//��ȡ���λ��
		m = GetMouseMsg();

		//�ж����λ��
		if (m.x >= 200 && m.x <= 340 && m.y >= 150 && m.y <= 190) {  //����� ��ʼ��Ϸ ��λ��
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				readFile();  //��ȡ�ļ�
				playGame();  //��ʼ��Ϸ
			}
		}
		else if (m.x >= 220 && m.y <= 340 && m.y >= 250 && m.y <= 290) {  //����� ���а� ��λ��
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				rankList();  //���а�
			}
		}
	}
}

//������Ϸ
void drawGame() {
	//��ջ���
	cleardevice();
	//��Ϸ����ͼƬ
	putimage(0, 0, &gameBackground);

	//���¿�ʼ  ���ز˵�
	settextcolor(BLACK);          //����������ɫ
	settextstyle(30, 0, "������κ"); //����������ʽ
	setbkmode(TRANSPARENT);       //���ֱ���͸��
	outtextxy(800, 300, "���¿�ʼ");
	outtextxy(800, 350, "���ز˵�");

	//��ʾ�齫
	int i;
	for (i = 0; i < 17; i++) {
		putimage(25 + i * 55, 150, &mjImage[mj[i]]);
	}
}

//��������齫
void randMj() {
	//�������17����ͬ���͵���
	int i,j, tmp;
	int mj_tmp[30];
	for (i = 0; i < 30; i++) {   //ÿһ�������4��
		mj_tmp[i] = 4;
	}
	srand(unsigned(time(NULL)));  //ʱ�����Ӻ���
	switch (rand() % 3) {   //����ж����������ӡ�Ͳ�ӻ�������
	case 0:
		for (i = 0; i < 17; i++) {
			tmp = rand() % 9 + 1;   //Ͳ��
			if (mj_tmp[tmp] > 0) {   //�����������
				mj_tmp[tmp]--;  //����һ����
				mj[i] = tmp;
			}
			else {
				i--;
			}
		}
		break;
	case 1:
		for (i = 0; i < 17; i++) {
			tmp = rand() % 9 + 10;   //����
			if (mj_tmp[tmp] > 0) {   //�����������
				mj_tmp[tmp]--;  //����һ����
				mj[i] = tmp;
			}
			else {
				i--;
			}
		}
		break;
	case 2:
		for (i = 0; i < 17; i++) {
			tmp = rand() % 9 + 19;   //����
			if (mj_tmp[tmp] > 0) {   //�����������
				mj_tmp[tmp]--;  //����һ����
				mj[i] = tmp;
			}
			else {
				i--;
			}
		}
		break;
	}
}

//����Ϸ
void playGame() {
	//��������齫
	randMj();

	//���ɻ���
	initgraph(GAME_WIDTH, GAME_HEIGHT);
	drawGame();   //������Ϸ����
	
	//��ʱ
	time_t start, end;
	start = time(NULL);  //��ʱ��ʼ

	MOUSEMSG m;  //������   ������ȡ�����Ϣ
	int n = 0;    //ÿ���ν���һ��
	while (true) {

		//��ȡ���λ��
		m = GetMouseMsg();
		int i;
		for (i = 0; i < 17; i++) {
			if (m.x >= 25 + i * 55 && m.x <= 75 + i * 55 && m.y >= 150 && m.y <= 200) {   //�ж����λ��
				if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
					choose[n] = i;  //ѡ��һ��
					setcolor(RED);
					rectangle(25 + i * 55, 150, 75 + i * 55, 200);   //��ѡ�е�ͼƬ�ú�������
					n++;
					break;
				}
			}
		}
		if (m.x >= 800 && m.x <= 920 && m.y >= 300 && m.y <= 330) {   //���¿�ʼ
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				playGame();   //���¿�ʼ��Ϸ
			}
		}
		else if (m.x >= 800 && m.x <= 920 && m.y >= 350 && m.y <= 380) {  //���ز˵�
			if (m.uMsg == WM_LBUTTONDOWN) {   //��갴��
				mouseMenu();   //��Ϸ�˵�
			}
		}
		if (n > 1) {   //ѡ��������
			n = n % 2;
			int t = mj[choose[0]];
			mj[choose[0]] = mj[choose[1]];
			mj[choose[1]] = t;
			drawGame();   //������Ϸ����
			if (isWin()) {  //�ж���Ϸ�Ƿ�ʤ��
				settextcolor(RED);          //����������ɫ
				settextstyle(50, 0, "������κ"); //����������ʽ
				setbkmode(TRANSPARENT);       //���ֱ���͸��
				outtextxy(400, 50, "��Ϸʤ��");
				end = time(NULL);  //��ʱ����
				//����ʱ��÷�
				int score = 0;
				if (end - start < 60) {
					outtextxy(380, 250, "�÷֣�5��");
					score = 5;
				}
				else if (end - start < 180) {
					outtextxy(380, 250, "�÷֣�3��");
					score = 3;
				}
				else {
					outtextxy(380, 250, "�÷֣�1��");
					score = 1;
				}
				// �����ַ������������������û�����
				char str[11];
				InputBox(str, 10, "�������ǳ�:",0,0,0, false);
				//����ɼ����ṹ��
				int n = strlen(str);
				str[n] = '\0';
				int flg = 0;
				for (i = 0; i < len; i++) {  //�жϸ��û��Ƿ����
					if (strcmp(str, users[i].name) == 0) {  //������
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

				saveFile();  //д���ļ�
			}
			 
		}
	}
}


//�ж��Ƿ�ʤ��
bool isWin() {
	int i = 0;
	bool flg = false;
	while (i < 16) {
		if (mj[i] == mj[i + 1] && mj[i] == mj[i + 2]) {  //����
			i = i + 3;
		}
		else if ((mj[i+1] == mj[i] + 1 && mj[i+2] == mj[i] + 2) || (mj[i + 1] == mj[i] - 1 && mj[i + 2] == mj[i] - 2)) {  //˳��
			i = i + 3;
		}
		else if (mj[i] == mj[i + 1] && flg == false) {  //��   Ψһ
			i = i + 2;
			flg = true;
		}
		else {
			return false;
		}
	}
	return true;
}

//��ȡ�ļ�
void readFile() {
	FILE* fp;
	if ((fp = fopen("score.txt", "r+")) == NULL) {   //���ļ�   ���û���Զ�����
		fp = fopen("score.txt", "w+");
	}
	struct user tmp;
	//���ļ�����ȫ�������û��ṹ��������
	while (fread(&users[len++], sizeof(struct user), 1, fp));
	len--;
	fclose(fp);  //�ر��ļ�
}

//�÷�д���ļ�
void saveFile() {
	FILE* fp;
	fp = fopen("score.txt", "w");
	int i;
	for (i = 0; i < len; i++) {
		fwrite(&users[i], sizeof(struct user), 1, fp);
	}
	fclose(fp);
}

//���а�
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
	//��ʾǰ����
	settextcolor(RED);          //����������ɫ
	settextstyle(50, 0, "������κ"); //����������ʽ
	setbkmode(TRANSPARENT);       //���ֱ���͸��
	outtextxy(230, 20, "���а�");
	settextcolor(BLACK);
	settextstyle(30, 0, "������κ"); //����������ʽ
	outtextxy(100, 100, "����");
	outtextxy(250, 100, "�ǳ�");
	outtextxy(400, 100, "����");
	outtextxy(100, 200, "״Ԫ");
	outtextxy(100, 300, "����");
	outtextxy(100, 400, "̽��");
	char s[10];
	if (len >= 3) {
		for (i = 0; i < 3; i++) {
			wsprintf(s, "%s", users[i].name);
			outtextxy(250, 100 * (i + 2), s);
			wsprintf(s, "%d��", users[i].score);
			outtextxy(400, 100 * (i + 2), s);
		}
	}
	else {
		for (i = 0; i < len; i++) {
			wsprintf(s, "%s", users[i].name);
			outtextxy(250, 100 * (i + 2), s);
			wsprintf(s, "%d��", users[i].score);
			outtextxy(400, 100 * (i + 2), s);
		}
	}
}

int main() {
	loadResource();
	mouseMenu();
	_getch();
}