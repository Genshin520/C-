#ifndef _MAIN_H_
#define _MAIN_H_
#include <graphics.h>
#include <mmsystem.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include<stdio.h>
using namespace std;
#pragma comment(lib, "Winmm.lib")
#define WINDOW_WIDTH		1280		// 窗口宽度
#define WINDOW_HEIGHT		720		// 窗口高度
#define BACKGROUND_SPEED	3	   // 背景图速度
#define PLAYER_WIDTH		80	   // 玩家宽度
#define PLAYER_HEIGHT		90	   // 玩家高度
#define PLAYER_SPEED		20	   // 玩家移动速度
#define BULLET_WIDTH		20		// 子弹宽度
#define BULLET_HEIGHT		20		// 子弹高度
#define BULLET_SPEED		1		// 子弹速度
class Game;
class Enemy;
enum EnemyType
{
	EASY,
	MEDIUM,
	HARD,
	UNKNOW
};

enum BulletType
{
	PLAYER,
	ENEMY
};

struct Bullet
{
	int x;
	int y;
	BulletType type;
	bool valid = true;
};

// 判断碰撞函数
inline bool CheckCollision(int x1, int y1, int x2, int y2, int w2, int h2)
{
	return x1 >= x2 && x1 <= x2 + w2 && y1 >= y2 && y1 <= y2 + h2;
}

// 判断子弹是否需要被删除
inline bool CheckBulletDeletion(const Bullet& bullet)
{
	return !bullet.valid;
}

// 游戏主类
class Game
{
public:
	Game();
	~Game();
	void PlayMusic();					// 播放音乐
	void Update();						// 游戏主逻辑
	HWND GetWindowHandle();				// 获取窗口句柄

public:
	bool is_quit;						// 游戏是否退出
	int score;							// 玩家得分
	int life;							// 玩家剩余生命
	int x, y;							// 玩家位置坐标

private:
	// 绘制半透明贴图
	static void Transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg);
	void HandleEvent();					// 处理玩家输入事件
	void DrawBackground();				// 绘制背景图
	void DrawBullet();					// 绘制子弹
	void DrawPlayer();					// 绘制玩家
	void DrawEnemy();					// 绘制敌人
	void DrawScore();					// 绘制玩家得分
	void DrawLife();					// 绘制玩家剩余生命值
	void Fire();						// 处理玩家开火
	void GenerateEnemy();				// 随机生成敌人

private:
	int timer;							// 计时器，用以开火
	int delay;							// 发射子弹计时器间隔
	HWND window_handle;					// 窗口句柄
	Enemy* current_enemy;				// 当前敌人对象
	static vector<Bullet> bullet_list;	// 子弹数组列表
	int baseline_background;			// 背景图动效绘制基线
	IMAGE imageBackground;
	IMAGE imagePlayer1;
	IMAGE imagePlayer2;
	IMAGE imageBulletPalyer;
	IMAGE imageBulletEnemy;
	IMAGE imageLife;

	friend class Enemy;					// 设置敌人类为友元，方便访问私有成员
	friend class EasyEnemy;
	friend class MediumEnemy;
	friend class HardEnemy;
};

// 敌人基类
class Enemy
{
public:
	Enemy(Game* game);
	~Enemy();
	void Draw();			// 处理绘图
	void Update();

public:
	EnemyType type;			// 敌机难度类型

private:
	Game* game;				// 游戏对象

protected:
	int life;				// 剩余生命
	int speed;				// 移动速度
	int delay;				// 发射子弹计时器间隔
	int timer;				// 计时器，用以开火
	int x, y;				// 敌人的位置
	int width, height;		// 敌人的宽度和高度
	IMAGE image;			// 敌机贴图

protected:
	virtual void Fire();	// 处理开火

	friend class Game;
};

// 简单敌人类
class EasyEnemy : public Enemy
{
public:
	EasyEnemy(Game* game);
	~EasyEnemy();
protected:
	void Fire();			// 重写基类开火函数

};

// 中等敌人类
class MediumEnemy : public Enemy
{
public:
	MediumEnemy(Game* game);
	~MediumEnemy();

protected:
	void Fire();

};

// 困难敌人类
class HardEnemy : public Enemy
{
public:
	HardEnemy(Game* game);
	~HardEnemy();
protected:
	void Fire();

};

#endif

vector<Bullet> Game::bullet_list;

