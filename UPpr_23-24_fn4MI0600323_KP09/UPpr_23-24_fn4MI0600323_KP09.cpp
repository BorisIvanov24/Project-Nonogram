#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>  
#include <time.h> 

constexpr unsigned MAX_LEN = 100, MAX_LEVEL_SIZE = 26;
const char EMPTY_SYMBOL = 249, FILL_SYMBOL = 254;

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

void unsignedToString(unsigned int n, char* str)
{
    if (!str)
        return;

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
        if (*str < '0' || *str>'9')
            return 0;

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

void myStrcat(char* first, const char* second)
{
    if (!first || !second)
        return;

    size_t firstLen = myStrlen(first);
    first += firstLen;
    myStrcpy(second, first);
}

int myStrcmp(const char* first, const char* second)
{
    if (!first || !second)
        return 0; //error

    if ((*first == '\0' && *second == '\0'))
        return 0;

    while ((*first) && (*second) && ((*first) == (*second))) 
    {
        first++;
        second++;
    }

    return (*first - *second);
}

void loadUsers(char**& users, size_t& usersSize, const char* source)
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

    usersSize = 2 * myAtoi(temp);
    users = new char* [usersSize];
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

void deleteMatrix(char** matrix, size_t rows)
{
    for (int i = 0; i < rows; i++)
    {
        delete[] matrix[i];
    }

    delete[] matrix;
}

void setValue(char matrix[][MAX_LEVEL_SIZE], size_t rows, char value)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            matrix[i][j] = value;
        }
        matrix[i][rows] = '\0';
    }
}

void loadMatrixFromFile(char matrix[][MAX_LEVEL_SIZE], size_t rows, char* fileName)
{
    std::ifstream sourceFile(fileName);

    if (!sourceFile.is_open())
    {
        std::cout << "Couldn't open file : " << fileName << std::endl;
        setValue(matrix, rows, ' ');
        return;
    }

    int index = 0;

    while (sourceFile)
    {
        sourceFile.getline(matrix[index], MAX_LEVEL_SIZE);
        index++;
    }

    sourceFile.clear();
    sourceFile.close();
}

void addUser(char** users, size_t usersSize, const char* username, const char* password)
{
    if (!username || !password)
        return;

    char temp[MAX_LEN] = "";
    unsignedToString((usersSize+2) / 2, temp);

    std::ofstream file("textFiles\\Users.txt");
    
    file << temp<<std::endl;
    
    for (int i = 0; i < usersSize; i++)
        file << users[i]<<std::endl;

    file << username << std::endl;
    file << password;

    file.clear();
    file.close();
}

bool isValidUser(char** users, size_t usersSize, const char* username, const char* password)
{
    if (!username || !password||myStrcmp(username, "") == 0|| myStrcmp(username, "") == 0)
        return false;

    for (unsigned i = 0; i < usersSize; i+=2)
    {
        if ((myStrcmp(username, users[i]) == 0)&&
            (myStrcmp(password, users[i+1]) == 0))
        {
            return true;

        }
    }
    return false;
}

bool isUsernameTaken(char** users, size_t usersSize, const char* username)
{
    if (!username)
        return false;

    for (unsigned i = 0; i < usersSize; i += 2)
    {
        if ((myStrcmp(username, users[i]) == 0))
            return true;
    }
    return false;
}

