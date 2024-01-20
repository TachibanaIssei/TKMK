/////////////////////////////////////////////////////
// PBR���C�e�B���O�֌W�̍\���̂��܂Ƃ߂��w�b�_�[�t�@�C��
/////////////////////////////////////////////////////

#ifndef _PBRLIGHTING_STRUCT_H_
#define _PBRLIGHTING_STRUCT_H_

#include "Shadowing_const.h"

// �f�B���N�V�������C�g�\���́B
struct DirectionalLight
{
    float3 direction;   // ���C�g�̕���
    int castShadow;     // �e���L���X�g����H
    float4 color;       // ���C�g�̐F
};

struct EyeInfo
{
    float4x4 mViewProjInv[MAX_VIEWPORT];  // �r���[�v���W�F�N�V�����s��̋t�s��
    float3 eyePos[MAX_VIEWPORT];          // �J�����̎��_
    float pad;
    int drawCameraNumber;   //�`�悵�Ă���J�����̔ԍ�
};

struct LightCB
{
    DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
    float3 ambientLight;    // ����
    EyeInfo eyeInfomation;
    float4x4 mlvp[MAX_VIEWPORT][NUM_SHADOW_MAP];
    int gameMode;           //�Q�[�����[�h(0:1���,2:2���,4:4���)
};

#endif