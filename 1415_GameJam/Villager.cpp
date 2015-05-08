
#include "flib.h"
#include "flib_vec2.h"
#include "Villager.h"
#include "Entity.h"
#include "Dynamic.h"
#include "HUD.h"
#include "Floor.h"
#include <string.h>



namespace
{
	const char * const SPRITE_NAME = "Villager.tga";
	const char * const NAMES_FILE = "Names.txt";

	const int NAME_MAX_SIZE = 250;

	const TGfxVec2 VILLAGER_SIZE = TGfxVec2(32.0f, 64.0f);
	const float VILLAGER_WALK_SPEED = 64.f;				// Pixels per seconds
	const int MIN_SECOND_CHANGE_MOVE = 2;
	const int MAX_SECOND_CHANGE_MOVE = 10;
	const float GRAVITY = 0.2f;
}


TGfxTexture * TVillager::s_pTexture = nullptr;
TGfxFile * TVillager::s_pNamesFile = nullptr;

TVillager::TVillager() :
TDynamic(),
m_eState(EState_Alive),
m_iNextMoveChoice(GfxTimeGetMilliseconds()),
m_iLastMoveChoice(GfxTimeGetMilliseconds()),
m_iLastMove(GfxTimeGetMilliseconds()),
m_pName(nullptr)
{
}

TVillager::TVillager(const TGfxVec2 & tPos) :
TDynamic(tPos, VILLAGER_SIZE, VILLAGER_WALK_SPEED),
m_eState(EState_Alive),
m_iNextMoveChoice(GfxTimeGetMilliseconds()),
m_iLastMoveChoice(GfxTimeGetMilliseconds()),
m_iLastMove(GfxTimeGetMilliseconds()),
m_pName(nullptr)
{
	m_pSprite = GfxSpriteCreate(s_pTexture);
	GfxSpriteSetPivot(m_pSprite, (m_tSize.x / 2.0f), m_tSize.y);

	//GetRandomName();
}

TVillager::~TVillager()
{
	if (m_pName != nullptr)
	{
		delete[] m_pName;
		m_pName = nullptr;
	}
}


void TVillager::S_Initialize()
{
	s_pTexture = GfxTextureLoad(SPRITE_NAME);

	s_pNamesFile = GfxFileOpenRead(NAMES_FILE);
}

void TVillager::GetRandomName()
{
	char * pCursor = static_cast<char *>(GfxMemAlloc(GfxFileSize(s_pNamesFile) + 1));
	GfxFileRead(s_pNamesFile, pCursor, GfxFileSize(s_pNamesFile));

	int iNamesCount = 0;
	int i = 0;

	for (i = 0; pCursor != nullptr && pCursor[i] != '\0'; i++)
	{
		if (pCursor[0] == '\r')
		{
			iNamesCount++;
		}

	}

	const int iRandomNameLine = GfxMathGetRandomInteger(0, iNamesCount);

	i = 0;

	for (int iLine = 0; iLine != iRandomNameLine; i++)
	{
		if (pCursor[i] == '\r')
		{
			iLine++;
		}
	}

	char pName[NAME_MAX_SIZE] = { 0 };

	for (int iLetter = 0/*, i = i*/; pCursor[i] != '\r' && pCursor[i] != '\n'; iLetter++)
	{
		pName[iLetter] = pCursor[i];
		i++;
	}

	m_pName = new char[NAME_MAX_SIZE]{ 0 };
	strcpy(m_pName, pName);
}


void TVillager::SpecificUpdate()
{
	RandomMove();
	Move();
}

void TVillager::Move()
{
	// Move on X

	if (m_tPos.y + 10.f < TFloor::GetPosition().y 
		||(m_tPos.x < TFloor::GetPosition().x - TFloor::GetLeftSize()
		|| m_tPos.x > TFloor::GetPosition().x + TFloor::GetRightSize()))
	{
		m_tVelocity.y += GRAVITY;
		m_tPos.y += m_tVelocity.y;
		if (m_tPos.y > 1080)
		{
			Die();
		}
		GfxSpriteSetPosition(m_pSprite, m_tPos.x, m_tPos.y);
		m_iLastMove = GfxTimeGetMilliseconds();
	}
	else
	{
		m_tVelocity.y = 0.f;
		m_tPos.y = TFloor::GetPosition().y;
		float fTimePassed = (GfxTimeGetMilliseconds() - m_iLastMove) / 1000.f;
		m_tPos.x += m_tVelocity.x * fTimePassed;
		GfxSpriteSetPosition(m_pSprite, m_tPos.x, m_tPos.y);
		m_iLastMove = GfxTimeGetMilliseconds();
	}

}
void TVillager::RandomMove()
{
	int iTimePassed = GfxTimeGetMilliseconds() - m_iLastMoveChoice;
	if (iTimePassed >= m_iNextMoveChoice)
	{
		m_iLastMoveChoice = GfxTimeGetMilliseconds();
		m_iNextMoveChoice = GfxMathGetRandomInteger(MIN_SECOND_CHANGE_MOVE, MAX_SECOND_CHANGE_MOVE) * 1000;
		int iSide = GfxMathGetRandomInteger(0, 1);
		if (iSide == 0)
		{
			m_tVelocity.x = VILLAGER_WALK_SPEED;
		}
		else
		{
			m_tVelocity.x = -VILLAGER_WALK_SPEED;
		}
	}
}

void TVillager::Die()
{
	m_eState = EState_Dead;

	THUD::S_OneMoreSuicide();
}

bool TVillager::IsMouseOver(const TGfxVec2 & tMousePos) const
{
	const bool bMouseOnX = tMousePos.x >= m_tPos.x - (m_tSize.x / 2.0f) && tMousePos.x < m_tPos.x + (m_tSize.x / 2.0f);
	const bool bMouseOnY = tMousePos.y >= m_tPos.y - m_tSize.y && tMousePos.y <= m_tPos.y;

	if (bMouseOnX && bMouseOnY)
	{
		return true;
	}

	return false;
}

void TVillager::Render() const
{
	GfxSpriteRender(m_pSprite);
}