void authenticateUser(char** users, size_t usersSize, char* username, char* password)
{
    char option;
    std::cout << "Do you want to login or register: Press l/r" << std::endl;
    std::cin >> option;

    while (option != 'l' && option != 'r')
    {
        std::cout << "Invalid input, try again."<<std::endl;
        std::cin >> option;
    }

    if (option == 'l') {

        std::cout << "username: ";
        std::cin.ignore();
        std::cin.getline(username, MAX_LEN);
        std::cout << "password: ";
        std::cin.getline(password, MAX_LEN);

        while (!isValidUser(users, usersSize, username, password)) {
            
            std::cout << "Invalid username or password, try again." << std::endl;
            std::cout << "username: ";
            std::cin.getline(username, MAX_LEN);
            std::cout << "password: ";
            std::cin.getline(password, MAX_LEN);
        }
        std::cout << "User logged in successfully" << std::endl;

    }
    else {
            std::cout << "username: ";
            std::cin.ignore();
            std::cin.getline(username, MAX_LEN);
            std::cout << "password: ";
            std::cin.getline(password, MAX_LEN);

            while (isUsernameTaken(users, usersSize, username)||myStrcmp(username, "") == 0||
                   myStrcmp(password, "") == 0)
            {
                if(myStrcmp(username, "") == 0 || myStrcmp(password, "") == 0)
                std::cout << "Invalid username or password, try again." << std::endl;
                else
                std::cout << "Username is taken, try again." << std::endl;
                
                std::cout << "username: ";
                std::cin.getline(username, MAX_LEN);
                std::cout << "password: ";
                std::cin.getline(password, MAX_LEN);
            }
            addUser(users, usersSize, username, password);
            std::cout << "User registered successfully" << std::endl;
        }
}

void calculateNumbersLeft(char levelMatrixPattern[][MAX_LEVEL_SIZE], unsigned numbersLeft[][MAX_LEVEL_SIZE],
                          size_t levelSize, unsigned& maxCols)
{
    for (unsigned i = 0; i < levelSize; i++)
    {
        unsigned counter = 0, index = 0;
        for(int j = 0;j<levelSize;j++)
        {
            numbersLeft[i][j] = 0;

            if (levelMatrixPattern[i][j] == '1')
            {
                counter++;
                if (j == levelSize - 1)
                {
                    numbersLeft[i][index] = counter;
                    index++;
                    // std::cout << numbersTop[i][index] << std::endl;
                }
               // std::cout << counter << std::endl;
            }
            else if (counter != 0)
                {
                    numbersLeft[i][index] = counter;
                   //std::cout << numbersTop[i][index] << std::endl;
                    counter = 0;
                    index++;
                }
        }
        if (index > maxCols)
            maxCols = index;
    }
}

void calculateNumbersTop(char levelMatrixPattern[][MAX_LEVEL_SIZE], unsigned numbersTop[][MAX_LEVEL_SIZE], 
                         size_t levelSize, unsigned& maxRows)
{
    maxRows = 0;
    for (unsigned i = 0; i < levelSize; i++)
    {
        unsigned counter = 0, index = 0;
        for (int j = 0; j < levelSize; j++)
        {
            numbersTop[j][i] = 0;

            if (levelMatrixPattern[j][i] == '1')
            {
                counter++;
                if (j == levelSize - 1)
                {
                    numbersTop[index][i] = counter;
                    index++;
                }
            }
            else if (counter != 0)
            {
                numbersTop[index][i] = counter;
                counter = 0;
                index++;
            }
        }
        if (index > maxRows)
            maxRows = index;
    }
} 

void printLevel(char levelMatrix[][MAX_LEVEL_SIZE], size_t levelSize, const unsigned numbersTop[][MAX_LEVEL_SIZE], 
                size_t numbersTopRows, const unsigned numbersLeft[][MAX_LEVEL_SIZE], 
                size_t numbersLeftCols, unsigned level, unsigned lives)
{
    std::cout <<std::endl<< "Level: " << level << " Lives: "<<lives<<std::endl;

    //print numbersTop
    for (int i = 0; i < numbersTopRows; i++)
    {
        std::cout << std::endl;
        std::cout<<std::setw(2 * numbersLeftCols + 1)<<' ';
        for (int j = 0; j < levelSize; j++)
        {
            if (numbersTop[i][j] != 0)
            {
                if(numbersTop[i][j]>9)
                std::cout << numbersTop[i][j] << "  ";
                else
                std::cout << numbersTop[i][j] << "   ";
            }
            else std::cout << "    ";
        }

    }

    std::cout << std::endl;

    //print numbersLeft and level

    for (int i = 0; i < levelSize; i++)
    {
        unsigned doubleDigits = 0;
        //print numbersLeft
        for (int j = 0; j < numbersLeftCols; j++)
        {
            if (numbersLeft[i][j] != 0)
            {
                if (numbersLeft[i][j] > 9)
                doubleDigits++;
                
                std::cout << numbersLeft[i][j] << ' ';
            }
            else
            {
                if (doubleDigits)
                {
                    std::cout << ' ';
                    doubleDigits--;
                }
                else
                std::cout << "  ";

            }
        }

        for (int j = 0; j < levelSize; j++)
        {
            std::cout << '(' << levelMatrix[i][j] << ") ";
        }
        std::cout << std::endl;
    }
    
}

