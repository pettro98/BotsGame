#include "General.h"


namespace game_module
{


		Pair::Pair(const Pair & pair)
			: First(pair.First)
			, Second(pair.Second)
		{ }

		Pair::Pair(size_type first, size_type second)
			: First(first)
			, Second(second)
		{ }

		bool Pair::operator == (const Pair & pair) const
		{
			return (First == pair.First && Second == pair.Second);
		}


	bool operator != (const Pair & pair1, const Pair & pair2)
	{
		return !(pair1 == pair2);
	}

	std::ostream & operator << (std::ostream & out, const Pair & pair)
	{
		out << "(" << pair.First << ", " << pair.Second << ")";
		return out;
	}

}
