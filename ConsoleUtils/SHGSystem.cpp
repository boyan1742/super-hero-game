#include "SHGSystem.h"
#include "SHGSystemCommandStem.h"
#include "../Exceptions/InvalidCommandException.h"
#include "../Utils/Utils.h"
#include "../Utils/Pair.hpp"
#include "../Data/Admin.h"
#include "../Data/Player.h"
#include "../Data/SuperheroHolder.h"

#include <iostream>

SHGSystem SHGSystem::m_instance;

void SHGSystem::Run()
{
    Setup();

    bool quit = false;
    while (!quit)
    {
        std::cout << "> ";
        String line;
        String::GetLine(std::cin, &line);
        Array<String> chunks = line.Split(' ');

        SHGSystemCommandType cmdType;
        try
        {
            cmdType = SHGSystemCommandStem::GetCommandType(chunks[0]);
        }
        catch (InvalidCommandException &ex)
        {
            std::cout << ex.what() << std::endl;
            continue;
        }
        catch (std::exception &ex)
        {
            std::cout << ex.what() << std::endl;
            continue;
        }
        catch (...)
        {
            std::cout << "Unknown error occurred!\n";
            continue;
        }

        switch (cmdType)
        {
            case SHGSystemCommandType::Quit:
                quit = true;
                break;
            case SHGSystemCommandType::Help:
                if (!m_loggedUser)
                    PrintHelpMessage();
                else
                {
                    if (m_loggedUser->GetUserType() == UserType::Admin)
                        PrintHelpMessageAdmin();
                    else
                        PrintHelpMessagePlayer();
                }
                break;
            case SHGSystemCommandType::Login:
                Login();
                break;
            case SHGSystemCommandType::Logout:
                Logout();
                break;
            case SHGSystemCommandType::AddPlayer:
                break;
            case SHGSystemCommandType::DeletePlayer:
                break;
            case SHGSystemCommandType::Market:
                break;
            case SHGSystemCommandType::Players:
                break;
            case SHGSystemCommandType::Buy:
                break;
            case SHGSystemCommandType::Stance:
                break;
            case SHGSystemCommandType::Attack:
                break;
            case SHGSystemCommandType::Baltop:
                break;
            case SHGSystemCommandType::Upgrade:
                break;
        }
    }

    if (m_loggedUser)
        Logout(true);
}
void SHGSystem::PrintHelpMessagePlayer() const
{
    std::cout <<
              "Commands: \n" <<
              "\tHelp - shows this screen.\n" <<
              "\tLogin - logs into the system.\n" <<
              "\tLogout - logs out of the system.\n" <<
              "\tMarket - shows which heroes are for sale in the market.\n" <<
              "\tPlayers - shows all players' statistics.\n" <<
              "\tBuy - buys a hero from the market.\n" <<
              "\tAttack - attacks a player.\n" <<
              "\tBaltop - shows top 3 players with most money.\n" <<
              "\tUpgrade - upgrades a hero using player's money.\n";
}
void SHGSystem::PrintHelpMessage() const
{
    std::cout <<
              "Commands: \n" <<
              "\tHelp - shows this screen.\n" <<
              "\tLogin - logs into the system.\n" <<
              "\tLogout - logs out of the system.\n";
}
void SHGSystem::PrintHelpMessageAdmin() const
{
    std::cout <<
              "Commands: \n" <<
              "\tHelp - shows this screen.\n" <<
              "\tLogin - logs into the system.\n" <<
              "\tLogout - logs out of the system.\n" <<
              "\tAddPlayer - adds new player.\n" <<
              "\tDeletePlayer - deletes a player.\n" <<
              "\tMarket - shows which heroes are for sale in the market.\n" <<
              "\tPlayers - shows all players' statistics.\n";
}
SHGSystem &SHGSystem::GetInstance()
{
    return m_instance;
}

