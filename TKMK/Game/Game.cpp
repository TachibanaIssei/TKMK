#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Result.h"
#include "GameCamera.h"
#include "KnightBase.h"
#include "Actor.h"
#include "GameUI.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"

Game::Game()
{
	
}

Game::~Game()
{
	if (m_backGround != nullptr)
	{
		DeleteGO(m_backGround);
	}
	DeleteGO(m_gamecamera);
	DeleteGO(m_knightplayer);
	DeleteGO(m_Neutral_Enemy);
	DeleteGO(m_gameUI);
}

bool Game::Start()
{
	//�v���C���[
	/*m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Jump].Load("Assets/animData/UnitychanJump.tka");
	m_animationClips[enAnimationClip_Jump].SetLoopFlag(false);*/

	m_modelRender.Init("Assets/modelData/LightingModel/light.tkm");
	m_playerModel.Init("Assets/modelData/LightingModel/shield.tkm");
	
	m_position = { -50.0f,50.0f,-50.0f };
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(Vector3(1.0f, 1.0f, 1.0f));

	m_rotation.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rotation);

	m_playerModel.SetPosition(Vector3(0.0f,70.0f,-60.0f));
	m_playerModel.SetScale(Vector3::One);
	m_playerModel.SetRotation(m_rotation);

	InitLight();

	//�X�^�W�A���̃��x���̐ݒ�
	m_level3DRender.Init("Assets/level3D/stadiumLevel.tkl", [&](LevelObjectData& objData) {

		if (objData.EqualObjectName(L"stadium04") == true) {
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}
		return false;
	});
	//���m�̐���
	/*m_knightbase = NewGO<KnightBase>(0, "knightbase");
	m_knightbase->SetSGame(this);*/
	m_knightplayer = NewGO<KnightPlayer>(0, "m_knightplayer");
	m_knightplayer->SetSGame(this);
	

	//�Q�[���J�����̐���
	//gamecamera = NewGO<GameCamera>(0, "gamecamera");

	//�����̓G�̐���
	m_Neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
	m_Neutral_Enemy->SetNeutral_EnemyGame(this);

	//GameUIの生成
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	
	m_spriteRender.Init("Assets/sprite/magicball.DDS", 256.0f, 256.0f);
	m_spriteRender.SetPosition(100.0f, 100.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	/*m_fontRender.SetText(L"hello");
	m_fontRender.SetPosition(-500.0f, 200.0f);
	m_fontRender.SetScale(3.0f);
	m_fontRender.SetRotation(90.0f);
	m_fontRender.SetShadowParam(true, 2.0f, g_vec4Black);*/

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName,
		const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		});

	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	/*g_camera3D->SetPosition({ 0.0f, 200.0f, 200.0f });
	g_camera3D->SetTarget({ 0.0f, 150.0f, 0.0f });*/

	//�����蔻���L��������B
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		Result* result =NewGO<Result>(0, "Result");
		DeleteGO(this);
	}


	TestPlayer();
	UpdateLight();

	m_spriteAlpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0, fabsf(sinf(m_spriteAlpha))));

	m_modelRender.Update();
	m_playerModel.Update();
	m_spriteRender.Update();
}

void Game::TestPlayer()
{
	//�d��
	//m_moveSpeed.y -= 980.0f * 1.0f / 60.0f;

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);

		//�W�����v������
		//m_moveSpeed.y += 500.0f;

	}
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
	}
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		//m_modelRender.PlayAnimation(enAnimationClip_Jump, 0.1f);
	}


	//�L�����R���𓮂���
	//m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	/*if (m_charCon.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;
	}*/

	//���f���𓮂���
	m_modelRender.SetPosition(m_position);
}

