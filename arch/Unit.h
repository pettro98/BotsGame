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
		virtual ~Unit() = default;
		Unit(Hex * hex);
		Pair coordinates() const;
		hex_color color() const;
		virtual size_type strength() const = 0;
		virtual unit_type type() const = 0; 
		virtual size_type cost() const = 0;
		void set_hex(Hex * hex);
	};

	Unit * unit_factory(unit_type type, size_type strength = 0);

	class ActiveUnit
		: public Unit
	{
	protected:
		size_type Strength;
	public:
		virtual ~ActiveUnit() = default;
		ActiveUnit(Hex * hex, size_type strength);
		size_type strength() const;
	};

	class Army
		: public ActiveUnit
	{
	private:
		bool Moved;
	public:
		~Army() = default;
		Army(Hex * hex, size_type strength);
		unit_type type() const;		
		size_type cost() const;
		static size_type income(size_type strength);
		bool moved() const;
		static size_type move_points();
		void set_strength(size_type strength);
		void set_moved(bool moved);
		void die(); // создает могилу на гексе, применяется при нехватке снабжения
	};

	struct Tower
		: public ActiveUnit
	{
		~Tower() = default;
		Tower(Hex * hex, size_type strength);
		unit_type type() const;
		size_type cost() const;
		static size_type income(size_type strength);
	};

	class Capital
		: public ActiveUnit
	{
	private:
		size_type DistrictMoney;
		size_type DistrictIncome;
		size_type FarmsNumber;
	public:
		~Capital() = default;
		Capital(Hex * hex);
		unit_type type() const;
		size_type cost() const;
		size_type district_money() const;
		size_type district_income() const;
		size_type farms_number() const;
		bool change_district_money(size_type money);
		void change_district_income(size_type income);
		void change_district_income(Hex * hex);
		void change_farms_number(size_type number = 1);
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
		size_type cost() const;
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
		size_type cost() const;
		static size_type income();
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
		size_type cost() const;
		static size_type income();
	};
}
