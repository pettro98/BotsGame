#include <vector>


namespace game_module
{

	class TPlayer {

		size_type index; // индекс игрока
		std::string name; // имя игрока
		void(*turn_function)(const TPlayer & player); // функция хода игрока

		bool moved; // показывает, ходил ли игрок в этом туре
		bool in_game; // показывает в игре ли участник

		std::map<size_type, TCapital> capitals; // столицы игрока

		TGame * game; // указатель на текущую игру

	public:

		// базовые конструкторы, операторы 
		TPlayer() = delete;
		TPlayer(TPlayer &&);
		TPlayer & operator = (TPlayer &&);
		TPlayer(const TPlayer &);
		TPlayer & operator = (const TPlayer &);
		//

		// основные деструктор, конструктор
		~TPlayer();
		TPlayer(const size_type & pl_index, const std::string pl_name,
			void(*pl_turn_function)(), TGame * game_pointer);
		//


		// получение полей класса

		size_type get_index() const;
		std::string get_name() const;

		bool get_moved() const;
		bool get_in_game() const;

		//

		bool check_in_game() const; // проверяет наличие столиц и делает вывод в игре ли участник

	private:

		void make_a_turn(); // функция вызова turn_function - функции хода игрока

		// меняем поля класса
		void change_moved();
		void change_in_game();

		bool remove_capital(TCapital * capital);
		bool remove_capital(size_type capital_index);
		bool add_capital(TCapital * capital);

		void change_game(TGame * game_pointer); // меняем указатель на игру при создании новой
		//

		TCapital * get_capital(size_type capital_index); 
		std::map<size_type, TCapital> * get_capitals(); 

	public:

		// API

		// инфо, я хз

		size_type get_max_turns() const; 
		size_type get_current_turn() const; 

		bool can_make_move(size_type scoord_1, size_type scoord_2,
			size_type ecoord_1, size_type ecoord_2) const; //
		bool can_make_move(size_type scoord_1, size_type scoord_2, pair end) const; //
		bool can_make_move(pair start, size_type ecoord_1, size_type ecoord_2) const; //
		bool can_make_move(pair start, pair end) const; //
		// проверяет, возможен ли ход
		
		size_type get_index(pair hex_coord) const; 
		size_type get_capital_district_index(pair hex_coord) const;
		size_type get_capital_district_money(pair hex_coord) const;
		size_type get_hex_strength() const;
		// получение полей гекса с координатами hex_coord

		static std::vector<pair> get_hex_neighbors(size_type coord_1, size_type coord_2); //
		static std::vector<pair> get_hex_neighbors(pair hex); //
		// возвращает координаты соседей гекса
	
		size_type get_hex_index(size_type coord_1, size_type coord_2) const; //
		size_type get_hex_index(pair) const; //
		//возвращает индекс владельца гекса

		std::vector<pair> get_farms(); 
		// координаты всех гексов с фермами данного игрока

		std::vector<pair> get_player_hexs(size_type player_index) const;
		// возвращает координаты гексов игрока с индексом player_index

		std::vector<size_type> get_players_indexes() const; 
		// возвращает индексы всех игроков в игре

		//


		// Действия
		

		bool make_move(size_type scoord_1, size_type scoord_2,
			size_type ecoord_1, size_type ecoord_2); //
		bool make_move(size_type scoord_1, size_type scoord_2, pair end); //
		bool make_move(pair start, size_type ecoord_1, size_type ecoord_2); //
		bool make_move(pair start, pair end); //
		// совершает ход, если он возможен

		bool buy_army(pair hex); 
		// создает(добавляет) армию силы 1 в гексе(армию), если это возможно

		bool buy_tower(pair hex, size_type strength);
		// создаёт башню силы strength в клетке, если это возможно

		bool buy_farm(pair hex);
		// создаёт ферму в клетке, если это возможно
		
		//

		//

		friend class TGame;

	};

}
