#pragma once

#include "MyRenderer.h"
#include "PostEffect/PostEffect.h"
#include "Light/SceneLight.h"
#include "Shadow/Shadow.h"
#include "Shadow/ShadowMapRender.h"

namespace nsK2EngineLow {
	class IRenderer;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	class RenderingEngine : public Noncopyable
	{
	public:
		struct SLightingCB
		{
			Light m_light;	//ライト
			Matrix mlvp[MAX_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP];	//ライトビュープロジェクション行列
		};

		struct GBufferCB
		{
			int drawCameraNumber;	//描画中のカメラの番号
		};

		enum EnGameMode {
			enGameMode_SoloPlay = 1,
			enGameMode_DuoPlay,
			enGameMode_TrioPlay,
			enGameMode_QuartetPlay,
			enGameMode_Num
		};
		/// <summary>
		/// 現在のカメラ描画
		/// </summary>
		enum EnCameraDrawing
		{
			enCameraDrawing_Solo = 0,		//1画面だけのとき
			enCameraDrawing_Left = 0,		//左画面
			enCameraDrawing_Right = 1,		//右画面
			enCameraDrawing_LeftUp = 0,		//左上
			enCameraDrawing_RightUp = 1,	//右上
			enCameraDrawing_LeftDown = 2,	//左下
			enCameraDrawing_RightDown = 3	//右下
		};

		void Init(const bool isSoftShadow);

		void InitRenderTargets();
		void InitCopyToFrameBufferSprite();

		/// <summary>
		/// 描画オブジェクトを追加
		/// </summary>
		/// <param name="renderObject">描画オブジェクト</param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.emplace_back(renderObject);
		}

		/// <summary>
		/// スプライトレンダークラスをリストに追加する
		/// </summary>
		/// <param name="spriteRender">スプライトレンダー</param>
		void AddSpriteList(SpriteRender* spriteRender, const bool drawTiming = false)
		{
			if (drawTiming)
			{
				m_laterSpriteList.push_back(spriteRender);
			}
			else {
				m_spriteList.push_back(spriteRender);
			}
		}
		void AddFrontSpriteList(SpriteRender* spriteRender)
		{
			m_spriteFrontDrawList.push_back(spriteRender);
		}

		/// <summary>
		/// 画面分割中のビューポートに描画するスプライトをリストに追加
		/// </summary>
		/// <param name="spriteRender"></param>
		/// <param name="viewportNo"></param>
		void AddSpriteDrawViewportList(SpriteRender* spriteRender, const int viewportNo)
		{
			m_spriteDrawViewportList[viewportNo].push_back(spriteRender);
		}

		/// <summary>
		/// フォントレンダークラスをリストに追加する
		/// </summary>
		/// <param name="fontRender">フォントレンダー</param>
		void AddFontList(FontRender* fontRender, const bool drawTiming = false)
		{
			if (drawTiming)
			{
				m_laterFontList.push_back(fontRender);
			}
			else
			{
				m_fontList.push_back(fontRender);
			}
		}
		/// <summary>
		/// 画面分割中のビューポートに描画するフォントレンダークラスをリストに追加する
		/// </summary>
		/// <param name="spriteRender"></param>
		/// <param name="viewportNo"></param>
		void AddFontDrawViewportList(FontRender* fontRender, const int viewportNo)
		{
			m_fontDrawViewportList[viewportNo].push_back(fontRender);
		}

		/// <summary>
		/// シャドウモデルを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void ShadowModelRendering(RenderContext& rc, Camera& camera, const int number);

		/// <summary>
		/// 描画処理を実行
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// シーンライトを取得
		/// </summary>
		/// <returns>シーンライト</returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}

		////////////////////////////////////////////////////////////////////////////////
		///シャドウマップの関数
		////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// シャドウマップのテクスチャを取得
			/// </summary>
			/// <returns>シャドウマップのテクスチャ</returns>
		Texture& GetShadowMapTexture(const int number)
		{
			return m_shadow.GetShadowMapTexture(number);
		}

