

#ifndef MARSTECH_ACTIVECONFIG_MOCK_H
#define MARSTECH_ACTIVECONFIG_MOCK_H


#include "mconfig/mactivecfg/IMsvActiveConfig.h"

MSV_DISABLE_ALL_WARNINGS

#include <gmock\gmock.h>

MSV_ENABLE_WARNINGS


class MsvActiveConfig_Mock:
	public IMsvActiveConfig
{
public:
	MOCK_METHOD3(Initialize, MsvErrorCode(const std::shared_ptr<IMsvConfigKeyMap<IMsvDefaultValue>> spConfigKeyMap, const char* configPath, const char* tableName));
	MOCK_METHOD0(Uninitialize, MsvErrorCode());
	MOCK_CONST_METHOD0(Initialized, bool());

	MOCK_CONST_METHOD2(GetValue, MsvErrorCode(int32_t cfgId, bool& value));
	MOCK_CONST_METHOD2(GetValue, MsvErrorCode(int32_t cfgId, double& value));
	MOCK_CONST_METHOD2(GetValue, MsvErrorCode(int32_t cfgId, int64_t& value));
	MOCK_CONST_METHOD2(GetValue, MsvErrorCode(int32_t cfgId, std::string& value));
	MOCK_CONST_METHOD2(GetValue, MsvErrorCode(int32_t cfgId, uint64_t& value));

	MOCK_METHOD2(SetValue, MsvErrorCode(int32_t cfgId, bool value));
	MOCK_METHOD2(SetValue, MsvErrorCode(int32_t cfgId, double value));
	MOCK_METHOD2(SetValue, MsvErrorCode(int32_t cfgId, int64_t value));
	MOCK_METHOD2(SetValue, MsvErrorCode(int32_t cfgId, const std::string& value));
	MOCK_METHOD2(SetValue, MsvErrorCode(int32_t cfgId, uint64_t value));

	MOCK_METHOD1(RegisterCallback, MsvErrorCode(std::shared_ptr<IMsvActiveConfigCallback> spCallback));
	MOCK_METHOD1(UnregisterCallback, MsvErrorCode(std::shared_ptr<IMsvActiveConfigCallback> spCallback));
};


#endif // MARSTECH_ACTIVECONFIG_MOCK_H
