
#include <Windows.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <string>

struct Linie
{
public:
	Linie::Linie() {};
	Linie::Linie(float x1, float y1, float x2, float y2)
	{ 
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	};
	std::string toString() { return ("x1: " + std::to_string(x1) + " y1: " + std::to_string(y1) + " x2: " + std::to_string(x2) + " y2: " + std::to_string(y2)); }

	float x1, x2;
	float y1, y2;
};

struct Vec2f
{
	Vec2f::Vec2f() {};
	Vec2f::Vec2f(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float x, y;
};

struct Vec3i
{
public:
	Vec3i::Vec3i() {};
	Vec3i::Vec3i(int x, int y, int z)
	{ 
		if (x < y && x < z)
		{
			this->x = x;
			if (y < z)
			{
				this->y = y;
				this->z = z;
			}
			else
			{
				this->y = z;
				this->z = y;
			}
		}
		else if (x < y && x > z)
		{
			this->y = x;
			this->x = z;
			this->z = y;
		}
		else if (x < z && x > y)
		{
			this->y = x;
			this->x = y;
			this->z = z;
		}
		else
		{
			this->z = x;
			if (y < z)
			{
				this->x = y;
				this->y = z;
			}
			else
			{
				this->x = z;
				this->y = y;
			}
		}
	};
	bool operator==(const Vec3i& other)
	{
		if (other.x == x && other.y == y && other.z == z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int x, y, z;
};

bool linie_A_schneidet_Linie_B(Linie linie1, Linie line2)
{
	float x12 = linie1.x1 - linie1.x2;
	float x34 = line2.x1 - line2.x2;
	float y12 = linie1.y1 - linie1.y2; 
	float y34 = line2.y1 - line2.y2; 

	float c = x12 * y34 - y12 * x34;

	if (fabs(c) < 0.01)
	{
		return false;
	}
	else
	{
		float a = linie1.x1 * linie1.y2 - linie1.y1 * linie1.x2;
		float b = line2.x1 * line2.y2 - line2.y1 * line2.x2;

		float x = (a * x34 - b * x12) / c;
		float y = (a * y34 - b * y12) / c;

		float line1LeftPoint;
		float line1RightPoint;
		if (linie1.x1 > linie1.x2)
		{
			line1LeftPoint = linie1.x2;
			line1RightPoint = linie1.x1;
		}
		else
		{
			line1LeftPoint = linie1.x1;
			line1RightPoint = linie1.x2;
		}

		float line1UpperPoint;
		float line1LowerPoint;
		if (linie1.y1 > linie1.y2)
		{
			line1UpperPoint = linie1.y1;
			line1LowerPoint = linie1.y2;
		}
		else
		{
			line1UpperPoint = linie1.y2;
			line1LowerPoint = linie1.y1;
		}

		if (x < line1LeftPoint || x > line1RightPoint || y < line1LowerPoint || y > line1UpperPoint)
		{
			return false;
		}
		else
		{
			return true;
		}	
	}
}

Vec2f getSchnittpunkt(Linie line1, Linie line2)
{
	float x12 = line1.x1 - line1.x2;
	float x34 = line2.x1 - line2.x2;
	float y12 = line1.y1 - line1.y2;
	float y34 = line2.y1 - line2.y2;

	float c = x12 * y34 - y12 * x34;

	float a = line1.x1 * line1.y2 - line1.y1 * line1.x2;
	float b = line2.x1 * line2.y2 - line2.y1 * line2.x2;

	float x = (a * x34 - b * x12) / c;
	float y = (a * y34 - b * y12) / c;

	return Vec2f(x, y);
}

void dateiAuslesenUndDatenSpeichern(std::string fileName, std::vector<Linie>& outVector)
{
	FILE* file;
	fopen_s(&file, fileName.c_str(), "r");
	if (file == NULL)
	{
		printf("Die Datei lässt sich nicht öffnen!\n");
	}
	else
	{
		int size;
		fscanf_s(file, "%i", &size);

		std::cout << size << std::endl;

		while (true)
		{
			Linie line;
			int res = fscanf_s(file, "%f %f %f %f", &line.x1, &line.y1, &line.x2, &line.y2);
			
			if (res == EOF)
			{
				break;
			}

			std::cout << line.toString() << std::endl;
			outVector.push_back(line);
		}
	}
	
	std::cout << std::endl;
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
		std::cout << "You chose the file \"" << filename << "\"\n";
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
		default: std::cout << "You cancelled.\n";
		}
	}

