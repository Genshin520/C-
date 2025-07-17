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
#define WINDOW_WIDTH		1280		// ���ڿ��
#define WINDOW_HEIGHT		720		// ���ڸ߶�
#define BACKGROUND_SPEED	3	   // ����ͼ�ٶ�
#define PLAYER_WIDTH		80	   // ��ҿ��
#define PLAYER_HEIGHT		90	   // ��Ҹ߶�
#define PLAYER_SPEED		20	   // ����ƶ��ٶ�
#define BULLET_WIDTH		20		// �ӵ����
#define BULLET_HEIGHT		20		// �ӵ��߶�
#define BULLET_SPEED		1		// �ӵ��ٶ�
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

// �ж���ײ����
inline bool CheckCollision(int x1, int y1, int x2, int y2, int w2, int h2)
{
	return x1 >= x2 && x1 <= x2 + w2 && y1 >= y2 && y1 <= y2 + h2;
}

// �ж��ӵ��Ƿ���Ҫ��ɾ��
inline bool CheckBulletDeletion(const Bullet& bullet)
{
	return !bullet.valid;
}

// ��Ϸ����
class Game
{
public:
	Game();
	~Game();
	void PlayMusic();					// ��������
	void Update();						// ��Ϸ���߼�
	HWND GetWindowHandle();				// ��ȡ���ھ��

public:
	bool is_quit;						// ��Ϸ�Ƿ��˳�
	int score;							// ��ҵ÷�
	int life;							// ���ʣ������
	int x, y;							// ���λ������

private:
	// ���ư�͸����ͼ
	static void Transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg);
	void HandleEvent();					// ������������¼�
	void DrawBackground();				// ���Ʊ���ͼ
	void DrawBullet();					// �����ӵ�
	void DrawPlayer();					// �������
	void DrawEnemy();					// ���Ƶ���
	void DrawScore();					// ������ҵ÷�
	void DrawLife();					// �������ʣ������ֵ
	void Fire();						// ������ҿ���
	void GenerateEnemy();				// ������ɵ���

private:
	int timer;							// ��ʱ�������Կ���
	int delay;							// �����ӵ���ʱ�����
	HWND window_handle;					// ���ھ��
	Enemy* current_enemy;				// ��ǰ���˶���
	static vector<Bullet> bullet_list;	// �ӵ������б�
	int baseline_background;			// ����ͼ��Ч���ƻ���
	IMAGE imageBackground;
	IMAGE imagePlayer1;
	IMAGE imagePlayer2;
	IMAGE imageBulletPalyer;
	IMAGE imageBulletEnemy;
	IMAGE imageLife;

	friend class Enemy;					// ���õ�����Ϊ��Ԫ���������˽�г�Ա
	friend class EasyEnemy;
	friend class MediumEnemy;
	friend class HardEnemy;
};

// ���˻���
class Enemy
{
public:
	Enemy(Game* game);
	~Enemy();
	void Draw();			// �����ͼ
	void Update();

public:
	EnemyType type;			// �л��Ѷ�����

private:
	Game* game;				// ��Ϸ����

protected:
	int life;				// ʣ������
	int speed;				// �ƶ��ٶ�
	int delay;				// �����ӵ���ʱ�����
	int timer;				// ��ʱ�������Կ���
	int x, y;				// ���˵�λ��
	int width, height;		// ���˵Ŀ�Ⱥ͸߶�
	IMAGE image;			// �л���ͼ

protected:
	virtual void Fire();	// ������

	friend class Game;
};

// �򵥵�����
class EasyEnemy : public Enemy
{
public:
	EasyEnemy(Game* game);
	~EasyEnemy();
protected:
	void Fire();			// ��д���࿪����

};

// �еȵ�����
class MediumEnemy : public Enemy
{
public:
	MediumEnemy(Game* game);
	~MediumEnemy();

protected:
	void Fire();

};

