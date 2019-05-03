/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Default Value Implementation
* @details		Contains implementation of @ref MsvDefaultValue.
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


#include "MsvDefaultValue.h"
#include "merror/MsvErrorCodes.h"


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvDefaultValue::MsvDefaultValue(std::any defaultValue):
	m_defaultValue(defaultValue)
{

}

MsvDefaultValue::MsvDefaultValue(bool defaultValue):
	MsvDefaultValue(std::any(defaultValue))
{

}

MsvDefaultValue::MsvDefaultValue(double defaultValue):
	MsvDefaultValue(std::any(defaultValue))
{

}

MsvDefaultValue::MsvDefaultValue(int64_t defaultValue):
	MsvDefaultValue(std::any(defaultValue))
{

}

MsvDefaultValue::MsvDefaultValue(const char* defaultValue):
	MsvDefaultValue(std::any(std::string(defaultValue)))
{

}

MsvDefaultValue::MsvDefaultValue(uint64_t defaultValue):
	MsvDefaultValue(std::any(defaultValue))
{

}

MsvDefaultValue::~MsvDefaultValue()
{
}


/********************************************************************************************************************************
*															IMsvDefaultValue public methods
********************************************************************************************************************************/


MsvErrorCode MsvDefaultValue::GetDefaultValue(bool& defaultValue) const
{
	if (!IsBool())
	{
		return MSV_INVALID_DATA_ERROR;
	}

	defaultValue = *(std::any_cast<bool>(&m_defaultValue));

	return MSV_SUCCESS;
}

MsvErrorCode MsvDefaultValue::GetDefaultValue(double& defaultValue) const
{
	if (!IsDouble())
	{
		return MSV_INVALID_DATA_ERROR;
	}

	defaultValue = *(std::any_cast<double>(&m_defaultValue));

	return MSV_SUCCESS;
}

MsvErrorCode MsvDefaultValue::GetDefaultValue(int64_t& defaultValue) const
{
	if (!IsInteger())
	{
		return MSV_INVALID_DATA_ERROR;
	}

	defaultValue = *(std::any_cast<int64_t>(&m_defaultValue));

	return MSV_SUCCESS;
}

MsvErrorCode MsvDefaultValue::GetDefaultValue(std::string& defaultValue) const
{
	if (!IsString())
	{
		return MSV_INVALID_DATA_ERROR;
	}

	defaultValue = *(std::any_cast<std::string>(&m_defaultValue));

	return MSV_SUCCESS;
}

MsvErrorCode MsvDefaultValue::GetDefaultValue(uint64_t& defaultValue) const
{
	if (!IsUnsigned())
	{
		return MSV_INVALID_DATA_ERROR;
	}

	defaultValue = *(std::any_cast<uint64_t>(&m_defaultValue));

	return MSV_SUCCESS;
}

bool MsvDefaultValue::IsBool() const
{
	return m_defaultValue.type() == typeid(bool);
}

bool MsvDefaultValue::IsDouble() const
{
	return m_defaultValue.type() == typeid(double);
}

bool MsvDefaultValue::IsInteger() const
{
	return m_defaultValue.type() == typeid(int64_t);
}

bool MsvDefaultValue::IsString() const
{
	return m_defaultValue.type() == typeid(std::string);
}

bool MsvDefaultValue::IsUnsigned() const
{
	return m_defaultValue.type() == typeid(uint64_t);
}


/** @} */	//End of group MCONFIG.