int main(int argc, char** argv)
{

	// 定义游戏对象
	Game game;
	game.PlayMusic();	// 开始播放背景音乐

	BeginBatchDraw();

	// 如果游戏不退出，则循环执行
	while (!game.is_quit)
	{
		game.Update();		// 更新游戏主要逻辑

		FlushBatchDraw();	// 执行未完成的绘制任务

		Sleep(1);			// 适当延时防止帧率过高
	}

	// 结束批量绘图模式
	EndBatchDraw();

	// 根据玩家得分得出评级
	if (game.score < 10)
		MessageBox(game.GetWindowHandle(), _T("等级：新手"), _T("游戏结束"), 0);
	else if (game.score < 20)
		MessageBox(game.GetWindowHandle(), _T("等级：专家"), _T("游戏结束"), 0);
	else
		MessageBox(game.GetWindowHandle(), _T("等级：大师"), _T("游戏结束"), 0);

	return 0;
}

Game::Game()
{
	window_handle = initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, EW_SHOWCONSOLE);
	SetWindowText(window_handle, _T("原神飞机大战"));		// 设置游戏窗口标题
	setbkmode(TRANSPARENT);		// 设置默认背景色为透明

	is_quit = false;
	score = 0;		// 初始化玩家得分为 0
	life = 10;		// 初始化玩家剩余生命为 10
	current_enemy = nullptr;
	baseline_background = 0;
	x = (WINDOW_WIDTH - PLAYER_WIDTH) / 2, y = WINDOW_HEIGHT - 200;	// 初始化玩家在屏幕中间靠下的位置
	timer = 0;
	delay = 40;

	// 加载所需要的图片素材
	loadimage(&imageBackground, _T("resources/images/background.png"));
	loadimage(&imagePlayer1, _T("resources/images/me1.png"));
	loadimage(&imagePlayer2, _T("resources/images/me1.png"));
	loadimage(&imageBulletPalyer, _T("resources/images/bullet1.png"));
	loadimage(&imageBulletEnemy, _T("resources/images/bullet2.png"));
	loadimage(&imageLife, _T("resources/images/life.png"));

	// 随机生成敌人
	GenerateEnemy();
}

Game::~Game()
{
	closegraph();
}

void Game::PlayMusic()
{
	mciSendString(_T("open resources/audio/bgm.mp3 alias mymusic"), NULL, 0, NULL);
	mciSendString(_T("play mymusic from 0"), NULL, 0, NULL);
	mciSendString(_T("setaudio mymusic volume to 20"), NULL, 0, NULL);
}

void Game::Update()
{
	// 更新背景图基线，即移动背景图
	baseline_background += BACKGROUND_SPEED;
	if (baseline_background > WINDOW_HEIGHT)
		baseline_background = 0;

	// 更新计时器
	timer = ++timer % delay;
	// 如果开火定时器到达时间，则发射子弹
	if (timer == 0) Fire();

	// 判断子弹碰撞以及是否飞出边界
	for (size_t i = 0; i < bullet_list.size(); i++)
	{
		// 如果子弹类型为玩家子弹，则判断是否与敌人发生碰撞，反之则判断是否与玩家发生碰撞
		if (bullet_list[i].type == PLAYER)
		{
			if (CheckCollision(bullet_list[i].x, bullet_list[i].y,
				current_enemy->x, current_enemy->y, current_enemy->width, current_enemy->height))
			{
				current_enemy->life--;
				bullet_list[i].valid = false;
			}
			// 判断子弹是否飞出边界
			else if (bullet_list[i].y + BULLET_HEIGHT < 0)
			{
				bullet_list[i].valid = false;
			}
		}
		else
		{
			if (CheckCollision(bullet_list[i].x, bullet_list[i].y,
				x, y, PLAYER_WIDTH, PLAYER_HEIGHT))
			{    
				life--;
				bullet_list[i].valid = false;
			}
			// 判断子弹是否飞出边界
			else if (bullet_list[i].y > WINDOW_HEIGHT)
			{
				bullet_list[i].valid = false;
			}
		}
	}

	// 如果玩家剩余生命清空则退出游戏
	if (life < 0) is_quit = true;

	// 删除发生碰撞后无效的子弹
	bullet_list.erase(remove_if(bullet_list.begin(), bullet_list.end(), CheckBulletDeletion), bullet_list.end());

	// 更新子弹位置
	for (size_t i = 0; i < bullet_list.size(); i++)
	{
		// 如果子弹类型为玩家子弹，则子弹向上飞行，反之子弹向下飞行
		if (bullet_list[i].type == PLAYER)
		{
			bullet_list[i].y -= 2 * BULLET_SPEED;
		}
		else
		{
			bullet_list[i].y += 4 * BULLET_SPEED;;
		}
	}

	// 如果敌机被击落则增加玩家得分并生成新的敌机
	if (current_enemy->life <= 0)
	{
		switch (current_enemy->type)
		{
		case EASY:
			score += 10;
			break;
		case MEDIUM:
			score += 20;
			break;
		case HARD:
			score += 30;
			break;
		default:
			break;
		}

		GenerateEnemy();
	}

	// 如果敌机飞出屏幕外则减少玩家得分并生成新的战机
	if (current_enemy->y > WINDOW_HEIGHT)
	{
		switch (current_enemy->type)
		{
		case EASY:
			score -= 10;
			break;
		case MEDIUM:
			score -= 20;
			break;
		case HARD:
			score -= 30;
			break;
		default:
			break;
		}

		GenerateEnemy();
	}

	// 更新敌机对象
	current_enemy->Update();

	HandleEvent();			// 处理事件
	DrawBackground();		// 绘制地图背景
	DrawBullet();			// 绘制子弹
	DrawPlayer();			// 绘制玩家
	DrawEnemy();			// 绘制敌人
	DrawScore();			// 绘制玩家得分
	DrawLife();				// 绘制玩家剩余生命
}

