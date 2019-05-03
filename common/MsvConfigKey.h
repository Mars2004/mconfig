/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Config Key Implementation
* @details		Contains implementation @ref MsvConfigKey of @ref IMsvConfigKey interface.
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


#ifndef MARSTECH_CONFIGKEY_H
#define MARSTECH_CONFIGKEY_H


#include "IMsvConfigKey.h"
#include "IMsvDefaultValue.h"

MSV_DISABLE_ALL_WARNINGS

#include <any>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Config Key Implementation.
* @details	Implementation for config key interface.
* @see		IMsvConfigKey
******************************************************************************************************/
class MsvConfigKey:
	public IMsvConfigKey
{
public:
	/**************************************************************************************************//**
	* @brief		Constructor.
	* @param[in]	group					Group ID (in config storage).
	* @param[in]	key					Config key (in config storage).
	* @param[in]	spDefaultValue		Shared pointer to default value.
	******************************************************************************************************/
	MsvConfigKey(const char* group, const char* key, std::shared_ptr<IMsvDefaultValue> spDefaultValue);

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvConfigKey();

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvConfigKey public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvConfigKey::GetData(std::string& group, std::string& key, bool& defaultValue) const
	******************************************************************************************************/
	virtual MsvErrorCode GetData(std::string& group, std::string& key, bool& defaultValue) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvConfigKey::GetData(std::string& group, std::string& key, double& defaultValue) const
	******************************************************************************************************/
	virtual MsvErrorCode GetData(std::string& group, std::string& key, double& defaultValue) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvConfigKey::GetData(std::string& group, std::string& key, int64_t& defaultValue) const
	******************************************************************************************************/
	virtual MsvErrorCode GetData(std::string& group, std::string& key, int64_t& defaultValue) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvConfigKey::GetData(std::string& group, std::string& key, std::string& defaultValue) const
	******************************************************************************************************/
	virtual MsvErrorCode GetData(std::string& group, std::string& key, std::string& defaultValue) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvConfigKey::GetData(std::string& group, std::string& key, uint64_t& defaultValue) const
	******************************************************************************************************/
	virtual MsvErrorCode GetData(std::string& group, std::string& key, uint64_t& defaultValue) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvConfigKey::IsBool() const
	******************************************************************************************************/
	virtual bool IsBool() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvConfigKey::IsDouble() const
	******************************************************************************************************/
	virtual bool IsDouble() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvConfigKey::IsInteger() const
	******************************************************************************************************/
	virtual bool IsInteger() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvConfigKey::IsString() const
	******************************************************************************************************/
	virtual bool IsString() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvConfigKey::IsUnsigned() const
	******************************************************************************************************/
	virtual bool IsUnsigned() const override;

	/*-----------------------------------------------------------------------------------------------------
	**											MsvConfigKey protected methods
	**---------------------------------------------------------------------------------------------------*/
protected:
	/**************************************************************************************************//**
	* @brief			Get data.
	* @details		Template method used in virtual Get methods.
	* @param[in]	group					Group ID (in config storage).
	* @param[in]	key					Config key (in config storage).
	* @param[in]	defaultValue		Default value.
	* @retval		MSV_NOT_INITIALIZED_ERROR	When config has not been initialized.
	* @retval		MSV_NOT_FOUND_ERROR			When config ID (cfgId) does not exist.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	template<class T> MsvErrorCode GetData(std::string& group, std::string& key, T& defaultValue) const;

protected:
	/**************************************************************************************************//**
	* @brief		Config group ID.
	* @details	It is group ID known by real configuration storage (file, etc.).
	******************************************************************************************************/
	std::string m_group;

	/**************************************************************************************************//**
	* @brief		Config key.
	* @details	It is config key (ID) known by real configuration storage (file, etc.).
	******************************************************************************************************/
	std::string m_key;

	/**************************************************************************************************//**
	* @brief		Default value.
	* @details	Default value which is used for missing values in real storage (file, etc.). It also contains
	*				type of configuration value.
	******************************************************************************************************/
	std::shared_ptr<IMsvDefaultValue> m_spDefaultValue;
};


#endif // !MARSTECH_CONFIGKEY_H

/** @} */	//End of group MCONFIG.