bool isWinner(char levelMatrix[][MAX_LEVEL_SIZE], char levelMatrixPattern[][MAX_LEVEL_SIZE], size_t levelSize)
{
    for (int i = 0; i < levelSize; i++)
    {
        for (int j = 0; j < levelSize; j++)
        {
            if (levelMatrixPattern[i][j] == '1' && levelMatrix[i][j] != FILL_SYMBOL)
                return false;
        }
    }
    return true;
}

void fillEmpty(char levelMatrix[][MAX_LEVEL_SIZE], char levelMatrixPattern[][MAX_LEVEL_SIZE],
               size_t levelSize)
{
    for (int i = 0; i < levelSize; i++)
    {
        bool match = true;
        for (int j = 0; j < levelSize; j++)
        {
            if (levelMatrixPattern[i][j] == '1' && levelMatrix[i][j] != FILL_SYMBOL)
            {
                match = false;
                break;
            }
        }

        if (match)
        {
            for (int j = 0; j < levelSize; j++)
            {
                if (levelMatrixPattern[i][j] == '0')
                    levelMatrix[i][j] = EMPTY_SYMBOL;
            }
        }
    }

    for (int i = 0; i < levelSize; i++)
    {
        bool match = true;
        for (int j = 0; j < levelSize; j++)
        {
            if (levelMatrixPattern[j][i] == '1' && levelMatrix[j][i] != FILL_SYMBOL)
            {
                match = false;
                break;
            }
        }

        if (match)
        {
            for (int j = 0; j < levelSize; j++)
            {
                if (levelMatrixPattern[j][i] == '0')
                    levelMatrix[j][i] = EMPTY_SYMBOL;
            }
        }
    }

}

bool loadUserData(char levelMatrix[][MAX_LEVEL_SIZE], char levelMatrixPattern[][MAX_LEVEL_SIZE],
                  unsigned& level, unsigned& lives, char* username)
{
    if (!username)
    {
        return 0;
    }

    char fileName[MAX_LEN] = "textFiles\\";
    myStrcat(fileName, username);
    myStrcat(fileName, ".txt");

    std::ifstream sourceFile(fileName);

    if (!sourceFile.is_open())
    {
        std::cout << "This user has not saved any data yet"<<std::endl;
        return false;
    }

    char temp[MAX_LEN] = "";
    
    sourceFile.getline(temp, MAX_LEN);
    level = myAtoi(temp);

    sourceFile.getline(temp, MAX_LEN);
    lives = myAtoi(temp);

    unsigned index = 0, levelSize = level*5;
    while (sourceFile&&index<levelSize)
    {
        sourceFile.getline(temp, MAX_LEN);
        myStrcpy(temp, levelMatrix[index]);
        index++;
    }
    
    index = 0;
    while (sourceFile && index < levelSize)
    {
        sourceFile.getline(temp, MAX_LEN);
        myStrcpy(temp, levelMatrixPattern[index]);
        index++;
    }

    sourceFile.clear();
    sourceFile.close();
    return true;
}

