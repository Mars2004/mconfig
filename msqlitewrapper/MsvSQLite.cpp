/**************************************************************************************************//**
* @addtogroup	MSQLITEWRAPPER
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech SQLite Wrapper Implementation
* @details		Contains implementation of @ref MsvSQLite.
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


#include "MsvSQLite.h"
#include "merror/MsvErrorCodes.h"

MSV_DISABLE_ALL_WARNINGS

#include <sstream>

MSV_ENABLE_WARNINGS


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/

MsvSQLite::MsvSQLite(std::shared_ptr<MsvLogger> spLogger):
	m_initialized(false),
	m_pConnection(nullptr),
	m_spLogger(spLogger)
{

}

MsvSQLite::~MsvSQLite()
{
	Uninitialize();
}


/********************************************************************************************************************************
*															IMsvSQLite public methods
********************************************************************************************************************************/


MsvErrorCode MsvSQLite::Initialize(const char* dbPath)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Initializing SQLite database \"{}\".", dbPath);

	if (Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "SQLite database has been already initialized.");
		return MSV_ALREADY_INITIALIZED_INFO;
	}

	if (sqlite3_open(dbPath, &m_pConnection))
	{
		//connection is not valid -> log and return
		MSV_LOG_ERROR(m_spLogger, "Open SQLite database \"{}\" failed with error: {0:x}", dbPath, sqlite3_errmsg(m_pConnection));
		return MSV_OPEN_ERROR;
	}

	m_initialized = true;

	MSV_LOG_INFO(m_spLogger, "SQLite database \"{}\" has been successfully initialized.", dbPath);

	return MSV_SUCCESS;
}

MsvErrorCode MsvSQLite::Uninitialize()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Uninitializing SQLite database.");

	if (!Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "SQLite database has not been initialized.");
		return MSV_NOT_INITIALIZED_INFO;
	}

	int result = sqlite3_close(m_pConnection);
	if (result != SQLITE_OK)
	{
		MSV_LOG_ERROR(m_spLogger, "Close SQLite database failed with error: {0:x}", result);

		if (result == SQLITE_BUSY)
		{
			return MSV_BUSY_ERROR;
		}

		return MSV_CLOSE_ERROR;
	}

	m_initialized = false;
	m_pConnection = nullptr;

	MSV_LOG_INFO(m_spLogger, "SQLite database has been successfully uninitialized.");

	return MSV_SUCCESS;
}

bool MsvSQLite::Initialized() const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	return m_initialized;
}

MsvErrorCode MsvSQLite::CreateTableIfNotExists(const char* tableName, const char* tableDef, const char* postCreateDefs)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Creating table \"{}\" (if not exists).", tableName);

	if (!Initialized())
	{
		MSV_LOG_ERROR(m_spLogger, "Trying to create table \"{}\" from uninitialized SQLite.", tableName);
		return MSV_NOT_INITIALIZED_ERROR;
	}

	std::stringstream sqlQuery;
	sqlQuery << "SELECT name FROM sqlite_master WHERE type='table' AND name = '" << tableName << "';";

	MsvSQLiteResult sqlResult;
	MSV_RETURN_FAILED(Execute(sqlQuery.str().c_str(), sqlResult));

	if (sqlResult.size() == 1)
	{
		//table exist -> return info
		MSV_LOG_INFO(m_spLogger, "Table \"{}\" already exists.", tableName);
		return MSV_ALREADY_EXISTS_INFO;
	}

	//table does not exists -> create it
	sqlResult.clear();
	sqlQuery.str("");

	sqlQuery << "CREATE TABLE IF NOT EXISTS " << tableName << "(" << tableDef << ");" << postCreateDefs;
	MSV_RETURN_FAILED(Execute(sqlQuery.str().c_str(), sqlResult));

	MSV_LOG_INFO(m_spLogger, "Table \"{}\" has been successfully created.", tableName);
	
	return MSV_SUCCESS;
}

