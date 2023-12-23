#include <iostream>
#include <fstream>

constexpr unsigned MAX_LEN = 64;//maximum length of one line in the file
unsigned usersLength;
char** users;

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
int main()
{
    loadUsers("Users.txt");
    for (unsigned i = 0; i < usersLength; i++)
        std::cout << users[i] << std::endl;

    for (unsigned i = 0; i < usersLength; i++)
        delete[] users[i];
    delete[] users;
}

