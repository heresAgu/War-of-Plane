#include<iostream>
#include<graphics.h>
#include<Windows.h>
#include <ctime>  // 包含time函数
#include <cstdlib>  // 包含rand和srand函数
#include<cmath>
using namespace std;

IMAGE bk;
IMAGE plane[2];
IMAGE bull[2];
IMAGE emy[3][2];
IMAGE explode[2];
IMAGE xin[2];
IMAGE C4[2];
IMAGE BOMBBAG[2];
IMAGE explode2[2];
IMAGE MISSILE[2];    //战斗机导弹
IMAGE HEART_ADD[2];


const int WIDTH = 480;
const int HEIGHT = 850;

const int BULLET_NUM = 15;
const int ENEMY_NUM = 200;
const int BOMB_NUM = 200;

const int ENEMYSPEED = 3;     //敌机速度 已废用
const int PLAYERSPEED = 4;    //玩家速度
const int BULLETSPEED = 7;    //子弹速度
const int PLAYERLIVES = 3;    //玩家血量上限
const int INVINCIPLE_DURATION = 1000;  //无敌时间持续1000ms
const int SCOREADD = 100;  //击落一架敌机 获得100分

const int BULLET_FREQUENCY = 400;   //子弹发射频率

const int C4_NUM = 20;    //炸弹最大数量
const int C4SPEED = 5;	  //炸弹速度

const int ENEMYTYPE0SPEED = 2;   //运输机速度
const int ENEMYTYPE1SPEED = 3;   //轰炸机速度
const int ENEMYTYPE2SPEED = 4;	 //战斗机速度

const int BAG_NUM = 3;    //炸药包最大数量
const int BOMB2_NUM = 3;   //炸药包爆炸最大数量
const int BAG_FLASH_TIME = 3000;     //炸药包闪烁时间2000ms
const int BAG_EXPLOSION_RADIUS = 70;   //爆炸范围
const int BAG_SPEED = 1;

const int MISSILE_NUM = 2;        //导弹数量
const int MISSILE_X_SPEED = 2;	  //水平方向速度
const int MISSILE_Y_SPEED = 10;    //竖直方向速度


const int HEART_ADD_NUM = 1;   //修理工具最大数量
const int FIX_SPEED = 1;
const int FIX_DURATION_TIME = 5000;
struct Plane {
	int x, y;
	bool live;
	int width;  //飞机宽度 
	int height;  //飞机高度
	int type;     //敌机类型
}player, bullet[BULLET_NUM], enemy[ENEMY_NUM], c4[C4_NUM];

// 爆炸效果结构体
struct Bomb {
	int x, y;
	bool live;
	int frame;  // 爆炸动画帧数计数器 
} bomb[BOMB_NUM],bomb2[BOMB2_NUM];

//炸药包结构体
struct Bombbag {
	int x, y;
	bool alive; 
	bool isbagbomed;  //是否已经爆炸
	int r;     //爆炸半径
	DWORD createtime;
}bombbag[BAG_NUM];

struct mis {     //导弹结构体
	int x, y;
	bool alive;
}missile[MISSILE_NUM];

struct xiuli {   //修理工具结构体
	int x, y;
	bool alive;
	DWORD createtime;
}heart_add[HEART_ADD_NUM];

bool heart[PLAYERLIVES];    //血量数组

bool ispaused = false;
int playerlives = PLAYERLIVES;
bool isplayerinvinciple = false;  // 碰撞后瞬间无敌
DWORD invinciplestarttime = 0;   //开始无敌时间



//游戏全局变量
bool isgameover = false;
int score = 0;
int enemykilled = 0;
DWORD gamestarttime = GetTickCount();
DWORD gameendtime = 0;
// 游戏结束菜单选项
enum GameOverOption {
	RESTART_GAME,
	EXIT_GAME
};
GameOverOption selectedOption = RESTART_GAME;  // 当前选中的选项

