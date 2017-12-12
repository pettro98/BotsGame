



namespace game_module
{
	class View
	{
	private:
		Map PreviousState;
	public:
		~View();
		View();
		void show(const Map & newState, const Result & results);
	};
}
