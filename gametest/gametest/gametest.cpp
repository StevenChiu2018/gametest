// gametest.cpp: 定義主控台應用程式的進入點。
//
#include "stdafx.h"
#include <iostream>
#include <cstdlib> 
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
using namespace std;

#define SIMULATION_TIMES 1000 //模擬次數
#define PLAYER_NUM 10 //遊玩人數

int table[126][10] = {
{ 1,2,3,4,5,6,7,8,9,10 },
{ 1,2,3,4,6,5,7,8,9,10 },
{ 1,2,3,4,7,5,6,8,9,10 },
{ 1,2,3,4,8,5,6,7,9,10 },
{ 1,2,3,4,9,5,6,7,8,10 },
{ 1,2,3,4,10,5,6,7,8,9 },
{ 1,2,3,5,6,4,7,8,9,10 },
{ 1,2,3,5,7,4,6,8,9,10 },
{ 1,2,3,5,8,4,6,7,9,10 },
{ 1,2,3,5,9,4,6,7,8,10 },
{ 1,2,3,5,10,4,6,7,8,9 },
{ 1,2,3,6,7,4,5,8,9,10 },
{ 1,2,3,6,8,4,5,7,9,10 },
{ 1,2,3,6,9,4,5,7,8,10 },
{ 1,2,3,6,10,4,5,7,8,9 },
{ 1,2,3,7,8,4,5,6,9,10 },
{ 1,2,3,7,9,4,5,6,8,10 },
{ 1,2,3,7,10,4,5,6,8,9 },
{ 1,2,3,8,9,4,5,6,7,10 },
{ 1,2,3,8,10,4,5,6,7,9 },
{ 1,2,3,9,10,4,5,6,7,8 },
{ 1,2,4,5,6,3,7,8,9,10 },
{ 1,2,4,5,7,3,6,8,9,10 },
{ 1,2,4,5,8,3,6,7,9,10 },
{ 1,2,4,5,9,3,6,7,8,10 },
{ 1,2,4,5,10,3,6,7,8,9 },
{ 1,2,4,6,7,3,5,8,9,10 },
{ 1,2,4,6,8,3,5,7,9,10 },
{ 1,2,4,6,9,3,5,7,8,10 },
{ 1,2,4,6,10,3,5,7,8,9 },
{ 1,2,4,7,8,3,5,6,9,10 },
{ 1,2,4,7,9,3,5,6,8,10 },
{ 1,2,4,7,10,3,5,6,8,9 },
{ 1,2,4,8,9,3,5,6,7,10 },
{ 1,2,4,8,10,3,5,6,7,9 },
{ 1,2,4,9,10,3,5,6,7,8 },
{ 1,2,5,6,7,3,4,8,9,10 },
{ 1,2,5,6,8,3,4,7,9,10 },
{ 1,2,5,6,9,3,4,7,8,10 },
{ 1,2,5,6,10,3,4,7,8,9 },
{ 1,2,5,7,8,3,4,6,9,10 },
{ 1,2,5,7,9,3,4,6,8,10 },
{ 1,2,5,7,10,3,4,6,8,9 },
{ 1,2,5,8,9,3,4,6,7,10 },
{ 1,2,5,8,10,3,4,6,7,9 },
{ 1,2,5,9,10,3,4,6,7,8 },
{ 1,2,6,7,8,3,4,5,9,10 },
{ 1,2,6,7,9,3,4,5,8,10 },
{ 1,2,6,7,10,3,4,5,8,9 },
{ 1,2,6,8,9,3,4,5,7,10 },
{ 1,2,6,8,10,3,4,5,7,9 },
{ 1,2,6,9,10,3,4,5,7,8 },
{ 1,2,7,8,9,3,4,5,6,10 },
{ 1,2,7,8,10,3,4,5,6,9 },
{ 1,2,7,9,10,3,4,5,6,8 },
{ 1,2,8,9,10,3,4,5,6,7 },
{ 1,3,4,5,6,2,7,8,9,10 },
{ 1,3,4,5,7,2,6,8,9,10 },
{ 1,3,4,5,8,2,6,7,9,10 },
{ 1,3,4,5,9,2,6,7,8,10 },
{ 1,3,4,5,10,2,6,7,8,9 },
{ 1,3,4,6,7,2,5,8,9,10 },
{ 1,3,4,6,8,2,5,7,9,10 },
{ 1,3,4,6,9,2,5,7,8,10 },
{ 1,3,4,6,10,2,5,7,8,9 },
{ 1,3,4,7,8,2,5,6,9,10 },
{ 1,3,4,7,9,2,5,6,8,10 },
{ 1,3,4,7,10,2,5,6,8,9 },
{ 1,3,4,8,9,2,5,6,7,10 },
{ 1,3,4,8,10,2,5,6,7,9 },
{ 1,3,4,9,10,2,5,6,7,8 },
{ 1,3,5,6,7,2,4,8,9,10 },
{ 1,3,5,6,8,2,4,7,9,10 },
{ 1,3,5,6,9,2,4,7,8,10 },
{ 1,3,5,6,10,2,4,7,8,9 },
{ 1,3,5,7,8,2,4,6,9,10 },
{ 1,3,5,7,9,2,4,6,8,10 },
{ 1,3,5,7,10,2,4,6,8,9 },
{ 1,3,5,8,9,2,4,6,7,10 },
{ 1,3,5,8,10,2,4,6,7,9 },
{ 1,3,5,9,10,2,4,6,7,8 },
{ 1,3,6,7,8,2,4,5,9,10 },
{ 1,3,6,7,9,2,4,5,8,10 },
{ 1,3,6,7,10,2,4,5,8,9 },
{ 1,3,6,8,9,2,4,5,7,10 },
{ 1,3,6,8,10,2,4,5,7,9 },
{ 1,3,6,9,10,2,4,5,7,8 },
{ 1,3,7,8,9,2,4,5,6,10 },
{ 1,3,7,8,10,2,4,5,6,9 },
{ 1,3,7,9,10,2,4,5,6,8 },
{ 1,3,8,9,10,2,4,5,6,7 },
{ 1,4,5,6,7,2,3,8,9,10 },
{ 1,4,5,6,8,2,3,7,9,10 },
{ 1,4,5,6,9,2,3,7,8,10 },
{ 1,4,5,6,10,2,3,7,8,9 },
{ 1,4,5,7,8,2,3,6,9,10 },
{ 1,4,5,7,9,2,3,6,8,10 },
{ 1,4,5,7,10,2,3,6,8,9 },
{ 1,4,5,8,9,2,3,6,7,10 },
{ 1,4,5,8,10,2,3,6,7,9 },
{ 1,4,5,9,10,2,3,6,7,8 },
{ 1,4,6,7,8,2,3,5,9,10 },
{ 1,4,6,7,9,2,3,5,8,10 },
{ 1,4,6,7,10,2,3,5,8,9 },
{ 1,4,6,8,9,2,3,5,7,10 },
{ 1,4,6,8,10,2,3,5,7,9 },
{ 1,4,6,9,10,2,3,5,7,8 },
{ 1,4,7,8,9,2,3,5,6,10 },
{ 1,4,7,8,10,2,3,5,6,9 },
{ 1,4,7,9,10,2,3,5,6,8 },
{ 1,4,8,9,10,2,3,5,6,7 },
{ 1,5,6,7,8,2,3,4,9,10 },
{ 1,5,6,7,9,2,3,4,8,10 },
{ 1,5,6,7,10,2,3,4,8,9 },
{ 1,5,6,8,9,2,3,4,7,10 },
{ 1,5,6,8,10,2,3,4,7,9 },
{ 1,5,6,9,10,2,3,4,7,8 },
{ 1,5,7,8,9,2,3,4,6,10 },
{ 1,5,7,8,10,2,3,4,6,9 },
{ 1,5,7,9,10,2,3,4,6,8 },
{ 1,5,8,9,10,2,3,4,6,7 },
{ 1,6,7,8,9,2,3,4,5,10 },
{ 1,6,7,8,10,2,3,4,5,9 },
{ 1,6,7,9,10,2,3,4,5,8 },
{ 1,6,8,9,10,2,3,4,5,7 },
{ 1,7,8,9,10,2,3,4,5,6 }
};

