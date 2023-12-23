#include <iostream>
#include <fstream>

constexpr unsigned MAX_LEN = 64;//maximum number of chars on one line in the file
unsigned usersLength, numberOfLevels;
char** users;
char*** levels;

char getCharFromDigit(int digit)
{
    if (digit < 0 || digit > 9)
        return '\0';
    return digit + '0';
}

unsigned getNumberLength(unsigned int n)
{

    if (n == 0)
        return 1;
    unsigned int res = 0;

    while (n != 0)
    {
        res++;
        n /= 10;
    }
    return res;
}

void toString(unsigned int n, char* str)
{
    unsigned int len = getNumberLength(n);

    for (int i = len - 1; i >= 0; i--)
    {
        str[i] = getCharFromDigit(n % 10);
        n /= 10;
    }
    str[len] = '\0';
}

int convertCharToDigit(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    return -1;
}

unsigned myAtoi(const char* str)
{
    if (!str)
        return 0;

    unsigned result = 0;
    while (*str)
    {
        int digit = convertCharToDigit(*str);
        if (digit == -1)
            return 0;
        (result *= 10) += digit;
        str++;
    }
    return result;
}

void myStrcpy(const char* source, char* dest)
{
    if (!source || !dest)
        return;
    while (*source)
    {
        *dest = *source;
        dest++;
        source++;
    }
    *dest = '\0';
}

unsigned myStrlen(const char* str)
{
    if (!str)
        return 0;

    unsigned result = 0;
    while (*str)
    {
        result++;
        str++;
    }
    return result;
}

void loadUsers(const char* source)
{
    if (!source)
    {
        return;
    }

    std::ifstream sourceFile(source);
    
    if (!sourceFile.is_open())
    {
        std::cout << "Couldn't open file : " << source << std::endl;
        return;
    }

    char temp[MAX_LEN] = "";
    sourceFile.getline(temp, MAX_LEN);

    usersLength = 2 * myAtoi(temp);
    users = new char* [usersLength];
    unsigned index = 0;

    while (sourceFile)
    {
        sourceFile.getline(temp, MAX_LEN);

        unsigned len = myStrlen(temp);
        if (len != 0)
        {
            users[index] = new char[len+1];//len+1 because of '\0'
            myStrcpy(temp, users[index]);
            index++;
        }
    }

    sourceFile.clear();
    sourceFile.close();
}

void loadLevels(const char* source)
{
    if (!source)
        return;

    std::ifstream sourceFile("levels.txt");

    if (!sourceFile.is_open())
    {
        std::cout << "Couldn't open file : " << source << std::endl;
        return;
    }

    char temp[MAX_LEN] = "";
    sourceFile.getline(temp, MAX_LEN);
    numberOfLevels = myAtoi(temp);//The first line of the file is the number of matrices

    levels = new char** [numberOfLevels];
    unsigned index = 0;

    while (sourceFile)
    {
        sourceFile.getline(temp, MAX_LEN);
        unsigned sizeOfMatrix = myAtoi(temp);//number of rolls and colls

        if (index < numberOfLevels) {
            
            levels[index] = new char* [sizeOfMatrix + 1];//bcs we want the last element to be nullptr
            levels[index][sizeOfMatrix] = nullptr;
            
            for (unsigned i = 0; i < sizeOfMatrix; i++)
            {
                levels[index][i] = new char[sizeOfMatrix+1];
                sourceFile.getline(temp, MAX_LEN);
                myStrcpy(temp, levels[index][i]);
            }
            index++;
        }
    }

    sourceFile.clear();
    sourceFile.close();

}

void deleteMemory()
{
    for (unsigned i = 0; i < usersLength; i++)
        delete[] users[i];
    
    delete[] users;

    for (unsigned i = 0; i < numberOfLevels; i++)
    {
        unsigned index = 0;
        while (levels[i][index] != nullptr)
        {
            delete[] levels[i][index];
            index++;
        }
        delete[] levels[i];
    }
    delete[] levels;
}

void addUser(const char* name, const char* password)
{
    if (!name || !password)
        return;

    char temp[MAX_LEN] = "";
    toString((usersLength+2) / 2, temp);

    std::ofstream file("users.txt");
    
    file << temp<<std::endl;
    
    for (int i = 0; i < usersLength; i++)
        file << users[i]<<std::endl;

    file << name << std::endl;
    file << password;

    file.clear();
    file.close();
}
int main()
{
    loadUsers("Users.txt");
    loadLevels("levels.txt");

    /*for (unsigned i = 0; i < usersLength; i++)
        std::cout << users[i] << std::endl; */

        addUser("Georgi Ivanov", "2401");
        loadUsers("Users.txt");
        /*for (unsigned i = 0; i < numberOfLevels; i++)
        {
        unsigned index = 0;
        while (levels[i][index] != nullptr)
        {
            std::cout << levels[i][index] << std::endl;
            index++;
        }
        std::cout << std::endl;
            
         }*/

deleteMemory();
}

