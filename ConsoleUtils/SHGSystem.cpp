#include "SHGSystem.h"
#include "SHGSystemCommandStem.h"
#include "../Exceptions/InvalidCommandException.h"
#include "../Utils/Utils.h"
#include "../Utils/Pair.hpp"
#include "../Data/Admin.h"
#include "../Data/Player.h"
#include "../Data/SuperheroHolder.h"
#include "../Utils/Logger.h"

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
            Logger::LogMessage(ex.what());
            continue;
        }
        catch (std::exception &ex)
        {
            Logger::LogError(ex.what());
            continue;
        }
        catch (...)
        {
            Logger::LogFatal("Unknown error occurred!", -3);
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
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Admin)
                    AddPlayer();
                break;
            case SHGSystemCommandType::DeletePlayer:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Admin)
                    DeletePlayer();
                break;
            case SHGSystemCommandType::Market:
                Market();
                break;
            case SHGSystemCommandType::Players:
                ShowPlayers();
                break;
            case SHGSystemCommandType::Buy:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Player)
                    BuyHero();
                break;
            case SHGSystemCommandType::Stance:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Player)
                    ChangeStance();
                break;
            case SHGSystemCommandType::Attack:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Player)
                    AttackPlayer();
                break;
            case SHGSystemCommandType::Baltop:
                Baltop();
                break;
            case SHGSystemCommandType::Upgrade:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Player)
                    UpgradeHero();
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
        Logger::LogError("You must be logged out to issue this command!");
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

    Optional<User *> user = ParseUser(file);
    if (!user.HasValue())
        throw std::runtime_error("Invalid user file format!");

    if (hashedPassword != user.GetData()->GetPassword())
    {
        Logger::LogError("Wrong password!");
        return;
    }

    if (user.GetData()->GetUserType() == UserType::Player && !SuperheroHolder::GetInstance().HasHeroes())
    {
        Logger::LogError("When no heroes are present, only admins can login!");
        return;
    }

    m_loggedUser = user.GetData();

    if (!m_loggedUser) throw std::runtime_error("Something went wrong while logging in!");

    std::cout << "Welcome, " << m_loggedUser->GetUsername() << "!\n\n";

    file.close();
}

