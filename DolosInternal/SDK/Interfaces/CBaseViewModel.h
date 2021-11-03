#pragma once

#ifndef VIEW_MODEL_H
#define VIEW_MODEL_H

#include "../../Utils/Macros.h"
#include "CBaseCombatWeapon.h"


class CBaseViewModel : public CBaseEntity {
public:

    NETVAR(EHANDLE, GetOwner    , "DT_BaseViewModel", "m_hOwner");
    NETVAR(EHANDLE, GetWeapon   , "DT_BaseViewModel", "m_hWeapon");

};


#endif // !VIEW_MODEL_H
