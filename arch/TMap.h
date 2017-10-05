





namespace game_module
{

	using size_type = short;
	using pair = std::pair<size_type, size_type>;

	class TMap
	{ 

		THex ** root; // указатель на двумерный массив гексов
		size_type dimension;
		std::string map_type;

	public:

		~TMap();
		TMap(size_type dimension, size_type player_number, const std::string & map_type);
		TMap(const TMap & map);
		TMap & operator = (const TMap & map);

		// базовые конструкторы, операторы
		TMap() = delete;
		TMap(TMap && map);
		TMap & operator = (TMap && map);
		//

		size_type get_index(const pair & coord) const; // выдаёт индекс гекса
		size_type get_index(size_type coord_1, size_type coord_2) const;
		size_type get_dimension() const;
		size_type get_map_type() const;

	private:

		THex & operator () (const pair & coord); // выдаёт гекс
		THex & operator () (size_type coord_1, size_type coord_2); // выдаёт гекс

		void generate_map( // вспомогательный метод, генерирующий карту, вызывается в конструкторе
			size_type dimension = 10, size_type player_number = 2,  // dimension - размерность карты,
			const std::string & map_type = "classic" // player_number - число игроков, map_typr - тип карты
		); 

	};




	class THex
	{

		pair coordinates; // координаты гекса
		size_type index; // индекс владельца
		TUnit * unit; // указатель на юнит в гексе 
		TCapital * capital; // указатель на столицу области


	public:


		~THex();
		THex(THex * phex);
		THex(size_type coord_1 ,size_type coord_2);
		THex & operator = (const THex & phex);
	

		// получение полей класса
		pair get_coordinates() const;
		size_type get_index() const;
		size_type get_capital_district_index() const;
		size_type get_capital_district_money() const;
		//

		bool occupied() const; // наличие юнита на гексе

	private:

		// изменение полей класса (координаты не меняются, они определяются при создании обьекта)
		void change_index(size_type new_index);
		void change_unit(TUnit * unit); // помещает нового юнита в гекс, старого удаляет
		bool create_unit(TUnit * unit); // помещает нового юнита в гекс если он не занят
		void remove_unit(); // разыменовывает указатель
		void delete_unit(); // вызывает delete для указателя
		//

		// получение полей класса
		TUnit * get_unit();
		TCapital * get_capital();
		//


		friend class TMap;
	};

}
