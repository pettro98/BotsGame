

namespace game_module
{


	class TUnit
	{

	protected:

		THex * hex; // гекс на котором юнит находится, юнит принадлежит владельцу гекса

	public:

		virtual ~TUnit();
		TUnit(THex * phex); // основной конструктор

		// получение информации о юните
		pair get_coordinates() const;
		size_type get_index() const;
		
		virtual size_type get_strength() = 0; 
		virtual size_type get_type() = 0; // возвращает тип юнита
		//

		// функция строгого сравнения сил двух юнитов
		bool battle(THex * phex) const; // возращает true если на клетке никого нет
		bool battle(const THex & phex) const; 
		bool battle(pair hex_coord) const;
		bool battle(TUnit * unit) const;
		bool battle(const TUnit & unit) const;
		bool battle(size_type strng) const;
		//

	private:

		void change_hex(THex * phex); // удаляет также юнита в гексе назначения, ибо нефиг
		// удаление юнита реализованно через гекс

		friend class TGame;

	};



	class TActiveUnit
		: public TUnit
	{

	protected:

		size_type strength;

	public:

		~TActiveUnit();
		TActiveUnit(THex * phex, size_type strng);
		size_type get_strength() const;

	};


	class TArmy
		: public TActiveUnit
	{
		
		~TArmy();
		TArmy(THex * phex, size_type strng);
		static size_type get_type();
		static size_type get_move_points();
		size_type get_payment() const;

	private:

		void change_strength(size_type strng);
		size_type operator ++(); // ++strength, если возможно
		void die(); // создает могилу на гексе, применяется при нехватке снабжения

		friend class TGame;
		friend class THex;

	};


	struct TTower
		: public TActiveUnit
	{
		~TTower();
		TTower(THex * phex, size_type strng);
		static size_type get_type();

	};


	class TCapital
		: public TActiveUnit
	{

		size_type district_index;
		size_type district_money;

	public:

		~TCapital();
		TCapital(THex * phex);
		TCapital(TCapital * capital);

		static size_type get_type();
		size_type get_district_index() const;
		size_type get_district_money() const;

	private:

		void change_district_index(size_type rhs);
		void change_district_money(size_type rhs); //прибавляет rhs, но нужна защита от переполнения

		friend class TGame;

	};



	struct TPassiveUnit
		:public TUnit
	{
		~TPassiveUnit();
		TPassiveUnit(THex * phex);
		static size_type get_strength(); // возвращает 0
	};


	struct TFarm
		: public TPassiveUnit
	{
		~TFarm();
		TFarm(THex * phex);
		static size_type get_type();
		static size_type get_income();
	};


	class TTree
		: public TPassiveUnit
	{
	
	protected:

		size_type turns_from_double;

	public:

		~TTree();
		TTree(THex * phex);
		bool ready_to_double() const;

	private:

		void has_doubled(); // обнуляет turns_from_double
		void operator ++(); // ++turns_from_double

		friend class TGame;

	};


	struct TPalm
		: public TTree
	{

		~TPalm();
		TPalm(THex * phex);
		static size_type turns_to_double(); // возвращает срок за который размножается данный тип
		static size_type get_type();

	};


	struct TPine
		: public TTree
	{

		~TPine();
		TPine(THex * phex);
		static size_type turns_to_double(); // возвращает срок за который размножается данный тип
		static size_type get_type();

	};


	struct Grave
		: public TPassiveUnit
	{
		~Grave();
		Grave(THex * phex);
		static size_type get_type();

	};


}
