
#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

class TEntity
{
public:

	TEntity();
	TEntity(const TGfxVec2 & tPos, const TGfxVec2 & tSize);
	virtual ~TEntity();


	void Render();


protected:

	TGfxSprite * m_pSprite;

	TGfxVec2 m_tPos;
	TGfxVec2 m_tSize;
};

#endif
