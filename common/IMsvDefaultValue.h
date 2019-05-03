/**************************************************************************************************//**
* @defgroup		MCONFIG MarsTech Config
* @brief			MarsTech Config
* @details		Contains implementation and all definitions of MarsTech Config.
* @copyright	GNU General Public License (GPLv3).
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Default Value Interface
* @details		Contains interface of MarsTech Default Value.
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


#ifndef MARSTECH_IDEFAULTVALUE_H
#define MARSTECH_IDEFAULTVALUE_H


#include "merror/MsvError.h"

MSV_DISABLE_ALL_WARNINGS

#include <string>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Default Value Interface.
* @details	Interface for default values.
******************************************************************************************************/
class IMsvDefaultValue
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvDefaultValue() {}

	/**************************************************************************************************//**
	* @brief			Get default bool value.
	* @details		Returns default bool value.
	* @param[out]	defaultValue	Default value used for missing values in real storage (file, etc.).
	* @retval		MSV_INVALID_DATA_ERROR		When default value type is not bool.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetDefaultValue(bool& defaultValue) const = 0;

	/**************************************************************************************************//**
	* @brief			Get default double value.
	* @details		Returns default double value.
	* @param[out]	defaultValue	Default value used for missing values in real storage (file, etc.).
	* @retval		MSV_INVALID_DATA_ERROR		When default value type is not double.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetDefaultValue(double& defaultValue) const = 0;

	/**************************************************************************************************//**
	* @brief			Get default integer value.
	* @details		Returns default int64_t value.
	* @param[out]	defaultValue	Default value used for missing values in real storage (file, etc.).
	* @retval		MSV_INVALID_DATA_ERROR		When default value type is not int64_t.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetDefaultValue(int64_t& defaultValue) const = 0;

	/**************************************************************************************************//**
	* @brief			Get default string value.
	* @details		Returns default string value.
	* @param[out]	defaultValue	Default value used for missing values in real storage (file, etc.).
	* @retval		MSV_INVALID_DATA_ERROR		When default value type is not string.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetDefaultValue(std::string& defaultValue) const = 0;

	/**************************************************************************************************//**
	* @brief			Get default unsigned integer value.
	* @details		Returns default uint64_t value.
	* @param[out]	defaultValue	Default value used for missing values in real storage (file, etc.).
	* @retval		MSV_INVALID_DATA_ERROR		When default value type is not uint64_t.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode GetDefaultValue(uint64_t& defaultValue) const = 0;

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


#endif // !MARSTECH_IDEFAULTVALUE_H

/** @} */	//End of group MCONFIG.