//初始化函数
void gameInit() {
	//游戏总设置
	score = 0;
	enemykilled = 0;
	gamestarttime = GetTickCount();
	gameendtime = 0;

	//玩家设置
	player.x = WIDTH / 2 - 24;
	player.y = HEIGHT - 100;
	player.live = true;
	playerlives = PLAYERLIVES;
	isplayerinvinciple = false;
	//子弹设置
	for (int i = 0;i < BULLET_NUM; i++)
	{
		bullet[i].x = -50;
		bullet[i].y = 0;
		bullet[i].live = false;
	}
	//敌人设置
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].live = false;
		enemy[i].x = 0;
		enemy[i].y = 0;
	}
	//爆炸设置
	for (int i = 0; i < BOMB_NUM; i++)
	{
		bomb[i].live = false;
		bomb[i].x = 0;
		bomb[i].y = 0;
		bomb[i].frame = 0;
	}
	//血量设置
	for (int i = 0;i < PLAYERLIVES; i++)
	{
		heart[i] = true;
	}
	//c4设置
	for (int i = 0;i < C4_NUM; i++)
	{
		c4[i].x = 0;
		c4[i].y = 0;
		c4[i].live = false;
	}
	//炸药包设置
	for (int i = 0; i < BAG_NUM; i++)
	{
		bombbag[i].x = 0;
		bombbag[i].y = 0;
		bombbag[i].alive = false;
		bombbag[i].isbagbomed = false;
		bombbag[i].r = BAG_EXPLOSION_RADIUS;      //爆炸半径70 可修改
		bombbag[i].createtime = 0;
	}
	//炸药包爆炸火焰设置
	for (int i = 0; i < BOMB2_NUM; i++)
	{
		bomb2[i].x = 0;
		bomb2[i].y = 0;
		bomb2[i].live = false;
		bomb2[i].frame = 0;
	}
	//导弹设置
	for (int i = 0; i < MISSILE_NUM; i++)
	{
		missile[i].x = 0;
		missile[i].y = 0;
		missile[i].alive = false;
	}
	//修理工具设置
	for (int i = 0; i < HEART_ADD_NUM; i++)
	{
		heart_add[i].x = 0;
		heart_add[i].y = 0;
		heart_add[i].alive = false;
		heart_add[i].createtime = 0;
	}
}





void loadImg()   //图片加载函数
{
	//player图片
	loadimage(&plane[0], "./image/114.png");
	loadimage(&plane[1], "./image/all_musk.png");    //掩码图 纯白色  解决图片重叠问题 // 这jb东西花了我两个多小时搞懂 无语了
	//子弹图片
	loadimage(&bull[0], "./image/bullet.png");
	loadimage(&bull[1], "./image/bullet_musk.png");
	//敌机图片
	loadimage(&emy[0][0], "./image/enemy1.png");
	loadimage(&emy[0][1], "./image/all_musk.png");
	loadimage(&emy[1][0], "./image/enemy2.png");
	loadimage(&emy[1][1], "./image/all_musk.png");
	loadimage(&emy[2][0], "./image/enemy3.png");
	loadimage(&emy[2][1], "./image/all_musk.png");
	//爆炸图片
	loadimage(&explode[0], "./image/bomb2.png");
	loadimage(&explode[1], "./image/all_musk.png");
	//血量图片
	loadimage(&xin[0], "./image/heart2.png");
	loadimage(&xin[1], "./image/heart2_musk.png");
	//c4图片
	loadimage(&C4[0], "./image/c4.png");
	loadimage(&C4[1], "./image/c4_musk.png");
	//炸药包图片
	loadimage(&BOMBBAG[0], "./image/bombbag.png");
	loadimage(&BOMBBAG[1], "./image/bombbag_musk.png");
	//大范围爆炸图片 即炸药包爆炸图片
	loadimage(&explode2[0], "./image/bomb3.png");
	loadimage(&explode2[1], "./image/bomb3_musk.png");
	//导弹图片
	loadimage(&MISSILE[0], "./image/missile.png");
	loadimage(&MISSILE[1], "./image/missile_musk.png");
	//修理工具图片
	loadimage(&HEART_ADD[0], "./image/heart_add.png");
	loadimage(&HEART_ADD[1], "./image/heart_add_musk.png");

}



