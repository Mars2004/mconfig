/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Config Key Map Base Implementation
* @details		Contains base implementation @ref MsvConfigKeyMapBase of @ref IMsvConfigKeyMap interface.
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


#ifndef MARSTECH_CONFIGKEYMAP_H
#define MARSTECH_CONFIGKEYMAP_H


#include "IMsvConfigKeyMap.h"

#include "merror/MsvErrorCodes.h"


/**************************************************************************************************//**
* @brief		MarsTech Config Key Map BaseImplementation.
* @details	Implementation for config key map interface.
* @note		This is helper class which can be used as config key data storage. Inserting config data
*				is allowed for its childrens.
* @see		IMsvConfigKeyMap
******************************************************************************************************/
template<class T> class MsvConfigKeyMapBase:
	public IMsvConfigKeyMap<T>
{
public:
	/**************************************************************************************************//**
	* @brief		Constructor.
	******************************************************************************************************/
	MsvConfigKeyMapBase()
	{

	}

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	~MsvConfigKeyMapBase()
	{
		m_keys.clear();
	}

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvConfigKeyMap public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvConfigKeyMap::GetMap() const
	******************************************************************************************************/
	virtual const std::map<int32_t, std::shared_ptr<T>>& GetMap() const override
	{
		return m_keys;
	}

	/*-----------------------------------------------------------------------------------------------------
	**											MsvConfigKeyMapBase protected methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @brief			Insert key data.
	* @details		Template method for inserting config keys data to map.
	* @note			This is helper method which can be used in child class constructor or initialize method
	*					for inserting config keys data.
	* @param[in]	cfgId		Config ID.
	* @param[in]	spData	Config key data.
	* @retval		MSV_ALREADY_EXISTS_ERROR	When config key is already in config key map.
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	MsvErrorCode InsertKeyData(int32_t cfgId, std::shared_ptr<T> spData)
	{
		if (m_keys.find(cfgId) != m_keys.end())
		{
			//key already exists
			return MSV_ALREADY_EXISTS_ERROR;
		}

		m_keys[cfgId] = spData;

		return MSV_SUCCESS;
	}

protected:
	/**************************************************************************************************//**
	* @brief			Config key map.
	* @details		Stores config keys data per config ID.
	* @see			GetMap
	******************************************************************************************************/
	std::map<int32_t, std::shared_ptr<T>> m_keys;
};


#endif // !MARSTECH_CONFIGKEYMAP_H

/** @} */	//End of group MCONFIG.
