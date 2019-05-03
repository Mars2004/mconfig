/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Default Value Implementation
* @details		Contains implementation @ref MsvDefaultValue of @ref IMsvDefaultValue interface.
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


#ifndef MARSTECH_DEFAULTVALUE_H
#define MARSTECH_DEFAULTVALUE_H


#include "IMsvDefaultValue.h"

MSV_DISABLE_ALL_WARNINGS

#include <any>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Default Value Implementation.
* @details	Implementation for default value interface.
* @see		IMsvDefaultValue
******************************************************************************************************/
class MsvDefaultValue:
	public IMsvDefaultValue
{
protected:
	/**************************************************************************************************//**
	* @brief		Constructor.
	* @details	Protected constructor with std::any. It should be called from public constructors to
	*				be able to store only allowed/supported types.
	* @param[in]	defaultValue		Default value (any of supported types).
	******************************************************************************************************/
	MsvDefaultValue(std::any defaultValue);

public:
	/**************************************************************************************************//**
	* @brief		Constructor.
	* @param[in]	defaultValue		Default bool value.
	******************************************************************************************************/
	MsvDefaultValue(bool defaultValue);

	/**************************************************************************************************//**
	* @brief		Constructor.
	* @param[in]	defaultValue		Default double value.
	******************************************************************************************************/
	MsvDefaultValue(double defaultValue);

	/**************************************************************************************************//**
	* @brief		Constructor.
	* @param[in]	defaultValue		Default int64_t value.
	******************************************************************************************************/
	MsvDefaultValue(int64_t defaultValue);

	/**************************************************************************************************//**
	* @brief		Constructor.
	* @param[in]	defaultValue		Default string value.
	******************************************************************************************************/
	MsvDefaultValue(const char* defaultValue);

	/**************************************************************************************************//**
	* @brief		Constructor.
	* @param[in]	defaultValue		Default uint64_t value.
	******************************************************************************************************/
	MsvDefaultValue(uint64_t defaultValue);

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvDefaultValue();

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvDefaultValue public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvDefaultValue::GetDefaultValue(bool& defaultValue) const
	******************************************************************************************************/
	virtual MsvErrorCode GetDefaultValue(bool& defaultValue) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDefaultValue::GetDefaultValue(double& defaultValue) const
	******************************************************************************************************/
	virtual MsvErrorCode GetDefaultValue(double& defaultValue) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDefaultValue::GetDefaultValue(int64_t& defaultValue) const
	******************************************************************************************************/
	virtual MsvErrorCode GetDefaultValue(int64_t& defaultValue) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDefaultValue::GetDefaultValue(std::string& defaultValue) const
	******************************************************************************************************/
	virtual MsvErrorCode GetDefaultValue(std::string& defaultValue) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDefaultValue::GetDefaultValue(uint64_t& defaultValue) const
	******************************************************************************************************/
	virtual MsvErrorCode GetDefaultValue(uint64_t& defaultValue) const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDefaultValue::IsBool() const
	******************************************************************************************************/
	virtual bool IsBool() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDefaultValue::IsDouble() const
	******************************************************************************************************/
	virtual bool IsDouble() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDefaultValue::IsInteger() const
	******************************************************************************************************/
	virtual bool IsInteger() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDefaultValue::IsString() const
	******************************************************************************************************/
	virtual bool IsString() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvDefaultValue::IsUnsigned() const
	******************************************************************************************************/
	virtual bool IsUnsigned() const override;

protected:
	/**************************************************************************************************//**
	* @brief		Default value.
	* @details	Contains any type of default value.
	******************************************************************************************************/
	std::any m_defaultValue;
};


#endif // !MARSTECH_DEFAULTVALUE_H

/** @} */	//End of group MCONFIG.
