//ignore all the comments
//they are notes i made for myself whiel learning

#include <chrono>
#include <exception>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <unordered_set>
#include <limits>
#include <fstream>
#include <algorithm>

const std::string GREEN = "\033[38;5;82m";
const std::string RED = "\033[38;5;196m";
const std::string YELLOW = "\033[38;5;220m";
const std::string MENU_PURPLE = "\033[38;5;141m";

const std::string RESET_COLOR = "\033[0m";

// måste komma ihåg ts
// & TECKNET BETYDER SKAPA REFERENS
// utan & kopieras variable
// fast med & så blir det en referens till originalet
// const + & blir read only
// med & blir det snabbare

void savePassword(const std::string &password, const std::string &name);
void catFile(const std::string &path2JagVetefan);

const std::string path = "../data/passwlist";

int main(int argc, char *argv[])
{
  std::cout << "" << std::endl;
  // defaults
  int length = 12;
  bool useIntegers = false;
  bool basicSymbols = false;
  bool advSymbols = false;
  bool flagParsed = false;
  // this set is for handling errors for unknown flags
  std::unordered_set<std::string> knownFlags = {
      "-l", "--length",
      "-b", "--basic-symbols",
      "-a", "--advanced-symbols",
      "-i", "--integers",
      "-h", "--help"};

  // in c++ u gotta increment i by urself, as long as i < argc
  // here I set i to equal 1, otherwise it will
  // see the "pwgen" command as a flag
  for (int i = 1; i < argc; ++i)
    {
    std::string arg = argv[i];

    if (arg == "-l" || arg == "--length") {
      flagParsed = true;

      if (i + 1 >= argc) {
        std::cout << RED << "error: length cant be parsed without an integer... exiting" << RESET_COLOR << std::endl;
        return 1;
      }

      int iHaveToVerifyLength;

      try {
        iHaveToVerifyLength = std::stoi(argv[i + 1]);
      }

      catch (const std::exception &) {
        std::cout << RED << "length gotta be passed with a number..." << RESET_COLOR << std::endl;
        return 1;
      }

      if (iHaveToVerifyLength >= 35)
      {
        std::cout << YELLOW << "info: password will be long, continue? yes or no" << RESET_COLOR << std::endl;

        std::cout << "-> ";
        std::string ask;
        std::cin >> ask;

        if (ask[0] != 'y' && ask[0] != 'Y')
        {
          std::cout << "ok..." << std::endl;
          return 1;
        }
        else
        {
          std::cout << "" << std::endl;
        }
        length = iHaveToVerifyLength;
      }
    }

    else if (arg == "-b" || arg == "--basic-symbols") {
      flagParsed = true;
      basicSymbols = true;
    }

    else if (arg == "-a" || arg == "--advanced-symbols") {
      flagParsed = true;
      advSymbols = true;
    }

    else if (arg == "-i" || arg == "--integers") {
      flagParsed = true;
      useIntegers = true;
    }

    else if (arg == "--list") {
      catFile(path);
      return 0;
    }

    else if (arg == "-h" || arg == "--help")
    {
      std::cout << MENU_PURPLE
                << "usage: pwgen [flags] (COPYRIGHT ADDENATOR 2026)\n"
                << "flags:\n"
                << "  -l, --length <n>         sets password length (default: 12)\n"
                << "  -i  --integers           include integers\n"
                << "  -b, --basic-symbols      include basic symbols\n"
                << "  -a  --advanced-symbols   include advanced symbols (these are usually blocked)\n"
                << "  -h, --help               show this help menu\n"
                << "  -s, --save               save password to file\n"
                << "  --list                   list saved passwords\n"
                << "                                                 \n"
                << "  if you get an error about the password file, make\n"
                << "  sure that /data/passwlist exists and is writable\n"
                << RESET_COLOR;
      return 0;
    }
    // if the arg string isn't empty, if char[0] of args is a flag, if not knownFlags
    else if (!arg.empty() && arg[0] == '-' && !knownFlags.count(arg))
    {
      std::cout << RED << "unknown flag parsed... exiting" << RESET_COLOR;
      return 1;
    };

  } // end for loop

  if (flagParsed == false)
  {
    std::cout << YELLOW << "info: no flags parsed, using default values\n"
              << RESET_COLOR << std::endl;
  }

  static std::random_device hw_random;       // seed
  static std::mt19937 generate(hw_random()); // generate with seed

  std::string charPool = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if (useIntegers)
    charPool += "1234567890";

  if (basicSymbols)
    charPool += "!@#$%^&*-_";

  if (advSymbols)
    charPool += "<>{}[]()\\//|`';";

  // idiot compiler ger warning jag skiter i, conversion skapar kaos
  std::uniform_int_distribution<int> distributed(0, charPool.size() - 1);

  std::string password;
  for (int i = 0; i < length; ++i)
  {
    password += charPool[distributed(generate)];
  }

  std::cout << GREEN << "password generated, press enter to reveal\n"
            << RESET_COLOR;
  // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cin.get();
  std::cout << password << "\n"
            << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  std::string name;
  std::string save;
  std::cout << MENU_PURPLE << "save password?" << GREEN << " yes" << RESET_COLOR << " or " << RED << "no\n"
            << RESET_COLOR;
  std::cout << "->";
  std::cin >> save;

  while (save.empty())
  {
    std::cout << RED << "error: answer cant be empty\n"
              << RESET_COLOR;
    std::cout << "->";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);
    // std::cin.clear();
  }

  std::transform(save.begin(), save.end(), save.begin(), ::tolower);

  if (save[0] == 'y')
  {
    std::cout << "\nenter a name for ur password...\n-> ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);
    // std::cin.clear();

    while (name.empty())
    {
      std::cout << RED << "error: answer cant be empty\n"
                << RESET_COLOR;
      std::cout << "->";
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::getline(std::cin, name);
      std::cin.clear();
    }

    savePassword(password, name);
  }
  else
  {
    std::cout << "ok..." << RESET_COLOR << std::endl;
  }
}

void savePassword(const std::string &password, const std::string &name)
{
  // ofstream is for writing
  std::ofstream passwordFile(path, std::ios::app); // idk why why only app works

  if (!passwordFile)
  {
    std::cout << RED << "error opening password file... see help menu" << RESET_COLOR << std::endl;
    return;
  }

  passwordFile << name << " = " << password << "\n";

  if (!passwordFile)
  {
    std::cout << RED << "error writing to password file... see help menu" << RESET_COLOR << std::endl;
    return;
  }

  passwordFile.close();

  std::cout << GREEN << "\npassword saved" << RESET_COLOR << std::endl;
}

void catFile(const std::string &path2JagVetefan)
{
  // en till dräng warning jag skiter i det funkar ok

  const std::ifstream passwordFile(path2JagVetefan);
  if (!passwordFile)
  {
    std::cout << RED << "error opening password file... see help menu" << RESET_COLOR << std::endl;
    return;
  }
  std::cout << GREEN << "saved passwords:\n"
            << RESET_COLOR;
  std::cout << passwordFile.rdbuf() << std::endl;
}
