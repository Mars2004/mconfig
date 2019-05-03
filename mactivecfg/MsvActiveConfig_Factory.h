/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Active Config Factory
* @details		Contains implementation of dependency injection factory @ref MsvActiveConfig_Factory
*					for @ref MsvActiveConfig.
* @author		Martin Svoboda
* @date			24.02.2019
* @copyright	GNU General Public License (GPLv3).
******************************************************************************************************/


/*
This file is part of MarsTech Config.

MarsTech Dependency Injection is free software: you can redistribute it and/or modify
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


#ifndef MARSTECH_ACTIVECONFIG_FACTORY_H
#define MARSTECH_ACTIVECONFIG_FACTORY_H


#include "mdi/MdiFactory.h"

#include "MsvActiveConfigStorage.h"


/**************************************************************************************************//**
* @class		MsvActiveConfig_Factory
* @brief		MarsTech Active Config Dependency Injection Factory.
* @details	Implementation of dependency injection factory for @ref MsvActiveConfig.
******************************************************************************************************/
MSV_FACTORY_START(MsvActiveConfig_Factory)
MSV_FACTORY_GET_1(IMsvActiveConfigStorage, MsvActiveConfigStorage, std::shared_ptr<MsvLogger>);
MSV_FACTORY_END


#endif // MARSTECH_ACTIVECONFIG_FACTORY_H

/** @} */	//End of group MCONFIG.
