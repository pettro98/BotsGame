#pragma once
#include "Unit.h"



namespace game_module
{


		Unit::~Unit()
		{
			delete Field;
		}

		Unit::Unit(Hex * hex)
			: Field(hex)
		{ }


		Pair Unit::coordinates() const
		{
			return Field->coordinates();
		}

		size_type Unit::index() const
		{
			return Field->index();
		}

		void Unit::set_hex(Hex * hex)
		{
			if (hex->occupied())
				hex->delete_hex_unit();
			Field = hex;
			hex->set_hex_unit(this);
		}

	


	//Unit * unit_factory(unit_type type, size_type strength = 0);
	// в случае недопустимого значения силы возвращает nullptr


		ActiveUnit::ActiveUnit(Hex * hex, size_type strng)
			: Unit(hex)
			, Strength(strng)
		{ }

		size_type  ActiveUnit::strength() const
		{
			return Strength;
		}


		Unit * unit_factory(unit_type type, Hex * hex, size_type strength)
		{

			Unit * result = nullptr;

			Unit * previous_unit = nullptr;
			if (hex->occupied())
				previous_unit = hex->get_hex_unit();


			switch (type)
			{
			case game_module::unit_type::none:
				break;
			case game_module::army:
			{
				if (strength >= 1 && strength <= 4)
					result = new Army(hex, strength);
				break;
			}
			case game_module::tower:
			{
				if (strength >= 1 && strength <= 2)
					result = new Tower(hex, strength);
				break;
			}
			case game_module::capital:
			{
				if (strength == 1)
					result = new Capital(hex);
				break;
			}
			case game_module::farm:
			{
				result = new Farm(hex);
				break;
			}
			case game_module::palm:
			{
				result = new Palm(hex);
				break;
			}
			case game_module::pine:
			{
				result = new Pine(hex);
				break;
			}
			case game_module::grave:
			{
				result = new Grave(hex);
				break;
			}
			default:
				break;
			}

			if (result)
				delete previous_unit;

			return result;

		}


		Army::Army(Hex * hex, size_type strng)
			: ActiveUnit(hex, strng)
		{ }

		unit_type Army::type() const
		{
			return army;
		}

		bool Army::moved() const
		{
			return Moved;
		}

		size_type Army::cost() const
		{
			return 2 * pow(3, Strength - 1);
		}

		size_type Army::move_points()
		{
			return 6;
		}

		void Army::set_strength(size_type strng)
		{
			Strength = strng;
		}

		void Army::set_moved(bool moved)
		{
			Moved = moved;
		}

		void Army::die() // создает могилу на гексе, применяется при нехватке снабжения
		{
			Grave * grave = new Grave(Field);
			Field->set_hex_unit(grave);
			this->~Army();
		}



		Tower::Tower(Hex * hex, size_type strng)
			: ActiveUnit(hex, strng)
		{ }

		unit_type Tower::type() const
		{
			return tower;
		}
		
		size_type Tower::cost() const
		{
			if (Strength == 1)
				return 2;
			return 7;
		}



		Capital::Capital(Hex * hex)
			: ActiveUnit(hex, 1)
		{ }

		unit_type Capital::type() const
		{
			return capital;
		}

		size_type Capital::district_index() const
		{
			return DistrictIndex;
		}

		size_type Capital::district_money() const
		{
			return DistrictMoney;
		}

		void Capital::set_district_index(size_type new_index)
		{
			DistrictIndex = new_index;
		}

		bool Capital::change_district_money(int money)  // прибавляет money к DistrictMoney,
											   // если получается меньше 0, приравнивает DistrictMoney к нулю
											   // и возвращает false
		{

			if (money < 0 && abs(money) > DistrictMoney) {
				DistrictMoney = 0;
				return false;
			}

			DistrictMoney += money;
			return true;

		}


		PassiveUnit::PassiveUnit(Hex * hex)
			: Unit(hex)
		{ }

		size_type PassiveUnit::strength() const
		{
			return 0;
		}



		Farm::Farm(Hex * hex)
			: PassiveUnit(hex)
		{ }

		unit_type Farm::type() const
		{
			return farm;
		}

		size_type Farm::income()
		{
			return 4;
		}


		Tree::Tree(Hex * hex)
			: PassiveUnit(hex)
		{ }


		bool Tree::ready_to_double() const
		{
			return TurnsFromDouble >= turns_to_double();
		}

		void Tree::has_doubled() // обнуляет turns_from_double
		{
			TurnsFromDouble = 0;
		}

		void Tree::operator ++() // ++turns_from_double
		{
			++TurnsFromDouble;
		}




		Palm::Palm(Hex * hex)
			: Tree(hex)
		{ }

		size_type Palm::turns_to_double() const // возвращает срок за который размножается данный тип
		{
			return 0;
		}

		unit_type Palm::type() const
		{
			return palm;
		}



		Pine::Pine(Hex * hex)
			: Tree(hex)
		{ }

		size_type Pine::turns_to_double() const // возвращает срок за который размножается данный тип
		{
			return 2;
		}

		unit_type Pine::type() const
		{
			return pine;
		}



		Grave::Grave(Hex * hex)
			: PassiveUnit(hex)
		{ }

		unit_type Grave::type() const
		{
			return grave;
		}

}
