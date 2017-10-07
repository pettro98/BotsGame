#include <map>


namespace game_module 
{

	using size_type = short;
	using pair = std::pair<size_type, size_type>;
	using delta_type = std::pair<pair, pair>;


	struct Results { }; // структура результатов игры


	class TGame {

		std::map<size_type, TPlayer> * players; // игроки, учавствующие в игре и их индексы
		TMap * map; // указатель на карту
		size_type current_turn; // текущий ход
		size_type max_turns; // количество ходов, через которое игра прекратится автоматически
		Results result; // результаты соревнования, обновляются по ходу игры

	public:

		~TGame();
		TGame(size_type max_trs, size_type map_dimension, size_type map_type,
			std::map<size_type, TPlayer> * plrs = nullptr);

		// базовые конструкторы, операторы 
		TGame();
		TGame(TGame && game);
		TGame & operator = (TGame && game);
		TGame(const TGame & game);
		TGame & operator = (const TGame & game);
		//

		// получение полей класса 
		size_type get_current_turn() const;
		size_type get_max_turns() const;
		//

		Results start_game(); // проводит игру
		bool check_end_game(); // проверяет не закончилась ли игра


	private:

		
		// вспомогательные методы API

		bool make_move(pair start, pair end); 
		bool buy_army(pair hex);
		bool buy_tower(pair hex, size_type strength);
		bool buy_farm(pair hex);

		//

		bool make_players(); // создает объекты TPlayer в случае если игроков не передали в конструктор
		// вызывается в кострукторе

		void remove_player(size_type player_index); // исключает игрока из игры 

		// 


	};

}