MsvErrorCode MsvSQLite::Execute(const char* query, MsvSQLiteResult& result)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	if (!Initialized())
	{
		MSV_LOG_ERROR(m_spLogger, "Trying to execute query from uninitialized SQLite.");
		return MSV_NOT_INITIALIZED_ERROR;
	}

	//SqliteResult sqlResult;
	char *pErrMsg = 0;
	if (sqlite3_exec(m_pConnection, query, ExecuteCallback, static_cast<void*>(&result), &pErrMsg))
	{
		//connection is not valid -> log and return (do not log query, it might contain sensitive data)
		MSV_LOG_ERROR(m_spLogger, "Execute query failed with error: {}", pErrMsg);
		sqlite3_free(pErrMsg);

		return MSV_EXECUTE_ERROR;
	}

	return MSV_SUCCESS;
}

MsvErrorCode MsvSQLite::RegisterCallback(std::shared_ptr<IMsvSQLiteCallback> spCallback)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	//there is no check if is initilized or not (it is possible to register callback before initialized - weard, but OK)

	MSV_LOG_INFO(m_spLogger, "Registering SQLite callback.");

	if (m_callbacks.empty())
	{
		//it is the first callback -> register to SQLite
		sqlite3_update_hook(m_pConnection, &OnChangeCallback, this);
	}

	std::forward_list<std::shared_ptr<IMsvSQLiteCallback>>::iterator endIt = m_callbacks.end();
	for (std::forward_list<std::shared_ptr<IMsvSQLiteCallback>>::iterator it = m_callbacks.begin(); it != endIt; ++it)
	{
		if (*it == spCallback)
		{
			//callback is already registered
			MSV_LOG_INFO(m_spLogger, "SQLite callback has been already registered.");
			return MSV_ALREADY_REGISTERED_INFO;
		}
	}

	//register callback
	m_callbacks.push_front(spCallback);

	MSV_LOG_INFO(m_spLogger, "SQLite callback has been successfully registered.");

	return MSV_SUCCESS;
}

MsvErrorCode MsvSQLite::UnregisterCallback(std::shared_ptr<IMsvSQLiteCallback> spCallback)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Unregistering SQLite callback.");

	m_callbacks.remove(spCallback);

	if (m_callbacks.empty())
	{
		//it was the last callback -> unregister from SQLite
		sqlite3_update_hook(m_pConnection, nullptr, nullptr);
	}

	MSV_LOG_INFO(m_spLogger, "SQLite callback has been successfully unregistered.");

	return MSV_SUCCESS;
}


/********************************************************************************************************************************
*															MsvSQLite protected methods
********************************************************************************************************************************/


void MsvSQLite::OnDataChanged(int operationType, const char *databaseName, const char *tableName, sqlite3_int64 rowId)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_DEBUG(m_spLogger, "SQLite data has changed (operationType: {}, dbName: \"{}\", tableName: \"{}\", rowid: {}).", operationType, databaseName, tableName, rowId);

	std::forward_list<std::shared_ptr<IMsvSQLiteCallback>>::iterator endIt = m_callbacks.end();
	for (std::forward_list<std::shared_ptr<IMsvSQLiteCallback>>::iterator it = m_callbacks.begin(); it != endIt; ++it)
	{
		(*it)->OnChange(operationType, databaseName, tableName, rowId);
	}
}


/********************************************************************************************************************************
*															MsvSQLite protected static methods
********************************************************************************************************************************/


int MsvSQLite::ExecuteCallback(void *data, int argc, char **argv, char **azColName)
{
	(void) azColName;
	MsvSQLiteResult* sqlResult = static_cast<MsvSQLiteResult*>(data);

	//create row
	MsvSQLiteRow row;
	for (int i = 0; i < argc; i++)
	{
		row.push_back(argv[i]);
	}

	//insert row to result
	sqlResult->push_back(row);

	return 0;
}

void MsvSQLite::OnChangeCallback(void *context, int operationType, const char *databaseName, const char *tableName, sqlite3_int64 rowId)
{
	static_cast<MsvSQLite*>(context)->OnDataChanged(operationType, databaseName, tableName, rowId);
}


/** @} */	//End of group MSQLITEWRAPPER.