int times =0;
int killer[2];
bool vote_flag;
int game_state;
int day;
double days = 0;
int R_num, B_num;//獲勝次數
int input_team;



/*character num
1 暗殺者	夜晚時若剛好是殺手，可殺2人
2 魔術師	夜晚遭到殺手殺害時可轉移目標(只能使用一次，使用後角色牌翻開，若使用脫逃卡則能力不會發動)
3 賭徒		抽取資源時可在抽取一次(丟棄上一次所抽取的)，每回合可用一次
4 採集者	每回合可獲得1個隨機顏色素材
5 夜貓子	夜晚可以採集一次(採集資源量減半)
6 牧師		復活一個死亡對象(投票前可使用，不能復活自己)
7 小偷		偷走1人全部素材(隨機顏色)(只能使用一次)，使用後角色牌翻開
8 貴族		投票可以投2票(只能使用一次)
9 黑市商人	素材全部替換(只能使用一次，使用後角色牌翻開)(ex: 手上擁有:紅x10 藍x10 綠x10 ->把綠的換成藍的 兌換後:紅x10 藍x20 綠x0)
10 自殺炸彈客	投票前可炸死一名玩家(只能使用一次)並終止投票，自己死亡該局不能行動。
*/


int randInt(int left,int  right) {
	std::random_device rd;
	std::default_random_engine gen = std::default_random_engine(rd());
	std::uniform_int_distribution<int> dis(left, right);
	return dis(gen);

}



