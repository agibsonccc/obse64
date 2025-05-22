#include "GameScript.h"
#include "ParamInfos.h"

template <float (* Fn)(float)>
bool UnaryMathFn(COMMAND_ARGS)
{
	*result = 0;

	float arg = 0;
	if(!ExtractArgs(EXTRACT_ARGS, &arg))
		return true;

	*result = Fn(arg);

	return true;
}

template <float (*Fn)(float, float)>
bool BinaryMathFn(COMMAND_ARGS)
{
	*result = 0;

	float arg1 = 0, arg2 = 0;
	if(!ExtractArgs(EXTRACT_ARGS, &arg1, &arg2))
		return true;

	*result = Fn(arg1, arg2);

	return true;
}

CommandInfo kCommandInfo_SquareRoot =
{
	"SquareRoot", "sqrt",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <sqrtf>
};

CommandInfo kCommandInfo_Sin =
{
	"rSin", "rsin",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <sinf>
};

CommandInfo kCommandInfo_Cos =
{
	"rCos", "rcos",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <cosf>
};

CommandInfo kCommandInfo_Tan =
{
	"rTan", "rtan",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <tanf>
};

CommandInfo kCommandInfo_ASin =
{
	"rASin", "rasin",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <asinf>
};

CommandInfo kCommandInfo_ACos =
{
	"rACos", "racos",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <acosf>
};

CommandInfo kCommandInfo_ATan =
{
	"rATan", "ratan",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <atanf>
};

CommandInfo kCommandInfo_Log =
{
	"Log", "log",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <logf>
};

CommandInfo kCommandInfo_Exp =
{
	"Exp", "exp",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <expf>
};
