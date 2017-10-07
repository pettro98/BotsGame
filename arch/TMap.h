

namespace game_module
{


	class TMap
	{ 

		THex ** root; // указатель на двумерный массив гексов
		size_type dimension; // размерность карты
		std::string map_type; // тип карты

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

		size_type get_dimension() const;
		size_type get_map_type() const;

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
		//

		bool occupied() const; // наличие юнита в гексе

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

}