struct player_set {
	int number;
	bool team;
	int c_num;
};

class card {//道具
public:
	card() { escape = 0; steal = 0; };
	int escape;//脫逃器
	int steal;//小偷卡
};

class source {//資源
public:
	source() { R = 0; B = 0; G = 0; };
	int R, G, B;
	int  get_source_amount();
	void used(int, int, int);//素材使用
	void get_source(int, int, int); //獲得素材
};

int source::get_source_amount()
{
	return(this->R + this->G + this->B);
}

void source::used(int R, int G, int B) {
	this->R -= R;
	this->G -= G;
	this->B -= B;
}

void source::get_source(int r, int g, int b) {
	this->R += r;
	this->G += g;
	this->B += b;
}

class character {
public:
	character() {};
	character(int);
	int id;
	int skill_state;//0:白天採集 1:夜晚開始 2:殺手殺人 3:殺手殺人結束 4:投票前 5:投票結束
	bool flag;	
};

character::character(int id) {
	this->id = id;
	switch (id){
	case 1:
		this->skill_state = 2;
		break;
	case 2:
		this->skill_state = 3;
		break;
	case 5:
		this->skill_state = 1;
		break;
	case 6:
		this->skill_state = 4;
		break;
	case 7:
		this->skill_state = 1;
		break;
	case 8:
		this->skill_state = 4;
		break;
	case 9:
		this->skill_state = 1;
		break;
	case 10:
		this->skill_state = 4;
		break;
	default:
		this->skill_state = -1;
		break;
	}
	flag = true;
}


class player {
public:
	player() {};
	player(player_set);
	int number;
	bool team;//0:blue 1:red
	source s;
	card bag;//道具背包
	bool dead_or_live; //1:live 0:dead
	character c; //角色
	void play(bool);// 1:白天 0:晚上
	void draw();
	int vote;// 0:可投票
	void buy_card();
	void skill(int);
};

player p[PLAYER_NUM];

player::player(player_set ps) {
	this->number = ps.number;
	this->team = ps.team;
	this->c = character(ps.c_num);
	this->dead_or_live = true;
	this->vote = 0;
}

void player::buy_card() {
	
	if (this->s.G >= 3 && this->bag.escape == 0) {//脫逃器
		if (randInt(0,3) == 0) {
			this->s.G -= 3;
			this->bag.escape++;
		}
	}
	
	if (this->s.R >= 4 && this->s.G >= 4 && this->s.B >= 4 && this->bag.steal ==0) {//偷竊卡
		if (randInt(0,4) == 0) {
			this->s.R -= 4;
			this->s.G -= 4;
			this->s.B -= 4;
			this->bag.steal++;
		}
	}
}

