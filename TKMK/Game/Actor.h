#pragma once
class Actor
{
public:
	Actor();
	virtual ~Actor();

protected:

	struct Status {
		int Hp;                    //ヒットポイント(体力)
		int Atk;                   //攻撃力
		float Speed;               //移動速度
	};


	//レベルアップしたときに上がるステータスの値
	//レベルアップ時のヒットポイント
	//レベルアップ時の攻撃力
	//レベルアップ時の移動速度
	struct LvUpStatus {
		int LvHp;                  //レベルアップ時のヒットポイント
		int LvAtk;                 //レベルアップ時の攻撃力
		float LvSpeed;               //レベルアップ時の移動速度
	};

	int Lv;                    //レベル
	//int Hp;                    //ヒットポイント(体力)
	//int Atk;                   //攻撃力
	//float Speed;               //移動速度
	int AtkSpeed;              //攻撃速度
	float Cooltime;            //スキルのクールタイム
	bool SkillState=false;     //スキルが使用可能かどうかの判定
	int Point;                 //敵を倒して手に入れたポイント
	int GetExp;                //中立の敵を倒したときの経験値
	int ExpTable;              //経験値テーブル
	bool isDeath = false;      //死んだかどうかの判定

public:

	float timer = 0.0f;

	//攻撃の処理
	//純粋仮想関数なので派生クラスで実装
	virtual void Attack() = 0;
	//攻撃を受けたときの処理
	virtual void Dameged(int damege) = 0;
	//スキルを打つ処理
	virtual void Skill() = 0;
	//必殺技を打つ処理
	virtual void UltimateSkill() = 0;
	//クールタイムを減らす処理
	void COOlTIME(float SkillCooltimer,bool skillstate);
	//現在のレベルを取得する関数
	inline int GetLevel() { return Lv; }
	//レベルアップの処理
	void LevelUp(LvUpStatus& lus, Status& nowStatus);
	//死んだときの処理
	virtual void Death() = 0;

};