void gamedraw()   //绘制函数
{
	cleardevice();
	if (!isgameover)
	{
		if (!isplayerinvinciple || (GetTickCount() - invinciplestarttime) % 200 < 100)  //闪烁特效 当处于无敌时间时 100毫秒绘制一次玩家
		{
			putimage(player.x, player.y, &plane[0], NOTSRCERASE);
			putimage(player.x, player.y, &plane[1], SRCINVERT);
		}

		//绘制子弹
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (bullet[i].live)
			{
				putimage(bullet[i].x, bullet[i].y, &bull[0], NOTSRCERASE);
				putimage(bullet[i].x, bullet[i].y, &bull[1], SRCINVERT);
			}
		}
		//绘制敌人
		for (int i = 0;i < ENEMY_NUM; i++)
		{
			if (enemy[i].live)
			{
				if (enemy[i].type == 0)
				{
					putimage(enemy[i].x, enemy[i].y, &emy[0][0], NOTSRCERASE);
					putimage(enemy[i].x, enemy[i].y, &emy[0][1], SRCINVERT);
				}
				else if (enemy[i].type == 1)
				{
					putimage(enemy[i].x, enemy[i].y, &emy[1][0], NOTSRCERASE);
					putimage(enemy[i].x, enemy[i].y, &emy[1][1], SRCINVERT);
				}
				else {
					putimage(enemy[i].x, enemy[i].y, &emy[2][0], NOTSRCERASE);
					putimage(enemy[i].x, enemy[i].y, &emy[2][1], SRCINVERT);
				}

			}
		}
		//绘制敌人炸弹
		for (int i = 0; i < C4_NUM; i++)
		{
			if (c4[i].live)
			{
				putimage(c4[i].x, c4[i].y, &C4[0], NOTSRCERASE);
				putimage(c4[i].x, c4[i].y, &C4[1], SRCINVERT);
			}
		}
		//绘制导弹
		for (int i = 0; i < MISSILE_NUM;i++)
		{
			if (missile[i].alive)
			{
				putimage(missile[i].x + 16, missile[i].y + 16, &MISSILE[0], NOTSRCERASE);
				putimage(missile[i].x + 16, missile[i].y + 16, &MISSILE[1], SRCINVERT);
			}
		}
		// 绘制爆炸效果
		for (int i = 0; i < BOMB_NUM; i++)
		{
			if (bomb[i].live)
			{
				putimage(bomb[i].x - 24, bomb[i].y - 24, &explode[0], NOTSRCERASE);
				putimage(bomb[i].x - 24, bomb[i].y - 24, &explode[1], SRCINVERT);
			}
		}

		//绘制炸药包
		for (int i = 0; i < BAG_NUM; i++)
		{
			if (bombbag[i].alive && !bombbag[i].isbagbomed)
			{
				DWORD elapsedtime = GetTickCount() - bombbag[i].createtime;

				if ((elapsedtime / 200) % 2 == 0)
				{
					putimage(bombbag[i].x + 12, bombbag[i].y + 12, &BOMBBAG[0], NOTSRCERASE);
					putimage(bombbag[i].x + 12, bombbag[i].y + 12, &BOMBBAG[1], SRCINVERT);
				}

				int timeleft = BAG_FLASH_TIME - elapsedtime;
				if (timeleft > 0)
				{
					int secondsleft = (timeleft + 999) / 1000;
					char timetext[20];
					sprintf_s(timetext, "%d", secondsleft);
					settextcolor(WHITE);
					settextstyle(20, 0, "宋体");
					outtextxy(bombbag[i].x + 20, bombbag[i].y , timetext);
				}
			}
		}

		//绘制大爆炸
		for (int i = 0; i < BOMB2_NUM; i++)
		{
			if (bomb2[i].live)
			{
				putimage(bomb2[i].x - 75, bomb2[i].y - 64, &explode2[0], NOTSRCERASE);
				putimage(bomb2[i].x - 75, bomb2[i].y - 64, &explode2[1], SRCINVERT);
			}
		}

		//绘制修理工具
		for (int i = 0; i < HEART_ADD_NUM; i++)
		{
			if (heart_add[i].alive)
			{
				putimage(heart_add[i].x + 16, heart_add[i].y + 16, &HEART_ADD[0], NOTSRCERASE);
				putimage(heart_add[i].x + 16, heart_add[i].y + 16, &HEART_ADD[1], SRCINVERT);
			}
		}
		//文字
		settextcolor(WHITE);
		settextstyle(20, 0, "宋体");
		outtextxy(10, 50, "生命:");
		// 绘制heart(血量) 
		for (int i = 0; i < PLAYERLIVES; i++)
		{
			if (i < playerlives)
			{
				putimage(60 + i * 40, 45, &xin[0], NOTSRCERASE);
				putimage(60 + i * 40, 45, &xin[1], SRCINVERT);
			}
		}
		if (isplayerinvinciple)  //判断无敌状态
		{
			settextcolor(WHITE);
			outtextxy(20, 80, "无敌状态");
		}
	}
	//暂停效果
	if (ispaused)
	{
		// 显示暂停文字
		settextcolor(WHITE);
		settextstyle(40, 0, "黑体");
		outtextxy(WIDTH / 2 - 80, HEIGHT / 2 - 20, "游戏暂停");

		settextcolor(WHITE);
		settextstyle(20, 0, "宋体");
		outtextxy(WIDTH / 2 - 70, HEIGHT / 2 + 30, "按P键继续游戏");
	}
	if (!ispaused && !isgameover)
	{
		settextcolor(WHITE);
		settextstyle(20, 0, "宋体");
		outtextxy(10, 10, "按P键暂停游戏");
	}
	settextcolor(WHITE);
	//计算游戏时间
	DWORD currenttime = GetTickCount();
	int elapseseconds = (currenttime - gamestarttime) / 1000;
	int	minutes = elapseseconds / 60;
	int seconds = elapseseconds % 60;
	//击杀数
	char killText[50];
	sprintf_s(killText, "击杀: %d", enemykilled);
	outtextxy(WIDTH - 150, 10, killText);
	//游戏时间
	char timeText[50];
	sprintf_s(timeText, "时间: %02d:%02d", minutes, seconds);
	outtextxy(WIDTH - 150, 40, timeText);
	// 得分
	char scoreText[50];
	sprintf_s(scoreText, "得分: %d", score);
	outtextxy(WIDTH - 150, 70, scoreText);

}