void SHGSystem::Logout(bool bypass)
{
    static const Array<String> answers(new String[]{"yes", "no", "y", "n"}, 4, true);

    if (!m_loggedUser)
    {
        Logger::LogError("You must be logged in to issue this command!");
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

    CheckIfAdminAccountExists();
    SetupHeroes();
}
void SHGSystem::CheckIfAdminAccountExists() const
{
    std::ifstream file("usernames.dat", std::ios::binary);

    //layout:
    //  <type: A or P>|<hashed username>
    //examples:
    //  A|3254755
    //  P|6533

    if (!file.is_open())
    {
        //make the user to make an admin account!
        Logger::LogError("Cannot find an admin account! You must create one now!");
        while (!CreateAdmin());
        return;
    }

    int counter = 0;
    bool adminExists = false;
    while (!file.eof())
    {
        String line;
        size_t len;
        file.read((char *) &len, sizeof(size_t));
        String::ReadExact(file, len, &line);

        if (file.eof())
            break;

        Array<String> split = line.Split('|');
        split[0].ToLower();

        if (split[0] == "a")
        {
            adminExists = true;
            break;
        } else if (split[0] == "p")
        {}
        else
            throw std::runtime_error((String("Invalid username at line: ") + counter).c_str());

        counter++;
    }

    file.close();

    if (!adminExists)
    {
        Logger::LogError("Cannot find an admin account! You must create one now!");
        while (!CreateAdmin());
        std::cout << "\nSuccessfully created user!\n\n";
    }

}

void SHGSystem::SetupHeroes() const
{
    std::ifstream file("sh.dat", std::ios::binary);

    if (!file.is_open())
    {
        Logger::LogWarning("No heroes are present! Contact an admin.");
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
        Logger::LogError("Invalid first name!");
        return false;
    }

    std::cout << "Enter last name: ";
    String::GetLine(std::cin, &lastName);
    if (!Utils::ValidateName(lastName))
    {
        Logger::LogError("Invalid last name!");
        return false;
    }

    std::cout << "Enter email: ";
    String::GetLine(std::cin, &email);
    if (!Utils::ValidateEmail(email))
    {
        Logger::LogError("Invalid email!");
        return false;
    }

    std::cout << "Enter username: ";
    String::GetLine(std::cin, &username);
    if (!Utils::ValidateUsername(username))
    {
        Logger::LogError("Invalid username!");
        return false;
    }

    std::cout << "Enter password: ";
    String::GetLine(std::cin, &password);
    if (!Utils::ValidatePassword(password))
    {
        Logger::LogError("Invalid password!");
        return false;
    }


    String usernameHash = Utils::HashString(username);

    if (!AddAccountToFile(UserType::Admin, usernameHash))
    {
        Logger::LogError("Cannot add user to user file!");
        return false;
    }

    String passwordHash = Utils::HashString(password);

    std::ofstream file(("./users/" + usernameHash + ".dat").c_str(), std::ios::binary);

    if (!file.is_open())
    {
        Logger::LogError("Cannot create admin file for user \"" + username + "\"!");
        return false;
    }

    size_t usernameLen = username.GetSize() + 1;
    size_t passLen = passwordHash.GetSize() + 1;
    size_t fnLen = firstName.GetSize() + 1;
    size_t lnLen = lastName.GetSize() + 1;
    size_t emLen = email.GetSize() + 1;

    UserType type = UserType::Admin;

    file.write((const char *) &type, sizeof(UserType));

    file.write((const char *) &passLen, sizeof(size_t));
    file.write(passwordHash.c_str(), passLen);

    file.write((const char *) &usernameLen, sizeof(size_t));
    file.write(username.c_str(), usernameLen);

    file.write((const char *) &fnLen, sizeof(size_t));
    file.write(firstName.c_str(), fnLen);

    file.write((const char *) &lnLen, sizeof(size_t));
    file.write(lastName.c_str(), lnLen);

    file.write((const char *) &emLen, sizeof(size_t));
    file.write(email.c_str(), emLen);

    file.close();

    return true;
}
bool SHGSystem::CreatePlayer() const
{
    String firstName, lastName, email, username, password;
    std::cout << "Enter first name: ";
    String::GetLine(std::cin, &firstName);
    if (!Utils::ValidateName(firstName))
    {
        Logger::LogError("Invalid first name!");
        return false;
    }

    std::cout << "Enter last name: ";
    String::GetLine(std::cin, &lastName);
    if (!Utils::ValidateName(lastName))
    {
        Logger::LogError("Invalid last name!");
        return false;
    }

    std::cout << "Enter email: ";
    String::GetLine(std::cin, &email);
    if (!Utils::ValidateEmail(email))
    {
        Logger::LogError("Invalid email!");
        return false;
    }

    std::cout << "Enter username: ";
    String::GetLine(std::cin, &username);
    if (!Utils::ValidateUsername(username))
    {
        Logger::LogError("Invalid username!");
        return false;
    }

    std::cout << "Enter password: ";
    String::GetLine(std::cin, &password);
    if (!Utils::ValidatePassword(password))
    {
        Logger::LogError("Invalid password!");
        return false;
    }

    String usernameHash = Utils::HashString(username);

    if (!AddAccountToFile(UserType::Player, usernameHash))
    {
        Logger::LogError("Cannot add user to user file!");
        return false;
    }

    String passwordHash = Utils::HashString(password);

    std::ofstream file(("./users/" + usernameHash + ".dat").c_str(), std::ios::binary);

    if (!file.is_open())
    {
        Logger::LogError("Cannot create player file for user \"" + username + "\"!");
        return false;
    }

    size_t usernameLen = username.GetSize() + 1;
    size_t passLen = passwordHash.GetSize() + 1;
    size_t fnLen = firstName.GetSize() + 1;
    size_t lnLen = lastName.GetSize() + 1;
    size_t emLen = email.GetSize() + 1;

    UserType type = UserType::Player;

    file.write((const char *) &type, sizeof(UserType));

    file.write((const char *) &passLen, sizeof(size_t));
    file.write(passwordHash.c_str(), passLen);

    file.write((const char *) &usernameLen, sizeof(size_t));
    file.write(username.c_str(), usernameLen);

    file.write((const char *) &fnLen, sizeof(size_t));
    file.write(firstName.c_str(), fnLen);

    file.write((const char *) &lnLen, sizeof(size_t));
    file.write(lastName.c_str(), lnLen);

    file.write((const char *) &emLen, sizeof(size_t));
    file.write(email.c_str(), emLen);

    double money = 0;
    file.write((const char *) &money, sizeof(double));

    size_t heroesCount = 0;
    file.write((const char *) &heroesCount, sizeof(size_t));

    file.close();

    return true;
}
bool SHGSystem::AddAccountToFile(UserType type, const String &usernameHash) const
{
    if (usernameHash.IsEmpty())
        return false;

    std::ifstream file("usernames.dat", std::ios::binary);

    if (!file.is_open()) //possibly the file doesn't exist, so create one?
    {
        std::ofstream f("usernames.dat", std::ios::binary);
        String line;
        line.Append(type == UserType::Admin ? "A|" : "P|");

        line.Append(usernameHash);

        size_t len = line.GetSize() + 1; // null terminator

        f.write((const char *) &len, sizeof(size_t));
        f.write(line.c_str(), len);
        f.close();
        return true;
    }

    Array<String> usernameHashes;
    while (!file.eof())
    {
        String line;
        size_t len;
        file.read((char *) &len, sizeof(size_t));
        String::ReadExact(file, len, &line);
        if (!file.eof())
            break;

        Array<String> split = line.Split('|');
        if (split.IsEmpty())
            continue;

        usernameHashes.Add(std::move(split[1]));
    }

    file.close();

    bool userExists = false;
    for (size_t i = 0; i < usernameHashes.GetLength(); i++)
    {
        if (usernameHashes[i] == usernameHash)
        {
            userExists = true;
            break;
        }
    }

    if (!userExists)
    {
        std::ofstream f("usernames.dat", std::ios::binary | std::ios::app);

        String line;
        line.Append(type == UserType::Admin ? "A|" : "P|");

        line.Append(usernameHash);

        size_t len = line.GetSize() + 1; // null terminator

        f.write((const char *) &len, sizeof(size_t));
        f.write(line.c_str(), len);

        f.close();

        return true;
    }

    return false;
}
void SHGSystem::AddPlayer()
{
    while (!CreatePlayer())
    {
        static const Array<String> answers(new String[]{"yes", "no", "y", "n"}, 4, true);

        static String response = "";
        if ((answers.Contains(response) && (response == "no" || response == "n")))
        {
            Logger::LogMessage("Aborting creating new player!");
            break;
        }
    }
}
void SHGSystem::DeletePlayer()
{

}
void SHGSystem::Market()
{
    Array<size_t> heroes = SuperheroHolder::GetInstance().GetMarketHeroes();
    if (heroes.IsEmpty()) //empty, try to load from disk!
    {
        SuperheroHolder::GetInstance().LoadMarket();
        heroes = SuperheroHolder::GetInstance().GetMarketHeroes();
        if (heroes.IsEmpty()) // empty? maybe the file is not found or is corrupt! Exit
            return;
    }

    std::cout << "Market: \n";
    for (int i = 0; i < heroes.GetLength(); ++i)
    {
        Superhero hero = SuperheroHolder::GetInstance().GetSuperhero(heroes[i]);
        std::cout << "\t" << hero.GetNickname() << " → $" << hero.GetCost() <<
                  "\t\tElement: " <<
                  (hero.GetElement() == SuperheroElements::Earth ?
                   "♥" : hero.GetElement() == SuperheroElements::Fire ?
                         "♦" : "♣") << "\n";
    }
    std::cout << "\n";
}
void SHGSystem::ShowPlayers()
{
    if (!m_loggedUser)
        return;

    Array<User *> users;

    std::ifstream file("usernames.dat", std::ios::binary);

    while (!file.eof())
    {
        String line;
        size_t len;
        file.read((char *) &len, sizeof(size_t));
        String::ReadExact(file, len, &line);

        if (file.eof())
            break;

        Array<String> split = line.Split('|');
        if (split.IsEmpty())
            continue;

        std::ifstream userFile(("./users/" + split[1] + ".dat").c_str(), std::ios::binary);

        Optional<User *> user = ParseUser(userFile);
        if (user.HasValue())
            users.Add(user.GetData());

        userFile.close();
    }

    file.close();

    std::cout << "Players: \n";

    switch (m_loggedUser->GetUserType())
    {
        case UserType::Admin:
            for (int i = 0; i < users.GetLength(); ++i)
            {
                if (users[i]->GetUserType() == UserType::Admin)
                    std::cout << "\t[Admin] " << users[i]->GetUsername() <<
                              "\n\t\t" << users[i]->GetEmail() << "\n";
                else if (users[i]->GetUserType() == UserType::Player)
                {
                    std::cout << "\t[Player] " << users[i]->GetUsername() << " : $" <<
                              ((Player *) users[i])->GetMoney() <<
                              "\n\t\t" << users[i]->GetFirstName() << " " << users[i]->GetLastName() <<
                              "\n\t\t" << users[i]->GetEmail() <<
                              "\n\t\t[ ";

                    Array<size_t> heroes = ((Player *) users[i])->GetSuperheroes();
                    if (heroes.IsEmpty())
                    {
                        std::cout << "No Heroes ]";
                        continue;
                    }

                    for (int j = 0; j < heroes.GetLength(); ++j)
                    {
                        std::cout <<
                                  SuperheroHolder::GetInstance().GetSuperhero(heroes[i]).GetNickname() <<
                                  (j == heroes.GetLength() - 1 ? " " : ", ");
                    }
                    std::cout << "]";
                }
            }
            std::cout << "\n";
            break;
        case UserType::Player:
            for (int i = 0; i < users.GetLength(); ++i)
            {
                if (users[i]->GetUserType() == UserType::Admin)
                    continue;
                else if (users[i]->GetUserType() == UserType::Player)
                    std::cout << "\t" << users[i]->GetUsername() << " : $" <<
                              ((Player *) users[i])->GetMoney();
            }
            std::cout << "\n";
            break;
        default:
            Logger::LogError("Unknown user type!");
            break;
    }
}
void SHGSystem::BuyHero()
{

}
void SHGSystem::ChangeStance()
{

}
void SHGSystem::AttackPlayer()
{

}
void SHGSystem::UpgradeHero()
{

}
void SHGSystem::Baltop()
{
    Array<User *> users;

    std::ifstream file("usernames.dat", std::ios::binary);

    while (!file.eof())
    {
        String line;
        size_t len;
        file.read((char *) &len, sizeof(size_t));
        String::ReadExact(file, len, &line);

        if (file.eof())
            break;

        if (file.eof())
            break;

        Array<String> split = line.Split('|');
        if (split.IsEmpty())
            continue;

        std::ifstream userFile(("./users/" + split[1] + ".dat").c_str(), std::ios::binary);

        Optional<User *> user = ParseUser(userFile);
        if (user.HasValue() && user.GetData()->GetUserType() == UserType::Player)
            users.Add(user.GetData());

        userFile.close();
    }

    file.close();

    if (users.IsEmpty())
    {
        Logger::LogError("No players are present!");
        return;
    }

    //bubble sort
    for (int i = 0; i < users.GetLength() - 1; ++i)
    {
        for (int j = i + 1; j < users.GetLength(); ++j)
        {
            if (((Player *) users[i])->GetMoney() > ((Player *) users[j])->GetMoney())
            {
                User *temp = users[i];
                users[i] = users[j];
                users[j] = temp;
            }
        }
    }

    std::cout << "Top balance: \n";
    for (int i = 0; i < users.GetLength(); ++i)
    {
        std::cout << "\t" << users[i]->GetUsername() << " : $" <<
                  ((Player *) users[i])->GetMoney() << "\n";
    }
    std::cout << "\n\n";
}
Optional<User *> SHGSystem::ParseUser(std::ifstream &file)
{
    UserType type;
    file.read((char *) &type, sizeof(UserType));

    size_t passwordLen;
    file.read((char *) &passwordLen, sizeof(size_t));

    String hashedPassword;
    String::ReadExact(file, passwordLen, &hashedPassword);

    size_t usernameLen;
    file.read((char *) &usernameLen, sizeof(size_t));

    String username;
    String::ReadExact(file, usernameLen, &username);

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

        return Optional<User *>(new Admin(firstName, lastName, email, username, hashedPassword));
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
        return Optional<User *>(new Player(*pl));
    }

    return Optional<User *>();
}