	return filename;
}

int main()
{
	std::string dateiName = getFileName();

	std::vector<Linie> linien;

	dateiAuslesenUndDatenSpeichern(dateiName, linien);

	std::vector<std::vector<int>> sich_schneidene_Linien;
	std::vector<Vec3i> unoptimierter_DreieckSpeicher;

	for (size_t x = 0; x < linien.size(); x++)
	{
		std::vector<int> mit_X_schneideneLinien;

		for (size_t y = 0; y < linien.size(); y++)
		{
			if (y != x)
			{
				if (linie_A_schneidet_Linie_B(linien[x], linien[y]))
				{
					mit_X_schneideneLinien.push_back(y);

					for (size_t z = 0; z < linien.size(); z++)
					{
						if (z != x && z != y)
						{
							if (linie_A_schneidet_Linie_B(linien[y], linien[z]))
							{
								if (linie_A_schneidet_Linie_B(linien[x], linien[z]))
								{
 									unoptimierter_DreieckSpeicher.push_back(Vec3i(x, y, z));
								}
							}							
						}
					}
				}
			}
		}

		sich_schneidene_Linien.push_back(mit_X_schneideneLinien);
	}

	std::cout << std::endl;

	for (size_t x = 0; x < sich_schneidene_Linien.size(); x++)
	{
		std::cout << "Line " << x << ": ";

		for (size_t y = 0; y < sich_schneidene_Linien[x].size(); y++)
		{
			std::cout << sich_schneidene_Linien[x].at(y) << " ";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (size_t x = 0; x < unoptimierter_DreieckSpeicher.size(); x++)
	{
		std::cout << unoptimierter_DreieckSpeicher[x].x << " " << unoptimierter_DreieckSpeicher[x].y << " " << unoptimierter_DreieckSpeicher[x].z << " " << std::endl;
	}
	std::cout << std::endl;

	std::vector<Vec3i> finaler_DreieckSpeicher;

	for (size_t x = 0; x < unoptimierter_DreieckSpeicher.size(); x++)
	{
		if (finaler_DreieckSpeicher.size() == 0)
		{
			finaler_DreieckSpeicher.push_back(unoptimierter_DreieckSpeicher[x]);
		}
		else
		{
			bool dreieckBereitsEnthalten = false;
			for (size_t y = 0; y < finaler_DreieckSpeicher.size() && !dreieckBereitsEnthalten; y++)
			{
				if (finaler_DreieckSpeicher[y] == unoptimierter_DreieckSpeicher[x])
				{
					dreieckBereitsEnthalten = true;					
				}
			}
			if (!dreieckBereitsEnthalten)
			{
				finaler_DreieckSpeicher.push_back(unoptimierter_DreieckSpeicher[x]);
			}
		}
	}

	for (size_t x = 0; x < finaler_DreieckSpeicher.size(); x++)
	{
		std::cout << finaler_DreieckSpeicher[x].x << " " << finaler_DreieckSpeicher[x].y << " " << finaler_DreieckSpeicher[x].z << " " << std::endl;
	}

	std::cout << std::endl << "Es sind " << finaler_DreieckSpeicher.size() << " Dreiecke vorhanden!" << std::endl << std::endl;

	std::cout << "x\ty\tx\ty\tx\ty" << std::endl;

	for (size_t x = 0; x < finaler_DreieckSpeicher.size(); x++)
	{
		std::vector<Vec2f> intersections;
		intersections.push_back(getSchnittpunkt(linien[finaler_DreieckSpeicher[x].x], linien[finaler_DreieckSpeicher[x].y]));
		intersections.push_back(getSchnittpunkt(linien[finaler_DreieckSpeicher[x].y], linien[finaler_DreieckSpeicher[x].z]));
		intersections.push_back(getSchnittpunkt(linien[finaler_DreieckSpeicher[x].z], linien[finaler_DreieckSpeicher[x].x]));

		for (size_t x = 0; x < intersections.size(); x++)
		{
			std::cout << intersections[x].x << "\t" << intersections[x].y << "\t";
		}
		std::cout << std::endl;
	}

	int s;
	std::cin >> s;
	return 0;
}