//游戏结束菜单
void drawGameOverMenu()       
{
	// 半透明黑色背景
	setfillcolor(BLACK);
	solidrectangle(0, 0, WIDTH, HEIGHT);

	// 游戏结束标题
	settextcolor(RED);
	settextstyle(50, 0, "黑体");
	outtextxy(WIDTH / 2 - 100, 100, "游戏结束");

	// 显示最终得分
	settextcolor(YELLOW);
	settextstyle(30, 0, "宋体");

	char finalScoreText[100];
	sprintf_s(finalScoreText, "最终得分: %d", score);
	outtextxy(WIDTH / 2 - 100, 200, finalScoreText);

	char killText[100];
	sprintf_s(killText, "击杀敌机: %d", enemykilled);
	outtextxy(WIDTH / 2 - 100, 250, killText);

	// 计算游戏时间
	int elapsedSeconds = (gameendtime - gamestarttime) / 1000;
	int minutes = elapsedSeconds / 60;
	int seconds = elapsedSeconds % 60;

	char timeText[100];
	sprintf_s(timeText, "游戏时间: %02d:%02d", minutes, seconds);
	outtextxy(WIDTH / 2 - 100, 300, timeText);

	// 绘制菜单选项
	settextstyle(40, 0, "宋体");

	// "重新开始"选项
	if (selectedOption == RESTART_GAME)
	{
		settextcolor(RED);  // 选中项为红色
	}
	else
	{
		settextcolor(WHITE);
	}
	outtextxy(WIDTH / 2 - 100, 400, "重新开始");

	// "退出游戏"选项
	if (selectedOption == EXIT_GAME)
	{
		settextcolor(RED);  // 选中项为红色
	}
	else
	{
		settextcolor(WHITE);
	}
	outtextxy(WIDTH / 2 - 100, 470, "退出游戏");

	// 操作提示
	settextcolor(LIGHTGRAY);
	settextstyle(20, 0, "宋体");
	outtextxy(WIDTH / 2 - 180, 550, "使用 ↑↓(w/s)键选择, Enter 键确认");
}


// 处理游戏结束菜单输入  按键
void handleGameOverInput()
{
	static bool upKeyPressed = false;
	static bool downKeyPressed = false;
	static bool enterKeyPressed = false;

	// 上键
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))
	{
		if (!upKeyPressed)
		{
			selectedOption = RESTART_GAME;
			upKeyPressed = true;
		}
	}
	else
	{
		upKeyPressed = false;
	}

	// 下键
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
	{
		if (!downKeyPressed)
		{
			selectedOption = EXIT_GAME;
			downKeyPressed = true;
		}
	}
	else
	{
		downKeyPressed = false;
	}

	// 回车键
	if (GetAsyncKeyState(VK_RETURN))
	{
		if (!enterKeyPressed)
		{
			if (selectedOption == RESTART_GAME)
			{
				// 重新开始游戏
				gameInit();
				isgameover = false;
			}
			else if (selectedOption == EXIT_GAME)
			{
				// 退出游戏
				closegraph();
				exit(0);   //退出
			}
			enterKeyPressed = true;
		}
	}
	else
	{
		enterKeyPressed = false;
	}
}





