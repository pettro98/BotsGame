#pragma once

#include "General.h"
#include "Unit.h"


namespace game_module
{
	class Unit;
	class Capital;
	/*!
	\brief Класс моделирует ячейку игрового поля.
	- Ячейка имеет шестиугольную форму.
	- Другое имя класса, используемое в докуметации - гекс.
	*/
	class Hex
	{
	private:
		/*!
		\brief Координаты гекса.
		- Координаты неотрицательны.
		- Первая координата указывает положение гекса на условной оси x,
		- направленной слева направо, вторая координата координата
		- положение гекса на условной оси y, напрвавленной сверху вниз.
		*/
		Pair Coordinates;
		/*!
		\brief! Цвет гекса.
		- Цвет гекса совпадает с цветом владельца гекса.
		*/
		hex_color Color;
		/*!
		\brief Указатель на юнит, находящийся в гексе.
		- В случае если юнита нет указатель равен nullptr.
		*/
		Unit * HexUnit;
		/*!
		\brief Указатель на столицу области, которой принадлежит гекс.
		- В случае если гекс не принадлежит области указатель равен nullptr.
		*/
		Capital * HexCapital;
	public:
		/*!
		\brief Деструктор.
		- Освобождает память из под юнита, который находится в гексе.
		*/
		~Hex();
		/*!
		\brief Конструктор.
		- Цвет гекса по умолчанию blank.
		- Все указатели по умолчанию nullptr.

		\param coord1 Первая координата.
		\param coord2 Вторая координата.
		*/
		Hex(size_type coord1, size_type coord2);
		/*!
		\brief Метод возвращает копию поля Coordinates объекта класса. 
		*/
		Pair coordinates() const;
		/*!
		\brief Метод возвращает копию копию поля Color объекта класса. 
		*/
		hex_color color() const;
		/*!
		\brief Метод возвращает тип юнита, находящегося в гексе.
		- Если в гексе нет юнита возвращает тип none.
		*/
		unit_type get_hex_unit_type() const;
		/*!
		\brief Метод возвращает поле HexUnit объекта класса.
		*/
		Unit * get_hex_unit();
		/*!
		\brief Метод возвращает переменную HexCapital класса.
		*/
		Capital * get_hex_capital();
		/*!
		\brief Метод сообщает хранится ли юнит в гексе.
		*/
		bool occupied() const;
		/*!
		\brief Оператор сравнения двух объектов класса.
		- Сравниваются переменные Coordinates двух гексов.

		\param hex гекс, с которым сравнивается данный.
		*/
		bool operator == (const Hex & hex) const;
		/*!
		\brief Метод заменяет цвет гекса на переданный.

		\param new_color новый цвет гекса.
		*/
		void set_color(hex_color new_color);
		/*!
		\brief Метод связывает новый юнит и данный гекс.
		- Метод удаляет старый юнит гекса.
		- Связывание подразумевает, что юнит и гекс хранят указатели друг на друга.

		\param unit указатель на юнит, с которым происходит связывание.
		*/
		void set_hex_unit(Unit * unit);
		/*!
		\brief Метод устанавливает новую столицу области гекса.
		- Если new_capital равен nullptr, это значение записывается в HexCapital.
		- Если гекс new_capital содержит юнит столицы, HexCapital начинает указывать на него.
		- Иначе HexCapital начинает указывать на столицу области гекса new_capital.

		\param new_capital указатель на гекс, по которому устанавливается новая столица области данного гекса.
		*/
		void set_hex_capital(Hex * new_capital);
		/*!
		\brief Метод убирает юнита из гекса.
		- Переменной HexUnit присваивает значение nullptr;
		*/
		void remove_hex_unit();
		/*
		\brief Метод удаляет юнит гекса.
		- Очищает память из под HexUnit и присваивает значение nullptr;
		*/
		void delete_hex_unit();
	};
	/*!
	\brief Оператор сравнения двух гексов.
	- Для сравнения используется оператор == объявленный в классе Hex.

	\param hex1 первый гекс.
	\param hex2 второй гекс.
	*/
	bool operator != (const Hex & hex1, const Hex & hex2);
	/*!
	\brief Оператор расчёта дистанции между двумя гексами.
	- Дистанция между двумя гексами есть минимальное, кол-во гексов которое нужно "пройти",
	чтобы попасть из одного в другой.
	- "Пройти" из данного гекса можно только в соседний (имеют общее ребро) ему.
	- Расстояние между двумя соседними гексами равно 1, расстояние между гексом и им самим равно 0.
	- Формула расчёта имеет нетривиальный вид.

	\param hex1 первый гекс.
	\param hex2 второй гекс.
	*/
	size_type get_distance(const Pair & hex1, const Pair & hex2);
}