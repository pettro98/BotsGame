#pragma once

#include <iostream>
#include <vector>


namespace game_module
{
	using size_type = int;
	/*!
	\brief Структура моделирует пару значений типа size_type.
	- Используется для хранения координат гексов.
	- Другое имя структуры, используемое в докуметации - пара.
	*/
	struct Pair
	{
		/*!
		\brief Первое значение пары.
		*/
		size_type First;
		/*!
		\brief Второе значение пары.
		*/
		size_type Second;
		/*! 
		\brief Деструктор по умолчанию		
		*/
		~Pair() = default;
		/*!
		\brief Конструктор без параметров по умолчанию
		*/
		Pair() = default;
		/*!
		\brief Конструктор копирования.

		\param pair Пара, которую необходимо скопировать.
		*/
		Pair(const Pair & pair);
		/*!
		\brief Конструктор.

		\param first Первое значение пары.
		\param second Второе значение пары.
		*/
		Pair(size_type first, size_type second);
		/*!
		\brief Оператор сравнения.

		\param pair Пара, с которой происходит сравнение.
		*/
		bool operator == (const Pair & pair) const;
		/*!
		\brief Оператор меньше.
		- Сравниваются первые значения пар, если они равны, то сравниваются вторые значения пар.

		\param pair Пара, с которой происходит сравнение.
		*/
		bool operator < (const Pair & pair) const;
	};
	/*!
	\brief Оператор сравнения двух пар.
	- Используется оператор сравнения структуры Pair.

	\param pair1 Первая пара, проходящая сравнение.
	\param pair2 Вторая пара, проходящая сравнение.
	*/
	bool operator != (const Pair & pair1, const Pair & pair2);
	/*!
	\brief Оператор вывода пары в поток.
	- Пара выводится в виде (First, Second).

	\param out Поток вывода.
	\param pair Пара, которую необходимо вывести.
	*/
	std::ostream & operator << (std::ostream & out, const Pair & pair);
	/*!
	\brief Перечисление хранит типы юнитов, которые могут храниться в гексе.
	*/
	enum unit_type
	{
		none = 0, /// Указывает, что в гексе нет юнита.
		army, /// Указывает, что в гексе находится армия.
		tower, /// Указывает, что в гексе находится башня.
		capital, /// Указывает, что в гексе находится столица.
		farm, /// Указывает, что в гексе находится ферма.
		palm, /// Указывает, что в гексе находится пальма.
		pine, /// Указывает, что в гексе находится ель.
		grave /// Указывает, что в гексе находится могила.
	};
	/*!
	\brief Функция возвращает true.
	- Функция необходима для работы с некоторыми методами класса Map и других.

	\param type Тип, который надо проверить.
	*/
	bool is_type(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу none.

	\param type Тип, который надо проверить.
	*/
	bool is_none(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу army.

	\param type Тип, который надо проверить.
	*/
	bool is_army(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу tower.

	\param type Тип, который надо проверить.
	*/
	bool is_tower(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу capital.

	\param type Тип, который надо проверить.
	*/
	bool is_capital(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу farm.

	\param type Тип, который надо проверить.
	*/
	bool is_farm(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу palm.

	\param type Тип, который надо проверить.
	*/
	bool is_palm(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу pine.

	\param type Тип, который надо проверить.
	*/
	bool is_pine(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу grave.

	\param type Тип, который надо проверить.
	*/
	bool is_grave(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу tree.

	\param type Тип, который надо проверить.
	*/
	bool is_tree(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу статичного объекта.
	- Статичными объектами считаются типы: palm, pine, grave.

	\param type Тип, который надо проверить.
	*/
	bool is_static(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу юнита игрока.
	- Юнитами игрока считаются типы: army, tower, farm, capital.

	\param type Тип, который надо проверить.
	*/
	bool is_player_unit(unit_type type);
	/*!
	\brief Функция сообщает соответствует ли принятый тип типу, none или статичного объекта.
	- Статичными объектами считаются типы: palm, pine, grave.

	\param type Тип, который надо проверить.
	*/
	bool is_ready_to_take(unit_type type);
	/*!
	\brief Перечисление хранит возможные цвета гексов.
	*/
	enum hex_color {
		black = -1, /// Указывает, что гекс не участвует в игре.
		blank, /// Указывает, что гекс не принадлежит игроку.
		red, /// Указывает, что гекс принадлежит красному игроку.
		green, /// Указывает, что гекс принадлежит зелёному игроку.
		cyan, /// Указывает, что гекс принадлежит голубому игроку.
		purple, /// Указывает, что гекс принадлежит фиолетовому игроку.
		blue, /// Указывает, что гекс принадлежит синему игроку.
		yellow, /// Указывает, что гекс принадлежит жёлтому игроку.
		extra /// Указывает, что гекс имеет особый цвет. Используется в методе solve_map класса Map.
	};
	/*!
	\brief Функция возвращает строку с именем принятого цвета.

	\param color Цвет, строку с именем которого надо вернуть.
	*/
	std::string get_color_string(hex_color color);
	/*!
	\brief Функция возвращает true.
	- Функция необходима для работы с некоторыми методами класса Map и других.

	\param color Цвет, который надо проверить.
	*/
	bool is_color(hex_color color);
	/*!
	\brief Функция сообщает соответствует ли принятый цвет цвету black.

	\param color Цвет, который надо проверить.
	*/
	bool is_black(hex_color color);
	/*!
	\brief Функция сообщает соответствует ли принятый цвет цвету blank.

	\param color Цвет, который надо проверить.
	*/
	bool is_blank(hex_color color);
	/*!
	\brief Функция сообщает соответствует ли принятый цвет цвету extra.

	\param color Цвет, который надо проверить.
	*/
	bool is_extra(hex_color color);
	/*!
	\brief Функция сообщает соответствует ли принятый цвет цвету игрока.
	- Цветом игрока считаются цвета: red, green, cyan, purple, blue, yellow.

	\param color Цвет, который надо проверить.
	*/
	bool is_player_color(hex_color color);
	/*!
	\brief Структура хранит результаты игры.
	*/
	struct Result
	{
	public:
		/*!
		\brief Цвет победившего игрока.
		*/
		hex_color winner;
		/*!
		\brief Вектор номеров последних ходов игроков.
		*/
		std::vector<size_type> last_turn;
		/*!
		\brief Вектор сумм сил построенных за игру армий игроков.
		*/
		std::vector<size_type> built_armies;
		/*!
		\brief Вектор кол-в построенных за игру ферм игроков.
		*/
		std::vector<size_type> built_farms;
		/*!
		\brief Вектор сумм сил построенных за игру башен игроков.
		*/
		std::vector<size_type> built_towers;
		/*!
		\brief Вектор кол-в совершённых за игру перемещений армий игроков.
		*/
		std::vector<size_type> moves;
		/*!
		\brief Конструтор.
		*/
		Result();
	};
}
