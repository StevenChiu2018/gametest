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
using namespace std;

#define SIMULATION_TIMES 1000 //模擬次數
#define PLAYER_NUM 10 //遊玩人數

int times =0;
int record[2] = { 0,0 };//獲勝次數
int killer[2];
bool vote_flag;
int game_state;
bool killer_dead=false;
int day;
void killing();
int R_num, B_num;



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
		if (rand() % 4 == 0) {
			this->s.G -= 3;
			this->bag.escape++;
		}
	}
	if (this->s.R >= 4 && this->s.G >= 4 && this->s.B >= 4) {//偷竊卡
		if (rand() % 3 == 0) {
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
			srand(time(NULL));
			int x = rand() % 3;
			int y = rand() % 3;
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
					if (rand() % 4 ==0) {//是否要救
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
			if (rand() % k == 0) {
				int x;//要偷的人
				while (1) {
					x = rand() % 10;
					if (x != this->number)//不能是自己
						break;
				}
				int color = rand() % 3;
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
			int k = 10 - day;
			if (k <= 2)
				k = 2;
			if (rand() % k == 0) {
				int x, y;//x: 目標兌換顏色  y:使用兌換顏色
				x = rand() % 3;
				while (1) {
					y = rand() % 3;
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
	srand(time(NULL));
	int x = rand() % 3;
	int y = rand() % 6;
	int bonus = 0;
	if (this->c.id == 4)//採集者
		bonus = 2;
	if (this->c.id == 3) { //賭徒
		if (rand() % 1+y == 0) { //重抽(原本資源越少重抽率越高)
			x = rand() % 3;
			y = rand() % 6;
		}
	}
	switch (x)  //0:R 1:G B:2
	{
	case 0:
		this->s.R += y + 1 + bonus;
		break;
	case 1:
		this->s.G += y + 1 + bonus;
		break;
	case 2:
		this->s.B += y + 1 + bonus;
		break;
	default:
		break;
	}
}

void gameend(bool team) {
	fstream file;
	file.open("output.txt", ios::out);
	if (!file)
		cout << "opne file error\n";
	else {
		file << "場次:" << times << "\t" << "勝利隊伍:";
		if (team) {
			file << "藍";
		}
		else {
			file << "紅";
		}
		file << "\t" << "天數:" << day << endl;
	}
	if (team == true) {
		R_num++;
	}
	else if(team == false){
		B_num++;
	}
	times++;
}

void player::play(bool t) {
	if (t) {//白天
		if (this->dead_or_live == 0)
		{
			if (this->c.id == 4)//採集者死亡也可獲得物資
			{
				srand(time(NULL));
				int x = rand() % 3;
				switch (x)  //0:R 1:G B:2
				{
				case 0:
					this->s.R += 2;
					break;
				case 1:
					this->s.G += 2;
					break;
				case 2:
					this->s.B += 2;
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
	srand(time(NULL));
	killer[0] = rand() % 10;
	while (1) {
		killer[1] = rand() % 10;
		if (p[killer[0]].team != p[killer[1]].team)//兩個殺手不同隊伍
			break;
	}
}



void killer_exchange() {
	int tmp = killer[0];
	killer[0] = killer[1];
	killer[1] = killer[0];
}



void vote(int killer_num) {
	for (int i = 0; i < PLAYER_NUM; i++) {
		if (p[i].vote != 0) {
			p[i].vote++;
		}
		if (p[i].dead_or_live && p[i].c.id == 10) {//自殺炸彈客
			int k = 10 - day;
			if (k <= 2)
				k = 2;
			if (rand() % k) {//使用能力
				int x = rand() % 10;
				while (x == i || p[x].dead_or_live == false) {
					x = rand() % 10;
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
							if (p[i].team) {
								p[i].s.get_source(0, 0, 5);
							}
						}
					}
					while (1) {//rand新殺手
						srand(time(NULL));
						killer[0] = rand() % 10;
						if (p[killer[0]].team != p[killer[1]].team)
							break;
					}
				}
				killer_exchange();
				return;
			}
		}
	}
	int vote_result_num[10] = { 0 };//紀錄票數
	int vote_result[10] = { -1 };//紀錄投給誰
	for (int i = 0; i < PLAYER_NUM; i++) {
		if (p[i].dead_or_live && p[i].vote == 0) {
			srand(time(NULL));
			int flag = rand() % 2;
			if (flag == 0) {  //不參與投票
				continue;
			}
			else {  //投票
				while (1) {
					int vote_player = rand() % 10;
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
	bool check_equal = false;//最高票有2人以上
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
					if (p[i].team) {
						p[i].s.get_source(0, 0, 5);
					}
				}
			}
			while (1) {
				srand(time(NULL));
				killer[0] = rand() % 10;
				if (p[killer[0]].team != p[killer[1]].team)
					break;
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
		srand(time(NULL));
		int killed;
		while (1) {
			killed = rand() % 10;
			if (killed != killer[0] && p[killed].dead_or_live == true)//防止rand到殺手本人
				break;
		}

		p[killed].dead_or_live = false;
		kill_time--;
	}

	//脫逃器使用
	for (int i = 0; i < PLAYER_NUM; i++) {
		if (rand() % 2 && p[i].bag.escape>0) {
			p[i].bag.escape--;
			if (p[i].dead_or_live = false) {
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
			int x = rand() % 10;
			while (x == i || p[x].dead_or_live ==false ) {//非本人
				x = rand() % 10;
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
		int tmp = rand() % 3;
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
				if (p[i].team) {
					p[i].s.get_source(0, 0, 5);
				}
			}
		}
		while (1) {//新殺手
			srand(time(NULL));
			killer[0] = rand() % 10;
			if (p[killer[0]].team != p[killer[1]].team)
				break;
		}
		vote_flag = false;//殺手死亡不需要投票
		killer_exchange();//輪替
	}
}

void sell() {
	srand(time(NULL));
	for (int i = 0; i < PLAYER_NUM; i++) {//拍賣一輪
		int flag = rand() % 2;//賣不賣
		if (p[i].dead_or_live&&flag) //活著且賣
		{
			int JP = 1;//是否繼續競標
			int x = rand() % 3;//賣出顏色
			int y = (rand() % 5) + 2;//賣出某顏色2~6
			int z = rand() % 3;//買家出的顏色
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
							if ((j != i) && ((rand() % round) == 0) && (p[j].s.get_source_amount() > prize) && (j != playerGain))//不是賣家且想買且買得起且不覆蓋自己的出價
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
							s = rand() % 3;
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
							if ((j != i) && ((rand() % round) == 0) && (p[j].s.get_source_amount() > prize) && (j != playerGain))//不是賣家且想買且買得起且不覆蓋自己的出價
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
							s = rand() % 3;
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
							if ((j != i) && ((rand() % round) == 0) && (p[j].s.get_source_amount() > prize) && (j != playerGain))//不是賣家且想買且買得起且不覆蓋自己的出價
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
							s = rand() % 3;
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
		sell();
		//晚上
		revive();//全體復活
		//before kill
		game_state = 1;
		for (int i = 0; i < PLAYER_NUM; i++) {
			if (p[i].bag.steal > 0) {//使用小偷卡
				if (rand() % 5) {
					int x = rand() % 10;
					while (x == i) {
						x = rand() % 10;
					}
					int color = rand() % 3;
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
	fstream file;
	file.open("input.txt", ios::in);
	if (!file) {
		cout << "file not exist!" << endl;
		return -1;
	}
	string team, c_num;
	int number = 0;
	while (file >> team) { //player 設定(input.txt)
		stringstream ss;
		file >> c_num;
		ss << c_num;
		bool t;
		if (team == "0")
			t = false;
		else
			t = true;
		int c;
		ss >> c;
		player_set tmp;
		tmp.number = number;
		tmp.team = t;
		tmp.c_num = c;
		ps.push_back(tmp);
		number++;
	}
	file.close();
	gamestart(ps);
	while (1) {
		if (times == SIMULATION_TIMES) {
			break;
		}
		gamestart(ps);
		playing();
	}
	cout << "red:  " << R_num << "\t" << "blue:  " << B_num << endl;
	return 0;
}

