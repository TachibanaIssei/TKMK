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

struct LightCB
{
    DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
    float4x4 mViewProjInv;  // �r���[�v���W�F�N�V�����s��̋t�s��
    float3 eyePos;          // �J�����̎��_
    float3 ambientLight;    // ����
    float4x4 mlvp[NUM_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP];
};

#endif