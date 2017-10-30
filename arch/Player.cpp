#include "Player.h"





namespace game_module
{

	Player::Player(
		const size_type & pl_index, const std::string pl_name,
		Controller * game_controller
	)
		: Index(pl_index)
		, Name(pl_name)
		, GameController(game_controller)
	{ }


	size_type Player::index() const
	{
		return Index;
	}

	std::string Player::name() const
	{
		return Name;
	}

	size_type Player::get_capitals_number() const
	{
		return Capitals.size();
	}

	std::list<Pair> Player::get_capitals() const
	{
		return Capitals;
	}

	bool Player::operator == (const Player & player) const
	{
		return (Index == player.index());
	}


	void Player::add_capital(const Pair & capital)
	{
		Capitals.push_back(capital);
	}

	bool Player::remove_capital(const Pair & capital)
	{
		for (auto i = Capitals.begin(); i != Capitals.end(); ++i)
		{
			if (*i == capital) {
				Capitals.erase(i);
				return true;
			}
		}

		return false;
	}


	bool operator != (const Player & player1, const Player & player2)
	{
		return !(player1 == player2);
	}

}
