#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

//#define n 3


struct Sklad
{
    std::string name;
    float cost;
    int quantity;
    std::string type;
};

struct Students
{
    std::string name;
    std::string surname;
    int math;
    int informatic;
    int physic;

};
struct LectureNote {
    std::string discipline;
    std::string direction;
    std::string lecturer;
    int pages;
};


void EnterItems(Sklad* sklad, const int size)
{
    for (int i = 0; i < size; i ++ )
    {
        std::cout << "Enter name of item : ";
        std::cin >> sklad[i].name;
        std::cout << "\n";
        std::cout << "Enter cost of item : ";
        std::cin >> sklad[i].cost;
        std::cout << "\n";
        std::cout << "Enter a quantity of item :";
        std::cin >> sklad[i].quantity;
        std::cout << "\n";
        std::cout << "Enter a Type of units :" << "kg , l , g , units"<<"\n";
        std::cin >> sklad[i].type;
   }
}

void SearchItem(Sklad *sklad, const int size, std::string name)
{
    double totalprice = 0;
    for (int i = 0; i < size; i++)
    {
       if(name==sklad[i].name)
        {
            std::cout << "Name : "<<sklad[i].name<<"\n";
            std::cout << " Cost : " << sklad[i].cost << "\n";
            std::cout << "Quantity : " << sklad[i].quantity << "\n";
            std::cout << "Type of units : " << sklad[i].type << "\n";
            for (int j = 0; j < sklad[i].quantity; j++)
            {
                totalprice += sklad[i].cost;
            }
            std::cout << "Total price : " << totalprice << std::endl;

        }
       else if (i == size)
       {
           std::cout << "Can't found this item ";
       }
    }
}

void EnterData(Students *students, const int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << "Enter name :" << std::endl;
        std::cin >> students[i].name;
        std::cout << "Enter surname :" << std::endl;
        std::cin >> students[i].surname;
        std::cout << "Enter math grade :" << std::endl;
        std::cin >> students[i].math;
        std::cout << "Enter informatic grade :" << std::endl;
        std::cin >> students[i].informatic;
        std::cout << "Enter physics grade :" << std::endl;
        std::cin >> students[i].physic;
    }
}

void GetMiddleGrade(Students* students, const int size)
{
    int math = 0;
    int informatic = 0;
    int physics = 0;

    for (int i = 0; i < size; i++)
    {
        math += students[i].math;
        informatic += students[i].informatic;
        physics += students[i].physic;
    }
    math = math / size;
    informatic = informatic / size;
    physics = physics / size;

    std::cout << "Average grade of Math is : " << math << std::endl;
    std::cout << "Average grade of Informatic is : " << informatic<< std::endl;
    std::cout << "Average grade of Physics is : " << physics<< std::endl;

    int Mindex = -1;
    int Iindex = -1;
    int Pindex = -1;

    for (int i = 0; i < size; i++)
    {
        if (students[i].math > math)
            Mindex = i;
        if (students[i].informatic > informatic)
            Iindex = i;
        if (students[i].physic > physics)
            Pindex = i;
    }

    if (Mindex != -1)
        std::cout << students[Mindex].surname << " has a higher grade in Math" << std::endl;
    if (Iindex != -1)
        std::cout << students[Iindex].surname << " has a higher grade in Informatic" << std::endl;
    if (Pindex != -1)
        std::cout << students[Pindex].surname << " has a higher grade in Physics" << std::endl;
}

void addLectureNote(std::ofstream& catalogFile, const LectureNote& note) {
    catalogFile << note.discipline << '\n';
    catalogFile << note.direction << '\n';
    catalogFile << note.lecturer << '\n';
    catalogFile << note.pages << '\n';
}

std::string getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}

std::string calculateChecksum(std::ifstream& catalogFile) {
    std::string content;
    char c;
    while (catalogFile.get(c)) {
        content += c;
    }
    return content;
}

bool checkFileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void addEntryToFile(const std::string& filename, const LectureNote& note, const std::string& additionalText) {
    std::ofstream catalogFile(filename, std::ios::app);
    if (!catalogFile) {
        std::cerr << "Failed to open catalog file." << std::endl;
        return;
    }

    addLectureNote(catalogFile, note);
    catalogFile << additionalText << '\n';
    catalogFile.close();
}

int main()
{
    const std::string catalogFilename = "lecture_catalog.txt";
    const std::string checksumFilename = "checksum.txt";

    if (!checkFileExists(catalogFilename)) {
        std::ofstream catalogCreationFile(catalogFilename);
        if (!catalogCreationFile) {
            std::cerr << "Failed to create catalog file." << std::endl;
            return 1;
        }
        catalogCreationFile.close();
    }

    LectureNote note;

    std::cout << "Enter discipline: ";
    std::getline(std::cin >> std::ws, note.discipline);
    std::cout << "Enter direction: ";
    std::getline(std::cin >> std::ws, note.direction);
    std::cout << "Enter lecturer: ";
    std::getline(std::cin >> std::ws, note.lecturer);
    std::cout << "Enter number of pages: ";
    std::cin >> note.pages;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter additional text: ";
    std::string additionalText;
    std::getline(std::cin >> std::ws, additionalText);

    addEntryToFile(catalogFilename, note, additionalText);
    std::ofstream checksumFile(checksumFilename);
    if (!checksumFile) {
        std::cerr << "Failed to create checksum file." << std::endl;
        return 1;
    }

    std::ifstream catalogReadFile(catalogFilename);
    if (!catalogReadFile) {
        std::cerr << "Failed to open catalog file for checksum calculation." << std::endl;
        return 1;
    }

    std::string checksum = calculateChecksum(catalogReadFile);
    catalogReadFile.close();

    std::string currentDate = getCurrentDateTime();

    checksumFile << checksum << '\n';
    checksumFile << "Checksum created on: " << currentDate << '\n';
    checksumFile.close();

    std::cout << "Catalog entry and checksum created successfully." << std::endl;

    return 0;

/* ////////////////////////////// 2 EX
    const int n = 5;
    Students students[n];

    EnterData(students, n);
    GetMiddleGrade(students, n);


*/

    /*
    * ///////////////////// 1 EX
    const int size = 2;
    Sklad shop[size];

    EnterItems(shop, size);

    std::cout << "Enter name of item :" << std::endl;
    std::string name;
    std::cin >> name;
    SearchItem(shop, size,name);

*/


    /*
    * /////////////////////////////////////Example code
    int i, f = 0, obrz;
    struct
    {
        char FIO[20];
        char predmet[20];
        int kol_peresdach;
    } dolg[n];

    memset(&dolg, 0, sizeof(dolg));

    for (i = 0; i < n; i++)
    {
        printf("\nEnter FIO: ");
        scanf_s("%19s", dolg[i]->FIO, sizeof(dolg[i]->FIO));
        printf("Enter predmet: ");
        scanf_s("%19s", dolg[i]->predmet, sizeof(dolg[i]->predmet));
        printf("Enter kol_peresdach: ");
        scanf_s("%d", &dolg[i]->kol_peresdach);
    }

    printf("\nEnter obrz: ");
    scanf_s("%d", &obrz);

    for (i = 0; i < n; i++)
    {
        if (dolg[i]->kol_peresdach == obrz)
        {
            f++;
            printf("\n%15s\t%15s\t%3i", dolg[i]->FIO
                , dolg[i]->predmet, dolg[i]->kol_peresdach);
        }
    }

    if (f == 0)
        printf("\nError");

    _getch();
    return 0;
    */
}