/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Config Key Map Interface
* @details		Contains interface of MarsTech Config Key Map.
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


#ifndef MARSTECH_ICONFIGKEYMAP_H
#define MARSTECH_ICONFIGKEYMAP_H


#include "merror/MsvError.h"

MSV_DISABLE_ALL_WARNINGS

#include <map>
#include <memory>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Config Key Map Interface.
* @details	Interface for config key map.
******************************************************************************************************/
template<class T> class IMsvConfigKeyMap
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvConfigKeyMap() {}

	/**************************************************************************************************//**
	* @brief			Get config key map.
	* @details		Returns const reference to internal config key map storage.
	******************************************************************************************************/
	virtual const std::map<int32_t, std::shared_ptr<T>>& GetMap() const = 0;
};


#endif // !MARSTECH_ICONFIGKEYMAP_H

/** @} */	//End of group MCONFIG.