void Game::UpdateLight()
{
	//�f�B���N�V�������C�g�̌��̕���
	Quaternion qdRotY;
	if (g_pad[0]->IsPress(enButtonRight))
	{
		qdRotY.SetRotationDegY(1.0f);
	}
	else if (g_pad[0]->IsPress(enButtonLeft))
	{
		qdRotY.SetRotationDegY(-1.0f);
	}
	Vector3 dir = g_renderingEngine->GetDirLigDirection();
	qdRotY.Apply(dir);
	g_renderingEngine->SetDirLightDirection(dir);

	//�J�����̉�]
	/*qdRotY.SetRotationDegY(g_pad[0]->GetLStickXF());
	auto camPos = g_camera3D->GetPosition();
	qdRotY.Apply(camPos);
	g_camera3D->SetPosition(camPos);*/
	
	Vector3 rotAxis;
	/*auto toPos = g_camera3D->GetPosition() - g_camera3D->GetTarget();
	auto dir2 = toPos;
	dir2.Normalize();
	rotAxis.Cross(dir2, g_vec3AxisY);
	qdRotY.SetRotationDeg(rotAxis, g_pad[0]->GetLStickYF());
	qdRotY.Apply(toPos);
	g_camera3D->SetPosition(g_camera3D->GetTarget() + toPos);*/
	g_renderingEngine->SetEyePos(g_camera3D->GetPosition());

	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_spotPosition.x -= 1.0f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_spotPosition.x += 1.0f;
	}
	if (g_pad[0]->IsPress(enButtonB))
	{
		g_renderingEngine->UseHemiLight();
		if (g_pad[0]->IsPress(enButtonUp)) {
			m_spotPosition.y += 1.0f;
		}
		if (g_pad[0]->IsPress(enButtonDown)) {
			m_spotPosition.y -= 1.0f;
		}
	}
	else
	{
		g_renderingEngine->UnUseHemiLight();
		if (g_pad[0]->IsPress(enButtonUp)) {
			m_spotPosition.z += 1.0f;
		}
		if (g_pad[0]->IsPress(enButtonDown)) {
			m_spotPosition.z -= 1.0f;
		}
	}

	Quaternion qRotY;
	qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);
	Vector3 dir1 = g_renderingEngine->GetSpotLightDirection();
	qRotY.Apply(dir1);

	Vector3 rotAxis1;
	rotAxis.Cross(g_vec3AxisY, g_renderingEngine->GetSpotLightDirection());
	Quaternion qRotX;
	qRotX.SetRotation(rotAxis1, g_pad[0]->GetRStickYF() * 0.01f);
	qRotX.Apply(dir1);

	g_renderingEngine->SetSpotLightDirection(dir1);

	Quaternion qRot;
	qRot.SetRotation({ 0.0f,0.0f,-1.0f }, g_renderingEngine->GetSpotLightDirection());

	m_modelRender.SetPosition(m_spotPosition);
	g_renderingEngine->SetSpotLightPosition(m_spotPosition);
	//m_modelRender.SetRotation(qRot);
}

void Game::InitLight()
{
	//�f�B���N�V�������C�g�̐ݒ�
	Vector3 directionLightDir = Vector3{ 0.0f,-1.0f,1.0f };
	directionLightDir.Normalize();
	Vector3 directionLightColor = Vector3{ 0.5f,0.5f,0.5f };
	g_renderingEngine->SetDirectionLight(0, directionLightDir, directionLightColor);
	g_renderingEngine->SetAmbient({ 0.3f,0.3f,0.3f });

	//�|�C���g���C�g�̐ݒ�
	Vector3 pointColor = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 range = Vector3(100.0f, 3.0f, 0.0f);
	//m_lightPos.y = 50.0f;
	//g_renderingEngine->SetPointLight(m_lightPos, pointColor, range);

	//�X�|�b�g���C�g�̐ݒ�
	m_spotPosition = { 0.0f,50.0f,-50.0f };
	Vector3 color = { 10.0f,10.0f,0.0f };
	Vector3 spotrange = { 300.0f,5.0f,0.0f };
	Vector3 direction = { 1.0f,-1.0f,1.0f };
	direction.Normalize();
	Vector3 angle = { Math::DegToRad(25.0f),0.5f,0.0f };
	g_renderingEngine->SetSpotLight(m_spotPosition, color, spotrange, direction, angle);

	g_renderingEngine->UseHemiLight();
}

void Game::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_playerModel.Draw(rc);
	//m_spriteRender.Draw(rc);
	//m_fontRender.Draw(rc);
	
}
