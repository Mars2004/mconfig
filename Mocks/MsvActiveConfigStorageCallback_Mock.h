

#ifndef MARSTECH_ACTIVECONFIGSTORAGECALLBACK_MOCK_H
#define MARSTECH_ACTIVECONFIGSTORAGECALLBACK_MOCK_H


#include "mconfig/mactivecfg/IMsvActiveConfigStorageCallback.h"

#include <gmock/gmock.h>


class MsvActiveConfigStorageCallback_Mock:
	public IMsvActiveConfigStorageCallback
{
public:
	MOCK_METHOD2(OnValueChanged, void(int32_t, bool));
	MOCK_METHOD2(OnValueChanged, void(int32_t, double));
	MOCK_METHOD2(OnValueChanged, void(int32_t, int64_t));
	MOCK_METHOD2(OnValueChanged, void(int32_t, const char*));
	MOCK_METHOD2(OnValueChanged, void(int32_t, uint64_t));
};


#endif // MARSTECH_ACTIVECONFIGSTORAGECALLBACK_MOCK_H
