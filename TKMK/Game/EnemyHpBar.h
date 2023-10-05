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
	//敵キャラ上部のレベル画像を初期化
	void InitLvSprite();

	/// <summary>
	/// ゲージを左寄せする処理
	/// </summary>
	/// <param name="size">画像の元の大きさ</param>
	/// <param name="scale">現在のスケール倍率</param>
	/// <returns>変換前と変換後の差</returns>
	Vector3 LeftAlignHPBar(const Vector3& size, const Vector3& scale);

	/// <summary>
	/// キャラクターのレベルを設定する
	/// </summary>
	/// <param name="level"></param>
	void SetCharacterLevel(const int level);

private:
	/// <summary>
	/// キャラクターのレベルをm_lvSpriteの要素指定に使用する
	/// </summary>
	enum EnCharacterLevel
	{
		enCharacterLevel_One = 0,
		enCharacterLevel_Two,
		enCharacterLevel_Three,
		enCharacterLevel_Four,
		enCharacterLevel_Five,
		enCharacterLevel_Six,
		enCharacterLevel_Seven,
		enCharacterLevel_Eight,
		enCharacterLevel_Nine,
		enCharacterLevel_Ten,
	};

private:
	const static int m_maxLevel = 10;
	std::array<SpriteRender, m_maxLevel> m_lvSprite;
	SpriteRender		m_hpBar;		//HPバー画像
	SpriteRender		m_hpBack;		//HP背景画像
	Vector2				m_hpBerPosition = Vector2::Zero;		//HPバーのポジション
	Vector2				m_hpBackPosition = Vector2::Zero;		//HP背景のポジション
	Vector2				m_levelSpritePosition = Vector2::Zero;
	EnCharacterLevel	m_level = enCharacterLevel_One;
	int					m_viewportNo = 0;					//ビューポートの番号
	bool				m_drawFlag = false;
};

