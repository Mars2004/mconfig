/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Passive Config Base Implementation
* @details		Contains implementation of @ref MsvPassiveConfigBase.
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


#include "MsvPassiveConfigBase.h"
#include "merror/MsvErrorCodes.h"


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvPassiveConfigBase::MsvPassiveConfigBase():
	m_cfgIdWithError(INT32_MIN),
	m_lineNumberWithError(INT32_MIN)
{

}


MsvPassiveConfigBase::~MsvPassiveConfigBase()
{

}


/********************************************************************************************************************************
*															IMsvPassiveConfig public methods
********************************************************************************************************************************/


MsvErrorCode MsvPassiveConfigBase::GetValue(int32_t cfgId, bool& value) const
{
	return GetValue<bool>(cfgId, m_boolValues, value);
}

MsvErrorCode MsvPassiveConfigBase::GetValue(int32_t cfgId, double& value) const
{
	return GetValue<double>(cfgId, m_doubleValues, value);
}

MsvErrorCode MsvPassiveConfigBase::GetValue(int32_t cfgId, int64_t& value) const
{
	return GetValue<int64_t>(cfgId, m_integerValues, value);
}

MsvErrorCode MsvPassiveConfigBase::GetValue(int32_t cfgId, std::string& value) const
{
	return GetValue<std::string>(cfgId, m_stringValues, value);
}

MsvErrorCode MsvPassiveConfigBase::GetValue(int32_t cfgId, uint64_t& value) const
{
	return GetValue<uint64_t>(cfgId, m_unsignedValues, value);
}

MsvErrorCode MsvPassiveConfigBase::Initialize(std::shared_ptr<IMsvConfigKeyMap<IMsvConfigKey>> spConfigKeyMap, const char* configPath)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	//check if config has been already initialized
	if (!m_configPath.empty() || m_spConfigKeyMap)
	{
		//config has been already initialized -> return INFO
		return MSV_ALREADY_INITIALIZED_INFO;
	}

	//set required values for initialization
	m_configPath.assign(configPath);
	m_spConfigKeyMap = spConfigKeyMap;

	//load configuration from real storage implementation
	MsvErrorCode errorCode = ReloadConfiguration();
	if (MSV_FAILED(errorCode))
	{
		//load failed -> reset file name config key map
		m_configPath.clear();
		m_spConfigKeyMap.reset();
	}

	return errorCode;
}

void MsvPassiveConfigBase::ReadFailedData(int32_t& lineNumber, int32_t& cfgId) const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	lineNumber = m_lineNumberWithError;
	cfgId = m_cfgIdWithError;
}


/********************************************************************************************************************************
*															MsvPassiveConfigBase protected methods
********************************************************************************************************************************/


template<class T> MsvErrorCode MsvPassiveConfigBase::GetValue(int32_t cfgId, const std::map<int32_t, T>& values, T& value) const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	//check if config is initialized
	if (m_configPath.empty())
	{
		//config is not initilized -> return error
		return MSV_NOT_INITIALIZED_ERROR;
	}

	//find value
	typename std::map<int32_t, T>::const_iterator it = values.find(cfgId);

	if (it != values.end())
	{
		//value has been found
		value = it->second;
		return MSV_SUCCESS;
	}

	//value has not been found -> return error
	return MSV_NOT_FOUND_ERROR;
}

/** @} */	//End of group MCONFIG.