		/// <summary>
		/// ライトカメラを取得
		/// </summary>
		/// <returns>ライトカメラ</returns>
		Camera& GetLightCamera(const int number)
		{
			return m_shadow.GetLightCamera(number);
		}

		/// <summary>
		/// シャドウマップテクスチャにクエリを行う
		/// </summary>
		/// <param name="queryFunc"></param>
		void QueryShadowMapTexture(std::function< void(Texture& shadowMap) > queryFunc)
		{
			for (int i = 0; i < MAX_DIRECTIONAL_LIGHT; i++)
			{
				for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
				{
					queryFunc(m_shadowMapRenders[i].GetShadowMap(areaNo));
				}
			}
		}

		/// <summary>
		/// ソフトシャドウをtrueなら行う
		/// </summary>
		/// <returns></returns>
		const bool IsSoftShadow() const
		{
			return m_isSoftShadow;
		}

		/// <summary>
		/// ディレクションライトを設定
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <param name="direction">ライト方向</param>
		/// <param name="color">ライト色</param>
		void SetDirectionLight(const int lightNo, const Vector3 direction, const Vector3 color);
		/// <summary>
		/// キャストシャドウフラグを取得
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <returns>trueだったら影を落とす</returns>
		void SetDirectionLightCastShadow(const int lightNo, const bool flag);

		/// <summary>
		/// 環境光を設定
		/// </summary>
		/// <param name="ambient">環境光</param>
		void SetAmbient(const Vector3 ambient);

		/// <summary>
		/// ライティングの定数バッファを取得
		/// </summary>
		/// <returns>定数バッファ</returns>
		SLightingCB& GetLightingCB()
		{
			return m_lightingCB;
		}
		/// <summary>
		/// G-Bufferの定数バッファを取得
		/// </summary>
		/// <returns>定数バッファ</returns>
		GBufferCB& GetGBufferCB()
		{
			return m_gbufferCB;
		}

		////////////////////////////////////////////////////////////////////////////////
		///画面分割の関数
		////////////////////////////////////////////////////////////////////////////////

			/// <summary>
			/// ゲームモードをレンダリングエンジンに設定する
			/// </summary>
		void SetGameModeToRenderingEngine(const EnGameMode gameMode)
		{
			m_gameMode = gameMode;
		}
		/// <summary>
		/// ゲームモードを取得
		/// </summary>
		/// <returns></returns>
		const EnGameMode GetGameMode() const
		{
			return m_gameMode;
		}
		/// <summary>
		/// どちらのカメラを描画中か
		/// </summary>
		/// <returns></returns>
		const EnCameraDrawing GetCameraDrawing() const
		{
			return m_cameraDrawing;
		}

		/// <summary>
		/// フレームの最初に行われるエフェクトの処理
		/// </summary>
		void EffectBeginRender();

	private:
		void InitGBuffer();
		/// <summary>
		/// ディファードライティングで使用する画像の初期化
		/// </summary>
		void InitDeferredLightingSprite();
		/// <summary>
		/// シャドウマップレンダーを初期化
		/// </summary>
		void InitShadowMapRender();
		/// <summary>
		/// ビューポートをInitする
		/// </summary>
		void InitViewPorts();