// ���ѵ�����
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

	// ������Ϸ����
	Game game;
	game.PlayMusic();	// ��ʼ���ű�������

	BeginBatchDraw();

	// �����Ϸ���˳�����ѭ��ִ��
	while (!game.is_quit)
	{
		game.Update();		// ������Ϸ��Ҫ�߼�

		FlushBatchDraw();	// ִ��δ��ɵĻ�������

		Sleep(1);			// �ʵ���ʱ��ֹ֡�ʹ���
	}

	// ����������ͼģʽ
	EndBatchDraw();

	// ������ҵ÷ֵó�����
	if (game.score < 10)
		MessageBox(game.GetWindowHandle(), _T("�ȼ�������"), _T("��Ϸ����"), 0);
	else if (game.score < 20)
		MessageBox(game.GetWindowHandle(), _T("�ȼ���ר��"), _T("��Ϸ����"), 0);
	else
		MessageBox(game.GetWindowHandle(), _T("�ȼ�����ʦ"), _T("��Ϸ����"), 0);

	return 0;
}

Game::Game()
{
	window_handle = initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, EW_SHOWCONSOLE);
	SetWindowText(window_handle, _T("ԭ��ɻ���ս"));		// ������Ϸ���ڱ���
	setbkmode(TRANSPARENT);		// ����Ĭ�ϱ���ɫΪ͸��

	is_quit = false;
	score = 0;		// ��ʼ����ҵ÷�Ϊ 0
	life = 10;		// ��ʼ�����ʣ������Ϊ 10
	current_enemy = nullptr;
	baseline_background = 0;
	x = (WINDOW_WIDTH - PLAYER_WIDTH) / 2, y = WINDOW_HEIGHT - 200;	// ��ʼ���������Ļ�м俿�µ�λ��
	timer = 0;
	delay = 40;

	// ��������Ҫ��ͼƬ�ز�
	loadimage(&imageBackground, _T("resources/images/background.png"));
	loadimage(&imagePlayer1, _T("resources/images/me1.png"));
	loadimage(&imagePlayer2, _T("resources/images/me1.png"));
	loadimage(&imageBulletPalyer, _T("resources/images/bullet1.png"));
	loadimage(&imageBulletEnemy, _T("resources/images/bullet2.png"));
	loadimage(&imageLife, _T("resources/images/life.png"));

	// ������ɵ���
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
	// ���±���ͼ���ߣ����ƶ�����ͼ
	baseline_background += BACKGROUND_SPEED;
	if (baseline_background > WINDOW_HEIGHT)
		baseline_background = 0;

	// ���¼�ʱ��
	timer = ++timer % delay;
	// �������ʱ������ʱ�䣬�����ӵ�
	if (timer == 0) Fire();

	// �ж��ӵ���ײ�Լ��Ƿ�ɳ��߽�
	for (size_t i = 0; i < bullet_list.size(); i++)
	{
		// ����ӵ�����Ϊ����ӵ������ж��Ƿ�����˷�����ײ����֮���ж��Ƿ�����ҷ�����ײ
		if (bullet_list[i].type == PLAYER)
		{
			if (CheckCollision(bullet_list[i].x, bullet_list[i].y,
				current_enemy->x, current_enemy->y, current_enemy->width, current_enemy->height))
			{
				current_enemy->life--;
				bullet_list[i].valid = false;
			}
			// �ж��ӵ��Ƿ�ɳ��߽�
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
			// �ж��ӵ��Ƿ�ɳ��߽�
			else if (bullet_list[i].y > WINDOW_HEIGHT)
			{
				bullet_list[i].valid = false;
			}
		}
	}

	// ������ʣ������������˳���Ϸ
	if (life < 0) is_quit = true;

	// ɾ��������ײ����Ч���ӵ�
	bullet_list.erase(remove_if(bullet_list.begin(), bullet_list.end(), CheckBulletDeletion), bullet_list.end());

	// �����ӵ�λ��
	for (size_t i = 0; i < bullet_list.size(); i++)
	{
		// ����ӵ�����Ϊ����ӵ������ӵ����Ϸ��У���֮�ӵ����·���
		if (bullet_list[i].type == PLAYER)
		{
			bullet_list[i].y -= 2 * BULLET_SPEED;
		}
		else
		{
			bullet_list[i].y += 4 * BULLET_SPEED;;
		}
	}

	// ����л���������������ҵ÷ֲ������µĵл�
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

	// ����л��ɳ���Ļ���������ҵ÷ֲ������µ�ս��
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

	// ���µл�����
	current_enemy->Update();

	HandleEvent();			// �����¼�
	DrawBackground();		// ���Ƶ�ͼ����
	DrawBullet();			// �����ӵ�
	DrawPlayer();			// �������
	DrawEnemy();			// ���Ƶ���
	DrawScore();			// ������ҵ÷�
	DrawLife();				// �������ʣ������
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
		case 72:					// �ϼ�
			y -= PLAYER_SPEED;
			break;
		case 's':
		case 'S':
		case 80:					// �¼�
			y += PLAYER_SPEED;
			break;
		case 'a':
		case 'A':
		case 75:					// ���
			x -= PLAYER_SPEED;
			break;
		case 'd':
		case 'D':
		case 77:					// �Ҽ�
			x += PLAYER_SPEED;
			break;
		default:
			break;
		}
	}

	// �жϷ�ֹ����Ƴ���Ļ��
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
		// ����ӵ�����Ϊ����ӵ������������ӵ���ͼ����֮���Ƶл��ӵ���ͼ
		if (bullet_list[i].type == PLAYER)
			Transparentimage(nullptr, bullet_list[i].x, bullet_list[i].y, &imageBulletPalyer);
		else
			Transparentimage(nullptr, bullet_list[i].x, bullet_list[i].y, &imageBulletEnemy);
	}
}

