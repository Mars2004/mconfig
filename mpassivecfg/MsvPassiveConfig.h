/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Passive Config Implementation
* @details		Contains implementation @ref MsvPassiveConfig of @ref IMsvPassiveConfig interface.
* @author		Martin Svoboda
* @date			24.02.2019
* @copyright	GNU General Public License (GPLv3).
******************************************************************************************************/


/*
This file is part of MarsTech Config.

MarsTech Config is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MarsTech Promise Like Syntax is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef MARSTECH_PASSIVECONFIG_H
#define MARSTECH_PASSIVECONFIG_H


#include "MsvPassiveConfigBase.h"

MSV_DISABLE_ALL_WARNINGS

#include <mutex>
#include <map>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Passive Config Implementation.
* @details	Interface for passive configuration.
* @see		IMsvPassiveConfig
* @see		MsvPassiveConfigBase
******************************************************************************************************/
class MsvPassiveConfig:
	public MsvPassiveConfigBase
{
public:
	/**************************************************************************************************//**
	* @brief		Constructor.
	******************************************************************************************************/
	MsvPassiveConfig();

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvPassiveConfig();

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvPassiveConfig public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvPassiveConfig::ReloadConfiguration()
	******************************************************************************************************/
	virtual MsvErrorCode ReloadConfiguration() override;
};


#endif // !MARSTECH_PASSIVECONFIG_H

/** @} */	//End of group MCONFIG.