		/// <summary>
		/// モデル描画をビューポートの数実行する関数
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void DrawModelInViewPorts(RenderContext& rc);
		/// <summary>
		/// G-Bufferへの描画
		/// </summary>
		/// <param name="rc"></param>
		void RenderToGBuffer(RenderContext& rc);
		void DrawModelOnGBuffer(RenderContext& rc);
		/// <summary>
		/// ディファードライティング
		/// </summary>
		/// <param name="rc"></param>
		void DeferredLighting(RenderContext& rc);
		/// <summary>
		/// フォワードレンダリングでのモデル描画
		/// </summary>
		/// <param name="rc"></param>
		void FowardRendering(RenderContext& rc);
		/// <summary>
		/// シャドウマップへの描画
		/// </summary>
		/// <param name="rc"></param>
		void RenderToShadowMap(RenderContext& rc);
		/// <summary>
		/// スプライトを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void SpriteRendering(RenderContext& rc, const bool drawTiming = false);
		/// <summary>
		/// スプライトを画面分割中のビューポートに描画
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="viewportNo"></param>
		void SpriteViewportRendering(RenderContext& rc, const int viewportNo);
		void SpriteFrontRendering(RenderContext& rc);
		/// <summary>
		/// フォントを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void FontRendering(RenderContext& rc, const bool drawTiming = false);
		/// <summary>
		/// フォントを画面分割中のビューポートに描画
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="viewportNo"></param>
		void FontViewportRendering(RenderContext& rc, const int viewportNo);
		/// <summary>
		/// エフェクト描画を実行する
		/// </summary>
		/// <param name="rc"></param>
		void ExcuteEffectRender(RenderContext& rc);
		/// <summary>
		/// 2D描画を行う
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);
		/// <summary>
		/// 描画リストをclearする
		/// </summary>
		void ClearRenderList();
		/// <summary>
		/// ライトの定数バッファにライト情報を設定する
		/// </summary>
		void SetLightingCB();

	private:
		// GBufferの定義
		enum EnGBuffer
		{
			enGBufferAlbedoDepth,           // アルベドと深度値。αに深度値が記憶されています。
			enGBufferNormal,                // 法線
			enGBufferMetaricShadowSmooth,   // メタリック、影パラメータ、スムース。
			// メタリックがr、影パラメータがg、スムースがa。bは未使用。
			enGBufferNum,                   // G-Bufferの数
		};

		std::vector<IRenderer*>		m_renderObjects;
		std::vector<SpriteRender*>	m_spriteList;							//スプライトクラスのリスト
		std::vector<SpriteRender*>	m_laterSpriteList;						//描画順が遅いスプライトクラスのリスト
		std::vector<SpriteRender*>	m_spriteDrawViewportList[MAX_VIEWPORT];	//画面分割中のビューポートに描画するスプライトのリスト
		std::vector<SpriteRender*>	m_spriteFrontDrawList;					//全てにおいて前面に描画される
		std::vector<FontRender*>	m_fontList;								//フォントクラスのリスト
		std::vector<FontRender*>	m_laterFontList;						//laterSpriteの上から文字を描画
		std::vector<FontRender*>	m_fontDrawViewportList[MAX_VIEWPORT];	//画面分割中のビューポートに描画するフォントクラスのリスト

		SceneLight					m_sceneLight;				//シーンライト
		SLightingCB					m_lightingCB;				//ライトの定数バッファ
		GBufferCB					m_gbufferCB;				//G-Bufferの定数バッファ

		RenderTarget				m_mainRenderTarget;						//メインレンダーターゲット
		RenderTarget				m_2DRenderTarget;						//2Dレンダーターゲット
		RenderTarget				m_gBuffer[enGBufferNum];
		Sprite						m_2DSprite;
		Sprite						m_mainSprite;
		Sprite						m_copyToFrameBufferSprite;				//テクスチャを貼り付けるためのスプライトを初期化
		Sprite						m_diferredLightingSprite;				//ディファードライティングを行うためのスプライト
		ShadowMapRender				m_shadowMapRenders[MAX_DIRECTIONAL_LIGHT];		//シャドウマップへの描画処理
		Shadow						m_shadow;								//シャドウマップ
		PostEffect					m_postEffect;							//ポストエフェクト

		EnCameraDrawing m_cameraDrawing = enCameraDrawing_Left;

		D3D12_VIEWPORT m_soloViewPort;										//1画面用のビューポート
		D3D12_VIEWPORT m_duoViewPorts[DUO_VIEWPORT];						//2画面分割用のビューポート
		D3D12_VIEWPORT m_quarteViewPorts[MAX_VIEWPORT];						//4画面分割用のビューポート
		EnGameMode m_gameMode = enGameMode_SoloPlay;

		bool						m_isSoftShadow = false;					//ソフトシャドウを行う？
	};
}