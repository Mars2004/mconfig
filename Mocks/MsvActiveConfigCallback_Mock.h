

#ifndef MARSTECH_ACTIVECONFIGCALLBACK_MOCK_H
#define MARSTECH_ACTIVECONFIGCALLBACK_MOCK_H


#include "mconfig/mactivecfg/IMsvActiveConfigCallback.h"

#include <gmock/gmock.h>


class MsvActiveConfigCallback_Mock:
	public IMsvActiveConfigCallback
{
public:
	MOCK_METHOD2(OnValueChanged, void(int32_t, bool));
	MOCK_METHOD2(OnValueChanged, void(int32_t, double));
	MOCK_METHOD2(OnValueChanged, void(int32_t, int64_t));
	MOCK_METHOD2(OnValueChanged, void(int32_t, const char*));
	MOCK_METHOD2(OnValueChanged, void(int32_t, uint64_t));
};


#endif // MARSTECH_ACTIVECONFIGCALLBACK_MOCK_H
