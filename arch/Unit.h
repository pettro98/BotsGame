#pragma once

#include "Hex.h"



namespace game_module
{

	class Hex;

	class Unit
	{

	protected:

		Hex * Field; // гекс на котором юнит находится, юнит принадлежит владельцу гекса

	public:

		virtual ~Unit();

		Unit(Hex * hex);

		Pair coordinates() const;

		size_type index() const;

		virtual size_type strength() const = 0;
		virtual unit_type type() const = 0; 

		void set_hex(Hex * hex);

	};



	Unit * unit_factory(unit_type type, Hex * hex, size_type strength = 0);


	class ActiveUnit
		: public Unit
	{

	protected:

		size_type Strength;

	public:

		virtual ~ActiveUnit() = default;

		ActiveUnit(Hex * hex, size_type strng);

		size_type strength() const;

	};


	class Army
		: public ActiveUnit
	{

	private:

		bool Moved;

	public:

		~Army() = default;

		Army(Hex * hex, size_type strng);

		unit_type type() const;
		
		bool moved() const;

		size_type cost() const;

		static size_type move_points();

		void set_strength(size_type strng);

		void set_moved(bool moved);

		void die(); // создает могилу на гексе, применяется при нехватке снабжения

	};


	struct Tower
		: public ActiveUnit
	{
		~Tower() = default;

		Tower(Hex * hex, size_type strng);

		unit_type type() const;

		size_type cost() const;

	};


	class Capital
		: public ActiveUnit
	{

		size_type DistrictIndex;
		size_type DistrictMoney;

	public:

		~Capital() = default;

		Capital(Hex * hex);

		unit_type type() const;

		size_type district_index() const;

		size_type district_money() const;

		void set_district_index(size_type new_index);

		bool change_district_money(int money);  // прибавляет money к DistrictMoney,
												// если получается меньше 0, приравнивает DistrictMoney к нулю
												// и возвращает false

	};



	struct PassiveUnit
		:public Unit
	{
		virtual ~PassiveUnit() = default;

		PassiveUnit(Hex * hex);

		size_type strength() const;

	};


	struct Farm
		: public PassiveUnit
	{
		~Farm() = default;

		Farm(Hex * hex);

		unit_type type() const;

		static size_type income();

	};


	class Tree
		: public PassiveUnit
	{

	protected:

		size_type TurnsFromDouble;

	public:

		virtual ~Tree() = default;

		Tree(Hex * hex);

		virtual size_type turns_to_double() const = 0;

		bool ready_to_double() const;

		void has_doubled(); // обнуляет turns_from_double

		void operator ++(); // ++turns_from_double

	};


	struct Palm
		: public Tree
	{

		~Palm() = default;

		Palm(Hex * hex);

		size_type turns_to_double() const; // возвращает срок за который размножается данный тип

		unit_type type() const;

	};


	struct Pine
		: public Tree
	{

		~Pine() = default;

		Pine(Hex * hex);

		size_type turns_to_double() const; // возвращает срок за который размножается данный тип

		unit_type type() const;
	};


	struct Grave
		: public PassiveUnit
	{
		~Grave() = default;

		Grave(Hex * hex);

		unit_type type() const;

	};


}
