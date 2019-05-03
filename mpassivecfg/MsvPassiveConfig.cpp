/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Passive Config Implementation
* @details		Contains implementation of @ref MsvPassiveConfig.
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


#include "MsvPassiveConfig.h"
#include "merror/MsvErrorCodes.h"

MSV_DISABLE_ALL_WARNINGS

#include "3rdParty/inih/INIReader.h"

MSV_ENABLE_WARNINGS


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvPassiveConfig::MsvPassiveConfig():
	MsvPassiveConfigBase()
{

}


MsvPassiveConfig::~MsvPassiveConfig()
{

}


/********************************************************************************************************************************
*															IMsvPassiveConfig public methods
********************************************************************************************************************************/


MsvErrorCode MsvPassiveConfig::ReloadConfiguration()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	//check if config has been already initialized (we need these two values for successfull reload)
	if (m_configPath.empty() || !m_spConfigKeyMap)
	{
		//config is not initialized -> return error
		return MSV_NOT_INITIALIZED_ERROR;
	}

	//clear all values (they might be already loaded)
	m_boolValues.clear();
	m_doubleValues.clear();
	m_integerValues.clear();
	m_stringValues.clear();
	m_unsignedValues.clear();

	//open INI file and parse it
	INIReader reader(m_configPath);
	if (reader.ParseError() != 0)
	{
		//parse INI file failed (or does not exist)
		m_lineNumberWithError = reader.ParseError();
		return MSV_PARSE_ERROR;
	}

	MsvErrorCode errorCode = MSV_SUCCESS;

	//get all values specified in m_spConfigKeyMap
	const std::map<int32_t, std::shared_ptr<IMsvConfigKey>>& configKeys = m_spConfigKeyMap->GetMap();
	std::map<int32_t, std::shared_ptr<IMsvConfigKey>>::const_iterator endIt = configKeys.end();
	for (std::map<int32_t, std::shared_ptr<IMsvConfigKey>>::const_iterator it = configKeys.begin(); it != endIt; ++it)
	{
		std::shared_ptr<IMsvConfigKey> spConfigKey = it->second;

		std::string group, key;
		if (spConfigKey->IsBool())
		{
			//get bool value
			bool defaultValue;
			if (MSV_FAILED(errorCode = spConfigKey->GetData(group, key, defaultValue)))
			{
				//failed -> set failed cfgId and break
				m_cfgIdWithError = it->first;
				break;
			}
			m_boolValues.insert(std::pair<int32_t, bool>(it->first, reader.GetBoolean(group, key, defaultValue)));
		}
		else if (spConfigKey->IsDouble())
		{
			//get double value
			double defaultValue;
			if (MSV_FAILED(errorCode = spConfigKey->GetData(group, key, defaultValue)))
			{
				//failed -> set failed cfgId and break
				m_cfgIdWithError = it->first;
				break;
			}
			m_doubleValues.insert(std::pair<int32_t, double>(it->first, reader.GetReal(group, key, defaultValue)));
		}
		else if (spConfigKey->IsInteger())
		{
			//get int64_t value
			int64_t defaultValue;
			if (MSV_FAILED(errorCode = spConfigKey->GetData(group, key, defaultValue)))
			{
				//failed -> set failed cfgId and break
				m_cfgIdWithError = it->first;
				break;
			}
			m_integerValues.insert(std::pair<int32_t, int64_t>(it->first, reader.GetInteger(group, key, static_cast<long>(defaultValue))));
		}
		else if (spConfigKey->IsString())
		{
			//get string value
			std::string defaultValue;
			if (MSV_FAILED(errorCode = spConfigKey->GetData(group, key, defaultValue)))
			{
				//failed -> set failed cfgId and break
				m_cfgIdWithError = it->first;
				break;
			}
			m_stringValues.insert(std::pair<int32_t, std::string>(it->first, reader.Get(group, key, defaultValue)));
		}
		else if (spConfigKey->IsUnsigned())
		{
			//get int64_t value
			uint64_t defaultValue;
			if (MSV_FAILED(errorCode = spConfigKey->GetData(group, key, defaultValue)))
			{
				//failed -> set failed cfgId and break
				m_cfgIdWithError = it->first;
				break;
			}
			//working as string (inih does not have support for unsigned types)
			uint64_t value = strtoull(reader.Get(group, key, std::to_string(defaultValue)).c_str(), nullptr, 10);
			m_unsignedValues.insert(std::pair<int32_t, int64_t>(it->first, value));
		}
		else
		{
			//unknown type -> set failed cfgId (with unknown type) and break
			errorCode = MSV_UNKNOWN_ERROR;
			m_cfgIdWithError = it->first;
			break;
		}
	}

	//check if all succeeded
	if (MSV_FAILED(errorCode))
	{
		//somethink failed -> clear all values (they might not be valid)
		m_boolValues.clear();
		m_doubleValues.clear();
		m_integerValues.clear();
		m_stringValues.clear();
		m_unsignedValues.clear();
	}

	return errorCode;
}

/** @} */	//End of group MCONFIG.
