#pragma once

struct Status;
class Actor;

class EnemyHpBar : public IGameObject
{
public:
	EnemyHpBar() {};
	~EnemyHpBar();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Init(const int viewportNo = -1);

	/// <summary>
	/// HPバーの座標を計算する
	/// </summary>
	/// <param name="cameraNo">カメラ番号</param>
	/// <param name="status">ステータス</param>
	/// <param name="characterPosition">キャラクターの座標</param>
	/// <param name="lv">キャラクターのレベル</param>
	void CalcHpBarPosition(const int cameraNo, Status* status,const Vector3& characterPosition,const int lv);
	/// <summary>
	/// 描画フラグを設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetDrawFlag(const bool flag)
	{
		m_drawFlag = flag;
	}

private:
	/// <summary>
	/// ゲージを左寄せする処理
	/// </summary>
	/// <param name="size">画像の元の大きさ</param>
	/// <param name="scale">現在のスケール倍率</param>
	/// <returns>変換前と変換後の差</returns>
	Vector3 LeftAlignHPBar(const Vector3& size, const Vector3& scale);

private:
	SpriteRender		m_HP_Bar;		//HPバー画像
	SpriteRender		m_HP_Frame;		//HP枠画像
	SpriteRender		m_HP_Back;		//HP背景画像
	FontRender			m_fontLv;
	Vector2				m_HPBer_Pos = Vector2::Zero;		//HPバーのポジション
	Vector2				m_HPWindow_Pos = Vector2::Zero;		//HP枠のポジション
	Vector2				m_HPBack_Pos = Vector2::Zero;		//HP背景のポジション
	Vector2				m_levelFontPos = Vector2::Zero;		//HPバーの横に表示するレベル

	int					m_viewportNo = 0;					//ビューポートの番号
	bool				m_drawFlag = false;
};

