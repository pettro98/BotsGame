#include "General.h"
#include "Unit.h"


namespace game_module 
{

	class Hex
	{
	private:
		pair Coordinates; // координаты гекса
		size_type Index; // индекс владельца
		Unit * HexUnit; // указатель на юнит в гексе 
		Capital * HexCapital; // указатель на столицу области

	public:
		~Hex();
		Hex(Hex * phex);
		Hex(size_type coord_1, size_type coord_2);
		Hex & operator = (const Hex & hex);

		pair coordinates() const;
		size_type index() const;
		Unit * get_hex_unit();
		Capital * get_hex_capital();

		bool occupied() const; // наличие юнита в гексе

		bool operator == (const Hex & hex) const;

		void set_index(size_type new_index);
		void set_hex_capital(Capital * new_capital); // заменяет указатель capital
		void remove_hex_unit(); // разыменовывает указатель unit
		void delete_hex_unit(); // вызывает delete для указателя unit и разыменовывает его
		void set_hex_unit(Unit * unit); // помещает нового юнита в гекс, старого удаляет
	};

	bool operator != (const Hex & hex1, const Hex & hex2);

}
