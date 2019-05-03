/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Active Config Implementation
* @details		Contains implementation of @ref MsvActiveConfig.
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


#include "MsvActiveConfig.h"
#include "MsvActiveConfig_Factory.h"

#include "merror/MsvErrorCodes.h"

MSV_DISABLE_ALL_WARNINGS

#include <vector>
#include <sstream>

MSV_ENABLE_WARNINGS


/********************************************************************************************************************************
*															MsvSQLiteCallback implementation
********************************************************************************************************************************/


MsvActiveConfig::MsvActiveConfigStorageCallback::MsvActiveConfigStorageCallback(MsvActiveConfig* pConfig):
	m_pConfig(pConfig)
{

}

void MsvActiveConfig::MsvActiveConfigStorageCallback::OnValueChanged(int32_t cfgId, bool newValue)
{
	if (m_pConfig)
	{
		m_pConfig->OnValueChanged<bool>(cfgId, newValue);
	}
}

void MsvActiveConfig::MsvActiveConfigStorageCallback::OnValueChanged(int32_t cfgId, double newValue)
{
	if (m_pConfig)
	{
		m_pConfig->OnValueChanged<double>(cfgId, newValue);
	}
}

void MsvActiveConfig::MsvActiveConfigStorageCallback::OnValueChanged(int32_t cfgId, int64_t newValue)
{
	if (m_pConfig)
	{
		m_pConfig->OnValueChanged<int64_t>(cfgId, newValue);
	}
}

void MsvActiveConfig::MsvActiveConfigStorageCallback::OnValueChanged(int32_t cfgId, const char* newValue)
{
	if (m_pConfig)
	{
		m_pConfig->OnValueChanged<const char*>(cfgId, newValue);
	}
}

void MsvActiveConfig::MsvActiveConfigStorageCallback::OnValueChanged(int32_t cfgId, uint64_t newValue)
{
	if (m_pConfig)
	{
		m_pConfig->OnValueChanged<uint64_t>(cfgId, newValue);
	}
}


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/

MsvActiveConfig::MsvActiveConfig(std::shared_ptr<MsvActiveConfig_Factory> spFactory, std::shared_ptr<MsvLogger> spLogger):
	m_initialized(false),
	m_spFactory(spFactory ? spFactory : MsvActiveConfig_Factory::Get()),
	m_spLogger(spLogger)
{

}

MsvActiveConfig::MsvActiveConfig(std::shared_ptr<MsvLogger> spLogger):
	MsvActiveConfig(nullptr, spLogger)
{

}

MsvActiveConfig::~MsvActiveConfig()
{
	Uninitialize();
}


/********************************************************************************************************************************
*															IMsvActiveConfig public methods
********************************************************************************************************************************/