//产生子弹
void createbullet()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x = player.x + 16;
			bullet[i].y = player.y - 16;
			bullet[i].live = true;
			//产生一个 跳出循环
			break;
		}
	}
}

//子弹移动
void bulletmove(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].live)
		{
			bullet[i].y -= speed;
			if (bullet[i].y < 0)
			{
				bullet[i].live = false;
				bullet[i].x = -50;
				bullet[i].y = 0;
			}
		}

	}
}


//产生敌机   //使用随机概率 每次5%概率出现敌机 
void createenemy_probability()
{
	if (rand() % 20 == 0)
	{
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (!enemy[i].live)
			{
				enemy[i].live = true;
				enemy[i].type = rand() % 3;
				enemy[i].x = rand() % (WIDTH - 24);
				enemy[i].y = -48;
				break;
			}
		}
	}

}

//敌机运动
void enemymove()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			int speed;
			if (enemy[i].type == 0) speed = ENEMYTYPE0SPEED;
			else if (enemy[i].type == 1) speed = ENEMYTYPE1SPEED;
			else speed = ENEMYTYPE2SPEED;
			enemy[i].y += speed;
			if (enemy[i].y > HEIGHT) {
				enemy[i].live = false;
			}
		}
	}
}

//产生炸弹函数
void createc4_probability()
{
	for (int i = 0; i < ENEMY_NUM;i++)
	{
		if (enemy[i].live && enemy[i].type == 1)    //type:0-运输机,1-轰炸机,2-战斗机
		{
			if (rand() % 400 == 0)
			{
				for (int k = 0; k < C4_NUM; k++)
				{
					if (!c4[k].live)
					{
						c4[k].live = true;
						c4[k].x = enemy[i].x + 16;
						c4[k].y = enemy[i].y + 40;
						break;
					}
				}
			}
		}
	}
}
//炸弹运动函数
void c4move(int speed)
{
	for (int i = 0; i < C4_NUM;i++)
	{
		if (c4[i].live)
		{
			c4[i].y += speed;
			if (c4[i].y > HEIGHT)
			{
				c4[i].live = false;
			}
		}
	}
}
//爆炸函数
void explosion(int x, int y)
{
	for (int i = 0; i < BOMB_NUM; i++)
	{
		if (!bomb[i].live)
		{
			bomb[i].live = true;
			bomb[i].x = x;
			bomb[i].y = y;
			bomb[i].frame = 10;
			break;
		}
	}
}
void updateexplosions(int speed)     //爆炸持续动画
{
	for (int i = 0; i < BOMB_NUM; i++)
	{
		if (bomb[i].live)
		{
			bomb[i].frame--;
			bomb[i].y += speed;
			if (bomb[i].frame <= 0)
			{
				bomb[i].live = false;  // 爆炸结束
			}
		}
	}
}
//产生炸药包函数
void createbomebag()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live && enemy[i].type == 0 && enemy[i].y >= HEIGHT/3)
		{
			if (rand() % 1000 == 0)
			{
				for (int k = 0; k < BOMB2_NUM; k++)
				{
					if (!bombbag[k].alive)
					{
						bombbag[k].alive = true;
						bombbag[k].x = enemy[i].x + 4;
						bombbag[k].y = enemy[i].y + 12;
						bombbag[k].isbagbomed = false;
						bombbag[k].createtime = GetTickCount();
						break;
					}
				}
			}
		}
	}
}
//炸药包移动函数
void bagmove(int speed)
{
	for (int i = 0; i < BAG_NUM;i++)
	{
		if (bombbag[i].alive)
		{
			bombbag[i].y += speed;
		}
	}
}
//炸药包爆炸函数
void explosion2(int x , int y)    
{
	for (int i = 0; i < BOMB2_NUM; i++)
	{
		if (!bomb2[i].live)
		{
			bomb2[i].live = true;
			bomb2[i].x = x;
			bomb2[i].y = y;
			bomb2[i].frame = 20;
			break;   //只要一个爆炸火花即可
		}
	}
}
//爆炸2持续动画
void updateexplosions2(int speed)     //可以选择性去掉参数  主要还是和小炸弹函数对应
{ 
	for (int i = 0; i < BOMB2_NUM; i++)
	{
		if (bomb2[i].live)
		{
			bomb2[i].frame--;
			bomb2[i].y += speed;
			if (bomb2[i].frame <= 0)
			{
				bomb2[i].live = false;
			}
		}
	}
}

