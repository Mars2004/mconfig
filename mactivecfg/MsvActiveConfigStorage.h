/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Active Config Storage Implementation
* @details		Contains implementation @ref MsvActiveConfigStorage of @ref IMsvActiveConfigStorage interface.
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


#ifndef MARSTECH_ACTIVECONFIGSTORAGE_H
#define MARSTECH_ACTIVECONFIGSTORAGE_H


#include "IMsvActiveConfigStorage.h"
#include "mconfig/msqlitewrapper/IMsvSQLite.h"
#include "mconfig/msqlitewrapper/IMsvSQLiteCallback.h"

#include "mlogging/mlogging.h"

MSV_DISABLE_ALL_WARNINGS

#include <mutex>
#include <forward_list>

MSV_ENABLE_WARNINGS


//forward declaration of MarsTech Active Config Storage Dependency Injection Factory
class MsvActiveConfigStorage_Factory;


/**************************************************************************************************//**
* @brief		MarsTech Active Config Storage Implementation.
* @details	Implementation for active configuration storage.
* @see		IMsvActiveConfigStorage
******************************************************************************************************/
class MsvActiveConfigStorage:
	public IMsvActiveConfigStorage
{
public:
	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	spFactory		Shared pointer to dependency injection factory.
	* @param[in]	spLogger			Shared pointer to logger for logging.
	* @see			MsvActiveConfigStorage_Factory
	******************************************************************************************************/
	MsvActiveConfigStorage(std::shared_ptr<MsvActiveConfigStorage_Factory> spFactory = nullptr, std::shared_ptr<MsvLogger> spLogger = nullptr);

	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	spLogger			Shared pointer to logger for logging.
	* @see			MsvActiveConfig_Factory
	******************************************************************************************************/
	MsvActiveConfigStorage(std::shared_ptr<MsvLogger> spLogger);

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvActiveConfigStorage();

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvActiveConfigStorage public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::Initialize(const std::shared_ptr<IMsvConfigKeyMap<IMsvDefaultValue>> spConfigKeyMap, const char* configPath, const char* groupName)
	******************************************************************************************************/
	virtual MsvErrorCode Initialize(const std::shared_ptr<IMsvConfigKeyMap<IMsvDefaultValue>> spConfigKeyMap, const char* configPath = "config.db", const char* groupName = "MsvConfig") override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::Uninitialize()
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::Initialized() const
	******************************************************************************************************/
	virtual bool Initialized() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::GetValue(int32_t cfgId, bool& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, bool& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::GetValue(int32_t cfgId, double& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, double& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::GetValue(int32_t cfgId, int64_t& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, int64_t& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::GetValue(int32_t cfgId, std::string& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, std::string& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::GetValue(int32_t cfgId, uint64_t& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, uint64_t& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::StoreValue(int32_t cfgId, bool value)
	******************************************************************************************************/
	virtual MsvErrorCode StoreValue(int32_t cfgId, bool value) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::StoreValue(int32_t cfgId, double value)
	******************************************************************************************************/
	virtual MsvErrorCode StoreValue(int32_t cfgId, double value) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::StoreValue(int32_t cfgId, int64_t value)
	******************************************************************************************************/
	virtual MsvErrorCode StoreValue(int32_t cfgId, int64_t value) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::StoreValue(int32_t cfgId, const std::string& value)
	******************************************************************************************************/
	virtual MsvErrorCode StoreValue(int32_t cfgId, const std::string& value) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::StoreValue(int32_t cfgId, uint64_t value)
	******************************************************************************************************/
	virtual MsvErrorCode StoreValue(int32_t cfgId, uint64_t value) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::RegisterCallback(std::shared_ptr<IMsvActiveConfigStorageCallback> spCallback)
	******************************************************************************************************/
	virtual MsvErrorCode RegisterCallback(std::shared_ptr<IMsvActiveConfigStorageCallback> spCallback) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfigStorage::UnregisterCallback(std::shared_ptr<IMsvActiveConfigStorageCallback> spCallback)
	******************************************************************************************************/
	virtual MsvErrorCode UnregisterCallback(std::shared_ptr<IMsvActiveConfigStorageCallback> spCallback) override;

	/*-----------------------------------------------------------------------------------------------------
	**											MsvActiveConfigStorage protected methods
	**---------------------------------------------------------------------------------------------------*/
protected:
	/**************************************************************************************************//**
	* @brief			Value has been changed.
	* @details		This method is called when value has been changed. It notifies all registered callbacks.
	* @param[in]	cfgId			Config ID of changed value.
	* @param[in]	newValue		New value, current value.
	******************************************************************************************************/
	template<class T> void OnChange(int32_t cfgId, const T& newValue) const;

	/**************************************************************************************************//**
	* @brief			Store default value.
	* @details		Stores default value to real storage. 
	* @param[in]	cfgId		Config ID to set its value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_INVALID_DATA_ERROR			When value has different type then requested.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	MsvErrorCode StoreDefaultValue(int32_t cfgId);

	/**************************************************************************************************//**
	* @brief			Store value.
	* @details		Template method used in virtual Store methods.
	* @param[in]	cfgId		Config ID to set its value.
	* @param[in]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	template<class T> MsvErrorCode StoreValue(int32_t cfgId, const T& value);

protected:
	/**************************************************************************************************//**
	* @brief		Config storage mutex.
	* @details	Locks this object for thread safety access.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

	/**************************************************************************************************//**
	* @brief		Registered callbacks.
	* @details	Contains all registered callbacks which will be notified about data changes.
	* @see		RegisterCallback
	* @see		UnregisterCallback
	******************************************************************************************************/
	std::forward_list<std::shared_ptr<IMsvActiveConfigStorageCallback>> m_callbacks;

	/**************************************************************************************************//**
	* @brief		Initialize flag.
	* @details	Flag if config is initialized (true) or not (false).
	* @see		Initialize
	* @see		Uninitialize
	* @see		Initialized
	******************************************************************************************************/
	bool m_initialized;

	/**************************************************************************************************//**
	* @brief		Config key map.
	* @details	Config key map with config IDs and its default values.
	* @see		IMsvConfigKeyMap
	******************************************************************************************************/
	std::shared_ptr<IMsvConfigKeyMap<IMsvDefaultValue>> m_spConfigKeyMap;

	/**************************************************************************************************//**
	* @brief		Dependency injection factory.
	* @details	Contains get method for all injected objects.
	* @see		MsvActiveConfig_Factory
	******************************************************************************************************/
	std::shared_ptr<MsvActiveConfigStorage_Factory> m_spFactory;

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;

	/**************************************************************************************************//**
	* @brief		SQLite wrapper.
	* @details	Configuration data is stored in SQLite database. This object for data manipulation.
	******************************************************************************************************/
	std::shared_ptr<IMsvSQLite> m_spSQLite;

	/**************************************************************************************************//**
	* @brief		Table name.
	* @details	Configuration table name.
	******************************************************************************************************/
	std::string m_tableName;
};


#endif // !MARSTECH_ACTIVECONFIGSTORAGE_H

/** @} */	//End of group MCONFIG.
