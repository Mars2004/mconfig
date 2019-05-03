/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Active Config Storage Implementation
* @details		Contains implementation of @ref MsvActiveConfigStorage.
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


#include "MsvActiveConfigStorage.h"
#include "MsvActiveConfigStorage_Factory.h"

#include "merror/MsvErrorCodes.h"

MSV_DISABLE_ALL_WARNINGS

#include <vector>
#include <sstream>

MSV_ENABLE_WARNINGS


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvActiveConfigStorage::MsvActiveConfigStorage(std::shared_ptr<MsvActiveConfigStorage_Factory> spFactory, std::shared_ptr<MsvLogger> spLogger):
	m_initialized(false),
	m_spFactory(spFactory ? spFactory : MsvActiveConfigStorage_Factory::Get()),
	m_spLogger(spLogger)
{

}

MsvActiveConfigStorage::MsvActiveConfigStorage(std::shared_ptr<MsvLogger> spLogger):
	MsvActiveConfigStorage(nullptr, spLogger)
{

}

MsvActiveConfigStorage::~MsvActiveConfigStorage()
{
	Uninitialize();
}


/********************************************************************************************************************************
*															IMsvActiveConfigStorage public methods
********************************************************************************************************************************/


MsvErrorCode MsvActiveConfigStorage::Initialize(const std::shared_ptr<IMsvConfigKeyMap<IMsvDefaultValue>> spConfigKeyMap, const char* configPath, const char* groupName)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Initializing active configuration storage (configPath: \"{}\", groupName: \"{}\").", configPath, groupName);

	if (Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "Active configuration storage has been already initialized.");
		return MSV_ALREADY_INITIALIZED_INFO;
	}

	m_spConfigKeyMap = spConfigKeyMap;
	m_tableName = groupName;

	std::shared_ptr<IMsvSQLite> spSQLite = m_spFactory->GetIMsvSQLite(m_spLogger);
	if (!spSQLite)
	{
		//allocation failed
		MSV_LOG_ERROR(m_spLogger, "Get SQLite failed with error: {0:x}", MSV_ALLOCATION_ERROR);
		return MSV_ALLOCATION_ERROR;
	}

	MsvErrorCode errorCode = spSQLite->Initialize(configPath);
	if (MSV_FAILED(errorCode))
	{
		MSV_LOG_ERROR(m_spLogger, "Initialize SQLite failed with error: {0:x}", errorCode);
		return errorCode;
	}

	m_spSQLite = spSQLite;

	//initialize database if not exists
	if (MSV_FAILED(errorCode = spSQLite->CreateTableIfNotExists(m_tableName.c_str(),	"Id INTEGER PRIMARY KEY, Value")))
	{
		MSV_LOG_ERROR(m_spLogger, "Create active configuration table {} failed with error: {0:x}", m_tableName, errorCode);
		return errorCode;
	}

	if (errorCode != MSV_ALREADY_EXISTS_INFO)
	{
		//database has been created and it is empty -> set default values
		MSV_LOG_INFO(m_spLogger, "Active configuration table {} has been created - inserting default configuration values.", m_tableName);

		//set to true because of Initialized() check in StoreValue (we need to set default values to database)
		//it is OK, because this method is locked, so any Initialized() check will wait for the end of this method
		m_initialized = true;

		std::map<int32_t, std::shared_ptr<IMsvDefaultValue>>::const_iterator endIt = m_spConfigKeyMap->GetMap().end();
		for (std::map<int32_t, std::shared_ptr<IMsvDefaultValue>>::const_iterator it = m_spConfigKeyMap->GetMap().begin(); it != endIt; ++it)
		{
			if (MSV_FAILED(errorCode = StoreDefaultValue(it->first)))
			{
				//store default value failed
				MSV_LOG_ERROR(m_spLogger, "Store default value {} failed with error: {0:x}", it->first, errorCode);
				m_initialized = false;
				return errorCode;
			}
		}
	}

	m_initialized = true;

	MSV_LOG_INFO(m_spLogger, "Active configuration storage has been successfully initialized.");

	return errorCode;
}

