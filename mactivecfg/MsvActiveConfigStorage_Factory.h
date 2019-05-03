/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Active Config Storage Factory
* @details		Contains implementation of dependency injection factory @ref MsvActiveConfigStorage_Factory
*					for @ref MsvActiveConfigStorage.
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


#ifndef MARSTECH_ACTIVECONFIGSTORAGE_FACTORY_H
#define MARSTECH_ACTIVECONFIGSTORAGE_FACTORY_H


#include "mdi/MdiFactory.h"

#include "mconfig/msqlitewrapper/MsvSQLite.h"


/**************************************************************************************************//**
* @class		MsvActiveConfigStorage_Factory
* @brief		MarsTech Active Config Storage Dependency Injection Factory.
* @details	Implementation of dependency injection factory for @ref MsvActiveConfigStorage.
******************************************************************************************************/
MSV_FACTORY_START(MsvActiveConfigStorage_Factory)
MSV_FACTORY_GET_1(IMsvSQLite, MsvSQLite, std::shared_ptr<MsvLogger>);
MSV_FACTORY_END


#endif // MARSTECH_ACTIVECONFIGSTORAGE_FACTORY_H

/** @} */	//End of group MCONFIG.
