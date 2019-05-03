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

#define SIMULATION_TIMES 1000000 //模擬次數
#define PLAYER_NUM 10 //遊玩人數

int times = 1;
int record[2] = { 0,0 };//獲勝次數
int killer[2];

/*charter num
1 暗殺者
2 魔術師
3 賭徒
4 採集者
5 夜貓子
6 牧師
7 小偷
8 貴族
9 黑市商人
10 自殺炸彈客
*/



struct player_set {
	int number;
	bool team;
	int charter;
};

class source {//資源
public:
	source() { R = 0; B = 0; G = 0; };
	int R, G, B;
	void draw();//回合抽取素材
	void used(int, int, int);//素材使用
	void get_source(int, int, int); //獲得素材
};

void source::draw() {
	srand(time(NULL));
	int x = rand()%3;
	int y = rand() % 6;
	switch (x)  //0:R 1:G B:2
	{
	case 0:
		this->R += y + 1;
		break;
	case 1:
		this->G += y + 1;
		break;
	case 2:
		this->B += y + 1;
		break;
	default:
		break;
	}
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
	int id;
	void skill();
private:
	bool flag;
};

void character::skill() {

}


class player {
public:
	player() {};
	player(player_set);
	int number;
	bool team;//0:blue 1:red
	source s;
	bool dead_or_live; //1:live 0:dead
	character c; //角色
	void play(bool);// 1:白天 0:晚上
	int vote;// 0:可投票
};

player p[PLAYER_NUM];

player::player(player_set ps) {
	this->number = ps.number;
	this->team = ps.team;
	this->c.id = ps.charter;
	this->dead_or_live = true;
	this->vote = 0;
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
	void killer_exchange();
}

void gameend(int round, bool team) {
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
		file << "\t" << "回合數:" << round << endl;
	}
	times++;
}

void revive() {//全體復活
	for (int i = 0; i < PLAYER_NUM; i++)
		p[i].dead_or_live = true;
}

void killing() {
	srand(time(NULL));
	int killed;
	while (1) {
		killed = rand() % 10;
		if (killed != killer[0])//防止rand到殺手本人
			break;
	}

	p[killed].dead_or_live = false;
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
}

void playing() {//return which team win
	int day = 1;
	bool team;
	while (1) {
		//白天
		if (day != 1)
			vote(killer[0]);
		for (int i = 0; i < PLAYER_NUM; i++) {
			if (p[i].dead_or_live) {
				p[i].play(true);
			}
		}
		//晚上
		revive();
		for (int i = 0; i < PLAYER_NUM; i++) {
			p[i].play(true);
		}
	}
	gameend(day,team);
}







int main()
{
	vector<player_set> ps;
	fstream file;
	file.open("input.txt", ios::in);
	if (!file) {
		cout << "file not exist!" << endl;
		return -1;
	}
	string team,charter;
	int number = 1;
	while (file >> team) { //player 設定(input.txt)
		stringstream ss;
		file >> charter;
		ss << charter;
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
		tmp.charter = c;
		ps.push_back(tmp);
		number++;
	}
	file.close();
	gamestart(ps);
	int R = 0;//紅隊勝利次數
	int B = 0;//藍隊勝利次數
	while (1) {
		if (times == SIMULATION_TIMES) {
			break;
		}
		playing();
	}
	cout << "red:  " << R << "\t" << "blue:  " << B << endl;
    return 0;
}

