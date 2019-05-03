/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Passive Config Base Implementation
* @details		Contains implementation @ref MsvPassiveConfigBase of @ref IMsvPassiveConfig interface.
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


#ifndef MARSTECH_PASSIVECONFIGBASE_H
#define MARSTECH_PASSIVECONFIGBASE_H


#include "IMsvPassiveConfig.h"

MSV_DISABLE_ALL_WARNINGS

#include <mutex>
#include <map>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Passive Config Base Implementation.
* @details	Base implementation for passive configuration.
* @note		It does not implement @ref IMsvPassiveConfig::ReloadConfiguration(). It should be implemented
*				by a child class.
* @see		IMsvPassiveConfig
******************************************************************************************************/
class MsvPassiveConfigBase:
	public IMsvPassiveConfig
{
public:
	/**************************************************************************************************//**
	* @brief		Constructor.
	******************************************************************************************************/
	MsvPassiveConfigBase();

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvPassiveConfigBase();

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvPassiveConfig public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvPassiveConfig::GetValue(int32_t cfgId, bool& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, bool& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvPassiveConfig::GetValue(int32_t cfgId, double& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, double& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvPassiveConfig::GetValue(int32_t cfgId, int64_t& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, int64_t& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvPassiveConfig::GetValue(int32_t cfgId, std::string& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, std::string& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvPassiveConfig::GetValue(int32_t cfgId, uint64_t& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, uint64_t& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvPassiveConfig::Initialize(std::shared_ptr<IMsvConfigKeyMap<IMsvConfigKey>> spConfigKeyMap, const char* configPath)
	******************************************************************************************************/
	virtual MsvErrorCode Initialize(std::shared_ptr<IMsvConfigKeyMap<IMsvConfigKey>> spConfigKeyMap, const char* configPath = "config.ini") override;

	/**************************************************************************************************//**
	* @copydoc IMsvPassiveConfig::ReadFailedData(int32_t& lineNumber, int32_t& cfgId) const
	******************************************************************************************************/
	virtual void ReadFailedData(int32_t& lineNumber, int32_t& cfgId) const override;


	/*-----------------------------------------------------------------------------------------------------
	**											MsvPassiveConfigBase protected methods
	**---------------------------------------------------------------------------------------------------*/
protected:
	/**************************************************************************************************//**
	* @brief			Get value.
	* @details		Template method used in virtual Get methods.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[in]	values	Config key storage with loaded values.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	template<class T> MsvErrorCode GetValue(int32_t cfgId, const std::map<int32_t, T>& values, T& value) const;

protected:
	/**************************************************************************************************//**
	* @brief		Config mutex.
	* @details	Locks this object for thread safety access.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

	/**************************************************************************************************//**
	* @brief		Failed config ID.
	* @details	Contains config ID which reading failed.
	* @see		ReadFailedData
	******************************************************************************************************/
	int32_t m_cfgIdWithError;

	/**************************************************************************************************//**
	* @brief		Failed line number.
	* @details	Contains line number on which parsing config file failed.
	* @see		ReadFailedData
	******************************************************************************************************/
	int32_t m_lineNumberWithError;

	/**************************************************************************************************//**
	* @brief		Config file path.
	* @details	File path to configuration stored in file.
	* @see		Initialize
	******************************************************************************************************/
	std::string m_configPath;

	/**************************************************************************************************//**
	* @brief		Config key map.
	* @details	Contains config keys with default values and its definitions (config file path IDs).
	* @see		Initialize
	******************************************************************************************************/
	std::shared_ptr<IMsvConfigKeyMap<IMsvConfigKey>> m_spConfigKeyMap;

	/*-----------------------------------------------------------------------------------------------------
	**										MsvPassiveConfigBase value storages (maps)
	**---------------------------------------------------------------------------------------------------*/

	/**************************************************************************************************//**
	* @brief		Bool values.
	* @details	Storage (map) of bool values.
	* @see		ReloadConfiguration
	* @see		GetValue(int32_t cfgId, bool& value) 
	******************************************************************************************************/
	std::map<int32_t, bool> m_boolValues;

	/**************************************************************************************************//**
	* @brief		Double values.
	* @details	Storage (map) of double values.
	* @see		ReloadConfiguration
	* @see		GetValue(int32_t cfgId, double& value)
	******************************************************************************************************/
	std::map<int32_t, double> m_doubleValues;

	/**************************************************************************************************//**
	* @brief		Integer values.
	* @details	Storage (map) of int64_t values.
	* @see		ReloadConfiguration
	* @see		GetValue(int32_t cfgId, int64_t& value)
	******************************************************************************************************/
	std::map<int32_t, int64_t> m_integerValues;

	/**************************************************************************************************//**
	* @brief		String values.
	* @details	Storage (map) of string values.
	* @see		ReloadConfiguration
	* @see		GetValue(int32_t cfgId, std::string& value)
	******************************************************************************************************/
	std::map<int32_t, std::string> m_stringValues;

	/**************************************************************************************************//**
	* @brief		Unsigned integer values.
	* @details	Storage (map) of uint64_t values.
	* @see		ReloadConfiguration
	* @see		GetValue(int32_t cfgId, uint64_t& value)
	******************************************************************************************************/
	std::map<int32_t, uint64_t> m_unsignedValues;
};


#endif // !MARSTECH_PASSIVECONFIGBASE_H

/** @} */	//End of group MCONFIG.
