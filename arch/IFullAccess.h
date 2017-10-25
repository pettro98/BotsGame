#pragma once

#include "IGetData.h"
#include "ISetData.h"


namespace game_module
{

	class IFullAccess
		: public IGetData
		, public ISetData
	{ };

}
