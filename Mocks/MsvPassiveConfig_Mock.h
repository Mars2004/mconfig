

#ifndef MARSTECH_PASSIVECONFIG_MOCK_H
#define MARSTECH_PASSIVECONFIG_MOCK_H


#include "mconfig/mpassivecfg/IMsvPassiveConfig.h"

#include <gmock/gmock.h>


class MsvPassiveConfig_Mock:
	public IMsvPassiveConfig
{
public:
	MOCK_METHOD3(Initialize, MsvErrorCode(std::shared_ptr<IMsvConfigKeyMap<IMsvConfigKey>> spConfigKeyMap, const char* configPath));

	MOCK_CONST_METHOD2(GetValue, MsvErrorCode(int32_t cfgId, bool& value));
	MOCK_CONST_METHOD2(GetValue, MsvErrorCode(int32_t cfgId, double& value));
	MOCK_CONST_METHOD2(GetValue, MsvErrorCode(int32_t cfgId, int64_t& value));
	MOCK_CONST_METHOD2(GetValue, MsvErrorCode(int32_t cfgId, std::string& value));
	MOCK_CONST_METHOD2(GetValue, MsvErrorCode(int32_t cfgId, uint64_t& value));

	MOCK_CONST_METHOD2(ReadFailedData, void(int32_t& lineNumber, int32_t& cfgId));
	MOCK_METHOD0(ReloadConfiguration, MsvErrorCode());
};


#endif // MARSTECH_PASSIVECONFIG_MOCK_H
