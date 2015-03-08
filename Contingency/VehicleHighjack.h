/*************************************************************************
$Id$
$DateTime$
Description: Allows vehicle highjack system.

-------------------------------------------------------------------------
History:
- 22:02:2015: Created by Dustin Roden

*************************************************************************/

#ifndef __VEHICLE_HIGHJACK_H__
#define __VEHICLE_HIGHJACK_H__

class CVehicleHighjack
{
public:
	CVehicleHighjack();
	virtual ~CVehicleHighjack();

	virtual bool Highjack(const EntityId &userId, IVehicleSeat *pSeat);
	virtual void PlayAnimation(string animName, const EntityId &userId, int layer, int token, bool looped, float speed, float transition);

private:
	IVehicleSeat* GetClosestSeat(IVehicle *pVehicle, const EntityId &userId);
};

#endif