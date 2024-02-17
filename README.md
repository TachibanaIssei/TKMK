#  **「SWORD ARENA」ポートフォリオ** <!-- omit in toc -->
### 河原電子ビジネス専門学校ゲームクリエイター科　2年　<!-- omit in toc -->
### 氏名：橘　一生　<!-- omit in toc -->
---

# 1. 目次

- [1. 目次](#1-目次)
- [2. 作品概要](#2-作品概要)
- [3. 担当ソースコード](#3-担当ソースコード)
- [4. 操作説明](#4-操作説明)
- [5. ゲーム大賞に向けたチーム制作の取り組み](#5-ゲーム大賞に向けたチーム制作の取り組み)
- [6. プレイヤー](#6-プレイヤー)
  - [6.1. 通常攻撃](#61-通常攻撃)
  - [6.2. スキル](#62-スキル)
  - [6.3. 必殺技](#63-必殺技)
  - [6.4. 経験値バー](#64-経験値バー)
- [7. 敵CPU](#7-敵cpu)
  - [7.1. 評価値](#71-評価値)
    - [7.1.1. 評価値を計算する](#711-評価値を計算する)
    - [7.1.2. 剣士の数だけ評価値の計算を行う](#712-剣士の数だけ評価値の計算を行う)
    - [7.1.3. 評価値を比較しターゲットを決定する](#713-評価値を比較しターゲットを決定する)
- [8. 技術紹介(グラフィックス)](#8-技術紹介グラフィックス)
  - [8.1. デプスシャドウ](#81-デプスシャドウ)
    - [8.1.1. シャドウマップを作成する](#811-シャドウマップを作成する)
    - [8.1.2. ライトビュースクリーン空間でのZ値を計算する](#812-ライトビュースクリーン空間でのz値を計算する)
    - [8.1.3. シャドウマップに書き込まれているZ値と比較する](#813-シャドウマップに書き込まれているz値と比較する)
  - [8.2. 川瀬式ブルームフィルター](#82-川瀬式ブルームフィルター)
    - [8.2.1. モデルを描画する](#821-モデルを描画する)
    - [8.2.2. 輝度抽出をする](#822-輝度抽出をする)
    - [8.2.3. 輝度抽出したテクスチャにガウシアンブラーをかけボケ画像を作成](#823-輝度抽出したテクスチャにガウシアンブラーをかけボケ画像を作成)
    - [8.2.4. 3で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成](#824-3で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成)
    - [8.2.5. 4で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成](#825-4で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成)
    - [8.2.6. 5で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成](#826-5で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成)
    - [8.2.7. 4枚のボケ画像の合成し、シーンに加算合成する](#827-4枚のボケ画像の合成しシーンに加算合成する)
- [9. U22プログラミング・コンテスト2023に向けた個人での取り組み](#9-u22プログラミングコンテスト2023に向けた個人での取り組み)
- [10. 複数人プレイ](#10-複数人プレイ)
  - [10.1. 画面分割](#101-画面分割)
    - [10.1.1. ビューポートを用意する](#1011-ビューポートを用意する)
    - [10.1.2. ビューポートにモデルを描画する](#1012-ビューポートにモデルを描画する)
- [11. ゲームUIの変更](#11-ゲームuiの変更)
  - [11.1. ミニマップの削除](#111-ミニマップの削除)
  - [11.2. ポイント表示UIの位置変更](#112-ポイント表示uiの位置変更)
- [12. PBR(物理ベースレンダリング)](#12-pbr物理ベースレンダリング)
  - [12.1. PBRのワークフロー](#121-pbrのワークフロー)
  - [12.2. 拡散反射光](#122-拡散反射光)
  - [12.3. 鏡面反射光](#123-鏡面反射光)
  - [12.4. ライティングを計算する](#124-ライティングを計算する)
- [13. ディファードレンダリング](#13-ディファードレンダリング)
  - [13.1. G-Buffer](#131-g-buffer)
    - [13.1.1. ワールド座標](#1311-ワールド座標)
    - [13.1.2. カメラ番号](#1312-カメラ番号)
    - [13.1.3. フォワードレンダリングとの融合](#1313-フォワードレンダリングとの融合)
- [14. カスケードシャドウ](#14-カスケードシャドウ)
  - [14.1. アルゴリズムの流れ](#141-アルゴリズムの流れ)
    - [14.1.1. 分割エリアの定義](#1411-分割エリアの定義)
    - [14.1.2. 分割エリアを描画するためのライトビュープロジェクション行列の計算](#1412-分割エリアを描画するためのライトビュープロジェクション行列の計算)
      - [14.1.2.1. 通常のデプスシャドウマップ技法用のライトビュープロジェクション行列を作成する](#14121-通常のデプスシャドウマップ技法用のライトビュープロジェクション行列を作成する)
      - [14.1.2.2. 分割エリアの定義に従って、各エリアを内包する視錐台の8頂点を求める](#14122-分割エリアの定義に従って各エリアを内包する視錐台の8頂点を求める)
      - [14.1.2.3. 各エリアの8頂点をライトビュープロジェクション空間に変換する](#14123-各エリアの8頂点をライトビュープロジェクション空間に変換する)
      - [14.1.2.4. 8頂点の最大値と最小値を利用して、ライトビュープロジェクション空間に収めるクロップ行列を計算する](#14124-8頂点の最大値と最小値を利用してライトビュープロジェクション空間に収めるクロップ行列を計算する)
      - [14.1.2.5. ライトビュープロジェクション行列とクロップ行列を乗算して最終的な行列を求める](#14125-ライトビュープロジェクション行列とクロップ行列を乗算して最終的な行列を求める)
  - [14.2. 影を描画する](#142-影を描画する)

---
# 2. 作品概要
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

# 3. 担当ソースコード
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

# 4. 操作説明
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/35fb175a-1b44-42a6-87f9-3cb4ddd9111d" width="480" alt="タイトル～ゲーム開始前・リザルト画面"><br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/37fc138b-6d92-4bed-a6f3-20c278384dad" width="480" alt="ゲームプレイ中"><br>

<div style="text-align: right;">

[目次へ](#目次)
</div>

#  5. ゲーム大賞に向けたチーム制作の取り組み
ここからは日本ゲーム大賞に向けてチーム制作で取り組んだ内容について説明していきます。

# 6. プレイヤー
プレイヤーについて紹介していきます。
## 6.1. 通常攻撃
Aボタンを押すことで発動できます。最大3段の攻撃を行うことができます。

<a href="https://youtu.be/5uqOoY6OSqI">
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/632b3bd2-6afc-477c-91b3-ee8e3ac6552d" style="width:480px; "alt="通常攻撃">
</a>

### 工夫した点 <!-- omit in toc -->
1. 3段目の攻撃後にカメラを揺らすことで叩きつける感じを表現しています
1. 攻撃が連続して出ていることを視覚的に分かりやすくするためにエフェクトを出しました<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/24056bde-6eee-43db-94ae-0932c53b0b87" width="250" alt="normalAttack_effect"><br>

## 6.2. スキル
Bボタンを押すことで発動できます。前方に進みながら剣で攻撃をします。クールタイムは5秒です。

<a href="https://youtu.be/BAr0PuAihBU">
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/5fb6584c-14ea-406d-aaa6-4f591293edb7" style="width:480px; "alt="スキル">
</a>

### 工夫した点 <!-- omit in toc -->
1. 足が滑っているように見えることを防ぐために足元に砂埃のエフェクトを発生させました<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/7b88d675-c376-4bec-bec8-b32e6d3d55a0" width="480" alt="スキル砂埃"><br>

## 6.3. 必殺技
レベル4以上のときにXボタンを押すことで発動できます。雷の魔法を使用して敵全体に高威力のダメージを与えます。使用するとレベルは1になります。

<a href="https://youtu.be/AsS_TNDLVYY">
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/e688cd89-9150-435b-a498-3c7bf2a7deee" style="width:480px; "alt="必殺技">
</a>

### 工夫した点 <!-- omit in toc -->
1. 必殺技が敵全体に当たっているということをプレイヤーに伝えるために必殺技が当たった敵の正面をカメラで映すようにしました。<br>
1. 雷が落ちる雰囲気を演出するために空を徐々に暗くなるようにしました。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/45649542-5e18-496b-9634-2123850ae41a" width="480" alt="空の色変化"><br>

## 6.4. 経験値バー
経験値のバーの処理について紹介します。
このゲームにおいて経験値が勝敗を左右するポイントなので、経験値バーの挙動を工夫しました。<br>
経験値の処理はプレイヤーが取得した経験値をUI側で、新たに用意して、プレイヤーの経験値の処理とは別に処理しました。
<br>
エネミーを倒したときに発生するオーブは、エネミーがいた場所の座標をスクリーン座標に変換して、経験値バーの場所まで線形補間で移動させました。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/123042729/82ba31ab-09f8-4e73-bb61-cb2420220115" width="480" alt="経験値オーブ"><br>

UI側で、プレイヤーが倒したエネミーから取得した経験値をUI側に保存して、現在の経験値テーブルに加算してレベルを上げて、経験値テーブルを次のレベルのものに変更するという処理をしていました。<br>

・経験値取得からUIに反映させるまでの流れ<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/123042729/94f3f168-a955-4a59-b759-ac30ee9484a9" width="480" alt="経験値オーブ"><br>

この段階では、エネミーを複数倒した時や連続で倒した時、必殺技を発動してレベルを消費した時にセーブした経験値の値がおかしくなり、UIの表示がおかしくなることが多々ありました。<br>



そのため、経験値の処理をステートで管理したり、取得した経験値の保存の仕方を変えたりすること不具合を解決しました。<br>

```c++
switch (m_enExpProcessState[enPlayerNumber])
{
  //前フレームと現在のフレームの経験値の値が違うかチェック
case GameUI::enChackExpState:
	ChackExp(player, enPlayerNumber);
	break;
  //経験値を増やす処理
case GameUI::enUpExpState:
	UpExp(enPlayerNumber);
	break;
  //経験値を減らす処理
case GameUI::enDownExpState:
	DownExp(enPlayerNumber);
	break;
  //レベルアップの処理
case GameUI::enLevelUpState:
	LevelUp(player, enPlayerNumber);
	break;
  //レベルダウンの処理
case GameUI::enLevelDownState:
	LevelDown(player, enPlayerNumber);
	break;
default:
	break;
}
```
<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/123042729/27576865-6df5-4a5d-849e-a87c7ca01f1e" width="480" alt="経験値オーブ"><br>

<div style="text-align: right;">

[目次へ](#目次)
</div>

# 7. 敵CPU
アリーナ内にはプレイヤー以外の剣士として敵CPUが存在しています。この敵CPUについて説明していきます。
## 7.1. 評価値
&emsp;それぞれの剣士が保有しているポイントやレベルによる強さの関係などを考慮したCPUの動作を実装したかったため、評価値を使用しました。<br>
&emsp;この評価値は、主に他の剣士を追うもしくは逃げる対象にするかの判断の際に使用しました。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/b6ad2b6b-1a27-4c43-babc-29bc6c0a5b9d" width="480" alt="評価値"><br>

### 7.1.1. 評価値を計算する
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
### 7.1.2. 剣士の数だけ評価値の計算を行う
フィールド上には自身以外に3体の剣士が存在しています。そのため評価値の計算を各剣士行い、```std::vector```型に格納しています。
```C++
//剣士の数だけ繰り返す
for (auto actor : actors)
{
	EvalData eval = CalculateTargetAI(actor); //評価値を計算
	m_evaluationValueActor.push_back(eval);   //結果をstd::vector型に格納
}
```
### 7.1.3. 評価値を比較しターゲットを決定する
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

<div style="text-align: right;">

[目次へ](#目次)
</div>

# 8. 技術紹介(グラフィックス)
## 8.1. デプスシャドウ
デプスシャドウとは、投影シャドウの考え方を発展させたものです。
投影シャドウと異なる点は「シャドウマップに書き込む値は、グレースケールではなく、ライトスクリーン空間での深度値(デプス)である。」という点です。<br>
実装手順は以下の通りです。

1. シャドウマップを作成する
2. ライトビュースクリーン空間でのZ値を計算する
3. シャドウマップに書き込まれているZ値と比較する

### 8.1.1. シャドウマップを作成する
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

### 8.1.2. ライトビュースクリーン空間でのZ値を計算する
影を受けるモデルのライトビュースクリーン空間でZ値を計算します。
```HLSL
float zInLVP = psIn.posInLVP.z / psIn.posInLVP.w;
```

---

### 8.1.3. シャドウマップに書き込まれているZ値と比較する
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

## 8.2. 川瀬式ブルームフィルター
&emsp;ポストエフェクトの１つであるブルームを川瀬正樹氏が発表した川瀬式ブルームフィルターを使用して実装しました。<br>
&emsp;普通のブルームはレンダリング後の画面から輝度抽出し、明るい部分にガウシアンブラーをかけて、元の画面に加算合成するものです。しかし、この方法だと強い光を当てた時、きれいに光が広がりません。<br>
&emsp;一方、川瀬式ブルームフィルターはダウンサンプリングをしながら、複数回ガウシアンブラーをかけます。それらのテクスチャを同解像度に拡大合成して、あふれテクスチャを生成し、元の画面に加算合成しています。これにより、きれいに光があふれるようになります。<br>

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

### 8.2.1. モデルを描画する
ポストエフェクトはレンダリングした絵に対し、レタッチを行ってエフェクトを追加していく処理のことです。
<br>そのため、まず全てのモデルを描画していきます。

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/cc102a1d-d98f-4961-9e16-5c58cb254fd2" width="480" alt="モデル描画">

---

### 8.2.2. 輝度抽出をする
レンダリング後の画面から輝度抽出をします。

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/8902cc43-fc5d-4ac5-8d86-9514aa1a1b0a" width="480" alt="輝度抽出">

---

### 8.2.3. 輝度抽出したテクスチャにガウシアンブラーをかけボケ画像を作成
輝度抽出したテクスチャをダウンサンプリングして、ガウシアンブラーをかけます。<br>
解像度を1920x1080 → 960x540 にダウンサンプリングしています。<br>
ダウンサンプリングをすることで頂点シェーダーの実行回数が減り処理速度が向上します。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/e17878b2-ba40-4306-ac54-6ded88ee472a" width="480" alt="ボケ画像1">

---

### 8.2.4. 3で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成
3で作成したボケ画像を960x540 → 480x270にダウンサンプリングしてさらにガウシアンブラーをかけます。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/9487a792-a09c-433e-969f-f64d394599e7" width="480" alt="ボケ画像2">

---

### 8.2.5. 4で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成
4で作成したボケ画像を480x270 → 240x135にダウンサンプリングしてさらにガウシアンブラーをかけます。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/0310b03e-6537-4c38-a751-d7b99e7ae6a5" width="480" alt="ボケ画像3">

---

### 8.2.6. 5で作成したボケ画像にさらにガウシアンブラーをかけボケ画像を作成
5で作成したボケ画像を240x135 → 120x67にダウンサンプリングしてさらにガウシアンブラーをかけます。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/49566cab-f9d5-403b-a37b-500e48126b73" width="480" alt="ボケ画像4">

---

### 8.2.7. 4枚のボケ画像の合成し、シーンに加算合成する
作成した4枚のボケ画像の平均を取って合成し、シーンに加算合成します。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/9df53879-dada-420c-b5b7-6d13e06981b3" width="480" alt="ブルーム">

<div style="text-align: right;">

[目次へ](#目次)
</div>

---

# 9. U22プログラミング・コンテスト2023に向けた個人での取り組み
&emsp;この作品はU22プログラミング・コンテスト2023でSAJ賞を受賞しました。<br>
&emsp;日本ゲーム大賞に向けた取り組みの後、8月からはU22プログラミング・コンテスト2023に向けて個人でこの作品のブラッシュアップを行っていきましたのでその取り組みについて説明していきます。<br>
&emsp;この作品を学園祭などで多くの人にプレイしていただきコメントをうけたものや、自身でも課題に感じていた部分が
* 複数人で遊べるようにしたい
* 画面の見づらさ
* グラフィック面の強化

などがあり、そこを補い更に良くするために、以下の項目を実装しました。

# 10. 複数人プレイ
画面を分割することで複数人でのプレイができるようにしました。画面は2人プレイ時は左右の2画面、3～4人プレイ時は4画面に分割されるようにしています。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/b965678a-9c87-47ca-b0da-f5e1cd0325f0" width="480" alt="2人プレイ">
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/9b4754a6-bd9b-4105-a477-760919d68d88" width="480" alt="4人プレイ">　<br>

## 10.1. 画面分割
画面分割の実装方法について説明します。<br>

### 10.1.1. ビューポートを用意する
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

### 10.1.2. ビューポートにモデルを描画する
1つのビューポートにモデルを描画した後、別のビューポートに切り替えモデルを描画していきます。<br>
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

<div style="text-align: right;">

[目次へ](#目次)
</div>

# 11. ゲームUIの変更
ブラッシュアップを行うなかで画面が見づらいという意見があったためゲームUIの見直しを行いました。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/f12e6b4f-ffd2-409c-83d3-b38677c9b592" width="480" alt="ゲームUI変更点">
## 11.1. ミニマップの削除
左上にあったミニマップを削除しました。<br>
理由は、画面の占有率が高いかつ必要性が低かったからです。
## 11.2. ポイント表示UIの位置変更
画面左側にあったポイント表示を画面上部のタイマーと同じ高さに配置しました。<br>
移動したことにより画面横部分にUIがなくなったため見やすさが向上したと思います。

<div style="text-align: right;">

[目次へ](#目次)
</div>

# 12. PBR(物理ベースレンダリング)
グラフィック面を強化するためにライティングの方法をPhongの反射モデルからPBRライティングに変更しました。<br>
PBRでは物理的に正しいシェーダーを使う必要があります。物理的に正しいシェーダーというのは以下の2つの法則を満たしたもののことを指しています。
1. エネルギー保存の法則
   * 入射してきた光より強い光をはね返さない
2. ヘルツホルツの相反性
   * 光が入射してくる方向と射出する方向が入れ替わっても光の射出量の結果が変わらない

## 12.1. PBRのワークフロー
Unityのメタリックモードに準拠しています。<br>
UnityのメタリックワークフローではPBRに関するパラメーターは次の3つになります。
1. アルベドカラー
2. メタリック
3. 滑らかさ

メタリックと滑らかさは1つのテクスチャにまとめて、メタリックはRチャンネル、滑らかさはαチャンネルを利用しています。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/5edd0abd-1570-44b4-aaab-d07649c60f7d" width="480" alt="剣士テクスチャ">

## 12.2. 拡散反射光
今回実装した拡散反射はフレネル反射を考慮したものです。<br>
フレネル反射とは、「光のサーフェイスへの入射角度が大きくなっていくと、反射率が上がる」という性質がある鏡面反射のことです。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/95749e46-d7b3-471a-8f2c-41bb14d26481" width="480" alt="フレネル反射"><br>

実装の際には以下の計算式を用いました。<br>
$F_{d90} = 0.5 + 2 \times roughness \times \cos^2\theta_d$と置いた場合
``` math
f_d = \frac{baseColor}{π}(1+(F_{D90}-1)(1-\cos{\theta_l})^5)(1+(F_{D90}-1)(1-\cos{\theta_v})^5)
```
1. $\frac{baseColor}{π}$:アルベドカラー
2. $1+(F_{D90}-1)(1-\cos{\theta_l})^5$:法線と光源に向かうベクトルに依存する拡散反射率
3. $1+(F_{D90}-1)(1-\cos{\theta_v})^5$:法線と視点に向かうベクトルに依存する拡散反射率

2.と3.を5乗しているのは、現実世界の現象は、指数関数的な変化をするからです。

<!--
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/5fe14c6d-0fe5-40a5-ae91-9b304645c12b" width="480" alt="フレネル反射">
-->

## 12.3. 鏡面反射光
&emsp;今回実装した鏡面反射はCook-Torranceモデルを利用しています。
Cook-Torranceモデルとは、光源色とは異なる色として観測される金属面で鏡面反射を正確に表現できるようにしたものです。

以下の計算式を使用しています。
```math
\frac{DFG}{E・N}
```
1. $D$:ベックマン分布項
2. $F$:フレネル項
3. $G$:幾何減衰項
4. $E$:視点へのベクトル
5. $N$:法線ベクトル

## 12.4. ライティングを計算する
滑らかさを使用して拡散反射光と鏡面反射光を合成し、ライティングの計算を行います。<br>
以下のプログラムは反射光を計算しているコードです。
```HLSL
// 反射光を計算する。
float3 CalcLighting(
    float3 ligDir,      // ライトの方向
    float3 ligColor,    // ライトの色
    float3 normal,      // 法線
    float3 toEye,       // 視点へのベクトル
    float4 albedoColor, // アルベドカラー
    float metaric,      // 金属度
    float smooth,       // 滑らかさ
    float3 specColor    // スペキュラカラー
    )
{
    // フレネル反射を考慮した拡散反射を計算
    float diffuseFromFresnel = CalcDiffuseFromFresnel(
        normal, -ligDir, toEye, smooth);

    // 正規化Lambert拡散反射を求める
    float NdotL = saturate(dot(normal, -ligDir));
    float3 lambertDiffuse = ligColor * NdotL / PI;

    // 最終的な拡散反射光を計算する
    float3 diffuse = albedoColor.xyz * diffuseFromFresnel * lambertDiffuse;

    // Cook-Torranceモデルを利用した鏡面反射率を計算する
    float3 spec = CookTorranceSpecular(
        -ligDir, toEye, normal, smooth)
        * ligColor;

    // 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
    // スペキュラカラーの強さを鏡面反射率として扱う
    spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metaric);

    // 滑らかさを使って、拡散反射光と鏡面反射光を合成する
    return max( float3( 0.0f, 0.0f, 0.0f ), diffuse * (1.0f - smooth) + spec * smooth );   
}
```

<div style="text-align: right;">

[目次へ](#目次)
</div>

# 13. ディファードレンダリング
&emsp;ディファードレンダリングとは、G-Bufferの内容を使ってポストエフェクト的にライティングの計算を行うレンダリング方法のことです。

## 13.1. G-Buffer
&emsp;ディファードレンダリングでは、ポリゴンをレンダリングするときにはライティングの計算は行わず、MRT(Multi Rendering Target)を活用して、G-Bufferと呼ばれる複数枚のテクスチャに「テクスチャカラー」「法線情報」「スペキュラ強度」「深度値」などを書き込みます。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/19f972c2-1551-4c8a-8684-a69fb548ce06" width="480" alt="G-Buffer"><br>

1. アルベドテクスチャ:RGBにアルベドカラー、αに深度値を書き込んでいます
2. メタリック・スムーステクスチャ:Rにメタリック、Gに影を落とすパラメータ、αにスムースを書き込んでいます
3. 法線テクスチャ：RGBに法線、αにカメラ番号を10で割った値を書き込んでいます
4. ライティング結果:1~3のG-Bufferを使ってライティングを行った画像です

### 13.1.1. ワールド座標
&emsp;ライティングを行う際にはワールド座標が必要ですが、G-Bufferには直接書き込んではいません。<br>
&emsp;今回は、1.のアルベドテクスチャのα値に書き込んでいる深度値(正規化スクリーン座標系のZ値)をディファードライティングを行う際にビュープロジェクション行列の逆行列を利用して、ワールド座標に復元しています。<br>
&emsp;G-Bufferを節約することでメモリ書き込みの節約につながり、ゲームのパフォーマンスの向上も期待できます。

### 13.1.2. カメラ番号
&emsp;3.の法線テクスチャのα値にカメラ番号を10で割った値を書き込んでいます。
このカメラ番号は、画面分割時にディファードライティングを行う際に使用しています。<br>
&emsp;書き込む際に10で割っている理由は、テクスチャフォーマットが``DXGI_FORMAT_R8G8B8A8_SNORM``のため書き込める値が-1.0~1.0の範囲となっているからです。<br>
&emsp;ディファードライティングを行う際に、10倍して復元を行っています。その際に精度の問題で整数にならない場合もあるためHLSLのround関数を使い、最も近い整数に丸めています。
```HLSL
//描画中のカメラの番号
int cameraNumber = round(normalTexture.Sample(Sampler,In.uv).a * 10.0f);
```

### 13.1.3. フォワードレンダリングとの融合
&emsp;今回の実装ではディファードレンダリングだけではなく、フォワードレンダリングも可能なハイブリッドエンジンになっています。
2つのレンダリング方法を融合することで、ディファードレンダリングが苦手な半透明オブジェクトの描画をフォワードレンダリングに任せることができるようになります。
このゲームでは、キャラクターやステージのモデルはディファードレンダリングで行い、空の描画をフォワードレンダリングで行っています。<br>

### 苦労した点 <!-- omit in toc -->
&emsp;実装の際にDrawコールを実行しているが空が描画されない問題が発生しました。<br>
この問題の原因はフォワードレンダリングで描画する際に使用する深度ステンシルビューをメインレンダーターゲットのものを使用しており、正しい前後関係が得られておらず空が描画はされているが奥に描画されていて見えていないというものでした。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/062fa6af-999d-4e06-9140-d8f4e1531cf0" width="480" alt="foward_depth"> <br>
深度ステンシルビューをG-Bufferを作成したときに作られたものを使用するように変更することで正しい前後関係を得ることができ、空が見えるようになりました。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/17e24af9-1234-48ce-aa5f-4134efd610e2" width="480" alt="foward_depth">

<div style="text-align: right;">

[目次へ](#目次)
</div>

# 14. カスケードシャドウ
&emsp;影はデプスシャドウを実装していましたが影を描画できる範囲が限られておりステージ全体に落とすことが難しい状態でした。そのためカスケードシャドウを実装することにしました。<br>
&emsp;カスケードシャドウマップは、複数枚のシャドウマップを利用することで影の品質を向上させます。カメラからの距離に応じて、「近距離描画用のシャドウマップ」「中距離描画用のシャドウマップ」「遠距離描画用のシャドウマップ」を変更して使用します。そして、近距離の描画ほど、高い解像度で狭いエリアを描画します。<br>

## 14.1. アルゴリズムの流れ
カスケードシャドウは、以下の4つのステップで構成されています。

1. 分割エリアの定義
2. 分割エリアを描画するためのライトビュープロジェクション行列の計算
3. 複数枚のシャドウマップにモデルを描画
4. 複数枚のシャドウマップを利用して影の描画

### 14.1.1. 分割エリアの定義
カメラからどこまでの距離を「近距離」「中距離」「遠距離」とするか、分割エリアを定義します。<br>
今回はカメラの遠平面までの距離に分割エリアの比率を乗算することで求めています。<br>

<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/1242e742-ac3c-4af4-9ef8-e57f7e477b49" width="480" alt="cascadeShadowMap">

```C++
std::array<float, NUM_SHADOW_MAP>	 m_cascadeAreaRateArray = { 0.1f, 0.3f,1.0f };	//カスケードシャドウの近～遠距離の比率

//分割エリアを計算する
float cascadeAreaTbl[NUM_SHADOW_MAP] = {
	g_camera3D[cameraNumber]->GetFar() * cascadeAreaRateTbl[SHADOW_MAP_AREA_NEAR],		//近影を映す最大深度値
	g_camera3D[cameraNumber]->GetFar() * cascadeAreaRateTbl[SHADOW_MAP_AREA_MIDDLE],	//中影を映す最大深度値
	g_camera3D[cameraNumber]->GetFar()													//遠影を映す最大深度値
};
```

### 14.1.2. 分割エリアを描画するためのライトビュープロジェクション行列の計算
各エリアのオブジェクトを正しくシャドウマップに描画するためのライトビュープロジェクション行列を計算する必要があります。その手順は以下の通りです。

1. 通常のデプスシャドウマップ技法用のライトビュープロジェクション行列を作成する
2. 分割エリアの定義に従って、各エリアを内包する視錐台の8頂点を求める
3. 各エリアの8頂点をライトビュープロジェクション空間に変換する
4. 8頂点の最大値と最小値を利用して、ライトビュープロジェクション空間に収めるクロップ行列を計算する
5. ライトビュープロジェクション行列とクロップ行列を乗算して最終的な行列を求める

#### 14.1.2.1. 通常のデプスシャドウマップ技法用のライトビュープロジェクション行列を作成する
&emsp;ライトをカメラに見立てて、ライトビュープロジェクション行列を求めます。<br>
&emsp;今回の実装では、太陽光からの影を表現したかったためプロジェクション行列に平行投影行列を利用しています。平行投影行列を利用することで、遠近感がない絵を描画することができ、光源から近くても遠くても大きさが変わらないようにすることができます。

#### 14.1.2.2. 分割エリアの定義に従って、各エリアを内包する視錐台の8頂点を求める
下の図の赤丸でマークしている16頂点を求めていきます。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/fe9a3e4a-51bb-4dd8-8e95-c17d4e9786e2" width="480" alt="camera_vertex"><br>

この頂点は、カメラの各種情報(位置、画角、前方方向、アスペクト比、カメラの上方向)がわかっていれば、三角比を利用して求められます。<br>
下の図は視錐台を横から見た図です。図中1.~3.にあるとおり、辺の長さは、画角と分割エリアの範囲がわかっていれば、tan関数を利用することで求めることができます。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/5261bd11-5d7a-4a00-9397-6ab23e2bffcc" width="480" alt="camera_vertex"><br>

#### 14.1.2.3. 各エリアの8頂点をライトビュープロジェクション空間に変換する
前のステップで求めた8頂点をライトビュープロジェクション空間に変換します。<br>
プログラムは以下のとおりです。
```C++
for (auto& v : vertex)
{
	lvpMatrix.Apply(v);
}
```

#### 14.1.2.4. 8頂点の最大値と最小値を利用して、ライトビュープロジェクション空間に収めるクロップ行列を計算する
&emsp;シャドウマップにすべてのオブジェクトを描画するために、変換した視錐台の8頂点を正規化スクリーン座標系の-1~1の範囲に収める必要があります。そのためにクロップ行列を計算していきます。<br>
&emsp;クロップ行列とは、$拡大行列×平行移動行列$で求められる行列のことです。

#### AABBを求める <!-- omit in toc -->
&emsp;ライトビュースクリーン空間に座標変換した8頂点を画面に収めるために、8頂点の最大値と最小値を求め、8頂点を内包するAABBを求めます。<br>
&emsp;8頂点の最大値と最小値を求めたら、このAABBが正規化スクリーン座標系にピッタリフィットするような拡大率を求めます。正規化スクリーン座標系の幅と高さの長さは2なので、拡大率を求めるプログラムは以下のようになります。
``` C++
float xScale = 2.0f / (vMax.x - vMin.x);
float yScale = 2.0f / (vMax.y - vMin.y);
```
&emsp;最後に座標を-1~1の範囲に収まるようにずらします。方法は、8頂点の最大値と最小値を利用して、AABBの中心座標を求め、その中心座標分だけ反対向きに平行移動させます。
``` C++
float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;
```

#### クロップ行列を求める　<!-- omit in toc -->
&emsp;8頂点を内包するAABBと、それをシャドウマップにピッタリ収めるための拡大率と平行移動量を求められたためこれらを利用して、クロップ行列を計算します。
``` C++
Matrix clopMatrix;
clopMatrix.m[0][0] = xScale;
clopMatrix.m[1][1] = yScale;
clopMatrix.m[3][0] = xOffset;
clopMatrix.m[3][1] = yOffset;
```

#### 14.1.2.5. ライトビュープロジェクション行列とクロップ行列を乗算して最終的な行列を求める
&emsp;求めたクロップ行列をライトビュープロジェクション行列に乗算し、最終的な行列を求めます。
```C++
m_lvpcMatrix[areaNo] = lvpMatrix * clopMatrix;
```

## 14.2. 影を描画する
&emsp;デプスシャドウマップと同じように、作成した行列を使って座標変換し、シャドウマップに描画して、モデルに影を落とします。<br>
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/abe308d5-33dc-48cd-8f12-a0271f030ae8" width="480" alt="shadowMap">
<img src="https://github.com/TachibanaIssei/TKMK/assets/121418275/87207175-e18f-46b0-9fdb-7b35137bee25" width="480" alt="cascadeShadow">

<div style="text-align: right;">

[目次へ](#目次)
</div>
