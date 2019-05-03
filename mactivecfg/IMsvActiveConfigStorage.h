/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Active Config Storage Interface
* @details		Contains interface of MarsTech Active Config Storage.
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


#ifndef MARSTECH_IACTIVECONFIGSTORAGE_H
#define MARSTECH_IACTIVECONFIGSTORAGE_H


#include "IMsvActiveConfigStorageCallback.h"
#include "mconfig/common/IMsvConfigKeyMap.h"
#include "mconfig/common/IMsvDefaultValue.h"

MSV_DISABLE_ALL_WARNINGS

#include <string>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Active Config Storage Interface.
* @details	Interface for active configuration storage.
* @see		MsvActiveConfigStorage
******************************************************************************************************/
class IMsvActiveConfigStorage
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvActiveConfigStorage() {}

	/**************************************************************************************************//**
	* @brief			Initialize active configuration storage.
	* @details		Initialize active configuration storage and loads configuration from config database.
	* @param[in]	spConfigKeyMap		Config key map with config IDs and its default values.
	* @param[in]	configPath			Path to active config database.
	* @param[in]	groupName			Active config group name (might be used as table name, etc.).
	* @retval		MSV_ALREADY_INITIALIZED_INFO	When config has been already initialized (this is info, not error).
	* @retval		MSV_ALREADY_EXISTS_INFO			Whend storage already exists (this is info, not error).
	* @retval		MSV_ALLOCATION_ERROR				When memory allocation failed.
	* @retval		MSV_INVALID_DATA_ERROR			When value has different type then requested.
	* @retval		MSV_NOT_FOUND_ERROR				When config value has not been found in database.
	* @retval		other error code					When failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Initialize(const std::shared_ptr<IMsvConfigKeyMap<IMsvDefaultValue>> spConfigKeyMap, const char* configPath = "config.db", const char* groupName = "MsvConfig") = 0;

	/**************************************************************************************************//**
	* @brief			Uninitialize active configuration storage.
	* @details		Uninitialize active configuration storage and release all loaded data.
	* @retval		MSV_NOT_INITIALIZED_INFO		When config storage has not been initialized (this is info, not error).
	* @retval		other error code					When failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() = 0;
	
	/**************************************************************************************************//**
	* @brief			Initialize check.
	* @details		Returns flag if active configuration storage is initialized (true) or not (false).
	* @retval		true		When initialized.
	* @retval		false		When not initialized.
	******************************************************************************************************/
	virtual bool Initialized() const = 0;

	/**************************************************************************************************//**
	* @brief			Get bool value.
	* @details		Reads bool value from storage and returns it.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	* @retval		other error code				When some other error occured.
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, bool& value) const = 0;

	/**************************************************************************************************//**
	* @brief			Get double value.
	* @details		Reads double value from storage and returns it.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	* @retval		other error code				When some other error occured.
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, double& value) const = 0;

	/**************************************************************************************************//**
	* @brief			Get integer value.
	* @details		Reads int64_t value from storage and returns it.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	* @retval		other error code				When some other error occured.
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, int64_t& value) const = 0;

	/**************************************************************************************************//**
	* @brief			Get string value.
	* @details		Reads string value from storage and returns it.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	* @retval		other error code				When some other error occured.
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, std::string& value) const = 0;

	/**************************************************************************************************//**
	* @brief			Get unsigned integer value.
	* @details		Reads uint64_t value from storage and returns it.
	* @param[in]	cfgId		Config ID to get its value.
	* @param[out]	value		Found and returned value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	* @retval		other error code				When some other error occured.
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, uint64_t& value) const = 0;

	/**************************************************************************************************//**
	* @brief			Store bool value.
	* @details		Stores bool value to active configuration.
	* @param[in]	cfgId		Config ID to store its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode StoreValue(int32_t cfgId, bool value) = 0;

	/**************************************************************************************************//**
	* @brief			Store double value.
	* @details		Stores double value to active configuration.
	* @param[in]	cfgId		Config ID to store its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode StoreValue(int32_t cfgId, double value) = 0;

	/**************************************************************************************************//**
	* @brief			Store integer value.
	* @details		Stores int64_t value to active configuration.
	* @param[in]	cfgId		Config ID to store its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode StoreValue(int32_t cfgId, int64_t value) = 0;

	/**************************************************************************************************//**
	* @brief			Store string value.
	* @details		Stores string value to active configuration.
	* @param[in]	cfgId		Config ID to store its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode StoreValue(int32_t cfgId, const std::string& value) = 0;

	/**************************************************************************************************//**
	* @brief			Store unsigned integer value.
	* @details		Stores uint64_t value to active configuration.
	* @param[in]	cfgId		Config ID to store its value.
	* @param[out]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		other error code				When failed.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode StoreValue(int32_t cfgId, uint64_t value) = 0;

	/**************************************************************************************************//**
	* @brief			Register active config storage callback.
	* @details		Registers active config storage callback which is called when active configuration has been changed.
	* @param[in]	spCallback		Callback to register.
	* @retval		MSV_ALREADY_REGISTERED_INFO	When callback has been already registered (this is info, not error).
	* @retval		MSV_SUCCESS							On success.
	* @see			IMsvActiveConfigStorageCallback
	******************************************************************************************************/
	virtual MsvErrorCode RegisterCallback(std::shared_ptr<IMsvActiveConfigStorageCallback> spCallback) = 0;

	/**************************************************************************************************//**
	* @brief			Unregister active config storage callback.
	* @details		Unregisters active config storage callback which is called when active configuration has been changed.
	* @param[in]	spCallback		Callback to unregister.
	* @retval		MSV_SUCCESS		On success.
	* @see			IMsvActiveConfigStorageCallback
	******************************************************************************************************/
	virtual MsvErrorCode UnregisterCallback(std::shared_ptr<IMsvActiveConfigStorageCallback> spCallback) = 0;
};


#endif // !MARSTECH_IACTIVECONFIGSTORAGE_H

/** @} */	//End of group MCONFIG.
