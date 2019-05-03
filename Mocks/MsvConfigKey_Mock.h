

#ifndef MARSTECH_CONFIGKEY_MOCK_H
#define MARSTECH_CONFIGKEY_MOCK_H


#include "mconfig/common/IMsvConfigKey.h"

#include <gmock/gmock.h>


class MsvConfigKey_Mock:
	public IMsvConfigKey
{
public:
	
	MOCK_CONST_METHOD3(GetData, MsvErrorCode(std::string& group, std::string& key, bool& defaultValue));
	MOCK_CONST_METHOD3(GetData, MsvErrorCode(std::string& group, std::string& key, double& defaultValue));
	MOCK_CONST_METHOD3(GetData, MsvErrorCode(std::string& group, std::string& key, int64_t& defaultValue));
	MOCK_CONST_METHOD3(GetData, MsvErrorCode(std::string& group, std::string& key, std::string& defaultValue));
	MOCK_CONST_METHOD3(GetData, MsvErrorCode(std::string& group, std::string& key, uint64_t& defaultValue));

	MOCK_CONST_METHOD0(IsBool, bool());
	MOCK_CONST_METHOD0(IsDouble, bool());
	MOCK_CONST_METHOD0(IsInteger, bool());
	MOCK_CONST_METHOD0(IsString, bool());
	MOCK_CONST_METHOD0(IsUnsigned, bool());
};


#endif // MARSTECH_CONFIGKEY_MOCK_H