void player::skill(int state) {
	if (this->c.flag && this->c.skill_state == state) {
		if (this->c.id == 5) {//夜貓子
			
			int x = randInt(0,2);
			
			int y = randInt(0,2);
			switch (x)  //0:R 1:G B:2
			{
			case 0:
				this->s.R += y + 1;
				break;
			case 1:
				this->s.G += y + 1;
				break;
			case 2:
				this->s.B += y + 1;
				break;
			default:
				break;
			}
		}
		else if (this->c.id == 6) {//牧師
			for (int i = 0; i < PLAYER_NUM; i++) {
				if (p[i].dead_or_live == false) {
					
					if (randInt(0,3) ==0) {//是否要救
						p[i].dead_or_live = true;
						this->c.flag = false;
						break;
					}
				}
			}
		}
		else if (this->c.id == 7) {//小偷
			int k = 10 - day;
			if (k <= 2)
				k = 2;
			
			if (randInt(0,k-1) == 0) {
				int x;//要偷的人
				while (1) {
					
					x = randInt(0,9);
					if (x != this->number)//不能是自己
						break;
				}
				
				int color = randInt(0,2);
				switch (color)
				{
				case 0:
					p[this -> number].s.R += p[x].s.R;
					p[x].s.R = 0;
				case 1:
					p[this->number].s.G += p[x].s.G;
					p[x].s.G = 0;
					
				case 2:
					p[this->number].s.B += p[x].s.B;
					p[x].s.B = 0;
					
				default:
					break;
				}
				this->c.flag = false;
			}
		}
		else if (this->c.id == 9) {//黑市商人
			int k = 20 - this->s.get_source_amount();
			if (k <= 2)
				k = 2;
			
			if (randInt(0,k-1) == 0) {
				int x, y;//x: 目標兌換顏色  y:使用兌換顏色
				if (!this->team) {//藍隊
					x = 2;
				}
				else if (this->team) {//紅隊
					x = 0;
				}
				while (1) {
					
					y = randInt(0,2);
					if (x != y)
						break;
				}
				switch (x)
				{
				case 0:
					switch (y)
					{
					case 1:
						this->s.R += this->s.G;
						this->s.G = 0;
					case 2:
						this->s.R += this->s.B;
						this->s.B = 0;
					default:
						break;
					}
				case 1:
					switch (y)
					{
					case 0:
						this->s.G += this->s.R;
						this->s.R = 0;
					case 2:
						this->s.G += this->s.B;
						this->s.B = 0;
					default:
						break;
					}
				case 2:
					switch (y)
					{
					case 0:
						this->s.B += this->s.R;
						this->s.R = 0;
					case 1:
						this->s.B += this->s.G;
						this->s.G = 0;
					default:
						break;
					}
				default:
					break;
				}
				this->c.flag = false;
			}
		}

	}
}

void player::draw() {
	int x = randInt(0,8);
	int r_p = 0;//紅色機率參數
	int g_p = 0;//綠色機率參數
	int b_p = 0;//藍色機率參數
	if (this->s.B > this->s.G && this->s.B > this->s.R) {//藍色資源偏多
		b_p = -1;
	}
	else if (this->s.G > this->s.R && this->s.G > this->s.B) {//綠色資源偏多
		g_p = -1;
	}
	else if (this->s.R > this->s.G && this->s.R > this->s.B) {//紅色資源偏多
		r_p = -1;
	}
	else {
		
		switch ((randInt(0,2)))
		{
		case 0:
			r_p = -1;
			break;
		case 1:
			g_p = -1;
			break;
		case 2:
			b_p = -1;
			break;
		default:
			break;
		}
	}
	if (!this->team) {//藍隊
		b_p += 1;
	}
	else {//紅隊
		r_p += 1;
	}

	if (x >= 0 && x <= 2 + r_p) {//紅色
		x = 0;
	}
	else if (x >= 3+r_p && x<= 5-b_p) {//綠色
		x = 1;
	}
	else if (x >= 6-b_p && x <= 8) {//藍色
		x = 2;
	}
	
	int y = randInt(0,5);
	int bonus = 0;
	if (this->c.id == 4)//採集者
		bonus = 1;
	if (this->c.id == 3) { //賭徒
		if (randInt(0,y) == 0) { //重抽(原本資源越少重抽率越高)
			y = randInt(0,5);
		}
	}
	//cout << this->number << ": \t";
	switch (x)  //0:R 1:G B:2
	{
	case 0:
		//cout << "R" << y + 1 + bonus << endl;
		this->s.R += y + 1 + bonus;
		break;
	case 1:
		//cout << "G" << y + 1 + bonus << endl;
		this->s.G += y + 1 + bonus;
		break;
	case 2:
		//cout << "B" << y + 1 + bonus << endl;
		this->s.B += y + 1 + bonus;
		break;
	default:
		break;
	}
}

