
#include <iostream>
#include <Windows.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

bool			wochenende;
bool			ferien;
int				anzahlJugendliche		= 0;
int				anzahlErwachsene		= 0;
int				anzahlKleinkinder		= 0;
int				anzahlGutscheine		= 0;
const double	einzelPreisErwachsener	= 3.5;
const double	einzelPreisJugendlicher = 2.5;
const int		jugendlichBis			= 16; //einschlieﬂlich 16
const int		kostenlosBis			= 3; //einschlieﬂlich 3
int				anzahlFamilienTicket_1_3 = 0;
int				anzahlFamilienTicket_2_2 = 0;

bool fertig = false;

static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

int main()
{
	while (!fertig)
	{
		std::string tempAntwort;

		std::cout << "Wie viele Erwachsene: ";
		std::getline(std::cin, tempAntwort);
		anzahlErwachsene = std::stoi(tempAntwort.c_str());

		std::cout << "Wie viele Jugendliche (Bis 16 Jahre): ";
		std::getline(std::cin, tempAntwort);
		anzahlJugendliche = std::stoi(tempAntwort.c_str());

		std::cout << "Wie viele Kleinkinder (Unter 4 Jahre): ";
		std::getline(std::cin, tempAntwort);
		anzahlKleinkinder = std::stoi(tempAntwort.c_str());

		std::cout << "An welchem Wochentag wollen sie das Schwimmbad besuchen ?" << std::endl;
		std::string wochentagAntwort;
		std::getline(std::cin, wochentagAntwort);

		std::transform(wochentagAntwort.begin(), wochentagAntwort.end(), wochentagAntwort.begin(), ::tolower);
		if (wochentagAntwort.size() > 3)
		{
			wochentagAntwort.erase(2, wochentagAntwort.size());
		}

		if		(strcmp(wochentagAntwort.c_str(), "mo") == 0) { wochenende = false;	}
		else if (strcmp(wochentagAntwort.c_str(), "di") == 0) { wochenende = false; }
		else if (strcmp(wochentagAntwort.c_str(), "mi") == 0) { wochenende = false; }
		else if (strcmp(wochentagAntwort.c_str(), "do") == 0) { wochenende = false; }
		else if (strcmp(wochentagAntwort.c_str(), "fr") == 0) { wochenende = false; }
		else if (strcmp(wochentagAntwort.c_str(), "sa") == 0) { wochenende = true;  }
		else if (strcmp(wochentagAntwort.c_str(), "so") == 0) { wochenende = true;  }

		if (wochenende)
		{
			std::cout << "Kein Rabatt fuer Wochenende" << std::endl;
		}
		else
		{
			std::cout << "20% Rabatt fuer Wochentags" << std::endl;
		}

		std::cout << "Liegt dieser Wochentag in den Ferien ? (Ja/Nein)" << std::endl;
		std::string ferienAntwort;
		std::getline(std::cin, ferienAntwort);

		std::transform(ferienAntwort.begin(), ferienAntwort.end(), ferienAntwort.begin(), ::tolower);
		if (ferienAntwort.size() > 1)
		{
			ferienAntwort.erase(1, ferienAntwort.size());
		}

		if		(strcmp(ferienAntwort.c_str(), "j") == 0) { ferien = true;  }
		else if (strcmp(ferienAntwort.c_str(), "n") == 0) { ferien = false; }

		std::cout << "Wie viele Gutscheine haben sie? : ";
		std::getline(std::cin, tempAntwort);
		anzahlGutscheine = std::stoi(tempAntwort);

		std::cout << std::endl;
		std::cout << "Erwachsene: " << anzahlErwachsene << std::endl;
		std::cout << "Jugendliche: " << anzahlJugendliche << std::endl;
		std::cout << "Kleinkinder: " << anzahlKleinkinder << std::endl;
		std::cout << "Gutscheine: " << anzahlGutscheine << std::endl;

		if (anzahlKleinkinder >= 1)
		{
			if (anzahlErwachsene == 0)
			{
				std::cout << "Kinder unter 4 Jahren brauchen eine Begleitperson!" << std::endl;
				continue;
			}
		}

		int unbenutzteErwachsene = anzahlErwachsene;
		int unbenutzteJugendliche = anzahlJugendliche;



		char stopper;
		std::cin >> stopper;
	}
}