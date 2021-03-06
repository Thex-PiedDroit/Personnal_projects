
#ifndef BUILDING_H_INCLUDED
#define BUILDING_H_INCLUDED

class TVillager;

class TBuilding : public TEntity
{
public:

	TBuilding();
	TBuilding(const TGfxVec2 & tPos, const TGfxVec2 & tSize);
	virtual ~TBuilding();


	static void S_InitializeBuildings();


	void Update();
	virtual void SpecificUpdate() = 0;

	bool DropCivilian(TVillager * pVillager);

	EBuildingType GetBuildingType()
	{
		return m_eBuildingType;
	}

	TGfxSprite * GetAssignedVillager()
	{
		return m_pAssignedVillagersSprite;
	}

	virtual bool AssignVillager(TVillager * pVillager) = 0;
	virtual void UnassignVillager(TVillager * pVillager) = 0;


	virtual void SpecificRender() = 0;


protected:

	EBuildingType m_eBuildingType;

	int m_iRequiredAge;

	TVillager ** m_pAssignedVillagers;
	int m_iAssignedVillagersCount;

	static TGfxTexture * s_pAssignedVillagersTileSet;
	TGfxSprite * m_pAssignedVillagersSprite;
};

#endif
