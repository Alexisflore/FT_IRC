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
		long long					getLimit();
		std::string					getPassword();
		bool						getModeValue(char mode);
		std::string					getParams(char mode);
		std::vector<std::pair<char, bool> >	getMode();
		std::string					getAuthorizedMode(int type);
		std::string					getModesAsString();
		void 						isModeAuthorized(char mode, t_mode* modes);
		std::map<char, std::string>	getParams();
		std::string					getParamsNeeded(int Type);

		/*--------------Setters--------------*/
		void					setModeByType(char mode, char value, bool needParams, std::string params, std::string nick);
	private:
		std::vector<std::pair<char, bool> >			_mode;
		std::vector<std::pair<char, std::string> >	_params;
		long long									_limit;
		std::string									_password;
		std::string									_modesUser;
		std::string									_modesChannel;
		std::string									_needParams;
};
