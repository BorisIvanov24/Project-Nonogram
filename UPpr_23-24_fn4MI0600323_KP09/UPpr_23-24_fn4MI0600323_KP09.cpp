#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

constexpr unsigned MAX_LEN = 100, MAX_LEVEL_SIZE = 30;//maximum number of chars on one line in the file
unsigned usersSize, numberOfLevels, lives;
char** users; //keeps all users and their passwords
char*** levels; //keeps all levels
char username[MAX_LEN] = "", password[MAX_LEN] = "";
char EMPTY_SYMBOL = 249, FILL_SYMBOL = 254;


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

int myStrcmp(const char* first, const char* second)
{
    if (!first || !second)
        return 0; //error

    while ((*first) && (*second) && ((*first) == (*second))) 
    {
        first++;
        second++;
    }

    return (*first - *second);

}

void inputInRange(unsigned& num, unsigned left, unsigned right)
{
    std::cin >> num;

    while (num < left || num>right)
    {
        std::cout << "Invalid input, try again!" << std::endl;
        std::cin >> num;
    }
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

void deleteCharMatrix(char** arr, unsigned rows)
{
    for (int i = 0; i < rows; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
}

void deleteMemory() //deletes levels and users
{
   
    deleteCharMatrix(users, usersSize);

    for (unsigned i = 0; i < numberOfLevels; i++)
    {
        unsigned index = 0;
        while (levels[i][index] != nullptr)
        {
            index++;
        }
        deleteCharMatrix(levels[i], index + 1);
    }
    delete[] levels;
}

void addUser(const char* username, const char* password)
{
    if (!username || !password)
        return;

    char temp[MAX_LEN] = "";
    unsignedToString((usersSize+2) / 2, temp);

    std::ofstream file("users.txt");
    
    file << temp<<std::endl;
    
    for (int i = 0; i < usersSize; i++)
        file << users[i]<<std::endl;

    file << username << std::endl;
    file << password;

    file.clear();
    file.close();
}

bool isValidUser(const char* username, const char* password)
{
    if (!username || !password)
        return;

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

bool isUsernameTaken(const char* username)
{
    if (!username)
        return;

    for (unsigned i = 0; i < usersSize; i += 2)
    {
        if ((myStrcmp(username, users[i]) == 0))
            return true;
    }
    return false;
}

void authenticateUser()
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

        while (!isValidUser(username, password)) {
            
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

            while (isUsernameTaken(username))
            {
                std::cout << "Username is taken, try again." << std::endl;
                std::cout << "username: ";
                std::cin.getline(username, MAX_LEN);
                std::cout << "password: ";
                std::cin.getline(password, MAX_LEN);
            }
            addUser(username, password);
            std::cout << "User registered successfully" << std::endl;
        }
}

void copyMatrix(char** source, char**& dest, unsigned& size)
{
    while (source[size] != nullptr)
        size++;

    dest = new char* [size + 1];
    dest[size] = nullptr;

    for (int i = 0; i < size; i++)
    {
        dest[i] = new char[size + 1]; //for \0

        if (!source[i])               //check for nullptr
            continue;

        myStrcpy(source[i], dest[i]);
    }
}

void setValue(char** matrix, size_t rows, char value)
{
    for (int i = 0; i < rows; i++)
    {
        if (!matrix[i])            //check for nullptr
            continue;

        for (int j = 0; j < rows; j++)
        {
            matrix[i][j] = value;
        }
    }
}

void calculateNumbersLeft(unsigned level, unsigned numbersLeft[][MAX_LEVEL_SIZE], size_t levelSize, unsigned& maxCols)
{

    for (unsigned i = 0; i < levelSize; i++)
    {
        unsigned counter = 0, index = 0;
        for(int j = 0;j<levelSize;j++)
        {
            numbersLeft[i][j] = 0;

            if (levels[level][i][j] == '1')
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

void calculateNumbersTop(unsigned level, unsigned numbersTop[][MAX_LEVEL_SIZE], size_t levelSize, unsigned& maxRows)
{

    for (unsigned i = 0; i < levelSize; i++)
    {
        unsigned counter = 0, index = 0;
        for (int j = 0; j < levelSize; j++)
        {
            numbersTop[j][i] = 0;

            if (levels[level][j][i] == '1')
            {
                counter++;
                if (j == levelSize - 1)
                {
                    numbersTop[index][i] = counter;
                    index++;
                    // std::cout << numbersTop[i][index] << std::endl;
                }
                // std::cout << counter << std::endl;
            }
            else if (counter != 0)
            {
                numbersTop[index][i] = counter;
                //std::cout << numbersTop[i][index] << std::endl;
                counter = 0;
                index++;
            }
        }
        if (index > maxRows)
            maxRows = index;
    }
} 

void printLevel(unsigned level, char** levelMatrix, size_t levelSize, const unsigned numbersTop[][MAX_LEVEL_SIZE], 
                size_t numbersTopRows, const unsigned numbersLeft[][MAX_LEVEL_SIZE], size_t numbersLeftCols)
{
    std::cout <<std::endl<< "Level: " << level << " Lives: "<<lives<<" Press s to save level." <<std::endl;

    //print numbersTop
    for (int i = 0; i < numbersTopRows; i++)
    {
        std::cout << std::endl;
        std::cout<<std::setw(2 * numbersLeftCols + 1)<<' ';
        for (int j = 0; j < levelSize; j++)
        {
            if (numbersTop[i][j] != 0)
                std::cout <<numbersTop[i][j]<<"   ";
            else std::cout << "    ";
        }

    }

    std::cout << std::endl;
    //print numbersLeft and level

    for (int i = 0; i < levelSize; i++)
    {
        //print numbersLeft
        for (int j = 0; j < numbersLeftCols; j++)
        {
            if (numbersLeft[i][j] != 0)
                std::cout << numbersLeft[i][j] << ' ';
            else std::cout << "  ";
        }

        for (int j = 0; j < levelSize; j++)
        {
            std::cout << '(' << levelMatrix[i][j] << ") ";
        }
        std::cout << std::endl;
    }
    
}

bool isWinner(char** levelMatrix, size_t levelSize, unsigned level)
{
    for (int i = 0; i < levelSize; i++)
    {
        for (int j = 0; j < levelSize; j++)
        {
            if (levels[level][i][j] == '1' && levelMatrix[i][j] != FILL_SYMBOL)
                return false;
        }
    }
    return true;
}

void fillEmpty(char** levelMatrix, size_t levelSize, unsigned level)
{
    for (int i = 0; i < levelSize; i++)
    {
        bool match = true;
        for (int j = 0; j < levelSize; j++)
        {
            if (levels[level][i][j] == '1' && levelMatrix[i][j] != FILL_SYMBOL)
            {
                match = false;
                break;
            }
        }

        if (match)
        {
            for (int j = 0; j < levelSize; j++)
            {
                if (levels[level][i][j] == '0')
                    levelMatrix[i][j] = EMPTY_SYMBOL;
            }
        }
    }

    for (int i = 0; i < levelSize; i++)
    {
        bool match = true;
        for (int j = 0; j < levelSize; j++)
        {
            if (levels[level][j][i] == '1' && levelMatrix[j][i] != FILL_SYMBOL)
            {
                match = false;
                break;
            }
        }

        if (match)
        {
            for (int j = 0; j < levelSize; j++)
            {
                if (levels[level][j][i] == '0')
                    levelMatrix[j][i] = EMPTY_SYMBOL;
            }
        }
    }

}

void inputCommand(char** levelMatrix, size_t levelSize, unsigned level)
{
    char ch = ' ';
    unsigned row = 0, col = 0;
    std::cout << "\nf row col: fills block\ne row col: places empty symbol" << std::endl;
    
    std::cin >> ch>>row>>col;
    while ((ch != 'f' && ch != 'e') || row > levelSize - 1 || col > levelSize - 1 ||
            levelMatrix[row][col] == EMPTY_SYMBOL || levelMatrix[row][col] == FILL_SYMBOL)
    {
        if (levelMatrix[row][col] == EMPTY_SYMBOL || levelMatrix[row][col] == FILL_SYMBOL)
        {
            std::cout << "You already filled this cell, try again" << std::endl;
        }
        else {
            std::cout << "Invalid command, try again!" << std::endl;
        }
        
        std::cin >>ch>>row>>col;

    }
    
    if (ch == 'f')
    {
        if (levels[level][row][col] != '1')
            {
                std::cout << "\nWrong!" <<std::endl;
                lives--;
            }
        else {
            std::cout << "\nCorrect!"<<std::endl;
            levelMatrix[row][col] = FILL_SYMBOL;
        }
    }
    else
    {
        if (levels[level][row][col] != '0')
        {
            std::cout << "Wrong!" << std::endl<<std::endl;
            lives--;
        }
        else {
            std::cout << "Correct!"<<std::endl<<std::endl;
            levelMatrix[row][col] = EMPTY_SYMBOL;
        }
    }
}

void play()
{
    unsigned level, levelSize = 0;
    char** levelMatrix = nullptr;

    std::cout << "\nSelect level " << 0 << " - " << numberOfLevels-1<<std::endl;
    
    inputInRange(level, 0, numberOfLevels-1);

    copyMatrix(levels[level], levelMatrix, levelSize);//returns levelSize without the nullptr
    setValue(levelMatrix, levelSize, ' ');
    lives = levelSize / 2;

    unsigned numbersTop[MAX_LEVEL_SIZE][MAX_LEVEL_SIZE], numbersLeft[MAX_LEVEL_SIZE][MAX_LEVEL_SIZE],
             numbersLeftCols = 0, numbersTopRows = 0;

    calculateNumbersLeft(level, numbersLeft, levelSize, numbersLeftCols);
    calculateNumbersTop(level, numbersTop, levelSize, numbersTopRows);
    
    while (!isWinner(levelMatrix, levelSize, level))
    {
        printLevel(level, levelMatrix, levelSize, numbersTop, numbersTopRows, numbersLeft, numbersLeftCols);
        inputCommand(levelMatrix, levelSize, level);
        
        if (lives == 0)
        {
            std::cout << "\nYou Lost! :(\n";
            break;
        }

        fillEmpty(levelMatrix, levelSize, level);

    }
    printLevel(level, levelMatrix, levelSize, numbersTop, numbersTopRows, numbersLeft, numbersLeftCols);


    if(lives!=0)
    std::cout << "Congratulations you won!\n";
}

int main()
{
    loadUsers("Users.txt");
    loadLevels("Levels.txt");
    
    authenticateUser();
    
    while (1)
    {
        play();
    }

deleteMemory();
}

