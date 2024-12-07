#pragma once

// MSG - message

const char* HI_MSG = "Hi, I'm app for generating textures!\nI have two commands:\n\tgen - generate and write texture to tmp directory\n\texit - close app\n\nLet's get started!\n\n";

const char* INCORRECT_MSG = "Incorrect input!\n";

const char* PROCESS_MSG = "Generating image with size ";

const char* COMPLETED_MSG = "Completed!\n";

const char* ERROR_MSG  = "Error generating occured...\n\n";

const char* END_MSG = "Goodbye!\n:)\n\n";

const char* DIVIDER = "\nImageGenApp: ";

const char* HELP_MSG = "\nYou can input theese commands below:\n\nhelp:\n\tShow help message\n\ngen:\n\tgen [width] [height] [image_type]\n\tGenerate new image to \'tmp\' folder\nThe following image types are avaliable:\n\t- box (single white box texture with noise)\n\t- grass (pixel grass texture)\nPlease, create only square 2^n resolution images, DONT BREAK MY PROGRAM PLSSSS\n\nexit:\n\tClose program :)\n";