void saveUserData(char levelMatrix[][MAX_LEVEL_SIZE], char levelMatrixPattern[][MAX_LEVEL_SIZE],
                  unsigned& level, unsigned& lives, char* username)
{
    if (!username)
    {
        return;
    }

    char fileName[MAX_LEN] = "textFiles\\";
    myStrcat(fileName, username);
    myStrcat(fileName, ".txt");

    std::ofstream sourceFile(fileName);

    /*if (!sourceFile.is_open())
    {
        std::cout << "This user has not saved any data yet" << std::endl;
        return 0;
    }*/

    char temp[MAX_LEN] = "";
    unsigned levelSize = level * 5;
    
    unsignedToString(level, temp);
    sourceFile << temp << std::endl;

    unsignedToString(lives, temp);
    sourceFile << temp << std::endl;

    for (int i = 0; i < levelSize; i++)
    {
        sourceFile << levelMatrix[i] << std::endl;
        //std::cout << levelMatrix[i] << std::endl << std::endl;
    }

    for (int i = 0; i < levelSize; i++)
    {
        sourceFile << levelMatrixPattern[i] << std::endl;
    }

    sourceFile.clear();
    sourceFile.close();
}

void loadLevelFromFile(unsigned level, char levelMatrixPattern[][MAX_LEVEL_SIZE])
{
    unsigned levelSize = 5 * level;
    char levelName[MAX_LEN] = "textFiles\\&-&.txt";
    levelName[10] = getCharFromDigit(level);
    levelName[12] = getCharFromDigit(1 + rand() % 2);
    loadMatrixFromFile(levelMatrixPattern, levelSize, levelName);
}

void getLevelInformation(char levelMatrix[][MAX_LEVEL_SIZE], char levelMatrixPattern[][MAX_LEVEL_SIZE], 
                         unsigned& level, unsigned& lives, char* username)
{
    char ch = ' ';
    std::cout << "If you want to play new game, press n" << std::endl;
    std::cout << "If you want to load game, press l" << std::endl;
    std::cin >> ch;

    while (ch != 'n' && ch != 'l')
    {
        std::cout << "Invalid command, try again" << std::endl;
        std::cin >> ch;
    }

    switch (ch)
    {
        case'l':
        {
            if (!loadUserData(levelMatrix, levelMatrixPattern, level, lives, username))
            {
                level = 1;
                unsigned levelSize = level * 5;

                lives = levelSize / 2;

                setValue(levelMatrix, levelSize, ' ');
                loadLevelFromFile(level, levelMatrixPattern);
            }
            break;
        }
        case'n':
        {
            level = 1;
            unsigned levelSize = level * 5;

            lives = levelSize / 2;

            setValue(levelMatrix, levelSize, ' ');
            loadLevelFromFile(level, levelMatrixPattern);

            break;
        }
    }
}

void getInputFromUser(char levelMatrix[][MAX_LEVEL_SIZE], char levelMatrixPattern[][MAX_LEVEL_SIZE],
                      size_t levelSize, unsigned& lives, unsigned level, char* username)
{
    char ch = ' ';
    unsigned row = 0, col = 0;
    char rowStr[MAX_LEN] = "", colStr[MAX_LEN] = "";

    std::cout << "\nIf you want to place fill, press f"<< std::endl;
    std::cout << "If you want to place empty, press e" << std::endl;
    std::cout << "If you want to save level, press s" << std::endl;

    std::cin >> ch;
    while (ch != 'f' && ch != 'e')
    {
        if (ch != 's')
        {
            std::cout << "Invalid command, try again!" << std::endl;
            std::cin >> ch;
        }
        else {
                saveUserData(levelMatrix, levelMatrixPattern, level, lives, username);
                std::cout << "Level saved successfully, please enter f or e" << std::endl;
                std::cin >> ch;
             }
    }

    switch (ch)
    {
        case 'f':
        case 'e':
        {
            std::cout << "row: ";
            std::cin >> rowStr;
            std::cout << "col: ";
            std::cin >> colStr;

            row = myAtoi(rowStr);
            col = myAtoi(colStr);
            
            while (row==0||row > levelSize || col==0||col > levelSize || levelMatrix[row-1][col-1] == FILL_SYMBOL ||
                levelMatrix[row-1][col-1] == EMPTY_SYMBOL)
            {
                std::cout << "\nNot valid coordinates, try again!" << std::endl;
                std::cout << "row: ";
                std::cin >> rowStr;
                std::cout << "col: ";
                std::cin >> colStr;
                row = myAtoi(rowStr);
                col = myAtoi(colStr);
            }

            if (ch == 'f')
            {
                if (levelMatrixPattern[row-1][col-1] != '1')
                {
                    std::cout << "\n\nWrong!" << std::endl;
                    lives--;
                }
                else {
                    std::cout << "\n\nCorrect!" << std::endl;
                    levelMatrix[row-1][col-1] = FILL_SYMBOL;
                }
            }
            else
            {
                if (levelMatrixPattern[row-1][col-1] != '0')
                {
                    std::cout << "\n\nWrong!" << std::endl << std::endl;
                    lives--;
                }
                else {
                    std::cout << "\n\nCorrect!" << std::endl << std::endl;
                    levelMatrix[row-1][col-1] = EMPTY_SYMBOL;
                }
            }
            
        }
    }
}