//修理工具产生函数
void createfix_possibility()
{
	for (int i = 0; i < HEART_ADD_NUM; i++) {
		if (!heart_add[i].alive)
		{
			if (rand() % 500 == 0 )
			{
				heart_add[i].alive = true;
				heart_add[i].x = rand() % 400 + 40;
				heart_add[i].y = rand() % 450 + 200;
				heart_add[i].createtime = GetTickCount();
				break;
			}
		}
	}
}
//修理工具移动函数
void fixmove()
{
	for (int i = 0; i < HEART_ADD_NUM; i++)
	{
		if (heart_add[i].alive)
		{
			heart_add[i].y += FIX_SPEED;
			if (heart_add[i].y > HEIGHT) heart_add[i].alive = false;
			DWORD currenttime = GetTickCount();
			if (currenttime - heart_add[i].createtime > FIX_DURATION_TIME)
				heart_add[i].alive = false;
		}
	}
}

//子弹击中函数
void bullethit()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			continue;
		}
		else {
			for (int k = 0; k < BULLET_NUM; k++)  //重叠 ，死亡；
			{
				//敌机
				int el = enemy[i].x + 3;   //enemyleft 
				int er = enemy[i].x + 45;   //enemyright
				int eu = enemy[i].y;       //enemyup
				int ed = enemy[i].y + 42;    //enemydown
				//子弹
				int bl = bullet[k].x + 5;
				int br = bullet[k].x + 11;
				int bu = bullet[k].y + 2;
				int bd = bullet[k].y + 16;

				if (bl > er || br < el || bu > ed || bd < eu) continue;
				else {
					bullet[k].live = false;
					bullet[k].x = -50;
					bullet[k].y = 0;
					enemy[i].live = false;
					explosion(enemy[i].x + 24, enemy[i].y + 24);
					score += SCOREADD;
					enemykilled++;
					break;
				}
			}
		}
	}
}

//炸药包爆炸计算及玩家受击函数
void bombbaghitted()
{
	for (int i = 0; i < BAG_NUM; i++)
	{
		if (bombbag[i].alive && !bombbag[i].isbagbomed)
		{
			DWORD currenttime = GetTickCount();
			DWORD elapsedtime = currenttime - bombbag[i].createtime;

			if (elapsedtime > BAG_FLASH_TIME)
			{
				//触发爆炸
				explosion2(bombbag[i].x + 12, bombbag[i].y + 12);
				// 创建多个爆炸点，模拟大范围爆炸
				for (int j = 0; j < 3;j++)
				{
					int offsetX = (rand() % 61) - 30;  // -30 到 30
					int offsetY = (rand() % 61) - 30;  // -30 到 30
					explosion(bombbag[i].x + 12 + offsetX, bombbag[i].y + 12 + offsetY);
				}
				//判断玩家是否受击
				//炸弹中心
				int bcx = bombbag[i].x + 12;
				int bcy = bombbag[i].y + 12;

				if (!isplayerinvinciple)
				{
					int pcx = player.x + 24;    //player center x
					int pcy = player.y + 24;	//player conter y


					int dx = pcx - bcx;
					int dy = pcy - bcy;
					int dist = sqrt(dx * dx + dy * dy);

					//判断距离
					if (dist < BAG_EXPLOSION_RADIUS)
					{
						playerlives--;
						if (playerlives > 0)
						{
							isplayerinvinciple = true;
							invinciplestarttime = GetTickCount();
						}
						else
						{
							player.live = false;
							explosion(player.x + 24, player.y + 24);
							isgameover = true;
							gameendtime = GetTickCount();
						}
					}
				}
				for (int k = 0; k < ENEMY_NUM; k++)
				{
					if (enemy[k].live)
					{
						int ecx = enemy[k].x + 24;     //enemy center x
						int ecy = enemy[k].y + 24;     //enemy center y
						int dx = ecx - bcx;
						int dy = ecy - bcy;
						int dist2 = sqrt(dx * dx + dy * dy);
						if (dist2 < BAG_EXPLOSION_RADIUS)
						{
							enemy[k].live = false;
							explosion(enemy[k].x + 24, enemy[k].y + 24);
							score += SCOREADD;
							enemykilled++;
						}
					}
				}
				for (int j = 0; j < C4_NUM; j++)
				{
					if (c4[j].live)
					{
						int ccx = c4[j].x + 12;
						int ccy = c4[j].y + 12;
						int dx = ccx - bcx;
						int dy = ccy - bcy;
						int dist3 = sqrt(dx * dx + dy * dy);
						if (dist3 < BAG_EXPLOSION_RADIUS)
						{
							c4[j].live = false;
							explosion(c4[j].x + 12, c4[j].y + 12);

						}
					}
				}
				bombbag[i].alive = false;
				bombbag[i].isbagbomed = true;

			}

		}
	}
}
//产生导弹
void createmissile_probability()
{
	for (int i = 0;i < ENEMY_NUM; i++)
	{
		if (enemy[i].live && enemy[i].type == 2 && enemy[i].y <= HEIGHT /4)
		{
			if (rand() %200 == 0)
			{
				for (int k = 0; k < MISSILE_NUM; k++)
				{
					if (!missile[k].alive)
					{
						missile[k].alive = true;
						missile[k].x = enemy[i].x + 8;
						missile[k].y = enemy[i].y + 48;
						break;
					}
				}
			}
		}
	}
}
//导弹运动函数
void missilemove()
{
	for (int i = 0; i < MISSILE_NUM;i++)
	{
		if (missile[i].alive)
		{
			if (player.x -8 > missile[i].x ) missile[i].x += MISSILE_X_SPEED;
			else missile[i].x -= MISSILE_X_SPEED;
			missile[i].y += MISSILE_Y_SPEED;
			if (missile[i].y > HEIGHT) missile[i].alive = false;
		}
	}
}

