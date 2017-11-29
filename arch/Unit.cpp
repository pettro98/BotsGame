#pragma once
#include "Unit.h"



namespace game_module
{

		Unit::Unit(Hex * hex)
			: Field(hex)
		{ }

		Pair Unit::coordinates() const
		{
			return Field->coordinates();
		}

		hex_color Unit::color() const
		{
			return Field->color();
		}

		void Unit::set_hex(Hex * hex)
		{
			Field = hex;
		}

		Unit * unit_factory(unit_type type, size_type strength)
		{
			Unit * result = nullptr;
			switch (type)
			{
			case game_module::unit_type::none:
				break;
			case game_module::unit_type::army:
			{
				if (strength >= 1 && strength <= 4)
				{
					result = new Army(nullptr, strength);
				}
				break;
			}
			case game_module::unit_type::tower:
			{
				if (strength >= 2 && strength <= 3)
				{
					result = new Tower(nullptr, strength);
				}
				break;
			}
			case game_module::unit_type::capital:
			{
				if (strength == 1)
				{
					result = new Capital(nullptr);
				}
				break;
			}
			case game_module::unit_type::farm:
			{
				result = new Farm(nullptr);
				break;
			}
			case game_module::unit_type::palm:
			{
				result = new Palm(nullptr);
				break;
			}
			case game_module::unit_type::pine:
			{
				result = new Pine(nullptr);
				break;
			}
			case game_module::unit_type::grave:
			{
				result = new Grave(nullptr);
				break;
			}
			default:
				break;
			}	
			return result;
		}

		ActiveUnit::ActiveUnit(Hex * hex, size_type strength)
			: Unit(hex)
			, Strength(strength)
		{ }

		size_type  ActiveUnit::strength() const
		{
			return Strength;
		}

		Army::Army(Hex * hex, size_type strength)
			: ActiveUnit(hex, strength)
			, Moved(false)
		{ }

		unit_type Army::type() const
		{
			return game_module::unit_type::army;
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

		void Army::set_strength(size_type strength)
		{
			Strength = strength;
		}

		void Army::set_moved(bool moved)
		{
			Moved = moved;
		}

		void Army::die() // создает могилу на гексе, применяется при нехватке снабжения
		{
			Field->set_hex_unit(unit_factory(game_module::unit_type::grave));
		}

		Tower::Tower(Hex * hex, size_type strength)
			: ActiveUnit(hex, strength)
		{ }

		unit_type Tower::type() const
		{
			return game_module::unit_type::tower;
		}
		
		size_type Tower::cost() const
		{
			if (Strength == 2)
			{
				return 2;
			}
			return 7;
		}

		Capital::Capital(Hex * hex)
			: ActiveUnit(hex, 1)
			, DistrictMoney(0)
			, DistrictIncome(0)
			, FarmsNumber(0)
		{ }

		unit_type Capital::type() const
		{
			return game_module::unit_type::capital;
		}

		size_type Capital::cost() const
		{
			return 99;
		}

		size_type Capital::district_money() const
		{
			return DistrictMoney;
		}

		size_type Capital::district_income() const
		{
			return DistrictIncome;
		}

		size_type Capital::farms_number() const
		{
			return FarmsNumber;
		}

		bool Capital::change_district_money(size_type money)
		{
			if (money < 0 && abs(money) > DistrictMoney) 
			{
				DistrictMoney = 0;
				return false;
			}
			DistrictMoney += money;
			return true;
		}

		void Capital::change_district_income(size_type income)
		{
			DistrictIncome += income;
		}

		void Capital::change_district_income(Hex * hex)
		{
			if (hex->get_hex_capital() == this)
			{
				DistrictIncome -= 1;
				if (is_static(hex->get_hex_unit_type()))
				{
					DistrictIncome += 2;
				}
				else if (is_player_unit(hex->get_hex_unit_type()))
				{
					if (is_farm(hex->get_hex_unit_type()))
					{
						DistrictIncome -= Farm::income();
					}
					else if (is_army(hex->get_hex_unit_type()) 
						|| is_tower(hex->get_hex_unit_type()))
					{
						DistrictIncome += hex->get_hex_unit()->cost();
					}
				}
			}
		}

		void Capital::change_farms_number(size_type number)
		{
			FarmsNumber += number;
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
			return game_module::unit_type::farm;
		}

		size_type Farm::cost() const
		{
			return 12;
		}

		size_type Farm::income()
		{
			return 4;
		}

		Tree::Tree(Hex * hex)
			: PassiveUnit(hex)
			, TurnsFromDouble(0)
		{ }

		size_type Tree::cost() const
		{
			return 0;
		}

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
			return 1;
		}

		unit_type Palm::type() const
		{
			return game_module::unit_type::palm;
		}

		Pine::Pine(Hex * hex)
			: Tree(hex)
		{ }

		size_type Pine::turns_to_double() const // возвращает срок за который размножается данный тип
		{
			return 3;
		}

		unit_type Pine::type() const
		{
			return game_module::unit_type::pine;
		}

		Grave::Grave(Hex * hex)
			: PassiveUnit(hex)
		{ }

		unit_type Grave::type() const
		{
			return game_module::unit_type::grave;
		}

		size_type Grave::cost() const
		{
			return 0;
		}
}