void SHGSystem::Login()
{
    if (m_loggedUser)
    {
        std::cout << "You must be logged out to issue this command!";
        return;
    }

    std::cout << "Enter username: ";
    String username = "";
    String::GetLine(std::cin, &username);
    std::cout << "Enter password: ";
    String password = "";
    String::GetLine(std::cin, &password);

    String hashedUsername = Utils::HashString(username);
    String hashedPassword = Utils::HashString(password);

    std::ifstream file(("./users/" + hashedUsername + ".dat").c_str(), std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Cannot find user with username: \"" << username << "\"!\n";
        return;
    }

    UserType type;
    file.read((char *) &type, sizeof(UserType));

    size_t passwordLen;
    file.read((char *) &passwordLen, sizeof(size_t));

    String filePassword;
    String::ReadExact(file, passwordLen, &filePassword);

    if (filePassword != hashedPassword)
    {
        std::cout << "Invalid password!\n";
        return;
    }

    if (type == UserType::Admin)
    {
        //layout:
        //          fnSize|firstName|lnSize|lastName|eSize|email

        String firstName, lastName, email;

        size_t fnSize, lnSize, eSize;

        file.read((char *) &fnSize, sizeof(size_t));
        String::ReadExact(file, fnSize, &firstName);
        file.read((char *) &lnSize, sizeof(size_t));
        String::ReadExact(file, lnSize, &lastName);
        file.read((char *) &eSize, sizeof(size_t));
        String::ReadExact(file, eSize, &email);

        m_loggedUser = new Admin(firstName, lastName, email, username, hashedPassword);
    } else if (type == UserType::Player)
    {
        //layout:
        //          fnSize|firstName|lnSize|lastName|eSize|email|money|shSize|{SuperHero[shSize]}

        String firstName, lastName, email;

        size_t fnSize, lnSize, eSize, shSize;

        file.read((char *) &fnSize, sizeof(size_t));
        String::ReadExact(file, fnSize, &firstName);
        file.read((char *) &lnSize, sizeof(size_t));
        String::ReadExact(file, lnSize, &lastName);
        file.read((char *) &eSize, sizeof(size_t));
        String::ReadExact(file, eSize, &email);

        double money;
        file.read((char *) &money, sizeof(double));

        file.read((char *) &shSize, sizeof(size_t));
        Array<size_t> heroes(shSize);

        for (size_t i = 0; i < shSize; i++)
        {
            size_t id;
            file.read((char *) &id, sizeof(size_t));
            heroes.Add(id);
        }

        Player *pl = new Player(firstName, lastName, email, username, hashedPassword, money);
        pl->AddHeroes(heroes);
        m_loggedUser = pl;

    } else throw std::runtime_error(("Invalid file format for user: " + username).c_str());

    if (!m_loggedUser) throw std::runtime_error("Something went wrong while logging in!");

    std::cout << "Welcome " << m_loggedUser->GetUsername() << "!\n\n";

    file.close();
}

void SHGSystem::Logout(bool bypass)
{
    static const Array<String> answers(new String[]{"yes", "no", "y", "n"}, 4, true);

    if (!m_loggedUser)
    {
        std::cout << "You must be logged in to issue this command!";
        return;
    }

    static String response = "";
    if (!bypass)
    {
        std::cout << "Are you sure you want to sign out?\n";
        String::GetLine(std::cin, &response);
        response.ToLower();
    }

    if ((answers.Contains(response) && (response == "yes" || response == "y")) || bypass)
    {
        std::cout << "Logged out user: \"" << m_loggedUser->GetUsername() << "\"!\n";
        delete m_loggedUser;
        m_loggedUser = nullptr;
    }
}
void SHGSystem::Setup()
{
    //Check if there are superheroes.
    //Load if any.
    //Check if there is an admin account.
    //If not, prompt the user to make a new account.

    SetupHeroes();
    CheckIfAdminAccountExists();
}
void SHGSystem::CheckIfAdminAccountExists() const
{
    std::ifstream file("usernames.dat", std::ios::binary);

    //layout:
    //  <type: A or P>|<hashed username>
    //  A|3254755
    //  P|6533

    if (!file.is_open())
    {
        //make the user to make an admin account!
        std::cout << "Cannot find an admin account! You must create one now!\n\n";
        while (!CreateAdmin());
        return;
    }

    Array<Pair<UserType, String>> users;
    int i = 0;
    while (!file.eof())
    {
        String line;
        size_t len;
        file.read((char *) &len, sizeof(size_t));
        String::ReadExact(file, len, &line);

        UserType type;
        Array<String> split = line.Split('|');
        split[0].ToLower();

        if (split[0] == "a")
            type = UserType::Admin;
        else if (split[0] == "p")
            type = UserType::Player;
        else
            throw std::runtime_error(((String) "Invalid username at line: " + i).c_str());

        users.Add(Pair(std::move(type), std::move(split[1])));

        i++;
    }

    file.close();

    bool adminExists = false;
    for (size_t i = 0; i < users.GetLength(); i++)
    {
        if (users[i].GetFirst() == UserType::Admin)
        {
            adminExists = true;
            break;
        }
    }

    if (!adminExists)
        while (!CreateAdmin());

}

