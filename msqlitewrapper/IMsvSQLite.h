/**************************************************************************************************//**
* @addtogroup	MSQLITEWRAPPER
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech SQLite Wrapper Interface
* @details		Contains interface of MarsTech SQLite Wrapper.
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


#ifndef MARSTECH_ISQLITE_H
#define MARSTECH_ISQLITE_H


#include "IMsvSQLiteCallback.h"
#include "merror/MsvError.h"

MSV_DISABLE_ALL_WARNINGS

#include <string>
#include <vector>
#include <memory>

MSV_ENABLE_WARNINGS


typedef std::vector<std::string> MsvSQLiteRow;				//!< Sqlite row definition.
typedef std::vector<MsvSQLiteRow> MsvSQLiteResult;			//!< Sqlite result definition.


/**************************************************************************************************//**
* @brief		MarsTech SQLite Wrapper Interface.
* @details	Interface for SQLite database.
* @see		MsvSQLite
******************************************************************************************************/
class IMsvSQLite
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvSQLite() {}

	/**************************************************************************************************//**
	* @brief			Initialize SQLite.
	* @details		Opens SQLite database.
	* @param[in]	dbPath		Path to SQLite database file.
	* @retval		MSV_ALREADY_INITIALIZED_INFO	When SQLite has been already initialized (this is info, not error).
	* @retval		MSV_OPEN_ERROR						When open SQLite database failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Initialize(const char* dbPath = "data.db") = 0;

	/**************************************************************************************************//**
	* @brief			Uninitialize SQLite.
	* @details		Closes SQLite connection.
	* @retval		MSV_NOT_INITIALIZED_INFO		When SQLite has not been initialized (this is info, not error).
	* @retval		MSV_BUSY_ERROR						When SQLite is busy and can't be unitialized.
	* @retval		MSV_CLOSE_ERROR					When closing SQLite failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() = 0;

	/**************************************************************************************************//**
	* @brief			Initialize check.
	* @details		Returns flag if SQLite is initialized (true) or not (false).
	* @retval		true		When initialized.
	* @retval		false		When not initialized.
	******************************************************************************************************/
	virtual bool Initialized() const = 0;

	/**************************************************************************************************//**
	* @brief			Create table if not exists.
	* @details		Checks if table exists. Creates new table when it does not exists.
	* @param[in]	tableName			Table name.
	* @param[in]	tableDef				Table definition (columns definition).
	* @param[in]	postCreateDefs		Post create table definitions (indexes, etc.).
	* @retval		MSV_ALREADY_EXISTS_INFO			When table already exists (new table is not created, this is info, not error).
	* @retval		MSV_NOT_INITIALIZED_ERROR		When SQLite has not been initialized.
	* @retval		MSV_EXECUTE_ERROR					When execute SQL query failed.
	* @retval		MSV_SUCCESS							On success (new table has been created).
	* @note			Internally calls @ref Execute.
	* @see			Execute
	******************************************************************************************************/
	virtual MsvErrorCode CreateTableIfNotExists(const char* tableName, const char* tableDef, const char* postCreateDefs = "") = 0;

	/**************************************************************************************************//**
	* @brief			Execute SQL query.
	* @details		Execute SQL query and converts its result to @ref MsvSQLiteResult.
	* @param[in]	query			SQL query to execute.
	* @param[out]	result		Result of executed SQL query.
	* @retval		MSV_NOT_INITIALIZED_ERROR		When SQLite has not been initialized.
	* @retval		MSV_EXECUTE_ERROR					When execute SQL query failed.
	* @retval		MSV_SUCCESS							On success (new table has been created).
	******************************************************************************************************/
	virtual MsvErrorCode Execute(const char* query, MsvSQLiteResult& result) = 0;

	/**************************************************************************************************//**
	* @brief			Register SQLite callback.
	* @details		Registers SQLite callback which is called when data has been changed.
	* @param[in]	spCallback		Callback to register.
	* @retval		MSV_ALREADY_REGISTERED_INFO	When callback has been already registered (this is info, not error).
	* @retval		MSV_SUCCESS							On success.
	* @see			IMsvSQLiteCallback
	******************************************************************************************************/
	virtual MsvErrorCode RegisterCallback(std::shared_ptr<IMsvSQLiteCallback> spCallback) = 0;

	/**************************************************************************************************//**
	* @brief			Unregister SQLite callback.
	* @details		Unregisters SQLite callback which is called when data has been changed.
	* @param[in]	spCallback		Callback to unregister.
	* @retval		MSV_SUCCESS		On success.
	* @see			IMsvSQLiteCallback
	******************************************************************************************************/
	virtual MsvErrorCode UnregisterCallback(std::shared_ptr<IMsvSQLiteCallback> spCallback) = 0;
};


#endif // !MARSTECH_ISQLITE_H

/** @} */	//End of group MSQLITEWRAPPER.
