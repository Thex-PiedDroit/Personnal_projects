
#ifndef VILLAGER_H_INCLUDED
#define VILLAGER_H_INCLUDED

class TVillager : public TDynamic
{
public:

	TVillager();
	TVillager(const TGfxVec2 & tPos);
	~TVillager();


	static void S_Initialize();

	void GetRandomName();


	void Die();

	bool IsMouseOver(const TGfxVec2 & tMousePos) const;

protected:

	enum EState
	{
		EState_Alive,
		EState_Dead
	};

	EState m_eState;

	char * m_pName;

	static TGfxTexture * s_pTexture;
	static TGfxFile * s_pNamesFile;
};

#endif
