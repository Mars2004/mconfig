

#ifndef MARSTECH_CONFIGKEYMAP_MOCK_H
#define MARSTECH_CONFIGKEYMAP_MOCK_H


#include "mconfig/common/IMsvConfigKeyMap.h"

#include <gmock/gmock.h>


template<class T> class MsvConfigKeyMap_Mock:
	public IMsvConfigKeyMap<T>
{
public:
	MOCK_CONST_METHOD0(GetMap, const std::map<int32_t, std::shared_ptr<T>>&());
};


#endif // MARSTECH_CONFIGKEYMAP_MOCK_H
