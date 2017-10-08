#include <vector>
#include <list>

namespace game_module
{

	class TPlayer {

		size_type Index; // индекс игрока
		std::string Name; // имя игрока
		void(*Turn_function)(const TPlayer & player); // функция хода игрока

		bool Moved; // показывает, ходил ли игрок в этом туре
		bool In_game; // показывает в игре ли участник

		std::list<THex> * Armies; // армии игрока, вектор собирается заново в начале хода
		// при передвижении юнита он убирается из списка
		std::map<size_type, TCapital> Capitals; // столицы игрока

		TGame * Game; // указатель на текущую игру

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

		size_type index() const;
		std::string name() const;

		bool moved() const;
		bool in_game() const;

		//

		bool check_in_game() const; // проверяет наличие столиц и делает вывод в игре ли участник

		bool operator == (const TPlayer & player) const;

	private:

		void make_a_turn(); // функция вызова turn_function - функции хода игрока

		// меняем поля класса
		void change_moved();
		void change_in_game();

		void build_army_list(); // в начале хода добаляет всех юнитов игрока в лист
		void add_army(THex * army_hex);
		bool remove_army(THex * army_hex);
		bool remove_army(pair army_hex);

		void add_capital(TCapital * capital);
		bool remove_capital(TCapital * capital);
		bool remove_capital(size_type capital_index);

		void change_game(TGame * game_pointer); // меняем указатель на игру при создании новой
		//

		TCapital * get_capital(size_type capital_index); 
		std::map<size_type, TCapital> * get_capitals(); 

	public:

		// API

		// инфо

		size_type max_turns() const; 
		size_type current_turn() const; 

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

		static std::vector<pair> get_hex_neighbors(pair hex); //
		// возвращает координаты соседей гекса
	
		size_type hex_index(size_type coord_1, size_type coord_2) const; //
		size_type hex_index(pair) const; //
		//возвращает индекс владельца гекса

		std::list<THex> army_list() const;

		std::vector<pair> get_farms() const; 
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


	bool operator != (const TPlayer & player1, const TPlayer & player2);


}
