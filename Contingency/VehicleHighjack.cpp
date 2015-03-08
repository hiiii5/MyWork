/*************************************************************************
$Id$
$DateTime$
Description: Allows vehicle highjack system.

-------------------------------------------------------------------------
History:
- 22:02:2015: Created by Dustin Roden

*************************************************************************/

#include "stdafx.h"
#include "VehicleHighjack.h"
#include "VehicleSeat.h"

CVehicleHighjack::CVehicleHighjack()
{
}


CVehicleHighjack::~CVehicleHighjack()
{
}

bool CVehicleHighjack::Highjack(const EntityId &userId, IVehicleSeat *pSeat)
{
	CryLogAlways("[HIGHJACK] HIGHJACK");
	
	CRY_ASSERT(pSeat);

	// Get the closest seat
	pSeat = GetClosestSeat(pSeat->GetVehicle(), userId);
	CVehicleSeat *pSeatXML = (CVehicleSeat*)pSeat;

	// Perform initial check for long check
	if (pSeat->GetPassenger() > 0)
	{
		// Perform highjackable check
		if (pSeatXML->CanHighjack() && userId != pSeat->GetPassenger() && pSeat->GetVehicle()->IsCrewHostile(pSeat->GetPassenger()))
		{
			pSeat->Exit(true);
			pSeat->Enter(userId);

			return true;
		}
	}

	// If previous check didnt return, see if we can just enter it
	if (pSeat->Enter(userId))
		return true;

	return false;
}

void CVehicleHighjack::PlayAnimation(string animName, const EntityId &userId, int layer, int token, bool looped, float speed, float transition)
{
	ICharacterInstance *character = gEnv->pEntitySystem->GetEntity(userId)->GetCharacter(0);
	ISkeletonAnim* pSkeleton;
	if (character && (pSkeleton = character->GetISkeletonAnim()))
	{
		CryCharAnimationParams Params;
		Params.m_fAllowMultilayerAnim = 1;
		Params.m_nLayerID = layer;
		if (looped)
			Params.m_nFlags |= CA_LOOP_ANIMATION;


		Params.m_nFlags |= CA_REMOVE_FROM_FIFO;
		Params.m_nUserToken = token;

		Params.m_fTransTime = transition;
		Params.m_fPlaybackSpeed = speed;
		CryLogAlways("[HIGHJACK] ANIMATION");
		pSkeleton->StartAnimation(animName.c_str(), Params);
	}
}

IVehicleSeat* CVehicleHighjack::GetClosestSeat(IVehicle *pVehicle, const EntityId &userId)
{
	Vec3 playerPos = gEnv->pEntitySystem->GetEntity(userId)->GetPos();
	IVehicleSeat *pClosestSeat = pVehicle->GetSeatById(FirstVehicleSeatId);
	f32 lastSeatDist = 1000;

	if (pVehicle->GetSeatCount() > 1)
	{
		// Loop for all seats and get the closest one
		for (TVehicleSeatId id = FirstVehicleSeatId; id <= pVehicle->GetSeatCount(); id++)
		{
			if (pVehicle->GetSeatById(id)->GetSitHelper())
			{
				// Get the seat postion in world space
				Vec3 seatPos = pVehicle->GetSeatById(id)->GetSitHelper()->GetWorldSpaceTranslation();
				f32 seatDist = playerPos.GetDistance(seatPos);

				// Ensure distance is less than before
				if (seatDist < lastSeatDist)
				{
					lastSeatDist = seatDist;
					pClosestSeat = pVehicle->GetSeatById(id);
				}
			}
		}
	}

	return pClosestSeat;
}