void Game::DrawPlayer()
{
	// ���ݶ�ʱ������������������ͼʵ�ֶ���Ч��
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
	_stprintf_s(s, _T("��ҵ÷֣�%d"), score);
	settextstyle(25, 0, _T("����"));
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
	// ���������ӵ�����
	Bullet bullet1, bullet2;

	// ����һ���ӵ����õ���ҵ���ǰ��
	bullet1.x = x;
	bullet1.y = y;
	bullet1.type = PLAYER;

	// ���ڶ����ӵ����õ���ҵ���ǰ��
	bullet2.x = x + PLAYER_WIDTH - BULLET_WIDTH;
	bullet2.y = y;
	bullet2.type = PLAYER;

	// �������ӵ���ӵ��ӵ�������
	bullet_list.push_back(bullet1);
	bullet_list.push_back(bullet2);
}

void Game::GenerateEnemy()
{
	// �ͷŵ�ǰ�ĵ��˶���
	delete current_enemy;

	// ��������µĵ���
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
	// ������ʼ��
	DWORD* dst = GetImageBuffer(dstimg);
	DWORD* src = GetImageBuffer(srcimg);
	int src_width = srcimg->getwidth();
	int src_height = srcimg->getheight();
	int dst_width = (dstimg == NULL ? getwidth() : dstimg->getwidth());
	int dst_height = (dstimg == NULL ? getheight() : dstimg->getheight());

	// ������ͼ��ʵ�ʳ���
	int iwidth = (x + src_width > dst_width) ? dst_width - x : src_width;		// �������ұ߽�
	int iheight = (y + src_height > dst_height) ? dst_height - y : src_height;	// �������±߽�
	if (x < 0) { src += -x;				iwidth -= -x;	x = 0; }				// ��������߽�
	if (y < 0) { src += src_width * -y;	iheight -= -y;	y = 0; }				// �������ϱ߽�

	// ������ͼ��ʼλ��
	dst += dst_width * y + x;

	// ʵ��͸����ͼ
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
	// ���µл�λ��
	y += speed;

	// ���¼�ʱ��
	timer = ++timer % delay;
	// �������ʱ������ʱ�䣬�����ӵ�
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
	// �����ӵ�����
	Bullet bullet;

	// �����ӵ�λ��
	bullet.x = x + (width - BULLET_WIDTH) / 2;
	bullet.y = y + height;
	bullet.type = ENEMY;

	// ���ӵ���ӵ��ӵ�������
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
	// ���������ӵ�����
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
	// ���������ӵ�����
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