/**************************************************************************************************//**
* @addtogroup	MCONFIG
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Active Config Storage Callback Interface
* @details		Contains interface of MarsTech Active Config Storage Callback.
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


#ifndef MARSTECH_IACTIVECONFIGSTORAGE_CALLBACK_H
#define MARSTECH_IACTIVECONFIGSTORAGE_CALLBACK_H


#include "merror/MsvError.h"


/**************************************************************************************************//**
* @brief		MarsTech Active Config Storage Callback Interface.
* @details	Interface for active configuration storage callback which notifies about data changes
*				in an active config storage.
* @warning	This callback works only with one instance of active config storage and it is not cross process.
*				Other processes won't be notified about changes in an active config storage. Same for more
*				instances of an active config storage in one process.
******************************************************************************************************/
class IMsvActiveConfigStorageCallback
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvActiveConfigStorageCallback() {}

	/**************************************************************************************************//**
	* @brief			Bool value has been changed.
	* @details		This method is called when bool value has been changed (callback must be registered before).
	* @param[in]	cfgId			Config ID of changed value.
	* @param[in]	newValue		New value, current value.
	******************************************************************************************************/
	virtual void OnValueChanged(int32_t cfgId, bool newValue) = 0;

	/**************************************************************************************************//**
	* @brief			Double value has been changed.
	* @details		This method is called when double value has been changed (callback must be registered before).
	* @param[in]	cfgId			Config ID of changed value.
	* @param[in]	newValue		New value, current value.
	******************************************************************************************************/
	virtual void OnValueChanged(int32_t cfgId, double newValue) = 0;

	/**************************************************************************************************//**
	* @brief			Integer value has been changed.
	* @details		This method is called when int64_t value has been changed (callback must be registered before).
	* @param[in]	cfgId			Config ID of changed value.
	* @param[in]	newValue		New value, current value.
	******************************************************************************************************/
	virtual void OnValueChanged(int32_t cfgId, int64_t newValue) = 0;

	/**************************************************************************************************//**
	* @brief			String value has been changed.
	* @details		This method is called when string value has been changed (callback must be registered before).
	* @param[in]	cfgId			Config ID of changed value.
	* @param[in]	newValue		New value, current value.
	******************************************************************************************************/
	virtual void OnValueChanged(int32_t cfgId, const char* newValue) = 0;

	/**************************************************************************************************//**
	* @brief			Unsigned integer value has been changed.
	* @details		This method is called when uint64_t value has been changed (callback must be registered before).
	* @param[in]	cfgId			Config ID of changed value.
	* @param[in]	newValue		New value, current value.
	******************************************************************************************************/
	virtual void OnValueChanged(int32_t cfgId, uint64_t newValue) = 0;
};


#endif // !MARSTECH_IACTIVECONFIGSTORAGE_CALLBACK_H

/** @} */	//End of group MCONFIG.
