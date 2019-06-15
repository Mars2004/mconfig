/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Active Config Implementation
* @details		Contains implementation @ref MsvActiveConfig of @ref IMsvActiveConfig interface.
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


#ifndef MARSTECH_ACTIVECONFIG_H
#define MARSTECH_ACTIVECONFIG_H


#include "IMsvActiveConfig.h"
#include "IMsvActiveConfigStorage.h"

#include "mlogging/mlogging.h"

MSV_DISABLE_ALL_WARNINGS

#include <mutex>
#include <forward_list>

MSV_ENABLE_WARNINGS


//forward declaration of MarsTech Active Config Dependency Injection Factory
class MsvActiveConfig_Factory;


/**************************************************************************************************//**
* @brief		MarsTech Active Config Implementation.
* @details	Implementation for active configuration.
* @see		IMsvActiveConfig
******************************************************************************************************/
class MsvActiveConfig:
	public IMsvActiveConfig
{
protected:
	/**************************************************************************************************//**
	* @brief		MarsTech Active Config Storage Implementation.
	* @details	Implementation for active configuration storage callback for active config.
	* @see		IMsvActiveConfigStorageCallback
	******************************************************************************************************/
	class MsvActiveConfigStorageCallback:
		public IMsvActiveConfigStorageCallback
	{
	public:
		/**************************************************************************************************//**
		* @brief			Constructor.
		* @param[in]	pConfig			Pointer to parent (@ref MsvActiveConfig).
		* @see			MsvActiveConfig
		******************************************************************************************************/
		MsvActiveConfigStorageCallback(MsvActiveConfig* pConfig);

	protected:
		/**************************************************************************************************//**
		* @copydoc IMsvActiveConfigStorageCallback::OnValueChanged(int32_t cfgId, bool value)
		******************************************************************************************************/
		virtual void OnValueChanged(int32_t cfgId, bool newValue) override;

		/**************************************************************************************************//**
		* @copydoc IMsvActiveConfigStorageCallback::OnValueChanged(int32_t cfgId, double value)
		******************************************************************************************************/
		virtual void OnValueChanged(int32_t cfgId, double newValue) override;

		/**************************************************************************************************//**
		* @copydoc IMsvActiveConfigStorageCallback::OnValueChanged(int32_t cfgId, int64_t value)
		******************************************************************************************************/
		virtual void OnValueChanged(int32_t cfgId, int64_t newValue) override;

		/**************************************************************************************************//**
		* @copydoc IMsvActiveConfigStorageCallback::OnValueChanged(int32_t cfgId, const char* value)
		******************************************************************************************************/
		virtual void OnValueChanged(int32_t cfgId, const char* newValue) override;

		/**************************************************************************************************//**
		* @copydoc IMsvActiveConfigStorageCallback::OnValueChanged(int32_t cfgId, uint64_t value)
		******************************************************************************************************/
		virtual void OnValueChanged(int32_t cfgId, uint64_t newValue) override;

	protected:
		/**************************************************************************************************//**
	* @brief		Pointer to parent.
	* @details	Pointer to object which creates this one.
	******************************************************************************************************/
		MsvActiveConfig* m_pConfig;
	};

public:
	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	spFactory		Shared pointer to dependency injection factory.
	* @param[in]	spLogger			Shared pointer to logger for logging.
	* @see			MsvActiveConfig_Factory
	******************************************************************************************************/
	MsvActiveConfig(std::shared_ptr<MsvActiveConfig_Factory> spFactory = nullptr, std::shared_ptr<MsvLogger> spLogger = nullptr);

	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	spLogger			Shared pointer to logger for logging.
	******************************************************************************************************/
	MsvActiveConfig(std::shared_ptr<MsvLogger> spLogger);

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvActiveConfig();

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvActiveConfig public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::Initialize(const std::shared_ptr<IMsvConfigKeyMap<IMsvDefaultValue>> spConfigKeyMap, const char* configPath, const char* groupName)
	******************************************************************************************************/
	virtual MsvErrorCode Initialize(const std::shared_ptr<IMsvConfigKeyMap<IMsvDefaultValue>> spConfigKeyMap, const char* configPath = "config.db", const char* groupName = "MsvConfig") override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::Uninitialize()
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::Initialized() const
	******************************************************************************************************/
	virtual bool Initialized() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::GetValue(int32_t cfgId, bool& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, bool& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::GetValue(int32_t cfgId, double& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, double& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::GetValue(int32_t cfgId, int64_t& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, int64_t& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::GetValue(int32_t cfgId, std::string& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, std::string& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::GetValue(int32_t cfgId, uint64_t& value) const
	******************************************************************************************************/
	virtual MsvErrorCode GetValue(int32_t cfgId, uint64_t& value) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::SetValue(int32_t cfgId, bool value)
	******************************************************************************************************/
	virtual MsvErrorCode SetValue(int32_t cfgId, bool value) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::SetValue(int32_t cfgId, double value)
	******************************************************************************************************/
	virtual MsvErrorCode SetValue(int32_t cfgId, double value) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::SetValue(int32_t cfgId, int64_t value)
	******************************************************************************************************/
	virtual MsvErrorCode SetValue(int32_t cfgId, int64_t value) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::SetValue(int32_t cfgId, const std::string& value)
	******************************************************************************************************/
	virtual MsvErrorCode SetValue(int32_t cfgId, const std::string& value) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::SetValue(int32_t cfgId, uint64_t value)
	******************************************************************************************************/
	virtual MsvErrorCode SetValue(int32_t cfgId, uint64_t value) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::RegisterCallback(std::shared_ptr<IMsvActiveConfigCallback> spCallback)
	******************************************************************************************************/
	virtual MsvErrorCode RegisterCallback(std::shared_ptr<IMsvActiveConfigCallback> spCallback) override;

	/**************************************************************************************************//**
	* @copydoc IMsvActiveConfig::UnregisterCallback(std::shared_ptr<IMsvActiveConfigCallback> spCallback)
	******************************************************************************************************/
	virtual MsvErrorCode UnregisterCallback(std::shared_ptr<IMsvActiveConfigCallback> spCallback) override;

	/*-----------------------------------------------------------------------------------------------------
	**											MsvActiveConfig protected methods
	**---------------------------------------------------------------------------------------------------*/
protected:
	/**************************************************************************************************//**
	* @brief			Value has been changed.
	* @details		This method is called by @ref MsvActiveConfigStorageCallback when value has been changed
	*					(callback must be registered before). It sets new value to cache and notifies all
	*					registered callbacks.
	* @param[in]	cfgId			Config ID of changed value.
	* @param[in]	newValue		New value, current value.
	******************************************************************************************************/
	template<class T> void OnValueChanged(int32_t cfgId, T newValue);

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

	/**************************************************************************************************//**
	* @brief			Set value.
	* @details		Template method used in virtual Set methods.
	* @param[in]	cfgId		Config ID to set its value.
	* @param[in]	values	Config key storage with loaded values.
	* @param[in]	value		New value of config ID.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	template<class T> MsvErrorCode SetValue(int32_t cfgId, std::map<int32_t, T>& values, const T& value);

protected:
	/**************************************************************************************************//**
	* @brief		Config mutex.
	* @details	Locks this object for thread safety access.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

	/**************************************************************************************************//**
	* @brief		Registered callbacks.
	* @details	Contains all registered callbacks which will be notified about data changes.
	* @see		RegisterCallback
	* @see		UnregisterCallback
	******************************************************************************************************/
	std::forward_list<std::shared_ptr<IMsvActiveConfigCallback>> m_callbacks;

	/**************************************************************************************************//**
	* @brief		Initialize flag.
	* @details	Flag if config is initialized (true) or not (false).
	* @see		Initialize
	* @see		Uninitialize
	* @see		Initialized
	******************************************************************************************************/
	bool m_initialized;

	/**************************************************************************************************//**
	* @brief		Dependency injection factory.
	* @details	Contains get method for all injected objects.
	* @see		MsvActiveConfig_Factory
	******************************************************************************************************/
	std::shared_ptr<MsvActiveConfig_Factory> m_spFactory;

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;

	/**************************************************************************************************//**
	* @brief		Active config storage.
	* @details	Real configuration storage. Implements metods for real storage manimupalation a notifications.
	******************************************************************************************************/
	std::shared_ptr<IMsvActiveConfigStorage> m_spStorage;

	/**************************************************************************************************//**
	* @brief		Active config storage callback.
	* @details	Callback registered to config storage for notifications about data changes.
	******************************************************************************************************/
	std::shared_ptr<IMsvActiveConfigStorageCallback> m_spStorageCallback;

	/*-----------------------------------------------------------------------------------------------------
	**										MsvActiveConfig value storages (maps)
	**---------------------------------------------------------------------------------------------------*/

	/**************************************************************************************************//**
	* @brief		Bool values.
	* @details	Storage (map) of bool values.
	* @see		Initialize
	* @see		GetValue(int32_t cfgId, bool& value)
	******************************************************************************************************/
	std::map<int32_t, bool> m_boolValues;

	/**************************************************************************************************//**
	* @brief		Double values.
	* @details	Storage (map) of double values.
	* @see		Initialize
	* @see		GetValue(int32_t cfgId, double& value)
	******************************************************************************************************/
	std::map<int32_t, double> m_doubleValues;

	/**************************************************************************************************//**
	* @brief		Integer values.
	* @details	Storage (map) of int64_t values.
	* @see		Initialize
	* @see		GetValue(int32_t cfgId, int64_t& value)
	******************************************************************************************************/
	std::map<int32_t, int64_t> m_integerValues;

	/**************************************************************************************************//**
	* @brief		String values.
	* @details	Storage (map) of string values.
	* @see		Initialize
	* @see		GetValue(int32_t cfgId, std::string& value)
	******************************************************************************************************/
	std::map<int32_t, std::string> m_stringValues;

	/**************************************************************************************************//**
	* @brief		Unsigned integer values.
	* @details	Storage (map) of uint64_t values.
	* @see		Initialize
	* @see		GetValue(int32_t cfgId, uint64_t& value)
	******************************************************************************************************/
	std::map<int32_t, uint64_t> m_unsignedValues;
};


#endif // !MARSTECH_ACTIVECONFIG_H

/** @} */	//End of group MCONFIG.
