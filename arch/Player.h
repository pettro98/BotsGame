#pragma once

#include "General.h"
#include "Controller.h"
#include "Unit.h"


namespace game_module
{

	class Controller;

	class Player
	{
	private:
		size_type Index; // индекс игрока
		std::string Name; // имя игрока

		std::list<Pair> Capitals; // столицы игрока

		Controller * GameController;

	public:
		// базовые конструкторы, операторы 
		Player() = delete;
		Player(Player &&);
		Player & operator = (Player &&);
		Player(const Player &);
		Player & operator = (const Player &);
		//

		// основные деструктор, конструктор
		virtual ~Player() = default;
		Player(const size_type & pl_index, const std::string pl_name,
			Controller * game_controller);
		//


		// получение полей класса
		size_type index() const;
		std::string name() const;
		//

		size_type get_capitals_number() const;
		std::list<Pair> get_capitals() const;

		bool operator == (const Player & player) const;

		virtual void turn() = 0;

	private:
		void add_capital(const Pair & capital);
		bool remove_capital(const Pair & capital);

		friend class Game;
	};

	bool operator != (const Player & player1, const Player & player2);

}