MsvErrorCode MsvActiveConfigStorage::Uninitialize()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Uninitializing active configuration storage.");

	if (!Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "Active configuration storage has not been initialized.");
		return MSV_NOT_INITIALIZED_INFO;
	}
	
	MsvErrorCode errorCode = m_spSQLite->Uninitialize();
	if (MSV_FAILED(errorCode))
	{
		MSV_LOG_ERROR(m_spLogger, "Uninitialize SQLite failed with error: {0:x}", errorCode);
		return errorCode;
	}

	m_spSQLite.reset();

	m_initialized = false;

	MSV_LOG_INFO(m_spLogger, "Active configuration storage has been successfully uninitialized.");

	return errorCode;
}

bool MsvActiveConfigStorage::Initialized() const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	return m_initialized;
}

MsvErrorCode MsvActiveConfigStorage::GetValue(int32_t cfgId, bool& value) const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	std::string valueStr;
	MSV_RETURN_FAILED(GetValue(cfgId, valueStr));

	//SQLite does not have BOOLEAN, it stores false as 0 and true as 1
	value = (valueStr.compare("0") == 0) ? false : true;

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::GetValue(int32_t cfgId, double& value) const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	std::string valueStr;
	MSV_RETURN_FAILED(GetValue(cfgId, valueStr));

	//SQLite does not have DOUBLE, it stores data as string
	value = strtod(valueStr.c_str(), nullptr);

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::GetValue(int32_t cfgId, int64_t& value) const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	std::string valueStr;
	MSV_RETURN_FAILED(GetValue(cfgId, valueStr));

	//SQLite does not have INTEGER, it stores data as string
	value = atoll(valueStr.c_str());

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::GetValue(int32_t cfgId, std::string& value) const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	if (!Initialized())
	{
		MSV_LOG_ERROR(m_spLogger, "Active configuration storage is not initialized - error:", MSV_NOT_INITIALIZED_ERROR);
		return MSV_NOT_INITIALIZED_ERROR;
	}

	std::stringstream sqlQuery;
	sqlQuery << "SELECT Value FROM " << m_tableName << " WHERE Id = " << cfgId << ";";

	MsvSQLiteResult sqlResult;
	MsvErrorCode errorCode = m_spSQLite->Execute(sqlQuery.str().c_str(), sqlResult);

	//get value failed
	if (MSV_FAILED(errorCode))
	{
		MSV_LOG_ERROR(m_spLogger, "Get active configuration value {} from SQLite failed with error:", cfgId, errorCode);
		return errorCode;
	}
	
	//value does not exists -> store it to database and get (database might be created by older program version
	//we should check if value exists in default values and try to insert it to database)
	//it is compability for program updates
	if (sqlResult.empty() || sqlResult[0].empty())
	{
		MsvErrorCode tempErrorCode = MSV_SUCCESS;
		if (MSV_FAILED(errorCode = const_cast<MsvActiveConfigStorage*>(this)->StoreDefaultValue(cfgId)))
		{
			//store default value failed (returning original error code)
			MSV_LOG_ERROR(m_spLogger, "Store default value {} failed with error:", cfgId, errorCode);
		}

		//try to get stored default value
		if (MSV_FAILED(errorCode = m_spSQLite->Execute(sqlQuery.str().c_str(), sqlResult)))
		{
			//get stored default value failed (returning original error code)
			MSV_LOG_ERROR(m_spLogger, "Get stored default value {} from SQLite failed with error:", cfgId, errorCode);
		}
	}

	if (sqlResult.empty() || sqlResult[0].empty())
	{
		MSV_LOG_ERROR(m_spLogger, "Get active configuration value {} from SQLite failed with error:", cfgId, MSV_NOT_FOUND_ERROR);
		return MSV_NOT_FOUND_ERROR;
	}

	value = sqlResult[0][0];

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::GetValue(int32_t cfgId, uint64_t& value) const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	std::string valueStr;
	MSV_RETURN_FAILED(GetValue(cfgId, valueStr));

	//SQLite does not have UNSIGNED INTEGER, it stores data as string
	value = strtoull(valueStr.c_str(), nullptr, 10);

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::StoreValue(int32_t cfgId, bool value)
{
	MSV_RETURN_FAILED(StoreValue<std::string>(cfgId, value ? "1" : "0"));

	OnChange<bool>(cfgId, value);

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::StoreValue(int32_t cfgId, double value)
{
	MSV_RETURN_FAILED(StoreValue<double>(cfgId, value));

	OnChange<double>(cfgId, value);

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::StoreValue(int32_t cfgId, int64_t value)
{
	MSV_RETURN_FAILED(StoreValue<int64_t>(cfgId, value));

	OnChange<int64_t>(cfgId, value);

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::StoreValue(int32_t cfgId, const std::string& value)
{
	MSV_RETURN_FAILED(StoreValue<std::string>(cfgId, "'" + value + "'"));

	//OnChange<char>(cfgId, value.c_str());
	std::forward_list<std::shared_ptr<IMsvActiveConfigStorageCallback>>::iterator endCallbackIt = m_callbacks.end();
	for (std::forward_list<std::shared_ptr<IMsvActiveConfigStorageCallback>>::iterator callbackIt = m_callbacks.begin(); callbackIt != endCallbackIt; ++callbackIt)
	{
		//config ID is everywhere defined as int32_t -> we can static_cast without worries
		(*callbackIt)->OnValueChanged(cfgId, value.c_str());
	}

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::StoreValue(int32_t cfgId, uint64_t value)
{
	MSV_RETURN_FAILED(StoreValue<uint64_t>(cfgId, value));

	OnChange<uint64_t>(cfgId, value);

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::RegisterCallback(std::shared_ptr<IMsvActiveConfigStorageCallback> spCallback)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	//there is no check if is initilized or not (it is possible to register callback before initialized - weard, but OK)

	MSV_LOG_INFO(m_spLogger, "Registering Config Storage callback.");

	std::forward_list<std::shared_ptr<IMsvActiveConfigStorageCallback>>::iterator endIt = m_callbacks.end();
	for (std::forward_list<std::shared_ptr<IMsvActiveConfigStorageCallback>>::iterator it = m_callbacks.begin(); it != endIt; ++it)
	{
		if (*it == spCallback)
		{
			//callback is already registered
			MSV_LOG_INFO(m_spLogger, "Config Storage callback has been already registered.");
			return MSV_ALREADY_REGISTERED_INFO;
		}
	}

	//register callback
	m_callbacks.push_front(spCallback);

	MSV_LOG_INFO(m_spLogger, "Config Storage callback has been successfully registered.");

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfigStorage::UnregisterCallback(std::shared_ptr<IMsvActiveConfigStorageCallback> spCallback)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Unregistering Config Storage callback.");

	m_callbacks.remove(spCallback);

	MSV_LOG_INFO(m_spLogger, "Config Storage callback has been successfully unregistered.");

	return MSV_SUCCESS;
}


/********************************************************************************************************************************
*															MsvActiveConfigStorage protected methods
********************************************************************************************************************************/


template<class T> void MsvActiveConfigStorage::OnChange(int32_t cfgId, const T& newValue) const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	//MSV_LOG_DEBUG(m_spLogger, "Config data changed (cfgId: {}, value: \"{}\").", cfgId, value);

	std::forward_list<std::shared_ptr<IMsvActiveConfigStorageCallback>>::const_iterator endCallbackIt = m_callbacks.end();
	for (std::forward_list<std::shared_ptr<IMsvActiveConfigStorageCallback>>::const_iterator callbackIt = m_callbacks.begin(); callbackIt != endCallbackIt; ++callbackIt)
	{
		//config ID is everywhere defined as int32_t -> we can static_cast without worries
		(*callbackIt)->OnValueChanged(cfgId, newValue);
	}
}

MsvErrorCode MsvActiveConfigStorage::StoreDefaultValue(int32_t cfgId)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MsvErrorCode errorCode = MSV_SUCCESS;

	std::map<int32_t, std::shared_ptr<IMsvDefaultValue>>::const_iterator it = m_spConfigKeyMap->GetMap().find(cfgId);
	if (it != m_spConfigKeyMap->GetMap().end())
	{
		//store value
		if (it->second->IsBool())
		{
			bool defaultValue = false;
			if (MSV_FAILED(errorCode = it->second->GetDefaultValue(defaultValue)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get bool default value {} failed with error: {0:x}", it->first, errorCode);
				return errorCode;
			}

			MSV_RETURN_FAILED(StoreValue(it->first, defaultValue));
		}
		else if (it->second->IsDouble())
		{
			double defaultValue = 0.0;
			if (MSV_FAILED(errorCode = it->second->GetDefaultValue(defaultValue)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get double default value {} failed with error: {0:x}", it->first, errorCode);
				return errorCode;
			}

			MSV_RETURN_FAILED(StoreValue(it->first, defaultValue));
		}
		else if (it->second->IsInteger())
		{
			int64_t defaultValue = 0;
			if (MSV_FAILED(errorCode = it->second->GetDefaultValue(defaultValue)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get int64_t default value {} failed with error: {0:x}", it->first, errorCode);
				return errorCode;
			}

			MSV_RETURN_FAILED(StoreValue(it->first, defaultValue));
		}
		else if (it->second->IsString())
		{
			std::string defaultValue;
			if (MSV_FAILED(errorCode = it->second->GetDefaultValue(defaultValue)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get string default value {} failed with error: {0:x}", it->first, errorCode);
				return errorCode;
			}

			MSV_RETURN_FAILED(StoreValue(it->first, defaultValue));
		}
		else if (it->second->IsUnsigned())
		{
			uint64_t defaultValue = 0;
			if (MSV_FAILED(errorCode = it->second->GetDefaultValue(defaultValue)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get uint64_t default value {} failed with error: {0:x}", it->first, errorCode);
				return errorCode;
			}

			MSV_RETURN_FAILED(StoreValue(it->first, defaultValue));
		}
		else
		{
			MSV_LOG_ERROR(m_spLogger, "Unknown type of configuration value - error: {0:x}", MSV_INVALID_DATA_ERROR);
			return MSV_INVALID_DATA_ERROR;
		}
	}
	else
	{
		MSV_LOG_ERROR(m_spLogger, "Active configuration value {} has not been found - error:", cfgId, MSV_NOT_FOUND_ERROR);
		return MSV_NOT_FOUND_ERROR;
	}

	return MSV_SUCCESS;
}

template<class T> MsvErrorCode MsvActiveConfigStorage::StoreValue(int32_t cfgId, const T& value)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	if (!Initialized())
	{
		MSV_LOG_ERROR(m_spLogger, "Active configuration storage is not initialized - error:", MSV_NOT_INITIALIZED_ERROR);
		return MSV_NOT_INITIALIZED_ERROR;
	}

	//insert or replace (one query for inserting and updating)
	std::stringstream sqlQuery;
	sqlQuery << "INSERT OR REPLACE INTO " << m_tableName << "(Id, Value) VALUES(" << cfgId << ", " << value << ");";

	MsvSQLiteResult sqlResult;
	MsvErrorCode errorCode = m_spSQLite->Execute(sqlQuery.str().c_str(), sqlResult);
	if (MSV_FAILED(errorCode))
	{
		MSV_LOG_ERROR(m_spLogger, "Store configuration value {} to SQLite failed with error:", cfgId, errorCode);
		return errorCode;
	}

	return MSV_SUCCESS;
}


/** @} */	//End of group MCONFIG.