void Game::HandleEvent()
{
	ExMessage msg;
	while (peekmessage(&msg, EM_CHAR))
	{
		switch (msg.ch)
		{
		case 'w':
		case 'W':
		case 72:					// 上键
			y -= PLAYER_SPEED;
			break;
		case 's':
		case 'S':
		case 80:					// 下键
			y += PLAYER_SPEED;
			break;
		case 'a':
		case 'A':
		case 75:					// 左键
			x -= PLAYER_SPEED;
			break;
		case 'd':
		case 'D':
		case 77:					// 右键
			x += PLAYER_SPEED;
			break;
		default:
			break;
		}
	}

	// 判断防止玩家移出屏幕外
	if (x < 0) x = 0;
	if (x + PLAYER_WIDTH > WINDOW_WIDTH) x = WINDOW_WIDTH - PLAYER_WIDTH;
	if (y < 0) y = 0;
	if (y + PLAYER_HEIGHT > WINDOW_HEIGHT) y = WINDOW_HEIGHT - PLAYER_HEIGHT;
}

void Game::DrawBackground()
{
	Transparentimage(nullptr, 0, baseline_background - WINDOW_HEIGHT, &imageBackground);
	Transparentimage(nullptr, 0, baseline_background, &imageBackground);
}

void Game::DrawBullet()
{
	for (size_t i = 0; i < bullet_list.size(); i++)
	{
		// 如果子弹类型为玩家子弹，则绘制玩家子弹贴图，反之绘制敌机子弹贴图
		if (bullet_list[i].type == PLAYER)
			Transparentimage(nullptr, bullet_list[i].x, bullet_list[i].y, &imageBulletPalyer);
		else
			Transparentimage(nullptr, bullet_list[i].x, bullet_list[i].y, &imageBulletEnemy);
	}
}

void Game::DrawPlayer()
{
	// 根据定时器交替绘制两张玩家贴图实现动画效果
	if (timer % 2 == 0)
		Transparentimage(nullptr, x, y, &imagePlayer1);
	else
		Transparentimage(nullptr, x, y, &imagePlayer2);
}

void Game::DrawEnemy()
{
	current_enemy->Draw();
}

void Game::DrawScore()
{
	TCHAR s[512];
	_stprintf_s(s, _T("玩家得分：%d"), score);
	settextstyle(25, 0, _T("黑体"));
	settextcolor(RGB(0, 15, 15));
	outtextxy(15, 15, s);
}

void Game::DrawLife()
{
	for (int i = 0; i < life; i++)
		Transparentimage(nullptr, WINDOW_WIDTH - 70 * (i + 1), WINDOW_HEIGHT - 57, &imageLife);
}

HWND Game::GetWindowHandle()
{
	return window_handle;
}

void Game::Fire()
{
	// 生成两个子弹对象
	Bullet bullet1, bullet2;

	// 将第一颗子弹放置到玩家的左前方
	bullet1.x = x;
	bullet1.y = y;
	bullet1.type = PLAYER;

	// 将第二颗子弹放置到玩家的右前方
	bullet2.x = x + PLAYER_WIDTH - BULLET_WIDTH;
	bullet2.y = y;
	bullet2.type = PLAYER;

	// 将两颗子弹添加到子弹数组中
	bullet_list.push_back(bullet1);
	bullet_list.push_back(bullet2);
}

