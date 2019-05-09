/**************************************************************************************************//**
* @defgroup		MSQLITEWRAPPER MarsTech SQLite Wrapper
* @brief			MarsTech SQLite Wrapper
* @details		Contains implementation and all definitions of MarsTech SQLite Wrapper.
* @copyright	GNU General Public License (GPLv3).
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech SQLite Callback Interface
* @details		Contains interface of MarsTech SQLite Callback.
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


#ifndef MARSTECH_ISQLITECALLBACK_H
#define MARSTECH_ISQLITECALLBACK_H


#include "mheaders/MsvCompiler.h"
MSV_DISABLE_ALL_WARNINGS

#include <cstdint>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech SQLite Callback Interface.
* @details	Interface for SQLite callback which notifies about data changes in a database.
* @warning	This callback works only with one instance of SQLite and it is not cross process. Other
*				process won't be notified about changes in a SQLite database. Same for more instances
*				of SQLite in one process.
******************************************************************************************************/
class IMsvSQLiteCallback
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvSQLiteCallback() {}

	/**************************************************************************************************//**
	* @brief			Data has been changed.
	* @details		This method is called when data has been changed (callback must be registered before).
	* @param[in]	operationType		Operation type (update, insert, delete, etc.).
	* @param[in]	databaseName		Database name (in which has been data changed).
	* @param[in]	tableName			Table name (in which has been data changed).
	* @param[in]	rowId					ID of change value (column).
	******************************************************************************************************/
	virtual void OnChange(int operationType, const char *databaseName, const char *tableName, int64_t rowId) = 0;
};


#endif // !MARSTECH_ISQLITECALLBACK_H

/** @} */	//End of group MSQLITEWRAPPER.
