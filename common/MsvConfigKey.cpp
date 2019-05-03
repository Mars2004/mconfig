/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Config Key Implementation
* @details		Contains implementation of @ref MsvConfigKey.
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


#include "MsvConfigKey.h"
#include "merror/MsvErrorCodes.h"


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvConfigKey::MsvConfigKey(const char* group, const char* key, std::shared_ptr<IMsvDefaultValue> spDefaultValue):
	m_group(group),
	m_key(key),
	m_spDefaultValue(spDefaultValue)
{

}

MsvConfigKey::~MsvConfigKey()
{
}


/********************************************************************************************************************************
*															IMsvConfigKey public methods
********************************************************************************************************************************/


MsvErrorCode MsvConfigKey::GetData(std::string& group, std::string& key, bool& defaultValue) const
{
	return GetData<bool>(group, key, defaultValue);
}

MsvErrorCode MsvConfigKey::GetData(std::string& group, std::string& key, double& defaultValue) const
{
	return GetData<double>(group, key, defaultValue);
}

MsvErrorCode MsvConfigKey::GetData(std::string& group, std::string& key, int64_t& defaultValue) const
{
	return GetData<int64_t>(group, key, defaultValue);
}

MsvErrorCode MsvConfigKey::GetData(std::string& group, std::string& key, std::string& defaultValue) const
{
	return GetData<std::string>(group, key, defaultValue);
}

MsvErrorCode MsvConfigKey::GetData(std::string& group, std::string& key, uint64_t& defaultValue) const
{
	return GetData<uint64_t>(group, key, defaultValue);
}

bool MsvConfigKey::IsBool() const
{
	return m_spDefaultValue->IsBool();
}

bool MsvConfigKey::IsDouble() const
{
	return m_spDefaultValue->IsDouble();
}

bool MsvConfigKey::IsInteger() const
{
	return m_spDefaultValue->IsInteger();
}

bool MsvConfigKey::IsString() const
{
	return m_spDefaultValue->IsString();
}

bool MsvConfigKey::IsUnsigned() const
{
	return m_spDefaultValue->IsUnsigned();
}


/********************************************************************************************************************************
*															MsvConfigKey protected methods
********************************************************************************************************************************/


template<class T> MsvErrorCode MsvConfigKey::GetData(std::string& group, std::string& key, T& defaultValue) const
{
	group = m_group;
	key = m_key;
	return m_spDefaultValue->GetDefaultValue(defaultValue);
}


/** @} */	//End of group MCONFIG.
