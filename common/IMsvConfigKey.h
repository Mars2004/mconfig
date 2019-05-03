/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Config Key Interface
* @details		Contains interface of MarsTech Passive Config Key.
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


#ifndef MARSTECH_ICONFIGKEY_H
#define MARSTECH_ICONFIGKEY_H


#include "merror/MsvError.h"

MSV_DISABLE_ALL_WARNINGS

#include <string>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Config Key Interface.
* @details	Interface for config key used for translation to real implementation keys.
******************************************************************************************************/
class IMsvConfigKey
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvConfigKey() {}

	/**************************************************************************************************//**
	* @brief			Get bool data.
	* @details		Returns data for bool value.
	* @param[out]	group				Config group ID known by real configuration storage (file, etc.).
	* @param[out]	key				Config key known by real configuration storage (file, etc.).
	* @param[out]	defaultValue	Default value used for missing values in real storage (file, etc.).
	* @retval		MSV_INVALID_DATA_ERROR		When value type is not bool.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetData(std::string& group, std::string& key, bool& defaultValue) const = 0;

	/**************************************************************************************************//**
	* @brief			Get double data.
	* @details		Returns data for double value.
	* @param[out]	group				Config group ID known by real configuration storage (file, etc.).
	* @param[out]	key				Config key known by real configuration storage (file, etc.).
	* @param[out]	defaultValue	Default value used for missing values in real storage (file, etc.).
	* @retval		MSV_INVALID_DATA_ERROR		When value type is not double.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetData(std::string& group, std::string& key, double& defaultValue) const = 0;

	/**************************************************************************************************//**
	* @brief			Get integer data.
	* @details		Returns data for int64_t value.
	* @param[out]	group				Config group ID known by real configuration storage (file, etc.).
	* @param[out]	key				Config key known by real configuration storage (file, etc.).
	* @param[out]	defaultValue	Default value used for missing values in real storage (file, etc.).
	* @retval		MSV_INVALID_DATA_ERROR		When value type is not bool.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetData(std::string& group, std::string& key, int64_t& defaultValue) const = 0;

	/**************************************************************************************************//**
	* @brief			Get string data.
	* @details		Returns data for string value.
	* @param[out]	group				Config group ID known by real configuration storage (file, etc.).
	* @param[out]	key				Config key known by real configuration storage (file, etc.).
	* @param[out]	defaultValue	Default value used for missing values in real storage (file, etc.).
	* @retval		MSV_INVALID_DATA_ERROR		When value type is not string.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetData(std::string& group, std::string& key, std::string& defaultValue) const = 0;

	/**************************************************************************************************//**
	* @brief			Get unsigned integer data.
	* @details		Returns data for uint64_t value.
	* @param[out]	group				Config group ID known by real configuration storage (file, etc.).
	* @param[out]	key				Config key known by real configuration storage (file, etc.).
	* @param[out]	defaultValue	Default value used for missing values in real storage (file, etc.).
	* @retval		MSV_INVALID_DATA_ERROR		When value type is not uint64_t.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetData(std::string& group, std::string& key, uint64_t& defaultValue) const = 0;

	/**************************************************************************************************//**
	* @brief			Check if config key type is bool.
	* @details		Returns flag if config key type is bool (true) or not (false).
	* @retval		true		When config key type is bool.
	* @retval		false		When config key type is not bool.
	******************************************************************************************************/
	virtual bool IsBool() const = 0;

	/**************************************************************************************************//**
	* @brief			Check if config key type is double.
	* @details		Returns flag if config key type is double (true) or not (false).
	* @retval		true		When config key type is double.
	* @retval		false		When config key type is not double.
	******************************************************************************************************/
	virtual bool IsDouble() const = 0;

	/**************************************************************************************************//**
	* @brief			Check if config key type is integer.
	* @details		Returns flag if config key type is int64_t (true) or not (false).
	* @retval		true		When config key type is int64_t.
	* @retval		false		When config key type is not int64_t.
	******************************************************************************************************/
	virtual bool IsInteger() const = 0;

	/**************************************************************************************************//**
	* @brief			Check if config key type is string.
	* @details		Returns flag if config key type is string (true) or not (false).
	* @retval		true		When config key type is string.
	* @retval		false		When config key type is not string.
	******************************************************************************************************/
	virtual bool IsString() const = 0;

	/**************************************************************************************************//**
	* @brief			Check if config key type is unsigned integer.
	* @details		Returns flag if config key type is uint64_t (true) or not (false).
	* @retval		true		When config key type is uint64_t.
	* @retval		false		When config key type is not uint64_t.
	******************************************************************************************************/
	virtual bool IsUnsigned() const = 0;
};


#endif // !MARSTECH_ICONFIGKEY_H

/** @} */	//End of group MCONFIG.
