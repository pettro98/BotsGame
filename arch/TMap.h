





namespace game_module
{

	class TMap { // класс карты гексов

		using size_type = short;

		THex ** root; // указатель на двумерный массив гексов

	public:

		// деструктор, основной конструктор
		~TMap();
		TMap(size_type dimension, size_type player_number, const std::string & map_type);
		//

		// базовые конструкторы, операторы // все ли нужны?
		TMap();
		TMap(TMap && map);
		TMap & operator = (TMap && map);
		TMap(const TMap & map);
		TMap & operator = (const TMap & map);
		//

	private:

		THex & operator () (size_type coord_1, size_type coord_2); // выдаёт гекс
		//size_type 

		size_type get_index(size_type coord_1, size_type coord_2) const; // выдаёт индекс гекса

		void generate_map( // вспомогательный метод, генерирующий карту, вызывается в конструкторе
			size_type dimension = 10, size_type player_number = 2,  // dimension - размерность карты,
			const std::string & map_type = "classic" // player_number - число игроков, map_typr - тип карты
		); 

	};




	class THex  // класс одного гекса (поля)
	{

		using size_type = short;

		size_type coordinate1; // координата 1
		size_type coordinate2; // координата 2
		size_type index; // индекс владельца

	public:

		// деструктор, конструктор
		~THex();
		THex(size_type coord_1 ,size_type coord_2);
		//

		// получение полей класса
		size_type get_coordinate1() const;
		size_type get_coordinate2() const;
		size_type get_index() const;
		//

	private:

		// изменение полей класса (координаты не меняются, они определяются при создании обьекта)
		void change_index(size_type);
		//

		friend class TMap;
	};

}
