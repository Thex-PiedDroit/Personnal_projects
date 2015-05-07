
#ifndef HOUSE_H_INCLUDED
#define HOUSE_H_INCLUDED

class THouse : public TBuilding
{
public:

	THouse();
	THouse(const TGfxVec2 & tPos);
	~THouse();


	static void S_Initialize();


	void SpecificUpdate();


private:

	static TGfxTexture * s_pTexture;

	int m_iLastSpawnTime;
};

#endif
