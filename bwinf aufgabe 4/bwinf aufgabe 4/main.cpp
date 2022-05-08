
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <time.h>

void dateiLesenUndDatenSpeichern(std::string fileName, std::vector<std::string>& kennzeichenListe);
std::string getFileName();
char machUmlauteGrößer(char ansi);
char ansi2ascii(char ansi);
void stringToUpperCase(std::string& string);
bool kuerzelExistiert(std::string kuerzel, std::vector<std::string>& kuerzelListe);
bool enthältUmlaute(std::string string);
std::string zweistelligeZahlGenerieren();
void KuerzelHinzufügen(std::string bisherigesKennzeichen, std::string restString, std::vector<std::string>& kuerzelListe, std::vector<std::string>& fertigeKennzeichen);
void KennzeichenZusatzHinzufügen(std::string fertigesKennzeichen, std::string restString, std::vector<std::string>& kuerzelListe, std::vector<std::string>& fertigeKennzeichen);

void dateiLesenUndDatenSpeichern(std::string fileName, std::vector<std::string>& kennzeichenListe)
{
	std::ifstream ifStream(fileName);
	std::string line;
	
	while (std::getline(ifStream, line))
	{
		if (line.length() > 0)
		{
			for (size_t i = 0; i < line.size(); i++)
			{
				line[i] = ansi2ascii(line[i]);
			}
			kennzeichenListe.push_back(line);
		}
	}
}

std::string getFileName()
{
	// http://www.cplusplus.com/forum/windows/169960/

	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Wähle bitte die richtige Datei aus!";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		std::cout << "Du hast die Datie: \"" << filename << "\" ausgesucht.\n\n";
	}
	else
	{
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE:	std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE:	std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION:	std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE:	std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE:	std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE:	std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE:	std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE:		std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK:			std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE:		std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE:		std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL:	std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "Dateiauswahl abgebrochen.\n";
		}
	}

	return filename;
}

void stringToUpperCase(std::string& string)
{
	for (size_t i = 0; i < string.length(); i++)
	{		
		string[i] = machUmlauteGrößer(string[i]);
		string[i] = toupper(string[i]);
	}
}

char machUmlauteGrößer(char ascii)
{
	switch (ascii)
	{	
	case char(132) : return char(142);                    // Ä
	case char(148) : return char(153);                    // Ö
	case char(129) : return char(154);                    // Ü	
	default: return ascii;
	}
}

char ansi2ascii(char ansi)
{
	switch (ansi)
	{
	case char(228) : return char(142);                    // ä
	case char(246) : return char(153);                    // ö
	case char(252) : return char(154);                    // ü
	case char(196) : return char(142);                    // Ä
	case char(214) : return char(153);                    // Ö
	case char(220) : return char(154);                    // Ü
	case char(223) : return char(225);                    // ß
	default: return ansi;
	}
}

bool kuerzelExistiert(std::string kuerzel, std::vector<std::string>& kuerzelListe)
{
	if (std::find(kuerzelListe.begin(), kuerzelListe.end(), kuerzel) != kuerzelListe.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool enthältUmlaute(std::string string)
{
	for (size_t i = 0; i < string.length(); i++)
	{	
		if (string[i] == char(142) || string[i] == char(153) || string[i] == char(154) || string[i] == char(225))
		{
			return true;
		}
	}
	return false;
}

std::string zweistelligeZahlGenerieren()
{	
	int zufallInt = rand() % 90 + 10;
	std::stringstream ss;
	ss << zufallInt;
	std::string str = ss.str();
	return str;
}

void KuerzelHinzufügen(std::string bisherigesKennzeichen, std::string restString, std::vector<std::string>& kuerzelListe, std::vector<std::string>& fertigeKennzeichen)
{
	if (fertigeKennzeichen.size() < 10)
	{
		if (restString.size() >= 3)
		{
			if (kuerzelExistiert(restString.substr(0, 3), kuerzelListe))
			{
				KennzeichenZusatzHinzufügen(bisherigesKennzeichen + restString.substr(0, 3) + "-", restString.substr(3, restString.size()), kuerzelListe, fertigeKennzeichen);
			}
		}
		if (restString.size() >= 2)
		{
			if (kuerzelExistiert(restString.substr(0, 2), kuerzelListe))
			{
				KennzeichenZusatzHinzufügen(bisherigesKennzeichen + restString.substr(0, 2) + "-", restString.substr(2, restString.size()), kuerzelListe, fertigeKennzeichen);
			}
		}
		if (kuerzelExistiert(restString.substr(0, 1), kuerzelListe))
		{
			KennzeichenZusatzHinzufügen(bisherigesKennzeichen + restString.substr(0, 1) + "-", restString.substr(1, restString.size()), kuerzelListe, fertigeKennzeichen);
		}		
	}		
}

void KennzeichenZusatzHinzufügen(std::string bisherigesKennzeichen, std::string restString, std::vector<std::string>& kuerzelListe, std::vector<std::string>& fertigeKennzeichen)
{
	if (fertigeKennzeichen.size() < 10)
	{
		if (restString.length() > 2)
		{
			if (!enthältUmlaute(restString.substr(0, 2)))
			{				
				KuerzelHinzufügen(bisherigesKennzeichen + restString.substr(0, 2) + " " + zweistelligeZahlGenerieren() + ", \t", restString.substr(2, restString.size()), kuerzelListe, fertigeKennzeichen);
				KuerzelHinzufügen(bisherigesKennzeichen + restString.substr(0, 1) + " " + zweistelligeZahlGenerieren() + ", \t", restString.substr(1, restString.size()), kuerzelListe, fertigeKennzeichen);
			}
		}
		else if (restString.length() <= 2 && restString.length() > 0)
		{
			if (!enthältUmlaute(restString))
			{
				bisherigesKennzeichen += restString + " " + zweistelligeZahlGenerieren();
				fertigeKennzeichen.push_back(bisherigesKennzeichen);
				std::cout << "Moegliche Kombination: \t" << bisherigesKennzeichen << std::endl;
			}
		}
		else if (restString.length() == 0)
		{
			// Der Versuch endet
		}
	}
}

int main()
{
	srand(time(NULL));

	std::string dateiName = getFileName();

	std::vector<std::string> kennzeichenListe;
	dateiLesenUndDatenSpeichern(dateiName, kennzeichenListe);

	for (size_t i = 0; i < kennzeichenListe.size(); i++)
	{
		std::cout << kennzeichenListe[i] << std::endl;
	}

	std::cout << std::endl;

	std::cout << "Welches Wort soll in Kennzeichen geschrieben werden?" << std::endl << "Wort: ";
	std::string wort;
	std::getline(std::cin, wort);
	std::cout << "Du hast das Wort: " << wort << " eingegeben!" << std::endl;
	
	stringToUpperCase(wort);

	std::vector<std::string> fertigeKennzeichen;

	KuerzelHinzufügen("", wort, kennzeichenListe, fertigeKennzeichen);

	if (fertigeKennzeichen.size() == 0)
	{
		std::cout << "Dieses Wort laesst sich nicht in Kennzeichen darstellen!" << std::endl;
	}

	int s;
	std::cin >> s;

	return 0;
}