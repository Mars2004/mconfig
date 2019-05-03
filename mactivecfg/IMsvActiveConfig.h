/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Active Config Interface
* @details		Contains interface of MarsTech Active Config.
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


#ifndef MARSTECH_IACTIVECONFIG_H
#define MARSTECH_IACTIVECONFIG_H


#include "IMsvActiveConfigCallback.h"
#include "mconfig/common/IMsvConfigKeyMap.h"
#include "mconfig/common/IMsvDefaultValue.h"

#include "merror/MsvError.h"

MSV_DISABLE_ALL_WARNINGS

#include <string>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Active Config Interface.
* @details	Interface for active configuration.
* @see		MsvActiveConfig
******************************************************************************************************/
class IMsvActiveConfig
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvActiveConfig() {}

	/**************************************************************************************************//**
	* @brief			Initialize active configuration.
	* @details		Initialize active configuration and loads configuration from config database.
	* @param[in]	spConfigKeyMap		Config key map with config IDs and its default values.
	* @param[in]	configPath			Path to active config database.
	* @param[in]	groupName			Active config group name (might be used as table name, etc.).
	* @retval		MSV_ALREADY_INITIALIZED_INFO	When config has been already initialized (this is info, not error).
	* @retval		MSV_ALLOCATION_ERROR				When memory allocation failed.
	* @retval		MSV_INVALID_DATA_ERROR			When value has different type then requested.
	* @retval		MSV_NOT_FOUND_ERROR				When config value has not been found in database.
	* @retval		other error code					When failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Initialize(std::shared_ptr<IMsvConfigKeyMap<IMsvDefaultValue>> spConfigKeyMap, const char* configPath = "config.db", const char* groupName = "MsvConfig") = 0;

	/**************************************************************************************************//**
	* @brief			Uninitialize active configuration.
	* @details		Uninitialize active configuration and release all loaded data.
	* @retval		MSV_NOT_INITIALIZED_INFO		When config has not been initialized (this is info, not error).
	* @retval		other error code					When failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() = 0;
	
	/**************************************************************************************************//**
	* @brief			Initialize check.
	* @details		Returns flag if active configuration is initialized (true) or not (false).
	* @retval		true		When initialized.
	* @retval		false		When not initialized.
	******************************************************************************************************/
	virtual bool Initialized() const = 0;

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
	* @brief			Set bool value.
	* @details		Sets bool value to active configuration.
	* @param[in]	cfgId		Config ID to set its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode SetValue(int32_t cfgId, bool value) = 0;

	/**************************************************************************************************//**
	* @brief			Set double value.
	* @details		Sets double value to active configuration.
	* @param[in]	cfgId		Config ID to set its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode SetValue(int32_t cfgId, double value) = 0;

	/**************************************************************************************************//**
	* @brief			Set integer value.
	* @details		Sets int64_t value to active configuration.
	* @param[in]	cfgId		Config ID to set its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode SetValue(int32_t cfgId, int64_t value) = 0;

	/**************************************************************************************************//**
	* @brief			Set string value.
	* @details		Sets string value to active configuration.
	* @param[in]	cfgId		Config ID to set its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode SetValue(int32_t cfgId, const std::string& value) = 0;

	/**************************************************************************************************//**
	* @brief			Set unsigned integer value.
	* @details		Sets uint64_t value to active configuration.
	* @param[in]	cfgId		Config ID to set its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode SetValue(int32_t cfgId, uint64_t value) = 0;

	/**************************************************************************************************//**
	* @brief			Register active config callback.
	* @details		Registers active config callback which is called when active configuration has been changed.
	* @param[in]	spCallback		Callback to register.
	* @retval		MSV_ALREADY_REGISTERED_INFO	When callback has been already registered (this is info, not error).
	* @retval		MSV_SUCCESS							On success.
	* @see			IMsvActiveConfigCallback
	******************************************************************************************************/
	virtual MsvErrorCode RegisterCallback(std::shared_ptr<IMsvActiveConfigCallback> spCallback) = 0;

	/**************************************************************************************************//**
	* @brief			Unregister active config callback.
	* @details		Unregisters active config callback which is called when active configuration has been changed.
	* @param[in]	spCallback		Callback to unregister.
	* @retval		MSV_SUCCESS		On success.
	* @see			IMsvActiveConfigCallback
	******************************************************************************************************/
	virtual MsvErrorCode UnregisterCallback(std::shared_ptr<IMsvActiveConfigCallback> spCallback) = 0;

	/*-----------------------------------------------------------------------------------------------------
	**										IMsvDefaultValue inline public methods
	**---------------------------------------------------------------------------------------------------*/

	/**************************************************************************************************//**
	* @brief			Set string value.
	* @details		Sets string value to active configuration.
	* @param[in]	cfgId		Config ID to set its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	inline MsvErrorCode SetValue(int32_t cfgId, const char* value)
	{
		return SetValue(cfgId, std::string(value));
	}
};


#endif // !MARSTECH_IACTIVECONFIG_H

/** @} */	//End of group MCONFIG.