void gameend(bool team) {
	fstream file;
	file.open( "table"+ to_string(input_team) +".txt", ios::app);
	if (!file)
		cout << "opne file error\n";
	else {
		file << "場次:" << times << "\t" << "勝利隊伍:";
		if (team) {
			file << "紅";
		}
		else {
			file << "藍";
		}
		file << "\t" << "天數:" << day << endl;
	}
	if (team == true) {
		R_num++;
	}
	else if(team == false){
		B_num++;
	}
	days += day;
	times++;
	file.close();
}

void player::play(bool t) {
	if (t) {//白天
		if (this->dead_or_live == 0)
		{
			if (this->c.id == 4)//採集者死亡也可獲得物資
			{
				int x = randInt(0,2);
				switch (x)  //0:R 1:G B:2
				{
				case 0:
					this->s.R += 1;
					break;
				case 1:
					this->s.G += 1;
					break;
				case 2:
					this->s.B += 1;
					break;
				default:
					break;
				}
			}
			return;
		}
		else
		{
			this->draw();//採資源
			this->buy_card();//換道具
		}
	}
	else {//晚上
	}
}


void gamestart(vector<player_set> ps) {//初始化p
	for (int i = 0; i < 10; i++) {
		player tmp(ps[i]);
		p[i] = tmp;
	}
	
	killer[0] = randInt(0,9);
	
	int t = randInt(0,4);
	if (!p[killer[0]].team) {//藍隊
		killer[1] = 5 + t;
	}
	else {
		killer[1] = t;
	}
}



void killer_exchange() {
	int tmp = killer[0];
	killer[0] = killer[1];
	killer[1] = tmp;
}



