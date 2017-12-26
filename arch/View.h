#pragma once

#include "Map.h"
#include "General.h"
#include "IGetData.h"


namespace game_module
{
	class View
	{
	private:
		Map PreviousState;
		IGetData* MyGetData;
	public:
		~View();
		View();
		View(IGetData* getData);
		void show(const Map & newState, const Result & results);
	};
}