MsvErrorCode MsvActiveConfig::Initialize(std::shared_ptr<IMsvConfigKeyMap<IMsvDefaultValue>> spConfigKeyMap, const char* configPath, const char* groupName)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Initializing active configuration (configPath: \"{}\", groupName: \"{}\").", configPath, groupName);

	if (Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "Active configuration has been already initialized.");
		return MSV_ALREADY_INITIALIZED_INFO;
	}

	std::shared_ptr<IMsvActiveConfigStorage> spStorage = m_spFactory->GetIMsvActiveConfigStorage(m_spLogger);
	if (!spStorage)
	{
		//allocation failed
		MSV_LOG_ERROR(m_spLogger, "Get active configuration storage failed with error: {0:x}", MSV_ALLOCATION_ERROR);
		return MSV_ALLOCATION_ERROR;
	}

	MsvErrorCode errorCode = spStorage->Initialize(spConfigKeyMap, configPath, groupName);
	if (MSV_FAILED(errorCode))
	{
		//initialize config storage failed
		MSV_LOG_ERROR(m_spLogger, "Initialize active configuration storage failed with error: {0:x}", errorCode);
		return errorCode;
	}

	std::map<int32_t, std::shared_ptr<IMsvDefaultValue>>::const_iterator endIt = spConfigKeyMap->GetMap().end();
	for (std::map<int32_t, std::shared_ptr<IMsvDefaultValue>>::const_iterator it = spConfigKeyMap->GetMap().begin(); it != endIt; ++it)
	{
		int32_t cfgId = it->first;

		if (it->second->IsBool())
		{
			bool value = false;
			if (MSV_FAILED(errorCode = spStorage->GetValue(it->first, value)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get bool value {} from configuration storage failed with error: {0:x}", it->first, errorCode);
				break;
			}

			m_boolValues[cfgId] = value;
		}
		else if (it->second->IsDouble())
		{
			double value = 0.0;
			if (MSV_FAILED(errorCode = spStorage->GetValue(it->first, value)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get double value {} from configuration storage failed with error: {0:x}", it->first, errorCode);
				break;
			}

			m_doubleValues[cfgId] = value;
		}
		else if (it->second->IsInteger())
		{
			int64_t value = 0;
			if (MSV_FAILED(errorCode = spStorage->GetValue(it->first, value)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get int64_t value {} from configuration storage failed with error: {0:x}", it->first, errorCode);
				break;
			}

			m_integerValues[cfgId] = value;
		}
		else if (it->second->IsString())
		{
			std::string value;
			if (MSV_FAILED(errorCode = spStorage->GetValue(it->first, value)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get string value {} from configuration storage failed with error: {0:x}", it->first, errorCode);
				break;
			}

			m_stringValues[cfgId] = value;
		}
		else if (it->second->IsUnsigned())
		{
			uint64_t value;
			if (MSV_FAILED(errorCode = spStorage->GetValue(it->first, value)))
			{
				MSV_LOG_ERROR(m_spLogger, "Get uint64_t value {} from configuration storage failed with error: {0:x}", it->first, errorCode);
				break;
			}

			m_unsignedValues[cfgId] = value;
		}
		else
		{
			errorCode = MSV_INVALID_DATA_ERROR;
			MSV_LOG_ERROR(m_spLogger, "Unknown type of configuration value - error: {0:x}", errorCode);
			break;
		}
	}

	//create storage callback before (it is shared pointer and it would be released when it was defined in if)
	std::shared_ptr<IMsvActiveConfigStorageCallback> spStorageCallback(new (std::nothrow) MsvActiveConfigStorageCallback(this));
	if (MSV_SUCCEEDED(errorCode))
	{
		//read configuration values from configuration storage succeeded -> register callback if it was successfully created
		if (!spStorageCallback)
		{
			MSV_LOG_ERROR(m_spLogger, "Create active config storage callback failed with error: {0:x}", MSV_ALLOCATION_ERROR);
			errorCode = MSV_ALLOCATION_ERROR;
		}

		if (MSV_FAILED(errorCode = spStorage->RegisterCallback(spStorageCallback)))
		{
			//just log (next error code check will handle errors)
			MSV_LOG_ERROR(m_spLogger, "Register active configuration storage callback failed with error: {0:x}", errorCode);
		}
	}

	if (MSV_FAILED(errorCode))
	{
		//read configuration values from configuration storage failed or register callback failed -> clear all values
		m_boolValues.clear();
		m_doubleValues.clear();
		m_integerValues.clear();
		m_stringValues.clear();
		m_unsignedValues.clear();

		return errorCode;
	}
	
	//set member values (storage, storage callback and initialize flag)
	m_spStorage = spStorage;
	m_spStorageCallback = spStorageCallback;
	m_initialized = true;

	MSV_LOG_INFO(m_spLogger, "Active configuration has been successfully initialized.");

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfig::Uninitialize()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Uninitializing active configuration.");

	if (!Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "Active configuration has not been initialized.");
		return MSV_NOT_INITIALIZED_INFO;
	}

	MsvErrorCode errorCode = m_spStorage->UnregisterCallback(m_spStorageCallback);
	if (MSV_FAILED(errorCode))
	{
		//just log error
		MSV_LOG_ERROR(m_spLogger, "Unregister active configuration storage callback failed with error: {0:x}", errorCode);
	}

	m_spStorageCallback.reset();

	if (MSV_FAILED(errorCode = m_spStorage->Uninitialize()))
	{
		MSV_LOG_ERROR(m_spLogger, "Uninitialize active configuration storage failed with error: {0:x}", errorCode);
		return errorCode;
	}

	m_spStorage.reset();

	m_boolValues.clear();
	m_doubleValues.clear();
	m_integerValues.clear();
	m_stringValues.clear();
	m_unsignedValues.clear();

	m_initialized = false;

	MSV_LOG_INFO(m_spLogger, "Active configuration has been successfully uninitialized.");

	return errorCode;
}

bool MsvActiveConfig::Initialized() const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	return m_initialized;
}

MsvErrorCode MsvActiveConfig::GetValue(int32_t cfgId, bool& value) const
{
	return GetValue<bool>(cfgId, m_boolValues, value);
}

MsvErrorCode MsvActiveConfig::GetValue(int32_t cfgId, double& value) const
{
	return GetValue<double>(cfgId, m_doubleValues, value);
}

MsvErrorCode MsvActiveConfig::GetValue(int32_t cfgId, int64_t& value) const
{
	return GetValue<int64_t>(cfgId, m_integerValues, value);
}

MsvErrorCode MsvActiveConfig::GetValue(int32_t cfgId, std::string& value) const
{
	return GetValue<std::string>(cfgId, m_stringValues, value);
}

MsvErrorCode MsvActiveConfig::GetValue(int32_t cfgId, uint64_t& value) const
{
	return GetValue<uint64_t>(cfgId, m_unsignedValues, value);
}

MsvErrorCode MsvActiveConfig::SetValue(int32_t cfgId, bool value)
{
	return SetValue<bool>(cfgId, m_boolValues, value);
}

MsvErrorCode MsvActiveConfig::SetValue(int32_t cfgId, double value)
{
	return SetValue<double>(cfgId, m_doubleValues, value);
}

MsvErrorCode MsvActiveConfig::SetValue(int32_t cfgId, int64_t value)
{
	return SetValue<int64_t>(cfgId, m_integerValues, value);
}

MsvErrorCode MsvActiveConfig::SetValue(int32_t cfgId, const std::string& value)
{
	return SetValue<std::string>(cfgId, m_stringValues, value);
}

MsvErrorCode MsvActiveConfig::SetValue(int32_t cfgId, uint64_t value)
{
	return SetValue<uint64_t>(cfgId, m_unsignedValues, value);
}

MsvErrorCode MsvActiveConfig::RegisterCallback(std::shared_ptr<IMsvActiveConfigCallback> spCallback)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	//there is no check if is initilized or not (it is possible to register callback before initialized - weard, but OK)

	MSV_LOG_INFO(m_spLogger, "Registering Config callback.");

	std::forward_list<std::shared_ptr<IMsvActiveConfigCallback>>::iterator endIt = m_callbacks.end();
	for (std::forward_list<std::shared_ptr<IMsvActiveConfigCallback>>::iterator it = m_callbacks.begin(); it != endIt; ++it)
	{
		if (*it == spCallback)
		{
			//callback is already registered
			MSV_LOG_INFO(m_spLogger, "Config callback has been already registered.");
			return MSV_ALREADY_REGISTERED_INFO;
		}
	}

	//register callback
	m_callbacks.push_front(spCallback);

	MSV_LOG_INFO(m_spLogger, "Config callback has been successfully registered.");

	return MSV_SUCCESS;
}

