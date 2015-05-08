#include "flib.h"
#include "flib_vec2.h"
#include "generics.h"
#include "Entity.h"
#include "Dynamic.h"
#include "Anim.h"
#include "Bird.h"
#include "Map.h"
#include "Villager.h"
#include "Floor.h"

using namespace Generics;

namespace
{
	const int NAME_MAX_SIZE = 250;

	const TGfxVec2 BIRD_SIZE = TGfxVec2(32.0f, 64.0f);
	const float BIRD_FLY_SPEED = 64.0f;					// Pixels per seconds

	const float TARGET_ZONE_RATIO = 75.f / PERCENTS;
	const float ESCAPE_ANGLE = 60.f; // Degree;
}


TGfxTexture * TBird::s_pTexture = nullptr;

TBird::TBird() :
TDynamic(),
m_eState(EState_Alive),
m_pFly(nullptr),
m_iLastMove(0)
{

}

TBird::TBird(const TGfxVec2 & tPos) :
TDynamic(tPos, BIRD_SIZE, BIRD_FLY_SPEED),
m_eState(EState_Alive),
m_pFly(nullptr),
m_iLastMove(0)
{
	m_pFly = new TAnim("Bird_Fly.tga", 7, 48, 48);
}

TBird::~TBird()
{
}


void TBird::Initialize()
{
	FindTarget();
}

void TBird::SpecificUpdate()
{
	GoToTarget();
	m_pSprite = m_pFly->Play(m_eDirection);
}


void TBird::Die()
{
	m_eState = EState_Dead;

}


void TBird::FindTarget()
{

	for (int i = 0; i < TMap::S_GetVillagerCount(); i++)
	{
		TVillager * pVillager = TMap::S_GetVillagers()[i];
		pVillager->GetPosition();
		if (TFloor::GetPosition().x - (TFloor::GetLeftSize() * TARGET_ZONE_RATIO) < pVillager->GetPosition().x
			&& TFloor::GetPosition().x + (TFloor::GetRightSize() * TARGET_ZONE_RATIO) > pVillager->GetPosition().x)
		{
			m_pTarget = pVillager;
			break;
		}
	}
}
void TBird::GoToTarget()
{
	GfxDbgPrintf("%f, %f \n", m_tPos.x, m_tPos.y);
	TGfxVec2 tDirection = m_pTarget->GetPosition() - m_tPos;
	tDirection = tDirection.Normalize();
	m_tVelocity = tDirection * m_fSpeed;
	m_tVelocity = ((GfxTimeGetMilliseconds() - float(m_iLastMove)) / SECONDS) * m_tVelocity;
}
void TBird::Escape()
{

}

bool TBird::IsAlive() const
{
	return m_eState == EState_Alive;
}

bool TBird::IsMouseOver(const TGfxVec2 & tMousePos) const
{
	const bool bMouseOnX = tMousePos.x >= m_tPos.x - (m_tSize.x / 2.0f) && tMousePos.x < m_tPos.x + (m_tSize.x / 2.0f);
	const bool bMouseOnY = tMousePos.y >= m_tPos.y - m_tSize.y && tMousePos.y <= m_tPos.y;

	if (bMouseOnX && bMouseOnY)
	{
		return true;
	}

	return false;
}

void TBird::Render() const
{
	if (m_pSprite != nullptr)
	{
		GfxSpriteRender(m_pSprite);
	}
}
