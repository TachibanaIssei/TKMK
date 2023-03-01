#pragma once
class Actor
{
public:
	Actor();

	virtual ~Actor();

protected:

	//レベルアップしたときに上がるステータスの値
	struct LvUpStatus {
		int LvHp;                  //レベルアップ時のヒットポイント
		int LvAtk;                 //レベルアップ時の攻撃力
		int LvSpeed;               //レベルアップ時の移動速度
	};

	int Lv;                    //レベル
	int Hp;                    //ヒットポイント(体力)
	int Atk;                   //攻撃力
	int Speed;                 //移動速度
	int AtkSpeed;              //攻撃速度
	int Cooltime;              //技を打った後のクールタイム
	int Point;                 //敵を倒して手に入れたポイント
	int ShieldPoint;           //シールド
	int Exp;                   //経験値
	int ExpTable;              //経験値テーブル
	bool isDeath = false;      //死んだかどうかの判定

public:
	//攻撃の処理
	//純粋仮想関数なので派生クラスで実装
	virtual void Attack() = 0;
	//攻撃を受けたときの処理
	virtual void Dameged(int damege) = 0;
	//スキルを打つ処理
	virtual void Skill() = 0;
	//クールタイムを減らす処理
	virtual void COOlTIME() = 0;
	//現在のレベルを取得する関数
	inline int GetLevel() { return Lv; }
	//レベルアップの処理
	void LevelUp(LvUpStatus lus);
	//死んだときの処理
	virtual void Death() = 0;
};

