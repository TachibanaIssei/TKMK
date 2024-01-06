///////////////////////////////////////
// PBR�x�[�X�̃f�B�t�@�[�h���C�e�B���O��SRV_UAV���W�X�^�ݒ�
///////////////////////////////////////

#ifndef _DEFERREDLIGHTING_SRV_UAV_REGISTER_H_
#define _DEFERREDLIGHTING_SRV_UAV_REGISTER_H_

#include "Shadowing_const.h"

Texture2D<float4> albedoTexture : register(t0);                                         // �A���x�h
Texture2D<float4> normalTexture : register(t1);                                         // �@��
Texture2D<float4> metallicShadowSmoothTexture : register(t2);                           // ���^���b�N�A�V���h�E�A�X���[�X�e�N�X�`���Br�ɋ����x�Ag�ɉe�p�����[�^�Aa�Ɋ��炩���B
Texture2D<float4> g_shadowMap[NUM_SHADOW_MAP] : register(t3);    //�V���h�E�}�b�v(t3~t14�܂Ŏg�p)

#endif