void playNonogram(unsigned level, unsigned& lives, char* username, char levelMatrix[][MAX_LEVEL_SIZE], 
                  char levelMatrixPattern[][MAX_LEVEL_SIZE])
{
    unsigned numbersTop[MAX_LEVEL_SIZE][MAX_LEVEL_SIZE], numbersLeft[MAX_LEVEL_SIZE][MAX_LEVEL_SIZE];
    unsigned numbersLeftCols = 0, numbersTopRows = 0, levelSize = level * 5;


    calculateNumbersTop(levelMatrixPattern, numbersTop, levelSize, numbersTopRows);
    calculateNumbersLeft(levelMatrixPattern, numbersLeft, levelSize, numbersLeftCols);

    while (!isWinner(levelMatrix, levelMatrixPattern, levelSize) && lives>0)
    {
        printLevel(levelMatrix, levelSize, numbersTop, numbersTopRows, numbersLeft, numbersLeftCols, level, lives);
        getInputFromUser(levelMatrix, levelMatrixPattern, levelSize, lives, level, username);

        fillEmpty(levelMatrix, levelMatrixPattern, levelSize);
    }

    if (lives == 0)
    {
        std::cout << "\nYou Lost! :(" << std::endl;
    }
    else
    {
        std::cout << "\nCongratulations, you won! :)" << std::endl;
    }
}

bool askUserContinueToPlayOrLeave(unsigned lives)
{
    if(lives)
    std::cout << "If you want to continue to next level, press n" << std::endl;
    else
    std::cout << "If you want to try again, press t" << std::endl;
    
    std::cout << "If you want to leave app, press l" << std::endl;

    char ch = ' ';

    std::cin >> ch;

    while (ch != 'n' && ch != 'l' && ch!='t')
    {
        std::cout << "Invalid command, try again" << std::endl;
        std::cin >> ch;
    }

    if (ch == 'l')
        return false;

    return true;
}

int main()
{
    size_t usersSize = 0;
    char** users = nullptr; //keeps all users and their passwords
    char username[MAX_LEN] = "", password[MAX_LEN] = "";
    char levelMatrix[MAX_LEVEL_SIZE][MAX_LEVEL_SIZE], levelMatrixPattern[MAX_LEVEL_SIZE][MAX_LEVEL_SIZE];
    unsigned lives = 0, level = 1;
    
    loadUsers(users, usersSize, "textFiles\\Users.txt");
    authenticateUser(users, usersSize, username, password);
    deleteMatrix(users, usersSize);  //Deleting memory

    srand((unsigned)time(NULL));

    getLevelInformation(levelMatrix, levelMatrixPattern, level, lives, username);

    while (true)
    {
        playNonogram(level, lives, username, levelMatrix, levelMatrixPattern);

        if (!askUserContinueToPlayOrLeave(lives))
            break;

        unsigned levelSize = level * 5;
        if (lives)
        {
            level++;
            levelSize = level * 5;
            lives = levelSize / 2;
            loadLevelFromFile(level, levelMatrixPattern);
            setValue(levelMatrix, levelSize, ' ');
        }
        else
        {
            lives = levelSize / 2;
        }
    }
}
