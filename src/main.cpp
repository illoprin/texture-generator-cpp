
#include "includes.h"
#include "const.h"

int main() {
	cout << HI_MSG;

	bool started = true;
	std::string user_input;
	while (started)
	{
		cout << DIVIDER; cin >> user_input;
		if (user_input == "exit")
		{
			started = false;
		}
		else if (user_input == "gen")
		{
			int width, height;
			cin >> width >> height;
			cout << PROCESS_MSG << width << " " << height << "\n";

			if (gen_image(width, height, "tmp") == 0)
			{
				cout << COMPLETED_MSG;
			}
			else
			{
				cout << ERROR_MSG;
			}

		}
		else
		{
			cout << INCORRECT_MSG;
		}
	}
	cout << END_MSG;

	return 0;
}