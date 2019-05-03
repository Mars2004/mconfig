/**************************************************************************************************//**
* @addtogroup	MSQLITEWRAPPER
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech SQLite Wrapper Implementation
* @details		Contains implementation @ref MsvSQLite of @ref IMsvSQLite interface.
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


#ifndef MARSTECH_SQLITE_H
#define MARSTECH_SQLITE_H


#include "IMsvSQLite.h"

#include "mlogging/mlogging.h"

MSV_DISABLE_ALL_WARNINGS

#include "3rdParty/sqlite/sqlite3.h"

#include <mutex>
#include <forward_list>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech SQLite Implementation.
* @details	Implementation of SQLite wrapper.
* @see		IMsvSQLite
******************************************************************************************************/
class MsvSQLite:
	public IMsvSQLite
{
public:
	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	spLogger			Shared pointer to logger for logging.
	******************************************************************************************************/
	MsvSQLite(std::shared_ptr<MsvLogger> spLogger = nullptr);

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvSQLite();

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvSQLite public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvSQLite::Initialize(const char* dbPath)
	******************************************************************************************************/
	virtual MsvErrorCode Initialize(const char* dbPath = "data.db") override;

	/**************************************************************************************************//**
	* @copydoc IMsvSQLite::Uninitialize()
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() override;

	/**************************************************************************************************//**
	* @copydoc IMsvSQLite::Initialized() const
	******************************************************************************************************/
	virtual bool Initialized() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvSQLite::CreateTableIfNotExists(const char* tableName, const char* tableDef, const char* postCreateDefs)
	******************************************************************************************************/
	virtual MsvErrorCode CreateTableIfNotExists(const char* tableName, const char* tableDef, const char* postCreateDefs = "") override;

	/**************************************************************************************************//**
	* @copydoc IMsvSQLite::Execute(const char* query, MsvSQLiteResult& result)
	******************************************************************************************************/
	virtual MsvErrorCode Execute(const char* query, MsvSQLiteResult& result) override;

	/**************************************************************************************************//**
	* @copydoc IMsvSQLite::RegisterCallback(std::shared_ptr<IMsvSQLiteCallback> spCallback)
	******************************************************************************************************/
	virtual MsvErrorCode RegisterCallback(std::shared_ptr<IMsvSQLiteCallback> spCallback) override;

	/**************************************************************************************************//**
	* @copydoc IMsvSQLite::UnregisterCallback(std::shared_ptr<IMsvSQLiteCallback> spCallback)
	******************************************************************************************************/
	virtual MsvErrorCode UnregisterCallback(std::shared_ptr<IMsvSQLiteCallback> spCallback) override;

	/*-----------------------------------------------------------------------------------------------------
	**											MsvSQLite protected methods
	**---------------------------------------------------------------------------------------------------*/
protected:
	/**************************************************************************************************//**
	* @brief			Data has been changed.
	* @details		This method is called when data has been changed (callback must be registered before).
	* @param[in]	operationType		Operation type (update, insert, delete, etc.).
	* @param[in]	databaseName		Database name (in which has been data changed).
	* @param[in]	tableName			Table name (in which has been data changed).
	* @param[in]	rowId					ID of change value (column).
	******************************************************************************************************/
	void OnDataChanged(int operationType, const char *databaseName, const char *tableName, sqlite3_int64 rowId);

	/**************************************************************************************************//**
	* @brief			Execute callback.
	* @details		Called by SQLite for eachr result row when execute is called. It prepares result in known format.
	* @param[in]	data			Pointer to result (@ref MsvSQLiteResult).
	* @param[in]	argc			Column count.
	* @param[in]	argv			Column data.
	* @param[in]	azColName	Column name.
	******************************************************************************************************/
	static int ExecuteCallback(void *data, int argc, char **argv, char **azColName);

	/**************************************************************************************************//**
	* @brief			Data has been changed.
	* @details		This static method is called when data has been changed (callback must be registered before).
	*					It calls non static method @ref OnDataChanged(int operationType, const char *databaseName, const char *tableName, sqlite3_int64 rowId).
	* @param[in]	context				Pointer to contex (this class).
	* @param[in]	operationType		Operation type (update, insert, delete, etc.).
	* @param[in]	databaseName		Database name (in which has been data changed).
	* @param[in]	tableName			Table name (in which has been data changed).
	* @param[in]	rowId					ID of change value (column).
	******************************************************************************************************/
	static void OnChangeCallback(void *context, int operationType, const char *databaseName, const char *tableName, sqlite3_int64 rowId);

protected:
	/**************************************************************************************************//**
	* @brief		SQLite mutex.
	* @details	Locks this object for thread safety access.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

	/**************************************************************************************************//**
	* @brief		Registered callbacks.
	* @details	Contains all registered callbacks which will be notified about data changes.
	* @see		RegisterCallback
	* @see		UnregisterCallback
	******************************************************************************************************/
	std::forward_list<std::shared_ptr<IMsvSQLiteCallback>> m_callbacks;

	/**************************************************************************************************//**
	* @brief		Initialize flag.
	* @details	Flag if config is initialized (true) or not (false).
	* @see		Initialize
	* @see		Uninitialize
	* @see		Initialized
	******************************************************************************************************/
	bool m_initialized;

	/**************************************************************************************************//**
	* @brief		SQLite connection.
	* @details	Pointer to real implementation of SQLite.
	******************************************************************************************************/
	sqlite3* m_pConnection;

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;
};


#endif // !MARSTECH_SQLITE_H

/** @} */	//End of group MSQLITEWRAPPER.
