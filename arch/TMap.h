

namespace game_module
{


	class TMap
	{ 

		THex ** Root; // указатель на двумерный массив гексов
		size_type Dimension; // размерность карты
		std::string Map_type; // тип карты

	public:

		~TMap(); // вызывает метод clear
		TMap(size_type dimension,
			size_type player_number,
			const std::string & map_type);
		TMap(const TMap & map);
		TMap & operator = (const TMap & map);

		// базовые конструкторы, операторы
		TMap() = delete;
		TMap(TMap && map) = delete ;
		TMap & operator = (TMap && map) = delete;
		//

		size_type dimension() const;
		size_type map_type() const;

	private:

		THex & operator () (const pair & coord); // выдаёт гекс
		THex & operator () (size_type coord_1, size_type coord_2); // выдаёт гекс

		void generate_map( // вспомогательный метод, генерирующий карту, вызывается в конструкторе
			size_type dimension = 10, size_type player_number = 2,  
			const std::string & map_type = "classic"
		); 

		void clear(); // очищает карту, в том числе удаляет всех юнитов на ней

		friend class TGame;

	};




	class THex
	{

		pair Coordinates; // координаты гекса
		size_type Index; // индекс владельца
		TUnit * Unit; // указатель на юнит в гексе 
		TCapital * Capital; // указатель на столицу области


	public:


		~THex();
		THex(THex * phex);
		THex(size_type coord_1 ,size_type coord_2);
		THex & operator = (const THex & hex);
	

		// получение полей класса
		pair coordinates() const;
		size_type index() const;
		//

		bool occupied() const; // наличие юнита в гексе

		bool operator == (const THex & hex) const;

	private:

		// изменение полей класса 
		void change_index(size_type new_index);
		void change_capital(TCapital * new_capital); // заменяет указатель capital
		bool create_unit(TUnit * unit); // помещает нового юнита в гекс если он не занят
		void remove_unit(); // разыменовывает указатель unit
		void delete_unit(); // вызывает delete для указателя unit и разыменовывает его
		void change_unit(TUnit * unit); // помещает нового юнита в гекс, старого удаляет
		//

		// получение полей класса
		TUnit * get_unit();
		TCapital * get_capital();
		//

		friend class TMap;
		friend class TGame;

	};

	bool operator != (const THex & hex1, const THex & hex2);
	size_type distance(const THex & hex1, const THex & hex2);

}
