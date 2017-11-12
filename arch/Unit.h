#pragma once

#include "Hex.h"


namespace game_module
{

	class Unit
	{
	protected:
		Hex * Field; // гекс на котором юнит находится, юнит принадлежит владельцу гекса

	public:
		virtual ~Unit();
		Unit(Hex * hex); // основной конструктор

		// получение информации о юните
		pair coordinates() const;
		size_type index() const;
		
		virtual size_type strength() = 0; 
		virtual unit_type type() = 0; // возвращает тип юнита
		//

		bool operator == (const Unit & unit) const;

		void set_hex(Hex * hex); // удаляет также юнита в гексе назначения	
	};

	bool operator != (const Unit & unit1, const Unit & unit2);
	
	Unit * unit_factory(size_type type, size_type strength = 0);
	// в случае недопустимого значения силы возвращает nullptr
	

	class ActiveUnit
		: public Unit
	{
	protected:
		size_type Strength;

	public:
		virtual ~ActiveUnit();
		ActiveUnit(Hex * hex, size_type strng);
		size_type strength() const;
	};


	class Army
		: public ActiveUnit
	{
	private:
		bool Moved;
		
	public:
		~Army();
		Army(Hex * hex, size_type strng);
		unit_type type();
		bool moved() const;
		static size_type move_points();

		void set_strength(size_type strng);
		void set_moved(bool moved);
		void die(); // создает могилу на гексе, применяется при нехватке снабжения
	};


	class Tower
		: public ActiveUnit
	{
	public:
		~Tower();
		Tower(Hex * hex, size_type strng);
		unit_type type();
	};


	class Capital
		: public ActiveUnit
	{
	private:
		size_type DistrictIndex;
		size_type DistrictMoney;

	public:
		~Capital();
		Capital(Hex * hex);
		Capital(Capital * capital);

		unit_type type();
		size_type district_index() const;
		size_type district_money() const;

		void set_district_index(size_type new_index);
		bool change_district_money(int money);  // прибавляет money к DistrictMoney,
		// если получается меньше 0, приравнивает DistrictMoney к нулю
		// и возвращает false
	};


	class PassiveUnit
		:public Unit
	{
	public:
		virtual ~PassiveUnit();
		PassiveUnit(Hex * hex);
		size_type strength(); // возвращает 0
	};


	class Farm
		: public PassiveUnit
	{
	public:
		~Farm();
		Farm(Hex * hex);
		unit_type type();
		static size_type income();
	};


	class Tree
		: public PassiveUnit
	{
	protected:
		size_type TurnsFromDouble;

	public:
		virtual ~Tree();
		Tree(Hex * hex);
		bool ready_to_double() const;

		void has_doubled(); // обнуляет turns_from_double
		void operator ++(); // ++turns_from_double
	};


	class Palm
		: public Tree
	{
	public:
		~Palm();
		Palm(Hex * hex);
		static size_type turns_to_double(); // возвращает срок за который размножается данный тип
		unit_type type();

	};


	class Pine
		: public Tree
	{
	public:
		~Pine();
		Pine(Hex * hex);
		static size_type turns_to_double(); // возвращает срок за который размножается данный тип
		unit_type type();
	};


	class Grave
		: public PassiveUnit
	{
	public:
		~Grave();
		Grave(Hex * hex);
		unit_type type();
	};
	
}
