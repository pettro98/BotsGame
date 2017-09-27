#include <map>




// класс крайне сырой, нужно добавить кучу методов для обеспечения работы API функций из TPlayer
// определиться с результатами игры:
// 1) какие результаты на интересуют?
// 2) какие разбаловки?
// 3) функции для их возврата
// реализация метода bool make_players() под вопросом


namespace game_module 
{

	class TGame {

		using size_type = short;

		std::map<size_type, TPlayer> * players; // игроки, учавствующие в игре и их индексы
		TMap * map; // указатель на карту
		size_type current_turn; // текущий ход
		size_type max_turns; // количество ходов, через которое игра прекратится автоматически

	public:

		~TGame();
		TGame(size_type max_trs, size_type map_dimension, size_type map_type,
			std::map<size_type, TPlayer> * plrs = nullptr);
		// при создании игры она сразу же и начинается

		// базовые конструкторы, операторы // все ли нужны?
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

	private:

		// вспомогательные методы

		bool make_players(); // создает объекты TPlayer в случае если игроков не передали в конструктор
		// вызывается в кострукторе

		void remove_player(size_type player_index); // исключает игрока из игры

		// 


	};

}
