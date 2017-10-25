#pragma once

#include "General.h"
#include "Controller.h"
#include "Unit.h"


namespace game_module
{

	class Player 
	{
	private:
		size_type Index; // индекс игрока
		std::string Name; // имя игрока

		std::map<size_type, pair> Capitals; // столицы игрока

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
		virtual ~Player();
		Player(const size_type & pl_index, const std::string pl_name,
			Controller * game_controller);
		//


		// получение полей класса
		size_type index() const;
		std::string name() const;
		//

		pair get_capital(size_type capital_index) const;
		size_type get_capital_numbers() const;
		std::map<size_type, pair> get_capitals() const;

		bool operator == (const Player & player) const;

		virtual void turn() = 0;

	private:
		void add_capital(Capital * capital);
		bool remove_capital(size_type capital_index);

		friend class Game;
	};

	bool operator != (const Player & player1, const Player & player2);

}