void SHGSystem::SetupHeroes() const
{
    std::ifstream file("sh.dat", std::ios::binary);

    if (!file.is_open())
    {
        std::cout << "[Warning] No heroes are present!\n";
        return;
    }

    size_t len;
    file.read((char *) &len, sizeof(size_t));

    for (int i = 0; i < len; ++i)
    {
        //layout:
        //          hUniqueID|hfnSize|hFirstName|hlnSize|hLastName|hnSize|hNickname|hElement|hMode|hPower|hCost

        String hFirstName, hLastName, hNickname;
        SuperheroElements hElement;
        SuperheroMode hMode;
        unsigned hPower;
        double hCost;

        size_t hfnSize, hlnSize, hnSize, hUniqueID;

        file.read((char *) &hUniqueID, sizeof(size_t));

        file.read((char *) &hfnSize, sizeof(size_t));
        String::ReadExact(file, hfnSize, &hFirstName);
        file.read((char *) &hlnSize, sizeof(size_t));
        String::ReadExact(file, hlnSize, &hLastName);
        file.read((char *) &hnSize, sizeof(size_t));
        String::ReadExact(file, hnSize, &hNickname);

        file.read((char *) &hElement, sizeof(SuperheroElements));
        file.read((char *) &hMode, sizeof(SuperheroMode));
        file.read((char *) &hPower, sizeof(unsigned));
        file.read((char *) &hCost, sizeof(double));

        SuperheroHolder::GetInstance().AddSuperhero(
                Superhero(hUniqueID, hFirstName, hLastName,
                          hNickname, hElement, hPower, hCost));
    }

    file.close();
}
bool SHGSystem::CreateAdmin() const
{
    String firstName, lastName, email, username, password;
    std::cout << "Enter first name: ";
    String::GetLine(std::cin, &firstName);
    if (!Utils::ValidateName(firstName))
    {
        std::cout << "Invalid first name!\n";
        return false;
    }

    std::cout << "Enter last name: ";
    String::GetLine(std::cin, &lastName);
    if (!Utils::ValidateName(lastName))
    {
        std::cout << "Invalid last name!\n";
        return false;
    }

    std::cout << "Enter email: ";
    String::GetLine(std::cin, &email);
    if (!Utils::ValidateEmail(email))
    {
        std::cout << "Invalid email!\n";
        return false;
    }

    std::cout << "Enter username: ";
    String::GetLine(std::cin, &username);
    if (!Utils::ValidateUsername(username))
    {
        std::cout << "Invalid username!\n";
        return false;
    }

    std::cout << "Enter password: ";
    String::GetLine(std::cin, &password);
    if (!Utils::ValidatePassword(password))
    {
        std::cout << "Invalid password!\n";
        return false;
    }

    String usernameHash = Utils::HashString(username);

    std::ofstream file(("./users/" + usernameHash + ".dat").c_str(), std::ios::binary);

    if (!file.is_open())
    {
        std::cout << "Cannot create admin file for user \"" << username << "\"!\n";
        return false;
    }

    size_t passLen = password.GetSize() + 1;
    size_t fnLen = firstName.GetSize() + 1;
    size_t lnLen = lastName.GetSize() + 1;
    size_t emLen = email.GetSize() + 1;

    UserType type = UserType::Admin;

    file.write((const char *) &type, sizeof(UserType));

    file.write((const char *) &passLen, sizeof(size_t));
    file.write(Utils::HashString(password).c_str(), passLen);

    file.write((const char *) &fnLen, sizeof(size_t));
    file.write(firstName.c_str(), fnLen);

    file.write((const char *) &lnLen, sizeof(size_t));
    file.write(lastName.c_str(), lnLen);

    file.write((const char *) &emLen, sizeof(size_t));
    file.write(email.c_str(), emLen);

    file.close();

    AddAccountToFile(UserType::Admin, usernameHash);

    return true;
}
bool SHGSystem::AddAccountToFile(UserType type, const String &usernameHash) const
{
    if (usernameHash.IsEmpty())
        return false;

    std::fstream file("usernames.dat", std::ios::in | std::ios::out | std::ios::binary);

    if (!file.is_open())
    {
        return false;
    }

    Array<String> users;
    int i = 0;
    while (!file.eof())
    {
        String line;
        String::GetLine(file, &line);

        Array<String> split = line.Split('|');

        users.Add(std::move(split[1]));

        i++;
    }

    bool userExists = false;
    for (size_t i = 0; i < users.GetLength(); i++)
    {
        if (users[i] == usernameHash)
        {
            userExists = true;
            break;
        }
    }

    if (!userExists)
    {
        String line;
        line.Append(type == UserType::Admin ? "A|" : "P|");

        line.Append(usernameHash);

        size_t len = line.GetSize() + 1; // null terminator

        file.write((const char *) &len, sizeof(size_t));
        file.write(line.c_str(), len);

        file.close();

        return true;
    }

    file.close();

    return false;
}
