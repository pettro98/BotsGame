



namespace game_module
{

	class View
	{

	private:

		IGetData * MyGetData;
		Map PreviousState;

	public:

		~View();
		View(IGetData * getData);
		void show(const Map & newState);

	};

}