void vote(int killer_num) {
	int vote_result_num[10] = { 0 };//紀錄票數
	int vote_result[10] = { -1 };//紀錄投給誰

	for (int i = 0; i < PLAYER_NUM; i++) {
		if (p[i].vote != 0) {
			p[i].vote++;
		}
		if (p[i].dead_or_live && p[i].c.id == 10) {//自殺炸彈客
			int k = 10 - day;
			if (k <= 2)
				k = 2;
			if (randInt(0,k-1) == 0) {//使用能力
				
				int x = randInt(0,9);
				while (x == i || p[x].dead_or_live == false) {
					
					x = randInt(0,9);
				}
				p[i].dead_or_live = false;
				p[x].dead_or_live = false;
				if (x == killer_num) {//炸到殺手
					if (!p[killer_num].team) {//藍隊殺手
						for (int i = 0; i < PLAYER_NUM; i++) {
							if (p[i].team) {
								p[i].s.get_source(5, 0, 0);
							}
						}
					}
					else {//紅隊殺手
						for (int i = 0; i < PLAYER_NUM; i++) {
							if (!p[i].team) {
								p[i].s.get_source(0, 0, 5);
							}
						}
					}
					//rand新殺手
					int t = randInt(0,4);
					if (!p[killer[1]].team) {
						killer[0] = 5 + t;
					}
					else {
						killer[0] = t;
					}
				}
				killer_exchange();
				return;
			}
		}
		if (p[i].dead_or_live && p[i].vote == 0) {

			if (randInt(0, 1)) {  //不參與投票
				continue;
			}
			else {  //投票
				while (1) {

					int vote_player = randInt(0, 9);
					if (p[vote_player].dead_or_live) {
						vote_result[i] = vote_player;
						vote_result_num[vote_player] += 1;
						break;
					}
				}
			}
		}
	}
	int max = 0;
	int max_index = -1;
	bool check_equal = true;//最高票有2人以上
	for (int i = 0; i < PLAYER_NUM; i++) {
		if (vote_result_num[i] > max)
		{
			max = vote_result_num[i];
			max_index = i;
			check_equal = false;
		}
		else if (vote_result_num[i] == max) {//同票
			check_equal = true;
		}
	}

	if (!check_equal) {//有最高票
		p[max_index].dead_or_live = false;//投票死亡
		if (max_index == killer_num) {//投對殺手
			if (!p[killer_num].team) {//藍隊殺手
				for (int i = 0; i < PLAYER_NUM; i++) {
					if (p[i].team) {
						p[i].s.get_source(5, 0, 0);
					}
				}
			}
			else {//紅隊殺手
				for (int i = 0; i < PLAYER_NUM; i++) {
					if (!p[i].team) {
						p[i].s.get_source(0, 0, 5);
					}
				}
			}
			//rand新殺手
			
			int t = randInt(0,4);
			if (!p[killer[1]].team) {
				killer[0] = 5 + t;
			}
			else {
				killer[0] = t;
			}
		}
		else {//投錯殺手
			for (int i = 0; i < PLAYER_NUM; i++) {//投錯處罰
				if (vote_result[i] == max_index)
					p[i].vote -= 2;
			}
		}
	}

	killer_exchange();
}


void revive() {//全體復活
	for (int i = 0; i < PLAYER_NUM; i++)
		p[i].dead_or_live = true;
}

void killing() {
	int killed_num = 0;
	int kill_time = 1;
	if (p[killer[0]].c.id == 1)//暗殺者可殺人2次
		kill_time = 2;
	while (kill_time) {
		killed_num++;
		int killed;
		while (1) {
			killed = randInt(0,9);
			if (killed != killer[0] && p[killed].dead_or_live == true)//防止rand到殺手本人
				break;
		}
		p[killed].dead_or_live = false;
		kill_time--;
	}

	//脫逃器使用
	for (int i = 0; i < PLAYER_NUM; i++) {
		if (randInt(0,1) && p[i].bag.escape>0) {
			p[i].bag.escape--;
			if (p[i].dead_or_live == false) {
				p[i].dead_or_live = true;
				killed_num--;
			}
		}
	}

	vote_flag = true;


	//魔術師
	bool killer_dead = false;
	for (int i = 0; i < PLAYER_NUM; i++) {
		if (p[i].dead_or_live == false && p[i].c.id == 2) { //魔術師被殺掉
			p[i].dead_or_live = true;
			p[i].c.flag = false;
			
			int x = randInt(0,9);
			while (x == i || p[x].dead_or_live == false ) {//非本人
				
				x = randInt(0,9);
			}
			if (x == killer[0]) { // 反殺殺手
				killed_num--;
				p[x].dead_or_live = false;
				killer_dead = true;
			}
			else { //轉移目標
				p[x].dead_or_live = false;
			}
		}
	}

	while (killed_num > 0) {//殺手殺人獎勵
		int tmp = randInt(0,2);
		switch (tmp) { //殺手殺人獎勵
		case 0:
			p[killer[0]].s.get_source(5, 0, 0);
			break;
		case 1:
			p[killer[0]].s.get_source(0, 5, 0);
			break;
		case 2:
			p[killer[0]].s.get_source(0, 0, 5);
			break;
		default:
			break;
		}
		killed_num--;
	}

	if (killer_dead) {
		if (!p[killer[0]].team) {//藍隊殺手
			for (int i = 0; i < PLAYER_NUM; i++) {
				if (p[i].team) {
					p[i].s.get_source(5, 0, 0);
				}
			}
		}
		else {//紅隊殺手
			for (int i = 0; i < PLAYER_NUM; i++) {
				if (!p[i].team) {
					p[i].s.get_source(0, 0, 5);
				}
			}
		}
		//rand新殺手
		
		int t = randInt(0,4);
		if (!p[killer[1]].team) {
			killer[0] = 5 + t;
		}
		else {
			killer[0] = t;
		}
		vote_flag = false;//殺手死亡不需要投票
		killer_exchange();//輪替
	}
}

