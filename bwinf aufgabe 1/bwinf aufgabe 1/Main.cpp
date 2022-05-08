
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

struct Person;
struct Zimmer;

unsigned int split(const std::string &txt, std::vector<std::string> &outStrings, char splitter);
void dateiLesenUndDatenSpeichern(std::string fileName, std::vector<Person>& personen);
std::string getFileName();
bool DoesVecContainUInt(std::vector<unsigned int>& vec, unsigned int zahl);
void deleteZimmer(std::vector<Zimmer>& zimmer, std::vector<Person>& personen, unsigned int zimmerNummer);
void personenVerlegen(unsigned int ausZimmerID, unsigned int insZimmerID, std::vector<Zimmer>& zimmer, std::vector<Person>& personen);

struct Person
{
	std::string name;
	std::vector<std::string> proPersonen;
	std::vector<std::string> conPersonen;

	unsigned int eigeneID;
	std::vector<unsigned int> proPersonenIDs;
	std::vector<unsigned int> conPersonenIDs;

	unsigned int zimmerID = -1;

	void Person::InIDsUmwandeln(std::vector<Person>& personen)
	{
		std::vector<Person>::iterator eigeneID_Iterator = std::find_if(personen.begin(), personen.end(), [this](Person person) { return person.name == this->name; });
		if (eigeneID_Iterator != personen.end())
		{
			eigeneID = eigeneID_Iterator - personen.begin();
		}

		for (size_t i = 0; i < proPersonen.size(); i++)
		{
			std::vector<Person>::iterator tempIterator = std::find_if(personen.begin(), personen.end(), [this, &i](Person person) { return person.name == this->proPersonen.at(i); });
			if (tempIterator != personen.end())
			{
				proPersonenIDs.push_back(tempIterator - personen.begin());
			}
		}
		for (size_t i = 0; i < conPersonen.size(); i++)
		{
			std::vector<Person>::iterator tempIterator = std::find_if(personen.begin(), personen.end(), [this, &i](Person person) { return person.name == this->conPersonen.at(i); });
			if (tempIterator != personen.end())
			{
				conPersonenIDs.push_back(tempIterator - personen.begin());
			}
		}
	}

	void Person::print()
	{
		std::cout << eigeneID << ". Name: " << name;

		if (proPersonen.size() > 0)
		{
			std::cout << " +";
		}		
		for (size_t i = 0; i < proPersonen.size(); i++)
		{
			std::cout << " " << proPersonenIDs[i] << ";";
		}
		if (conPersonen.size() > 0)
		{
			std::cout << " -";
		}		
		for (size_t i = 0; i < conPersonen.size(); i++)
		{
			std::cout << " " << conPersonenIDs[i] << ";";
		}
		std::cout << std::endl;
	}
};

struct Zimmer
{
	std::vector<unsigned int> personenIDs;
};

unsigned int split(const std::string &txt, std::vector<std::string> &outStrings, char splitter)
{
	unsigned int pos = txt.find(splitter);
	unsigned int anfangsIndex = 0;
	outStrings.clear();

	while (pos != std::string::npos) {
		if (txt.substr(anfangsIndex, pos - anfangsIndex).size() != 0)
		{
			outStrings.push_back(txt.substr(anfangsIndex, pos - anfangsIndex));
		}		
		anfangsIndex = pos + 1;

		pos = txt.find(splitter, anfangsIndex);
	}

	if (txt.substr(anfangsIndex, min(pos, txt.size())).size() != 0)
	{
		outStrings.push_back(txt.substr(anfangsIndex, min(pos, txt.size()) - anfangsIndex));
	}	

	return outStrings.size();
}

