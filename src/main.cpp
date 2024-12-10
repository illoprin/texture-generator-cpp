#include "includes.h"
#include "const.h"

TextureType get_type_from_str(std::string& str)
{
	if (str == "grass")
		return TextureType::Grass;
	else if (str == "box")
		return TextureType::Box;
	else if (str == "wall")
		return TextureType::MetalWall;
	else if (str == "office")
		return TextureType::OfficeWall;
	else
		return TextureType::NoTexture;
}

int main()
{
	cout << HI_MSG;

	bool started = true;
	std::string user_input;
	std::string image_type;
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
			cin >> width >> height >> image_type;
			cout << PROCESS_MSG << width << " " << height << "\n";
			TextureType type = get_type_from_str(image_type);
			if (gen_image(width, height, "tmp", type) == 0)
			{
				cout << COMPLETED_MSG;
			}
			else
			{
				cout << ERROR_MSG;
			}

		}
		else if (user_input == "help")
		{
			cout << HELP_MSG;
		}
		else
		{
			cout << INCORRECT_MSG;
		}
	}
	cout << END_MSG;

	return 0;
}