MsvErrorCode MsvActiveConfig::UnregisterCallback(std::shared_ptr<IMsvActiveConfigCallback> spCallback)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Unregistering Config callback.");

	m_callbacks.remove(spCallback);

	MSV_LOG_INFO(m_spLogger, "Config callback has been successfully unregistered.");

	return MSV_SUCCESS;
}


/********************************************************************************************************************************
*															MsvActiveConfig protected methods
********************************************************************************************************************************/


template<class T> void MsvActiveConfig::OnValueChanged(int32_t cfgId, T newValue)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_DEBUG(m_spLogger, "Config data changed (cfgId: {}, newValue: {}).", cfgId, newValue);
	  
	std::forward_list<std::shared_ptr<IMsvActiveConfigCallback>>::iterator endIt = m_callbacks.end();
	for (std::forward_list<std::shared_ptr<IMsvActiveConfigCallback>>::iterator it = m_callbacks.begin(); it != endIt; ++it)
	{
		//config ID is everywhere defined as int32_t -> we can static_cast without worries
		(*it)->OnValueChanged(static_cast<int32_t>(cfgId), newValue);
	}
}

template<class T> MsvErrorCode MsvActiveConfig::GetValue(int32_t cfgId, const std::map<int32_t, T>& values, T& value) const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	if (!Initialized())
	{
		MSV_LOG_ERROR(m_spLogger, "Active configuration is not initialized - error:", MSV_NOT_INITIALIZED_ERROR);
		return MSV_NOT_INITIALIZED_ERROR;
	}

	typename std::map<int32_t, T>::const_iterator it = values.find(cfgId);

	if (it != values.end())
	{
		value = it->second;
		return MSV_SUCCESS;
	}

	MSV_LOG_ERROR(m_spLogger, "Active configuration value {} has not been found - error:", cfgId, MSV_NOT_FOUND_ERROR);

	return MSV_NOT_FOUND_ERROR;
}

template<class T> MsvErrorCode MsvActiveConfig::SetValue(int32_t cfgId, std::map<int32_t, T>& values, const T& value)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	if (!Initialized())
	{
		MSV_LOG_ERROR(m_spLogger, "Active configuration is not initialized - error:", MSV_NOT_INITIALIZED_ERROR);
		return MSV_NOT_INITIALIZED_ERROR;
	}

	typename std::map<int32_t, T>::iterator it = values.find(cfgId);

	if (it != values.end())
	{
		//update database first
		MsvErrorCode errorCode = m_spStorage->StoreValue(cfgId, value);
		if (MSV_FAILED(errorCode))
		{
			MSV_LOG_ERROR(m_spLogger, "Store active configuration value {} to storage failed with error: {0:x}", cfgId, errorCode);
			return errorCode;
		}

		//set new value to cache
		it->second = value;

		return MSV_SUCCESS;
	}

	MSV_LOG_ERROR(m_spLogger, "Active configuration value {} has not been found - error:", cfgId, MSV_NOT_FOUND_ERROR);

	return MSV_NOT_FOUND_ERROR;
}


/** @} */	//End of group MCONFIG.
