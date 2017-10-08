

namespace game_module
{


	class TUnit
	{

	protected:

		THex * Hex; // гекс на котором юнит находится, юнит принадлежит владельцу гекса

	public:

		virtual ~TUnit();
		TUnit(THex * hex); // основной конструктор

		// получение информации о юните
		pair coordinates() const;
		size_type index() const;
		
		virtual size_type strength() = 0; 
		virtual size_type type() = 0; // возвращает тип юнита
		//

		// функция строгого сравнения сил двух юнитов
		bool battle(THex * hex) const; // возращает true если на клетке никого нет
		bool battle(const THex & hex) const; 
		bool battle(pair hex_coord) const;
		bool battle(TUnit * unit) const;
		bool battle(const TUnit & unit) const;
		bool battle(size_type strng) const;
		//

		bool operator == (const TUnit & unit) const;
		bool operator == (const THex & hex) const;

	private:

		void change_hex(THex * hex); // удаляет также юнита в гексе назначения, ибо нефиг
		// удаление юнита реализованно через гекс

		friend class TGame;

	};


	bool operator != (const TUnit & unit1, const TUnit & unit2);
	bool operator != (const TUnit & unit1, const THex & unit2);
	bool operator != (const THex & unit1, const TUnit & unit2);


	class TActiveUnit
		: public TUnit
	{

	protected:

		size_type Strength;

	public:

		~TActiveUnit();
		TActiveUnit(THex * hex, size_type strng);
		size_type strength() const;

	};


	class TArmy
		: public TActiveUnit
	{
		
		~TArmy();
		TArmy(THex * hex, size_type strng);
		static size_type type();
		static size_type move_points();
		size_type payment() const;

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
		TTower(THex * hex, size_type strng);
		static size_type type();

	};


	class TCapital
		: public TActiveUnit
	{

		size_type District_index;
		size_type District_money;

	public:

		~TCapital();
		TCapital(THex * hex);
		TCapital(TCapital * capital);

		static size_type type();
		size_type district_index() const;
		size_type district_money() const;

	private:

		void change_district_index(size_type rhs);
		void change_district_money(size_type rhs); //прибавляет rhs, но нужна защита от переполнения

		friend class TGame;

	};



	struct TPassiveUnit
		:public TUnit
	{
		~TPassiveUnit();
		TPassiveUnit(THex * hex);
		static size_type strength(); // возвращает 0
	};


	struct TFarm
		: public TPassiveUnit
	{
		~TFarm();
		TFarm(THex * hex);
		static size_type type();
		static size_type income();
	};


	class TTree
		: public TPassiveUnit
	{
	
	protected:

		size_type Turns_from_double;

	public:

		~TTree();
		TTree(THex * hex);
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
		TPalm(THex * hex);
		static size_type turns_to_double(); // возвращает срок за который размножается данный тип
		static size_type type();

	};


	struct TPine
		: public TTree
	{

		~TPine();
		TPine(THex * hex);
		static size_type turns_to_double(); // возвращает срок за который размножается данный тип
		static size_type type();

	};


	struct Grave
		: public TPassiveUnit
	{
		~Grave();
		Grave(THex * hex);
		static size_type type();

	};

	
}