//操作函数
DWORD t1 = 0, t2 = 0;
void playermove(int speed)
{
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) {
		if (player.y > 0)
			player.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) {
		if (player.y < HEIGHT - 48)
			player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) {
		if (player.x > -24)
			player.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) {
		if (player.x < WIDTH - 24)
			player.x += speed;
	}
	if (GetAsyncKeyState(VK_SPACE) && (t2 - t1 > BULLET_FREQUENCY)) {

		createbullet();
		t1 = t2;
	}
	t2 = GetTickCount();
}


//玩家受击函数 检查碰撞
void checkplayercollision()
{
	if (isplayerinvinciple) return;
	else {
		for (int i = 0; i < ENEMY_NUM; i++)  //开始判断是否碰撞
		{
			if (!enemy[i].live) continue;
			//玩家碰撞体积
			int pl = player.x + 8;
			int pr = player.x + 40;
			int pu = player.y + 4;
			int pd = player.y + 44;
			//敌机碰撞体积
			int el = enemy[i].x + 4;
			int er = enemy[i].x + 44;
			int eu = enemy[i].y + 2;
			int ed = enemy[i].y + 46;

			if (pl > er || pr < el || pu > ed || pd < eu) continue;
			else {
				explosion(enemy[i].x + 24, enemy[i].y + 24);
				enemy[i].live = false;
				playerlives--;
				if (playerlives > 0) //还有生命值剩余
				{
					//触发无敌时间
					isplayerinvinciple = true;
					invinciplestarttime = GetTickCount();
					//闪烁功能在绘制图片函数实现
				}
				else {
					player.live = false;
					explosion(player.x + 24, player.y + 24);
					isgameover = true;
					gameendtime = GetTickCount();
				}
				break;
			}

		}
	}
}
//判断玩家 炸弹受击函数
void checkplayercollision2()
{
	if (isplayerinvinciple) return;
	else {
		for (int i = 0; i < C4_NUM; i++)  //开始判断是否与炸弹碰撞
		{
			if (!c4[i].live) continue;
			//玩家碰撞体积
			int pl = player.x + 8;
			int pr = player.x + 40;
			int pu = player.y + 4;
			int pd = player.y + 44;
			//炸弹碰撞体积
			int cl = c4[i].x;
			int cr = c4[i].x + 16;
			int cu = c4[i].y + 5;
			int cd = c4[i].y + 21;

			if (pl > cr || pr < cl || pu > cd || pd < cu) continue;
			else {
				explosion(c4[i].x + 12, c4[i].y + 12);
				c4[i].live = false;
				playerlives--;
				if (playerlives > 0) //还有生命值剩余
				{
					//触发无敌时间
					isplayerinvinciple = true;
					invinciplestarttime = GetTickCount();
				}
				else {
					player.live = false;
					explosion(player.x + 24, player.y + 24);
					isgameover = true;
					gameendtime = GetTickCount();
				}
				break;
			}

		}
	}
}

