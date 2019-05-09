//
// pch.h
// Header for standard system include files.
//

#pragma once


#include "mlogging/MsvSpdLogLoggerProvider.h"

MSV_DISABLE_ALL_WARNINGS

#include "gtest/gtest.h"

MSV_ENABLE_WARNINGS


#ifndef MSV_TEST_WITH_LOGGING
#define MSV_TEST_WITH_LOGGING 0
#endif


enum class ConfigId: int32_t
{
	MSV_TEST_BOOL_1 = 0,
	MSV_TEST_BOOL_2,
	MSV_TEST_DOUBLE_1,
	MSV_TEST_DOUBLE_2,
	MSV_TEST_INTEGER_1,
	MSV_TEST_INTEGER_2,
	MSV_TEST_STRING_1,
	MSV_TEST_STRING_2,
	MSV_TEST_UNSIGNED_1,
	MSV_TEST_UNSIGNED_2
};
