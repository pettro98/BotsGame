#pragma once

#include "IGetData.h"
#include "ISetData.h"


namespace game_module
{
	/*!
	\brief Пустой класс объединяет методы классов IGetData и ISetData.
	*/
	class IFullAccess
		: public IGetData
		, public ISetData
	{};
}
