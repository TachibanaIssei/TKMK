#  **「SWORD ARENA」ポートフォリオ** <!-- omit in toc -->
### 河原電子ビジネス専門学校ゲームクリエイター科　2年　<!-- omit in toc -->
### 氏名：橘　一生　<!-- omit in toc -->
---

# 目次　<!-- omit in toc -->

- [1. 作品概要](#1-作品概要)
- [2. 担当ソースコード](#2-担当ソースコード)
- [3. 操作説明](#3-操作説明)
- [4. ゲーム大賞に向けたチーム制作の取り組み](#4-ゲーム大賞に向けたチーム制作の取り組み)
- [5. プレイヤー](#5-プレイヤー)
  - [5.1. 通常攻撃](#51-通常攻撃)
  - [5.2. スキル](#52-スキル)
  - [5.3. 必殺技](#53-必殺技)
- [6. 敵CPU](#6-敵cpu)
  - [6.1. 評価値](#61-評価値)
- [7. 技術紹介(グラフィックス)](#7-技術紹介グラフィックス)
  - [7.1. デプスシャドウ](#71-デプスシャドウ)
  - [7.2. 川瀬式ブルームフィルター](#72-川瀬式ブルームフィルター)
- [8. U22プログラミング・コンテスト2023に向けた個人での取り組み](#8-u22プログラミングコンテスト2023に向けた個人での取り組み)
- [9. 複数人プレイ](#9-複数人プレイ)
  - [9.1. 画面分割](#91-画面分割)
- [10. ゲームUIの変更](#10-ゲームuiの変更)
  - [10.1. ミニマップの削除](#101-ミニマップの削除)
  - [10.2. ポイント表示UIの位置変更](#102-ポイント表示uiの位置変更)

---
# 1. 作品概要
* タイトル<br>
 **SWORD ARENA**<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/dcdef1a9-5495-4f71-896f-767dfb74e30f" width="480" alt="タイトル">
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/2b6bfb3e-ed21-43c8-adf2-a946752aceb6" width="480" alt="必殺技"><br>

* ゲーム内容<br>
プレイヤー剣士が、AI剣士3名とアリーナ内で戦い3分間でポイントの多さを競うアクションゲーム

  <a href="https://youtu.be/Aard8Ffjhc0">
  <img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/bfff498d-bd4b-4ec1-9067-101fa1596f57" style="width:480px; "alt="動画">
  </a>

* ルール
  * 時間は3分間
  * フィールド上にいるエネミー(ゴーストとうさぎ)を倒すと経験値を獲得し、Lvが上がる
  * レベルが上がると、ステータスが上昇
  * Lvが4以上になると必殺技が使用可能
  * 必殺技を使用するとLvは1になる
  * 勝利条件は、ポイントの多寡で決まる
  * ポイントは自分以外の剣士を倒すと獲得できる

  <img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/14379387-f21b-4ff7-af1f-f98b81bc6da4" width="480" alt="リザルト"><br>
* 学校
  * 河原電子ビジネス専門学校

* 制作人数
  * 4人

* 制作期間
  * 2023年2月～2023年5月(日本ゲーム大賞2023「アマチュア部門」用期間)
  * 2023年6月～2023年7月(機能追加・リファクタリング用期間)
  * 2023年8月～2023年10月(U22プログラミング・コンテスト2023用期間)

* ゲームジャンル
  * 3Dアクションゲーム

* プレイ人数
  * 1~4人

* 対応ハード
  * PC
    * Xbox 360 コントローラー

* 使用言語
  * C++
  * HLSL

* 使用ツール
  * プログラム(C++)
    * Visual Studio 2022
  * プログラム(HLSL)
    * VS Code
  * 3Dソフト
    * 3ds Max
  * エフェクト
    * Effekseer
  * 画像
    * Adobe Photoshop 2023
  * バージョン管理
    * Git
    * GitHub
    * Fork(GitGUIクライアント)
  * タスク管理
    * Notion
  * 連絡
    * Slack

* 開発環境
  * エンジン
    * 学校内製の簡易エンジン(DirectX12)
  * OS
    * Windows11

<div style="text-align: right;">

[目次へ](#目次)
</div>

# 2. 担当ソースコード
<details><summary>
  cppファイル,hファイル
  </summary>

* Bloom.cpp
* Bloom.h
* CameraCollisionSolver.cpp
* CameraCollisionSolver.h
* CollisionObject.cpp
* CollisionObject.h
* FontRender.cpp
* FontRender.h
* Level3DRender.cpp
* Level3DRender.h
* Light.cpp
* Light.h
* ModelRender.cpp
* ModelRender.h
* PostEffect.cpp
* PostEffect.h
* RenderingEngine.cpp
* RenderingEngine.h
* Shadow.cpp
* Shadow.h
* SkyCube.cpp
* SkyCube.h
* SpriteRender.cpp
* SpriteRender.h
* Result.cpp
* Result.h

</details>

<details><summary>
  fxファイル
  </summary>

* model.fx
* ShadowReciever.fx
* skyCubeMap.fx
* sprite.fx(一部)
  * 定数バッファ"SpriteCB"の追加
  * CalcGrayScale関数の追加
* gaussianBlur.fx
* postEffect.fx

</details>
<!--
# 3. 改造したエンジンコード
<details><summary>
  改造したエンジンコード
  </summary>
* a
</details>
-->

<div style="text-align: right;">

[目次へ](#目次)
</div>

# 3. 操作説明
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/35fb175a-1b44-42a6-87f9-3cb4ddd9111d" width="480" alt="タイトル～ゲーム開始前・リザルト画面"><br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/37fc138b-6d92-4bed-a6f3-20c278384dad" width="480" alt="ゲームプレイ中"><br>

<div style="text-align: right;">

[目次へ](#目次)
</div>

#  4. ゲーム大賞に向けたチーム制作の取り組み
ここからは日本ゲーム大賞に向けてチーム制作で取り組んだ内容について説明していきます。

# 5. プレイヤー
プレイヤーについて紹介していきます。
## 5.1. 通常攻撃
Aボタンを押すことで発動できます。最大3段の攻撃を行うことができます。

<a href="https://youtu.be/5uqOoY6OSqI">
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/632b3bd2-6afc-477c-91b3-ee8e3ac6552d" style="width:480px; "alt="通常攻撃">
</a>

### 工夫した点 <!-- omit in toc -->
1. 3段目の攻撃後にカメラを揺らすことで叩きつける感じを表現しています
1. 攻撃が連続して出ていることを視覚的に分かりやすくするためにエフェクトを出しました<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/24056bde-6eee-43db-94ae-0932c53b0b87" width="250" alt="normalAttack_effect"><br>

## 5.2. スキル
Bボタンを押すことで発動できます。前方に進みながら剣で攻撃をします。クールタイムは5秒です。

<a href="https://youtu.be/BAr0PuAihBU">
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/5fb6584c-14ea-406d-aaa6-4f591293edb7" style="width:480px; "alt="スキル">
</a>

### 工夫した点 <!-- omit in toc -->
1. 滑っているように見えることを防ぐために足元に砂埃のエフェクトを発生させました<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/7b88d675-c376-4bec-bec8-b32e6d3d55a0" width="480" alt="スキル砂埃"><br>

## 5.3. 必殺技
レベル4以上のときにXボタンを押すことで発動できます。雷の魔法を使用して敵全体に高威力のダメージを与えます。使用するとレベルは1になります。

<a href="https://youtu.be/AsS_TNDLVYY">
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/e688cd89-9150-435b-a498-3c7bf2a7deee" style="width:480px; "alt="必殺技">
</a>

### 工夫した点 <!-- omit in toc -->
1. 必殺技が敵全体に当たっているということをプレイヤーに伝えるために必殺技が当たった敵の正面をカメラで映すようにしました。<br>
1. 雷が落ちる雰囲気を演出するために空を徐々に暗くなるようにしました。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/45649542-5e18-496b-9634-2123850ae41a" width="480" alt="空の色変化"><br>

# 6. 敵CPU
アリーナ内にはプレイヤー以外の剣士として敵CPUが存在しています。この敵CPUについて説明していきます。
## 6.1. 評価値
&emsp;それぞれの剣士が保有しているポイントやレベルによる強さの関係などを考慮したCPUの動作を実装したかったため、評価値を使用しました。<br>
&emsp;この評価値は、主に他の剣士を追うもしくは逃げる対象にするかの判断の際に使用しました。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/b6ad2b6b-1a27-4c43-babc-29bc6c0a5b9d" width="480" alt="評価値"><br>

### 6.1.1. 評価値を計算する
条件によって評価値を増減させ最終的な評価値を計算していきます。<br>
下のプログラムは、敵が近くにいるとき評価値を上げる処理のコードです。
``` C++
//自分に近い敵が居たらを評価値を上げる
if (Distance <= 200.0f)
{
	eval += 2500; //評価値を上げる
}

//さらに近い敵がいる場合評価値を上げる
if (Distance <= 50.0f)
{
	eval += 5000;
}
```
### 6.1.2. 剣士の数だけ評価値の計算を行う
フィールド上には自身以外に3体の剣士が存在しています。そのため評価値の計算を各剣士行い、```std::vector```型に格納しています。
```C++
//剣士の数だけ繰り返す
for (auto actor : actors)
{
	EvalData eval = CalculateTargetAI(actor); //評価値を計算
	m_evaluationValueActor.push_back(eval);   //結果をstd::vector型に格納
}
```
### 6.1.3. 評価値を比較しターゲットを決定する
最後に、計算した各剣士の評価値を比較し、ターゲットとする剣士を決定します。<br>
下のプログラムは、追いかけるターゲットを決定しているコードです。
``` C++
//評価値を計算した剣士の数だけ繰り返す
for (int i = 0; i < m_evaluationValueActor.size(); i++)
{
	// 追いかける判定
	if (
        //評価値を比較し、現在のターゲットより大きい場合はtrueとする
        m_evaluationValueActor[i].eval > noweval_Target && 
        //追いかけるモードの場合trueとする
		m_evaluationValueActor[i].chaseOrEscape == false &&
		actors[i] != this)
	{
        //ターゲットにする剣士の評価値を代入
		noweval_Target = m_evaluationValueActor[i].eval;
        //ターゲットにする剣士を更新する
		m_nowActorTarget = actors[i];
	}
}
```

# 7. 技術紹介(グラフィックス)
## 7.1. デプスシャドウ
デプスシャドウとは、投影シャドウの考え方を発展させたものです。
投影シャドウと異なる点は「シャドウマップに書き込む値は、グレースケールではなく、ライトスクリーン空間での深度値(デプス)である。」という点です。<br>
実装手順は以下の通りです。

1. シャドウマップを作成する
2. ライトビュースクリーン空間でのZ値を計算する
3. シャドウマップに書き込まれているZ値と比較する

### 7.1.1. シャドウマップを作成する
Z値を書き込んでシャドウマップを作成します。
```HLSL
/// <summary>
/// シャドウマップ描画用のピクセルシェーダー
/// </summary>
float4 PSShadowMapMain(SPSIn psIn) : SV_Target0
{
    //シャドウマップにZ値を書き込む
    return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 1.0f);
}
```
上記のピクセルシェーダーを実行するとこのようなシャドウマップが作成されます。

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/efdbf696-e882-4ffa-bab4-5dd9aec715ce" width="480" alt="シャドウマップ">

---

### 7.1.2. ライトビュースクリーン空間でのZ値を計算する
影を受けるモデルのライトビュースクリーン空間でZ値を計算します。
```HLSL
float zInLVP = psIn.posInLVP.z / psIn.posInLVP.w;
```

---

### 7.1.3. シャドウマップに書き込まれているZ値と比較する
シャドウマップに書き込まれているZ値と2.で求めたライトビュースクリーン空間でのZ値を比較して、遮蔽されていれば影を落とします。
```HLSL
// シャドウマップに描き込まれているZ値と比較する
// UV座標を使って、シャドウマップから深度値をサンプリング
float zInShadowMap = g_shadowMap.Sample(g_sampler,shadowMapUV).r;
if(zInLVP > zInShadowMap)
{
  // 遮蔽されている
  albedoColor.xyz *= 0.7f;
}
```
↓実際のゲーム画面<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/15dc35c1-4a44-4960-ad04-759dfde81d3d" width="480" alt="デプスシャドウ">

<div style="text-align: right;">

[目次へ](#目次)
</div>

## 7.2. 川瀬式ブルームフィルター
ポストエフェクトの１つであるブルームを川瀬正樹氏が発表した川瀬式ブルームフィルターを使用して実装しました。<br>
普通のブルームはレンダリング後の画面から輝度抽出し、明るい部分にガウシアンブラーをかけて、元の画面に加算合成するものです。しかし、この方法だと強い光を当てた時、きれいに光が広がりません。<br>
一方、川瀬式ブルームフィルターはダウンサンプリングをしながら、複数回ガウシアンブラーをかけます。それらのテクスチャを同解像度に拡大合成して、あふれテクスチャを生成し、元の画面に加算合成しています。これにより、きれいに光があふれるようになります。<br>

実装手順は以下の通りです。

1. モデルを描画する
2. 輝度抽出をする
3. 輝度抽出したテクスチャにガウシアンブラーをかけボケ画像を作成
4. 3で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成
5. 4で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成
6. 5で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成
7. 4枚のボケ画像の合成し、シーンに加算合成する

>ブルーム効果がわかりやすいようにライトの明るさを上げています。
   
---

### 7.2.1. モデルを描画する
ポストエフェクトはレンダリングした絵に対し、レタッチを行ってエフェクトを追加していく処理のことです。
<br>そのため、まず全てのモデルを描画していきます。

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/cc102a1d-d98f-4961-9e16-5c58cb254fd2" width="480" alt="モデル描画">

---

### 7.2.2. 輝度抽出をする
レンダリング後の画面から輝度抽出をします。

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/8902cc43-fc5d-4ac5-8d86-9514aa1a1b0a" width="480" alt="輝度抽出">

---

### 7.2.3. 輝度抽出したテクスチャにガウシアンブラーをかけボケ画像を作成
輝度抽出したテクスチャをダウンサンプリングして、ガウシアンブラーをかけます。<br>
解像度を1920x1080 → 960x540 にダウンサンプリングしています。<br>
ダウンサンプリングをすることで頂点シェーダーの実行回数が減り処理速度が向上します。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/e17878b2-ba40-4306-ac54-6ded88ee472a" width="480" alt="ボケ画像1">

---

### 7.2.4. 3で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成
3で作成したボケ画像を960x540 → 480x270にダウンサンプリングしてさらにガウシアンブラーをかけます。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/9487a792-a09c-433e-969f-f64d394599e7" width="480" alt="ボケ画像2">

---

### 7.2.5. 4で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成
4で作成したボケ画像を480x270 → 240x135にダウンサンプリングしてさらにガウシアンブラーをかけます。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/0310b03e-6537-4c38-a751-d7b99e7ae6a5" width="480" alt="ボケ画像3">

---

### 7.2.6. 5で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成
5で作成したボケ画像を240x135 → 120x67にダウンサンプリングしてさらにガウシアンブラーをかけます。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/49566cab-f9d5-403b-a37b-500e48126b73" width="480" alt="ボケ画像4">

---

### 7.2.7. 4枚のボケ画像の合成し、シーンに加算合成する
作成した4枚のボケ画像の平均を取って合成し、シーンに加算合成します。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/9df53879-dada-420c-b5b7-6d13e06981b3" width="480" alt="ブルーム">

<div style="text-align: right;">

[目次へ](#目次)
</div>

---

# 8. U22プログラミング・コンテスト2023に向けた個人での取り組み
&emsp;この作品はU22プログラミング・コンテスト2023でSAJ賞を受賞しました。<br>
&emsp;日本ゲーム大賞に向けた取り組みの後、8月からはU22プログラミング・コンテスト2023に向けて個人でこの作品のブラッシュアップを行っていきましたのでその取り組みについて説明していきます。<br>
&emsp;この作品を学園祭などで多くの人にプレイしていただきコメントをうけたものや、自身でも課題に感じていた部分が
* 複数人で遊べるようにしたい
* 画面の見づらさ
* グラフィック面の強化

などがあり、そこを補い更に良くするために、以下の項目を実装しました。

# 9. 複数人プレイ
画面を分割することで複数人でのプレイができるようにしました。画面は2人プレイ時は左右の2画面、3～4人プレイ時は4画面に分割されるようにしています。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/b965678a-9c87-47ca-b0da-f5e1cd0325f0" width="480" alt="2人プレイ">
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/9b4754a6-bd9b-4105-a477-760919d68d88" width="480" alt="4人プレイ">　<br>

## 9.1. 画面分割
画面分割の実装方法について説明します。<br>

### 9.1.1. ビューポートを用意する
ビューポートを下の表にある縦幅、横幅、数だけ用意します。

|                   | 1人プレイ | 2人プレイ | 3～4人プレイ | 
| ----------------- | --------- | --------- | ------------ | 
| ビューポート:縦幅 | 1920      | 960       | 960          | 
| ビューポート:横幅 | 1080      | 1080      | 540          | 
| ビューポート:数   | 1         | 2         | 4            | 

下のコードは2画面分割で使用するビューポートの初期化処理です。<br>
ビューポートの位置を左右や斜めに設定する必要があるため、```TopLeftX``` ```TopLeftY```に座標を代入しています。

```c++
//FRAME_BUFFER_WIDTH_HALF = 960
//FRAME_BUFFER_H = 1080

//左の画面
m_duoViewPorts[enCameraDrawing_Left].Width = FRAME_BUFFER_WIDTH_HALF; //横幅
m_duoViewPorts[enCameraDrawing_Left].Height = FRAME_BUFFER_H;         //縦幅
m_duoViewPorts[enCameraDrawing_Left].TopLeftX = 0;                    //左横の座標
m_duoViewPorts[enCameraDrawing_Left].TopLeftY = 0;                    //左縦の座標

//右の画面
m_duoViewPorts[enCameraDrawing_Right].Width = FRAME_BUFFER_WIDTH_HALF;
m_duoViewPorts[enCameraDrawing_Right].Height = FRAME_BUFFER_H;
m_duoViewPorts[enCameraDrawing_Right].TopLeftX = FRAME_BUFFER_WIDTH_HALF; //左横を画面の中心に持ってくる
m_duoViewPorts[enCameraDrawing_Right].TopLeftY = 0;
```
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/1b19fc4a-805b-4784-aac0-8a04d898a0a9" width="480" alt="ビューポート図">

### 9.1.2. ビューポートにモデルを描画する
1つのビューポートにモデルを描画した後、別のビューポートに切り替えモデルを描画していきます。<br>

//画像

下のプログラムは2画面分割でフォワードレンダリングのモデル描画を行っているコードです。
``` C++
//ビューポートの数だけ繰り返す
for (int currentViewport = 0; currentViewport < DUO_VIEWPORT; currentViewport++)
{
	rc.SetViewport(m_duoViewPorts[currentViewport]);  //ビューポートを設定する
	for (auto& renderObj : m_renderObjects) {
		renderObj->OnForwardRender(rc);   //モデル描画
	}
}
```

# 10. ゲームUIの変更
ブラッシュアップを行うなかで画面が見づらいという意見があったためゲームUIの見直しを行いました。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/f12e6b4f-ffd2-409c-83d3-b38677c9b592" width="480" alt="ゲームUI変更点">
## 10.1. ミニマップの削除
左上にあったミニマップを削除しました。<br>
理由は、画面の占有率が高いかつ必要性が低かったからです。
## 10.2. ポイント表示UIの位置変更
画面左側にあったポイント表示を画面上部のタイマーと同じ高さに配置しました。<br>
移動したことにより画面横部分にUIがなくなったため見やすさが向上したと思います。
