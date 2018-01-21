#pragma once

#include "IGetData.h"
#include "ISetData.h"


namespace game_module
{
	/*!
	\brief ������ ����� ���������� ������ ������� IGetData � ISetData.
	*/
	class IFullAccess
		: public IGetData
		, public ISetData
	{};
}