void dateiLesenUndDatenSpeichern(std::string fileName, std::vector<Person>& personen)
{
	std::ifstream ifStream (fileName);
	std::string line;

	Person tempPerson;
	bool erstePersonFertig = false;

	while (std::getline(ifStream, line))
	{
		std::vector<std::string> tempVec;
		split(line, tempVec, ' ');
		if (tempVec.size() != 0)
		{
			if (tempVec[0] == "+")
			{
				for (size_t i = 1; i < tempVec.size(); i++)
				{
					tempPerson.proPersonen.push_back(tempVec[i]);
				}
			}
			else if (tempVec[0] == "-")
			{
				for (size_t i = 1; i < tempVec.size(); i++)
				{
					tempPerson.conPersonen.push_back(tempVec[i]);
				}
			}
			else if (line.size() > 0)
			{
				if (erstePersonFertig)
				{
					if (tempPerson.name.length() > 0)
					{
						personen.push_back(tempPerson);
						tempVec.clear();
						tempPerson.proPersonen.clear();
						tempPerson.conPersonen.clear();
					}					
				}
				tempPerson.name = line;
				erstePersonFertig = true;
			}
		}		
	}

	if (tempPerson.name.length() > 0)
	{
		personen.push_back(tempPerson);
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

bool DoesVecContainUInt(std::vector<unsigned int>& vec, unsigned int zahl)
{
	if (std::find(vec.begin(), vec.end(), zahl) != vec.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void personenVerlegen(unsigned int ausZimmerID, unsigned int insZimmerID, std::vector<Zimmer>& zimmer, std::vector<Person>& personen)
{
	for (size_t i = 0; i < zimmer[ausZimmerID].personenIDs.size(); i++)
	{
		zimmer[insZimmerID].personenIDs.push_back(zimmer[ausZimmerID].personenIDs[i]);
		personen[zimmer[ausZimmerID].personenIDs[i]].zimmerID = insZimmerID;
	}
	deleteZimmer(zimmer, personen, ausZimmerID);
}

void deleteZimmer(std::vector<Zimmer>& zimmer, std::vector<Person>& personen, unsigned int zimmerNummer)
{
	zimmer.erase(zimmer.begin() + zimmerNummer);
	for (size_t i = zimmerNummer; i < zimmer.size(); i++)
	{
		for (size_t z = 0; z < zimmer[i].personenIDs.size(); z++)
		{
			personen[zimmer[i].personenIDs[z]].zimmerID -= 1;
		}
	}
}

int main()
{
	std::vector<Person> personen;
	std::vector<Zimmer> zimmer;

	bool aufteilungMöglich = true;

	std::string dateiName = getFileName();
	dateiLesenUndDatenSpeichern(dateiName, personen);

	for (size_t i = 0; i < personen.size(); i++)
	{
		personen[i].InIDsUmwandeln(personen);
	}

	for (size_t i = 0; i < personen.size(); i++)
	{
		personen[i].print();
	}
	
	for (size_t i = 0; i < personen.size() && aufteilungMöglich; i++)
	{
		if (personen[i].zimmerID == -1)
		{
			Zimmer tempZimmer;
			tempZimmer.personenIDs.push_back(i);
			zimmer.push_back(tempZimmer);
			personen[i].zimmerID = zimmer.size() - 1;
		}
		
		for (size_t z = 0; z < personen[i].proPersonenIDs.size() && aufteilungMöglich; z++)
		{
			if (personen[personen[i].proPersonenIDs[z]].zimmerID == -1)
			{
				bool personen_Im_Zimmer_Akzeptieren_Z = true;

				for (size_t w = 0; w < zimmer[personen[i].zimmerID].personenIDs.size(); w++)
				{
					if (DoesVecContainUInt(personen[zimmer[personen[i].zimmerID].personenIDs[w]].conPersonenIDs, personen[i].proPersonenIDs[z]))
					{
						std::cout << std::endl << "Aufteilung gescheitert, weil " << personen[zimmer[personen[i].zimmerID].personenIDs[w]].name << " nicht mit " << personen[i].name << " in ein Zimmer will" << std::endl;
						personen_Im_Zimmer_Akzeptieren_Z = false;
					}
				}

				bool person_Z_akzeptiert_Personen_Im_Zimmer = true;

				for (size_t w = 0; w < personen[personen[i].proPersonenIDs[z]].conPersonenIDs.size(); w++)
				{
					if (DoesVecContainUInt(zimmer[personen[i].zimmerID].personenIDs, personen[personen[i].proPersonenIDs[z]].conPersonenIDs[w]))
					{
						std::cout << std::endl << "Aufteilung gescheitert, weil " << personen[personen[i].proPersonenIDs[z]].name << " nicht mit " << personen[personen[personen[i].proPersonenIDs[z]].conPersonenIDs[w]].name << " in ein Zimmer will" << std::endl;
						person_Z_akzeptiert_Personen_Im_Zimmer = false;
					}
				}

				if (!personen_Im_Zimmer_Akzeptieren_Z || !person_Z_akzeptiert_Personen_Im_Zimmer)
				{
					aufteilungMöglich = false;
				}

				if (aufteilungMöglich)
				{
					zimmer[personen[i].zimmerID].personenIDs.push_back(personen[i].proPersonenIDs[z]);
					personen[personen[i].proPersonenIDs[z]].zimmerID = personen[i].zimmerID;
				}
			}
			else
			{
				unsigned int personZzimmerID = personen[personen[i].proPersonenIDs[z]].zimmerID;

				bool personen_Im_Zimmer_I_Akzeptieren_Personen_Im_ZimmerZ = true;

				for (size_t w = 0; w < zimmer[personen[i].zimmerID].personenIDs.size(); w++)
				{
					for (size_t x = 0; x < zimmer[personZzimmerID].personenIDs.size(); x++)
					{
						if (DoesVecContainUInt(personen[zimmer[personen[i].zimmerID].personenIDs[w]].conPersonenIDs, zimmer[personZzimmerID].personenIDs[x]))
						{
							std::cout << std::endl << "Aufteilung gescheitert, weil " << personen[zimmer[personen[i].zimmerID].personenIDs[w]].name << " nicht mit " << personen[zimmer[personZzimmerID].personenIDs[x]].name << " in ein Zimmer will" << std::endl;
							personen_Im_Zimmer_I_Akzeptieren_Personen_Im_ZimmerZ = false;
						}
					}
				}

				bool personen_Im_Zimmer_Z_Akzeptieren_Personen_Im_ZimmerI = true;

				for (size_t w = 0; w < zimmer[personZzimmerID].personenIDs.size(); w++)
				{
					for (size_t x = 0; x < zimmer[personen[i].zimmerID].personenIDs.size(); x++)
					{
						if (DoesVecContainUInt(personen[zimmer[personZzimmerID].personenIDs[w]].conPersonenIDs, zimmer[personen[i].zimmerID].personenIDs[x]))
						{
							std::cout << std::endl << "Aufteilung gescheitert, weil " << personen[zimmer[personZzimmerID].personenIDs[w]].name << " nicht mit " << personen[zimmer[personen[i].zimmerID].personenIDs[x]].name << " in ein Zimmer will" << std::endl;
							personen_Im_Zimmer_Z_Akzeptieren_Personen_Im_ZimmerI = false;
						}
					}
				}

				if (!personen_Im_Zimmer_I_Akzeptieren_Personen_Im_ZimmerZ || !personen_Im_Zimmer_Z_Akzeptieren_Personen_Im_ZimmerI)
				{
					aufteilungMöglich = false;
				}

				if (aufteilungMöglich)
				{
					personenVerlegen(personZzimmerID, personen[i].zimmerID, zimmer, personen);
				}
			}
		}
	}

	if (aufteilungMöglich)
	{
		std::cout << std::endl << "Eine Aufteilung, bei der alle Wuensche erfuellt sind:" << std::endl;

		for (size_t x = 0; x < zimmer.size(); x++)
		{
			std::cout << "Zimmer " << x << ": ";

			for (size_t y = 0; y < zimmer[x].personenIDs.size(); y++)
			{
				std::cout << personen[zimmer[x].personenIDs[y]].name << " ";
			}

			std::cout << std::endl;
		}
	}	

	int s;
	std::cin >> s;

	return 0;
}