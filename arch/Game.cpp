#include "Game.h"
#include "Bot.h"
#include "dynamic.h"
#include <algorithm> 
#include <string>
#include <map>
#include <chrono>
#include <thread>

namespace game_module
{

	Game::~Game()
	{
		for (auto & i : Players)
		{
			delete i;
		}
		delete GameMap;
		delete GameController;
		delete GameView;
	}

	Game::Game(size_type max_turns, size_type dimension_x, size_type dimension_y
		, std::string map_type, size_t players_number)
		: CurrentPlayer(game_module::hex_color::blank)
		, CurrentTurn(0)
		, MaxTurns(max_turns)
	{
		Players = get_bots();
		if (map_type == "duel")
		{
			while (Players.size() > 2)
			{
				delete Players.back();
				Players.pop_back();
			}
			while (Players.size() < 2)
			{
				add_bot();
			}
			GameMap = new Map(dimension_x, dimension_y, 2, map_type);
		}
		else if (map_type == "classic")
		{
			while (Players.size() > 4)
			{
				Players.pop_back();
			}
			while (Players.size() < 4)
			{
				add_bot();
			}
			GameMap = new Map(dimension_x, dimension_y, 4, map_type);
		}
		else if (map_type == "random")
		{
			if (players_number < 2 || players_number > 6)
			{
				players_number = 6;
			}
			while (Players.size() > players_number)
			{
				delete Players.back();
				Players.pop_back();
			}
			while (Players.size() < players_number)
			{
				add_bot();
			}
			GameMap = new Map(dimension_x, dimension_y, players_number, map_type);
		}
		else
		{
			throw("incorect_map_type");
		}
		GameController = new Controller(this);
		GameView = new View(this);
		place_players();
	}

	bool Game::check_end_game() const
	{
		return CurrentTurn > MaxTurns || GameController->get_players_number() < 2;
	}

	Hex * Game::operator () (const Pair & hex)
	{
		return (*GameMap)(hex);
	}

	Hex * Game::operator () (size_type coord1, size_type coord2)
	{
		return (*GameMap)(Pair(coord1, coord2));
	}

	Unit * Game::get_unit(const Pair & hex)
	{
		if ((*GameMap)(hex) != nullptr)
		{
			return (*GameMap)(hex)->get_unit();
		}
		return nullptr;
	}

	std::vector<Player *> & Game::get_players()
	{
		return Players;
	}

	Player * Game::get_player(hex_color seek_color)
	{
		for (auto & i : get_players())
		{
			if (i->color() == seek_color)
			{
				return i;
			}
		}
		return nullptr;
	}

	hex_color Game::get_current_player() const
	{
		return CurrentPlayer;
	}

	Map & Game::get_game_map()
	{
		return *GameMap;
	}

	size_type Game::current_turn() const
	{
		return CurrentTurn;
	}

	size_type Game::max_turns() const
	{
		return MaxTurns;
	}

	Result & Game::results()
	{
		return Results;
	}

	bool Game::player_in_game(hex_color color) const
	{
		for (auto & i : Players)
		{
			if (i->color() == color)
			{
				return (i->get_capitals_number() != 0);
			}
		}
		return false;
	}

	void Game::set_unit(const Pair & hex, Unit * unit)
	{
		if ((*GameMap)(hex) != nullptr)
		{
			(*GameMap)(hex)->set_unit(unit);
		}
	}

	void Game::set_color(const Pair & hex, hex_color new_color)
	{
		if ((*GameMap)(hex) != nullptr)
		{
			(*GameMap)(hex)->set_color(new_color);
		}
	}

	void Game::turn_passed()
	{
		++CurrentTurn;
	}

	void Game::set_current_player(hex_color new_index)
	{
		CurrentPlayer = new_index;
	}

	void Game::set_winner(hex_color player_color)
	{
		Results.winner = player_color;
	}

	void Game::set_last_turn(hex_color player_color, size_type turn)
	{
		Results.last_turn[player_color - 1] = turn;
	}

	void Game::add_to_built_armies(hex_color player_color, size_type str)
	{
		Results.built_armies[player_color - 1] += str;
	}

	void Game::add_to_built_farms(hex_color player_color)
	{
		++Results.built_farms[player_color - 1];
	}

	void Game::add_to_built_towers(hex_color player_color, size_type str)
	{
		Results.built_towers[player_color - 1] += str;
	}

	void Game::add_to_moves(hex_color player_color)
	{
		++Results.moves[player_color - 1];
	}

	bool Game::place_players()
	{
		std::vector<Pair> capitals;
		for (size_type i = 1; i < GameMap->dimension_x() - 1; ++i)
		{
			for (size_type j = 1; j < GameMap->dimension_y() - 1; ++j)
			{
				if (get_unit(Pair(i, j)) != nullptr
					&& is_capital(get_unit(Pair(i, j))->type()))
				{
					capitals.push_back(Pair(i, j));
				}
			}
		}
		for (size_t i = 0; i < capitals.size(); ++i)
		{
			Players[i]->add_capital(capitals[i]);
			(*this)(capitals[i])->set_color(Players[i]->color());
			(*this)(capitals[i])->set_capital((*this)(capitals[i]));
			static_cast<Capital *>(get_unit(capitals[i]))->change_district_income(1);
			for (auto & j : GameMap->get_neighbours(capitals[i]))
			{
				(*this)(j)->set_color(Players[i]->color());
				(*this)(j)->set_capital((*this)(capitals[i]));
				static_cast<Capital *>(get_unit(capitals[i]))->change_district_income(1);
			}
			Players[i]->set_controller(GameController);
		}
		return true;
	}

