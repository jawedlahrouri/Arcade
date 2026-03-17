/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** menu
*/

#ifndef MENU_HPP_
#define MENU_HPP_
#include <string>
#include <vector>

class Menu {
	private:
		std::string _libDirectory;
		std::vector<std::string> _graphicLibs;

	public:
    void Menuinit();

};

#endif /* !MENU_HPP_ */