//玩家受击函数 检查导弹碰撞
void checkplayercollision3()
{
	if (isplayerinvinciple) return;
	else {
		for (int i = 0;i < MISSILE_NUM; i++)
		{
			if (!missile[i].alive) continue;
			int pl = player.x + 8;
			int pr = player.x + 40;
			int pu = player.y + 4;
			int pd = player.y + 44;

			int ml = missile[i].x + 13;
			int mr = missile[i].x + 19;
			int mu = missile[i].y;
			int md = missile[i].y + 32;

			if (pl > mr || pr < ml || pu > md || pd < mu) continue;
			else {
				explosion(missile[i].x + 16, missile[i].y + 16);
				missile[i].alive = false;
				playerlives--;
				if (playerlives > 0) //还有生命值剩余
				{
					//触发无敌时间
					isplayerinvinciple = true;
					invinciplestarttime = GetTickCount();
				}
				else {
					player.live = false;
					explosion(player.x + 24, player.y + 24);
					isgameover = true;
					gameendtime = GetTickCount();
				}
				break;
			}

		}
	}
}
//判断玩家修理工具函数碰撞
void checkplayercollision4()
{
	for (int i = 0; i < HEART_ADD_NUM; i++)
	{
		if (!heart_add[i].alive) continue;
		int pl = player.x + 8;
		int pr = player.x + 40;
		int pu = player.y + 4;
		int pd = player.y + 44;

		int fl = heart_add[i].x;
		int fr = heart_add[i].x + 32;
		int fu = heart_add[i].y;
		int fd = heart_add[i].y + 32;
		if (pl > fr || pr < fl || pu > fd || pd < fu) continue;
		else {
			heart_add[i].alive = false;
			if(playerlives < 3)
			playerlives++;
		}
	}
}

//更新无敌时间状态函数
void updateinvinciplestate()
{
	if (isplayerinvinciple)
	{
		DWORD currenttime = GetTickCount();
		if (currenttime - invinciplestarttime > INVINCIPLE_DURATION)
		{
			isplayerinvinciple = false;   //无敌时间结束
		}
	}
}

//暂停游戏逻辑
bool pausekeypressed = false;
void gamepause()
{
	if (GetAsyncKeyState('P'))
	{
		if (!pausekeypressed)
		{
			ispaused = !ispaused;
			pausekeypressed = true;
		}
	}
	else {
		pausekeypressed = false;
	}

}


int main()
{
	srand(time(0));
	initgraph(WIDTH, HEIGHT, 1);
	loadImg();
	gameInit();
	BeginBatchDraw();  //双缓存 防止一闪一闪


	while (1)
	{
		if (isgameover)
		{
			handleGameOverInput();
			cleardevice();
			drawGameOverMenu();
			FlushBatchDraw();
			Sleep(10);
			continue;  // 跳过游戏逻辑更新
		}
		gamepause();

		if (ispaused) {
			gamedraw();
			FlushBatchDraw();
			Sleep(10);
			continue;    //如果暂停了 直接跳过下面
		}
		if (!isgameover)
		{
			playermove(PLAYERSPEED);     //玩家运动
			bulletmove(BULLETSPEED);     //子弹运动
			updateexplosions(ENEMYSPEED);  //更新爆炸
			updateexplosions2(BAG_SPEED);		//更新爆炸2
			createenemy_probability();   //产生敌人
			createc4_probability();      //产生炸弹
			createbomebag();			//产生炸药包
			createmissile_probability();//产生导弹
			createfix_possibility();   //产生修理工具
			enemymove();				//敌人移动
			c4move(C4SPEED);			//炸弹移动
			bagmove(BAG_SPEED);         //炸药包移动
			missilemove();		      //导弹移动
			fixmove();                   //修理工具移动
			bullethit();				//敌人受击
			bombbaghitted();			//炸弹包伤害函数
			checkplayercollision();    //检查敌机碰撞
			checkplayercollision2();    //检查炸弹碰撞
			checkplayercollision3();     //检查导弹碰撞
			checkplayercollision4();     //检查修理工具碰撞
			updateinvinciplestate();     //检查无敌状态
		}
		gamedraw();     //只要游戏存在 描绘函数始终作用
		FlushBatchDraw();
		Sleep(10);

	}
	EndBatchDraw();
	closegraph();
	return 0;
}
