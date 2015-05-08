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

	const char * const FLY_TILESET_NAME = "Bird_Fly.tga";

	const TGfxVec2 BIRD_SIZE = TGfxVec2(32.0f, 64.0f);
	const float BIRD_FLY_SPEED = 180.0f;					// Pixels per seconds

	const float TARGET_ZONE_RATIO = 75.f / PERCENTS;
	const float ESCAPE_ANGLE = 60.f; // Degree;

	const float EMPTY_SPACE_RANGE = 300.f;


	TGfxVec2 LEFT_ESCAPE_POINT ;
	TGfxVec2 RIGHT_ESCAPE_POINT;


	enum EStateChase
	{
		EState_GoingOnTarget = 0,
		EState_Escape,
	};
}


TGfxTexture * TBird::s_pBirdTileSet = nullptr;

TBird::TBird() :
TDynamic(),
m_eState(EState_Alive),
m_eAction(EBirdAction_ToTarget),
m_pTarget(nullptr),
m_pFly(nullptr),
m_iLastMove(0)
{


}

TBird::TBird(const TGfxVec2 & tPos) :
TDynamic(tPos, BIRD_SIZE, BIRD_FLY_SPEED),
m_eState(EState_Alive),
m_eAction(EBirdAction_ToTarget),
m_pTarget(nullptr),
m_pFly(nullptr),
m_iLastMove(0)
{
	m_pFly = new TAnim(s_pBirdTileSet, 7, 48, 48);

}

TBird::~TBird()
{
}


void TBird::S_Initialize()
{
	s_pBirdTileSet = GfxTextureLoad(FLY_TILESET_NAME);
	LEFT_ESCAPE_POINT = TGfxVec2(TFloor::GetPosition().x - TFloor::GetLeftSize() - EMPTY_SPACE_RANGE, 500.f);
	RIGHT_ESCAPE_POINT = TGfxVec2(TFloor::GetPosition().x + TFloor::GetRightSize() + EMPTY_SPACE_RANGE, 500.f);
}

void TBird::Initialize()
{
	FindTarget();
}

void TBird::SpecificUpdate()
{
	LEFT_ESCAPE_POINT = TGfxVec2(TFloor::GetPosition().x - TFloor::GetLeftSize() - EMPTY_SPACE_RANGE, 500.f);
	RIGHT_ESCAPE_POINT = TGfxVec2(TFloor::GetPosition().x + TFloor::GetRightSize() + EMPTY_SPACE_RANGE, 500.f);
	if (m_eAction == EBirdAction_ToTarget)
	{
		GoToTarget();
	}
	else
	{
		Escape();
	}
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
			if (pVillager->GetAction() != EAction_Grab)
			{
				m_pTarget = pVillager;
				break;
			}
		}
	}
}
void TBird::GoToTarget()
{

	TGfxVec2 tDirection = m_pTarget->GetPosition() - m_tPos;
	tDirection = tDirection.SquaredLength() >= 0.001f ? tDirection.Normalize() : TGfxVec2(0.0f, 0.0f);

	m_eDirection = tDirection.x >= 0.0f ? EDirection_Right : EDirection_Left;
	
	m_tVelocity = tDirection * (m_fSpeed * ((GfxTimeGetMilliseconds() - float(m_iLastMove)) / SECONDS));
	m_iLastMove = GfxTimeGetMilliseconds();

	if (m_pTarget->IsMouseOver(m_tPos))
	{
		m_pTarget->SetAction(EAction_Grab);
		m_pTarget->SetPosition(m_tPos);
		m_eAction = EBirdAction_DelivringTarget;
	}
}
void TBird::Escape()
{
	if ((LEFT_ESCAPE_POINT - m_tPos).Length() < (RIGHT_ESCAPE_POINT - m_tPos).Length())
	{
		TGfxVec2 tDirection = LEFT_ESCAPE_POINT - m_tPos;
		tDirection = tDirection.SquaredLength() >= 0.001f ? tDirection.Normalize() : TGfxVec2(0.0f, 0.0f);

		m_eDirection = tDirection.x >= 0.0f ? EDirection_Right : EDirection_Left;

		m_tVelocity = tDirection * (m_fSpeed * ((GfxTimeGetMilliseconds() - float(m_iLastMove)) / SECONDS));
		m_iLastMove = GfxTimeGetMilliseconds();
	}
	else
	{
		TGfxVec2 tDirection = RIGHT_ESCAPE_POINT - m_tPos;
		tDirection = tDirection.SquaredLength() >= 0.001f ? tDirection.Normalize() : TGfxVec2(0.0f, 0.0f);

		m_eDirection = tDirection.x >= 0.0f ? EDirection_Right : EDirection_Left;

		m_tVelocity = tDirection * (m_fSpeed * ((GfxTimeGetMilliseconds() - float(m_iLastMove)) / SECONDS));
		m_iLastMove = GfxTimeGetMilliseconds();
	}
	m_pTarget->SetPosition(m_tPos);

	if (m_tPos.x < LEFT_ESCAPE_POINT.x + (EMPTY_SPACE_RANGE/2.f))
	{
		m_pTarget->SetAction(EAction_Idle);
		FindTarget();
		m_eAction = EBirdAction_ToTarget;
	}
	if (m_tPos.x > RIGHT_ESCAPE_POINT.x - (EMPTY_SPACE_RANGE / 2.f))
	{
		m_pTarget->SetAction(EAction_Idle);
		FindTarget();
		m_eAction = EBirdAction_ToTarget;
	}
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