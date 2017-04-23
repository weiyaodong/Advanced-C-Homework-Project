#pragma once

typedef int size_type;

#define Call(VAR_NAME, FUNC_NAME, ...) FUNC_NAME(*(VAR_NAME),__VA_ARGS__)

struct String
{
	size_type length;
	
};
