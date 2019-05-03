/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Passive Config Interface
* @details		Contains interface of MarsTech Passive Config.
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


#ifndef MARSTECH_IPASSIVECONFIG_H
#define MARSTECH_IPASSIVECONFIG_H


#include "mconfig/common/IMsvConfigKey.h"
#include "mconfig/common/IMsvConfigKeyMap.h"


/**************************************************************************************************//**
* @brief		MarsTech Passive Config Interface.
* @details	Interface for passive configuration.
* @see		MsvPassiveConfigBase
******************************************************************************************************/
class IMsvPassiveConfig
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvPassiveConfig() {}

	/**************************************************************************************************//**
	* @brief			Get bool value.
	* @details		Finds and gets/returns bool value.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, bool& value) const = 0;

	/**************************************************************************************************//**
	* @brief			Get double value.
	* @details		Finds and gets/returns double value.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, double& value) const = 0;

	/**************************************************************************************************//**
	* @brief			Get integer value.
	* @details		Finds and gets/returns int64_t value.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, int64_t& value) const = 0;

	/**************************************************************************************************//**
	* @brief			Get string value.
	* @details		Finds and gets/returns string value.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, std::string& value) const = 0;

	/**************************************************************************************************//**
	* @brief			Get unsigned integer value.
	* @details		Finds and gets/returns uint64_t value.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, uint64_t& value) const = 0;

	/**************************************************************************************************//**
	* @brief			Initialize passive configuration.
	* @details		Initialize passive configuration and loads configuration from config file.
	* @param[in]	spConfigKeyMap		Config key map with config IDs and its default values.
	* @param[in]	configPath			Path to passive config file.
	* @retval		MSV_ALREADY_INITIALIZED_INFO	When config has been already initialized (this is info, not error).
	* @retval		MSV_PARSE_ERROR					When parsing configuration file failed.
	* @retval		MSV_INVALID_DATA_ERROR			When value has different type then requested.
	* @retval		MSV_UNKNOWN_ERROR					Unknown value type (not supported).
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Initialize(std::shared_ptr<IMsvConfigKeyMap<IMsvConfigKey>> spConfigKeyMap, const char* configPath = "config.ini") = 0;

	/**************************************************************************************************//**
	* @brief			Read/get failed data.
	* @details		Returns error data when @ref ReloadConfiguration or @ref Initialize failed. Config file
	*					line number on which parsing failed and config ID which failed to read.
	* @param[out]	lineNumber	Config file	line number on which parsing failed (INT32_MIN if not failed).
	* @param[out]	cfgId			Config ID which failed to read (INT32_MIN if not failed).
	******************************************************************************************************/
	virtual void ReadFailedData(int32_t& lineNumber, int32_t& cfgId) const = 0;

	/**************************************************************************************************//**
	* @brief			Reload configuration.
	* @details		Reloads configuration from config file.
	* @note			Call this method when passive configuration file has changed.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_PARSE_ERROR				When parsing configuration file failed.
	* @retval		MSV_INVALID_DATA_ERROR		When value has different type then requested.
	* @retval		MSV_UNKNOWN_ERROR				Unknown value type (not supported).
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode ReloadConfiguration() = 0;
};


#endif // !MARSTECH_IPASSIVECONFIG_H

/** @} */	//End of group MCONFIG.