void Game::GenerateEnemy()
{
	// 释放当前的敌人对象
	delete current_enemy;

	// 随机生成新的敌人
	int flag = rand() % 3;
	switch (flag)
	{
	case 0:
		current_enemy = new EasyEnemy(this);
		break;
	case 1:
		current_enemy = new MediumEnemy(this);
		break;
	case 2:
		current_enemy = new HardEnemy(this);
		break;
	default:
		break;
	}
}
void Game::Transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	// 变量初始化
	DWORD* dst = GetImageBuffer(dstimg);
	DWORD* src = GetImageBuffer(srcimg);
	int src_width = srcimg->getwidth();
	int src_height = srcimg->getheight();
	int dst_width = (dstimg == NULL ? getwidth() : dstimg->getwidth());
	int dst_height = (dstimg == NULL ? getheight() : dstimg->getheight());

	// 计算贴图的实际长宽
	int iwidth = (x + src_width > dst_width) ? dst_width - x : src_width;		// 处理超出右边界
	int iheight = (y + src_height > dst_height) ? dst_height - y : src_height;	// 处理超出下边界
	if (x < 0) { src += -x;				iwidth -= -x;	x = 0; }				// 处理超出左边界
	if (y < 0) { src += src_width * -y;	iheight -= -y;	y = 0; }				// 处理超出上边界

	// 修正贴图起始位置
	dst += dst_width * y + x;

	// 实现透明贴图
	for (int iy = 0; iy < iheight; iy++)
	{
		for (int ix = 0; ix < iwidth; ix++)
		{
			int sa = ((src[ix] & 0xff000000) >> 24);
			int sr = ((src[ix] & 0xff0000) >> 16);
			int sg = ((src[ix] & 0xff00) >> 8);
			int sb = src[ix] & 0xff;
			int dr = ((dst[ix] & 0xff0000) >> 16);
			int dg = ((dst[ix] & 0xff00) >> 8);
			int db = dst[ix] & 0xff;

			dst[ix] = ((sr + dr * (255 - sa) / 255) << 16)
				| ((sg + dg * (255 - sa) / 255) << 8)
				| (sb + db * (255 - sa) / 255);
		}
		dst += dst_width;
		src += src_width;
	}
}

Enemy::Enemy(Game* game)
{
	this->game = game;
	type = UNKNOW;
	width = 0, height = 0;
	x = 0, y = 0;
	delay = 0;
	life = 0;
	speed = 0;
}

Enemy::~Enemy()
{

}

void Enemy::Fire()
{

}

void Enemy::Draw()
{
	Game::Transparentimage(nullptr, x, y, &image);
}

void Enemy::Update()
{
	// 更新敌机位置
	y += speed;

	// 更新计时器
	timer = ++timer % delay;
	// 如果开火定时器到达时间，则发射子弹
	if (timer == 0) Fire();
}

EasyEnemy::EasyEnemy(Game* game) : Enemy(game)
{
	type = EASY;
	speed = 2;
	life = 2;
	delay = 75;
	width = 57, height = 43;
	x = rand() % (WINDOW_WIDTH - width);
	y = -height;
	loadimage(&image, _T("resources/images/enemy1.png"));
}

EasyEnemy::~EasyEnemy()
{

}

void EasyEnemy::Fire()
{
	// 生成子弹对象
	Bullet bullet;

	// 设置子弹位置
	bullet.x = x + (width - BULLET_WIDTH) / 2;
	bullet.y = y + height;
	bullet.type = ENEMY;

	// 将子弹添加到子弹数组中
	Game::bullet_list.push_back(bullet);
}

MediumEnemy::MediumEnemy(Game* game) : Enemy(game)
{
	type = MEDIUM;
	speed = 2;
	life = 4;
	delay = 125;
	width = 69, height = 99;
	x = rand() % (WINDOW_WIDTH - width);
	y = -height;
	loadimage(&image, _T("resources/images/enemy2.png"));
}

MediumEnemy::~MediumEnemy()
{

}

void MediumEnemy::Fire()
{
	// 生成两个子弹对象
	Bullet bullet1, bullet2;

	bullet1.x = x;
	bullet1.y = y + height;
	bullet1.type = ENEMY;

	bullet2.x = x + width - BULLET_WIDTH;
	bullet2.y = y + height;
	bullet2.type = ENEMY;

	Game::bullet_list.push_back(bullet1);
	Game::bullet_list.push_back(bullet2);
}

HardEnemy::HardEnemy(Game* game) : Enemy(game)
{
	type = HARD;
	speed = 1;
	life = 6;
	delay = 100;
	width = 165, height = 261;
	x = rand() % (WINDOW_WIDTH - width);
	y = -height;
	loadimage(&image, _T("resources/images/enemy3_n1.png"));
}

HardEnemy::~HardEnemy()
{

}

void HardEnemy::Fire()
{
	// 生成两个子弹对象
	Bullet bullet1, bullet2, bullet3;

	bullet1.x = x;
	bullet1.y = y + height;
	bullet1.type = ENEMY;

	bullet2.x = x + (width - BULLET_WIDTH) / 2;
	bullet2.y = y + height;
	bullet2.type = ENEMY;

	bullet3.x = x + width - BULLET_WIDTH;
	bullet3.y = y + height;
	bullet3.type = ENEMY;

	Game::bullet_list.push_back(bullet1);
	Game::bullet_list.push_back(bullet2);
	Game::bullet_list.push_back(bullet3);
}