void sell() {
	
	for (int i = 0; i < PLAYER_NUM; i++) {//拍賣一輪
		int flag = randInt(0,1);//賣不賣
		if (p[i].dead_or_live&&flag) //活著且賣
		{
			int JP = 1;//是否繼續競標
			int x = randInt(0,2);//賣出顏色
			int y = (randInt(0,4)) + 2;//賣出某顏色2~6
			int z = randInt(0,2);//買家出的顏色
			int round = 2;//持續喊價機率參數
			int prize = 0;
			int playerGain = -1;
			switch (x)  //0:R 1:G 2:B要賣的顏色
			{
			case 0:
				if (p[i].s.R >= y)
				{
					while (JP == 1)//有人喊就多一輪
					{
						JP = 0;
						for (int j = 0; j < PLAYER_NUM; j++)
						{
							if ((j != i) && ((randInt(0,round-1)) == 0) && (p[j].s.get_source_amount() > prize) && (j != playerGain))//不是賣家且想買且買得起且不覆蓋自己的出價
							{
								JP = 1;
								playerGain = j;
								prize++;
							}
						}
						round += 2;
					}
					if (playerGain < 0) { continue; }//沒人競標
					else
					{
						p[playerGain].s.R += y;
						p[i].s.R -= y;
						int s;
						while (prize > 0)
						{
							s = randInt(0,2);
							switch (s)  //0:R 1:G 2:B
							{
							case 0:
								if (p[playerGain].s.R > 0)
								{
									p[playerGain].s.R -= 1;
									p[i].s.R += 1;
									prize--;
								}
								break;
							case 1:
								if (p[playerGain].s.G > 0)
								{
									p[playerGain].s.G -= 1;
									p[i].s.G += 1;
									prize--;
								}
								break;
							case 2:
								if (p[playerGain].s.B > 0)
								{
									p[playerGain].s.B -= 1;
									p[i].s.B += 1;
									prize--;
								}
								break;
							default:
								break;
							}
						}
					}
				}

				break;
			case 1:
				if (p[i].s.G >= y)
				{
					while (JP == 1)//有人喊就多一輪
					{
						JP = 0;
						for (int j = 0; j < PLAYER_NUM; j++)
						{
							if ((j != i) && ((randInt(0,round-1)) == 0) && (p[j].s.get_source_amount() > prize) && (j != playerGain))//不是賣家且想買且買得起且不覆蓋自己的出價
							{
								JP = 1;
								playerGain = j;
								prize++;
							}
						}
						round += 2;
					}
					if (playerGain < 0) { continue; }//沒人競標
					else
					{
						p[playerGain].s.G += y;
						p[i].s.G -= y;
						int s;
						while (prize > 0)
						{
							s = randInt(0,2);
							switch (s)  //0:R 1:G 2:B
							{
							case 0:
								if (p[playerGain].s.R > 0)
								{
									p[playerGain].s.R -= 1;
									p[i].s.R += 1;
									prize--;
								}
								break;
							case 1:
								if (p[playerGain].s.G > 0)
								{
									p[playerGain].s.G -= 1;
									p[i].s.G += 1;
									prize--;
								}
								break;
							case 2:
								if (p[playerGain].s.B > 0)
								{
									p[playerGain].s.B -= 1;
									p[i].s.B += 1;
									prize--;
								}
								break;
							default:
								break;
							}
						}
					}
				}
				break;
			case 2:
				if (p[i].s.B >= y)
				{
					while (JP == 1)//有人喊就多一輪
					{
						JP = 0;
						for (int j = 0; j < PLAYER_NUM; j++)
						{
							if ((j != i) && ((randInt(0,round-1)) == 0) && (p[j].s.get_source_amount() > prize) && (j != playerGain))//不是賣家且想買且買得起且不覆蓋自己的出價
							{
								JP = 1;
								playerGain = j;
								prize++;
							}
						}
						round += 2;
					}
					if (playerGain < 0) { continue; }//沒人競標
					else
					{
						p[playerGain].s.B += y;
						p[i].s.B -= y;
						int s;
						while (prize > 0)
						{
							s = randInt(0,2);
							switch (s)  //0:R 1:G 2:B
							{
							case 0:
								if (p[playerGain].s.R > 0)
								{
									p[playerGain].s.R -= 1;
									p[i].s.R += 1;
									prize--;
								}
								break;
							case 1:
								if (p[playerGain].s.G > 0)
								{
									p[playerGain].s.G -= 1;
									p[i].s.G += 1;
									prize--;
								}
								break;
							case 2:
								if (p[playerGain].s.B > 0)
								{
									p[playerGain].s.B -= 1;
									p[i].s.B += 1;
									prize--;
								}
								break;
							default:
								break;
							}
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

void playing() {//return which team win
	day = 0;
	bool team;
	while (1) {
		day++;
		//cout << "day: " << day << endl;
		//白天
		if (vote_flag)
		{
			//before vote
			game_state = 4;
			for (int i = 0; i < PLAYER_NUM; i++) {
				p[i].skill(game_state);
			}
			//vote
			vote(killer[0]);
		}
		game_state = 0;
		for (int i = 0; i < PLAYER_NUM; i++) {//採集階段、道具兌換
			p[i].play(true);
			if (p[i].team && p[i].s.R >= PLAYER_NUM * 5) {//red勝利
				gameend(true);
				return;
			}
			else if (!p[i].team && p[i].s.B >= PLAYER_NUM * 5) {//blue勝利
				gameend(false);
				return;
			}
		}
		//sell();
		//晚上
		revive();//全體復活
		//before kill
		game_state = 1;
		for (int i = 0; i < PLAYER_NUM; i++) {
			if (p[i].bag.steal > 0) {//使用小偷卡
				if (randInt(0,4) == 0) {
					int x = randInt(0,9);
					while (x == i) {
						x = randInt(0,9);
					}
					int color = randInt(0,2);
					switch (color)
					{
					case 0:
						p[i].s.R += p[x].s.R;
						p[x].s.R = 0;
					case 1:
						p[i].s.G += p[x].s.G;
						p[x].s.G = 0;
					case 2:
						p[i].s.B += p[x].s.B;
						p[x].s.B = 0;

					default:
						break;
					}
				}
			}
			p[i].skill(game_state);
		}
		//kill
		killing();
	}
}





int main() {
	vector<player_set> ps;
	cin >> input_team;
	cout << "測試ing" << endl;
	R_num = 0;
	B_num = 0;
	if (input_team >= 0 && input_team < 126) {
		for (int i = 0; i < PLAYER_NUM; i++) {//player 設定(選定table)
			player_set tmp;
			if (i <= 4) {//前5個同隊
				tmp.number = i;
				tmp.team = false;
				tmp.c_num = table[input_team][i];
			}
			else {//後五個同隊
				tmp.number = i;
				tmp.team = true;
				tmp.c_num = table[input_team][i];
			}
			ps.push_back(tmp);
		}
	}
	else {
		return 0;
	}
	fstream file;
	file.open("table" + to_string(input_team) + ".txt", ios::in);
	file << "table" <<input_team << ":" <<endl;
	file.close();
	while (1) {
		if (times == SIMULATION_TIMES) {
			break;
		}
		gamestart(ps);
		playing();
	}
	file.open("table" + to_string(input_team) + ".txt", ios::app);
	if (!file)
		cout << "open file error\n";
	else {
		file << "red:  " << R_num << "\t" << "blue:  " << B_num << endl;
		file << "平均天數: " << days / double(SIMULATION_TIMES) << endl;
		file << "紅隊勝率:  " << double(R_num) / double(SIMULATION_TIMES) << "\t" << "藍隊勝率:  " << double(B_num) / double(SIMULATION_TIMES) << endl;
	}
	return 0;
}

