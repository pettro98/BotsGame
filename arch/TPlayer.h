#include <vector>




// начал этого класса, так как считаю, что надо распутывать клубок с конца
// его вид напрямую влияет на все остальные классы


namespace game_module
{

	class TPlayer {

		using size_type = short;
		using pair = std::pair<size_type, size_type>;
		using alpha_type = std::pair<std::pair<size_type, size_type>, std::pair<size_type, size_type>>;

		size_type index; // индекс игрока
		std::string name; // имя игрока
		bool moved; // показывает, ходил ли игрок в этом туре
		void(*turn_function)(const TPlayer & player); // функция хода игрока
		std::map<size_type, TCapital> capitals;
		TGame * game;

	public:

		// базовые конструкторы, операторы // пожалуй, нафиг не нужны? единичные экземпляры в TGame
		// при создании новой игры будем перекидывать указатели игроков в новую игру 
		// (важно при передаче обнулять предыдущие указатели перед деструктором)
		TPlayer() = delete;
		TPlayer(TPlayer &&);
		TPlayer & operator = (TPlayer &&);
		TPlayer(const TPlayer &);
		TPlayer & operator = (const TPlayer &);
		//

		// основные деструктор, конструктор
		~TPlayer();
		TPlayer(const size_type & pl_index, const std::string pl_name, void(*pl_turn_function)(), TGame * game_pointer);
		//


		// получение полей класса
		size_type get_index() const;
		std::string get_name() const;
		bool get_moved() const;
		TCapital get_capital(size_type capital_index) const;
		std::map<size_type, TCapital> get_capitals() const;
		size_type get_max_turns() const; // возвращает максимальное кол-во ходов в данной игре
		size_type get_current_turn() const; // возвращает номер текущего хода
		//


	private:

		void make_a_turn(); // функция вызова turn_function - функции хода игрока, проверяет moved
		// вызов из TGame

		// меняем поля класса
		void change_moved();
		bool remove_capital(TCapital * capital);
		bool remove_capital(size_type capital_index);
		bool add_capital(TCapital * capital);
		bool add_capital(size_type capital_index);
		void change_game(TGame * game_pointer); // меняем указатель на игру при создании новой
		//

	public:

		// API

		bool can_make_move(size_type scoord_1, size_type scoord_2, size_type ecoord_1, size_type ecoord_2) const; //
		bool can_make_move(size_type scoord_1, size_type scoord_2, pair end) const; //
		bool can_make_move(pair start, size_type ecoord_1, size_type ecoord_2) const; //
		bool can_make_move(pair start, pair end) const; //
		bool can_make_move(alpha_type alpha) const; // нужна ли?
		// проверяет, возможен ли ход

		bool make_move(size_type scoord_1, size_type scoord_2, size_type ecoord_1, size_type ecoord_2); //
		bool make_move(size_type scoord_1, size_type scoord_2, pair end); //
		bool make_move(pair start, size_type ecoord_1, size_type ecoord_2); //
		bool make_move(pair start, pair end); //
		bool make_move(alpha_type alpha); // нужна ли?
		// совершает ход, если он возможен, возвращает true, в случае успеха


		// функции с альфами и соседями вероятно бесполезны
		alpha_type get_player_alpha(size_type player_index) const; //возвращает последнюю альфу игрока с индексом player_index
		std::vector<alpha_type> get_player_alpha(size_type player_index, size_type turns_num) const; //
		// возвращает последние turns_num альф игрока с индексом player_index

		std::vector<alpha_type> get_alpha() const; // возвращает все альфы прошедшие с последнего хода данного игрока
		alpha_type get_alpha(size_type turn) const; // возвращает альфу хода под номером turn
		//если этого хода ещё не было возвращает что-то типа ((0,0)(0,0))?


		size_type neighbors_number(size_type coord_1, size_type coord_2) const; //
		size_type neighbors_number(pair hex) const; //
		//возвращает кол-во соседей гекса с индексом не игрока

		std::vector<pair> get_hex_neighbors(size_type coord_1, size_type coord_2) const; //
		std::vector<pair> get_hex_neighbors(pair hex) const; //
		// возвращает координаты соседей гекса
		//

		

		size_type get_hex_index(size_type coord_1, size_type coord_2) const; //
		size_type get_hex_index(pair) const; //
		//возвращает индекс владельца гекса

		std::vector<pair> get_player_hexs() const; // возращает координаты всех свободных гексов
		std::vector<pair> get_player_hexs(size_type player_index) const;
		// возвращает координаты гексов игрока с индексом player_index

		std::vector<size_type> get_players_indexes() const; 
		// возвращает индексы всех игроков в игре

		
		//


		friend class TGame;

	};

}
