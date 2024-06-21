#pragma once

#include "define.hpp"
# define CHANNEL_MODE 1
# define USER_MODE 2

class MODE {
	public:
		MODE();
		~MODE();
		MODE(const MODE &other);
		MODE &operator=(const MODE &other);

		/*--------------Getters--------------*/
		bool						getModeValue(char mode);
		std::string					getParams(char mode);
		std::map<char, bool>		getModes();
		std::string					getAuthorizedMode(int type);
		std::string					getModesAsString();
		void 						isModeAuthorized(char mode, t_mode* modes);
		std::map<char, std::string>	getParams();
		long long					getLimit();
		std::string					getParamsNeeded(int Type);

		/*--------------Setters--------------*/
		void					setModeByType(char mode, char value, bool needParams, std::string params, std::string nick);
	private:
		std::map<char, bool>		_mode;
		std::map<char, std::string>	_params;
		std::string					_modesUser;
		std::string					_modesChannel;
		std::string					_needParams;
};