	void Game::prepare_player()
	{
		for (auto & i : get_player(get_current_player())->get_capitals())
		{
			for (auto & j : GameController->get_district_units(i, game_module::unit_type::grave))
			{
				(*this)(j)->set_unit(unit_factory(game_module::unit_type::palm));
			}
			if (!static_cast<Capital *>((*this)(i)->get_capital())
				->change_district_money(GameController->get_district_income(i)))
			{
				for (auto & j : GameController->get_district_units(i, game_module::unit_type::army))
				{
					static_cast<Army *>((*this)(j)->get_unit())->die();
				}
				GameController->calculate_income(this->get_game_map().easy_solve_maze(i));
			}
			else
			{
				for (auto & j : GameController->get_district_units(i, game_module::unit_type::army))
				{
					static_cast<Army *>((*this)(j)->get_unit())->set_moved(false);
				}
			}
		}
	}

	void Game::double_trees()
	{
		for (size_type i = 0; i < get_game_map().dimension_x(); ++i)
		{
			for (size_type j = 0; j < get_game_map().dimension_y(); ++j)
			{
				if (is_tree((*this)(Pair(i, j))->get_unit_type()))
				{
					if (static_cast<Tree *>((*this)(Pair(i, j))->get_unit())->ready_to_double())
					{
						auto neighbours = get_game_map().get_neighbours(Pair(i, j));
						std::random_shuffle(neighbours.begin(), neighbours.end());
						for (auto & k : neighbours)
						{
							if (!(*this)(k)->occupied())
							{
								(*this)(k)->set_unit(unit_factory((*this)(Pair(i, j))
									->get_unit()->type()));
								static_cast<Tree *>((*this)(Pair(i, j))->get_unit())->has_doubled();
								if ((*this)(k)->get_capital())
								{
									(*this)(k)->get_capital()->change_district_income(Tree::income());
								}
								break;
							}
						}
					}
					else
					{
						++(*static_cast<Tree *>((*this)(Pair(i, j))->get_unit()));
					}
				}
			}
		}
	}

	hex_color Game::get_winner()
	{
		if (!check_end_game())
		{
			return game_module::hex_color::blank;
		}
		std::vector<hex_color> players(GameController->get_players_colors());
		if (players.size() == 1)
		{
			return players[0];
		}
		std::vector<size_type> players_scores;
		players_scores.resize(players.size());
		std::multimap<size_type, hex_color> result;
		for (size_t i = 0; i < players.size(); ++i)
		{
			for (auto & j : GameController->get_player_capitals(players[i]))
			{
				players_scores[i] += 10 * GameController->easy_solve_maze_count(j);
				players_scores[i] += 40 * GameController->get_farms_number(j);
				players_scores[i] += 20 * GameController->easy_solve_maze_count(j, is_army);
			}
			result.insert(std::make_pair(players_scores[i], players[i]));
		}
		return result.rbegin()->second;
	}

	std::vector<Player *> Game::get_bots()
	{
		std::vector<Player *> result;
		result.push_back(new BOT_0(red, "dev"));
        result.push_back(new BOT_1(green, "dev"));
#if BOTS_COUNT > 2
        result.push_back(new BOT_2(cyan, "dev"));
#elif BOTS_COUNT > 3
        result.push_back(new BOT_3(purple, "dev"));
#elif BOTS_COUNT > 4
		result.push_back(new BOT_4(blue, "dev"));
#elif BOTS_COUNT > 5
		result.push_back(new BOT_5(yellow, "dev"));
#endif
		return result;
	}

	void Game::add_bot()
	{
		std::vector<hex_color> players_colors = {
			red,
			green,
			cyan,
			purple,
			blue,
			yellow,
		};
		hex_color bot_color(game_module::hex_color::extra);
		for (auto & i : players_colors)
		{
			bool correct_color = true;
			for (auto & j : Players)
			{
				if (j->color() == i)
				{
					correct_color = false;
					break;
				}
			}
			if (correct_color)
			{
				bot_color = i;
				break;
			}
		}
		Bot * new_bot = new Bot(bot_color, get_color_string(bot_color));
		Players.push_back(new_bot);
	}

	void Game::start_game(bool show_map)
	{
		if (show_map)
		{
			GameView->show(*GameMap, Results);
		}
		while (this->current_turn() <= this->max_turns())
		{
			for (size_t i = 0; i < this->get_players().size(); ++i)
			{
				this->set_current_player(this->get_players()[i]->color());
				this->prepare_player();
				if (this->player_in_game(this->get_current_player()))
				{
					this->get_players()[i]->turn();
				}
			}
			for (size_t i = 0; i < this->get_players().size(); ++i)
			{
				if (!this->player_in_game(this->get_players()[i]->color())
					&& this->results().last_turn[this->get_players()[i]->color() - 1] == 0)
				{
					this->set_last_turn(this->get_players()[i]->color(), this->current_turn());
				}
			}
			if (this->check_end_game())
			{
				break;
			}
			this->turn_passed();
			if (show_map)
			{
                std::this_thread::sleep_for(std::chrono::seconds(1));
				GameView->show(*GameMap, Results);
			}
			this->double_trees();
		}
		if (show_map)
		{
			GameView->show(*GameMap, Results);
		}
	}
}
