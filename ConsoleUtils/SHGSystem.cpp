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
#include <fcntl.h>
#include <io.h>

SHGSystem SHGSystem::m_instance;

void SHGSystem::PrintWelcomeMessage() const
{
    static const int welcomeMsgBoxWidth = 42;

    std::cout << (char) 201;
    for (int i = 0; i < welcomeMsgBoxWidth; ++i)
        std::cout << (char) 205;
    std::cout << (char) 187 << "\n";

    std::cout << (char) 186;
    for (int i = 0; i < welcomeMsgBoxWidth; ++i)
        std::cout << " ";
    std::cout << (char) 186 << "\n";

    std::cout << (char) 186;
    _setmode(_fileno(stdout), _O_U16TEXT);
    std::wcout << L" \u2666 Welcome to S U P E R  HERO  G A M E! \u2666 ";
    _setmode(_fileno(stdout), _O_TEXT);
    std::cout << (char) 186 << "\n";

    std::cout << (char) 186;
    for (int i = 0; i < welcomeMsgBoxWidth; ++i)
        std::cout << " ";
    std::cout << (char) 186 << "\n";

    std::cout << (char) 200;
    for (int i = 0; i < welcomeMsgBoxWidth; ++i)
        std::cout << (char) 205;
    std::cout << (char) 188 << "\n\n";
}

void SHGSystem::Run()
{
    PrintWelcomeMessage();

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
                else
                    Logger::LogMessage("You must be logged in as an admin to execute this command!");
                break;
            case SHGSystemCommandType::DeletePlayer:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Admin)
                    DeletePlayer(String(""));
                else
                    Logger::LogMessage("You must be logged in as an admin to execute this command!");
                break;
            case SHGSystemCommandType::AddAdmin:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Admin)
                    CreateAdmin();
                else
                    Logger::LogError("Only admins can add other admins.");
                break;
            case SHGSystemCommandType::TerminateProfile:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Player)
                    DeletePlayer(m_loggedUser->GetUsername());
                else
                    Logger::LogError("Only players can delete their profiles.");
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
                else
                    Logger::LogMessage("You must be logged in as a player to execute this command!");
                break;
            case SHGSystemCommandType::Stance:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Player)
                    ChangeStance();
                else
                    Logger::LogMessage("You must be logged in as a player to execute this command!");
                break;
            case SHGSystemCommandType::Attack:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Player)
                    AttackPlayer();
                else
                    Logger::LogMessage("You must be logged in as a player to execute this command!");
                break;
            case SHGSystemCommandType::Baltop:
                Baltop();
                break;
            case SHGSystemCommandType::Upgrade:
                if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Player)
                    UpgradeHero();
                else
                    Logger::LogMessage("You must be logged in as a player to execute this command!");
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

    if (!HasUserInUsersFile(hashedUsername))
    {
        Logger::LogError("Unknown user!");
        return;
    }

    std::ifstream file(("./users/" + hashedUsername + ".dat").c_str(), std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Cannot find user with username: \"" << username << "\"!\n";
        return;
    }

    Optional<UniquePtr<User>> user = ParseUser(file);
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

    m_loggedUser = user.LendData().Release();

    if (!m_loggedUser) throw std::runtime_error("Something went wrong while logging in!");

    std::cout << "Welcome, " << m_loggedUser->GetUsername() << "!\n\n";

    if (m_loggedUser->GetUserType() == UserType::Admin && !SuperheroHolder::GetInstance().HasHeroes())
        AddFirstTimeHeroes();

    if (m_loggedUser->GetUserType() == UserType::Player)
    {
        if (!m_loggedPlayers.Contains(m_loggedUser->GetUsername()))
        {
            m_loggedPlayers.Add(m_loggedUser->GetUsername());
            dynamic_cast<Player *>(m_loggedUser)->AddMoney(150);
        }

        if (m_loggedPlayers.GetLength() == m_playerCount)
            m_loggedPlayers = Array<String>();
    }

    file.close();
}
void SHGSystem::Logout(bool bypass)
{
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

    if ((response == "yes" || response == "y") || bypass)
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
    SuperheroHolder::GetInstance().LoadSuperheroes();
    m_playerCount = GetPlayersCount();
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
        } else if (split[0] == "p") {}
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

void SHGSystem::AddFirstTimeHeroes() const
{
    std::cout << "No heroes are present! So you must add 3 heroes manually.\n";

    for (int i = 0; i < 3; ++i)
    {
        Optional<Superhero> hero = CreateHero();
        if (!hero.HasValue()) // the creation failed! repeat!
        {
            i--;
            continue;
        }

        size_t id = hero.GetData().GetUniqueID();
        SuperheroHolder::GetInstance().AddSuperhero(std::move(hero.LendData()));
        SuperheroHolder::GetInstance().AddHeroIDToMarket(id);
        std::cout << "\n------------------------------\n";
    }

    SuperheroHolder::GetInstance().SaveSuperheroes(); //save the superheroes to file, so we don't repeat this step next time!
    SuperheroHolder::GetInstance().SaveMarket(); //save the market to file, so we have correct market file!
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

    double money = 2500;
    file.write((const char *) &money, sizeof(double));

    size_t heroesCount = 0;
    file.write((const char *) &heroesCount, sizeof(size_t));

    file.close();

    return true;
}
Optional<Superhero> SHGSystem::CreateHero() const
{
    String firstName, lastName, nickname, element;
    unsigned power;
    double cost;

    std::cout << "Enter first name: ";
    String::GetLine(std::cin, &firstName);
    std::cout << "Enter last name: ";
    String::GetLine(std::cin, &lastName);
    std::cout << "Enter nickname: ";
    String::GetLine(std::cin, &nickname);
    std::cout << "Enter element: ";
    String::GetLine(std::cin, &element);

    SuperheroElements elem = SuperheroElements::Fire;
    element.ToLower();
    if (!Utils::ParseSuperHeroElement(element, &elem))
    {
        Logger::LogError(R"(Unknown element! Only allowed options: "fire", "water", "earth"! Entered: )" + element);
        return Optional<Superhero>();
    }


    std::cout << "Enter power: ";
    std::cin >> power;
    std::cout << "Enter cost: ";
    std::cin >> cost;

    std::cin.ignore();

    if (cost < 0)
    {
        Logger::LogError("The cost of a hero must be >= 0!");
        return Optional<Superhero>();
    }

    //find new id → generate id, if this id exists in the collection, generate new id. Repeat the process until a free id is found!
    size_t id = rand();
    while (!SuperheroHolder::GetInstance().IsUniqueIDFree(id))
        id = rand();

    return Optional<Superhero>(Superhero(id, firstName, lastName, nickname, elem, power, cost));
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
        if (file.eof())
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
Optional<UniquePtr<User>> SHGSystem::ParseUser(std::ifstream &file) const
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

        return Optional<UniquePtr<User>>(new Admin(firstName, lastName, email, username, hashedPassword));
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
        Array<Pair<size_t, SuperheroMode>> heroes(shSize);

        for (size_t i = 0; i < shSize; i++)
        {
            //layout:
            //          hUniqueID|hMode

            size_t id;
            SuperheroMode mode;

            file.read((char *) &id, sizeof(size_t));
            file.read((char *) &mode, sizeof(SuperheroMode));
            heroes.Add(Pair(id, mode));
        }

        Player pl = Player(firstName, lastName, email, username, hashedPassword, money);
        pl.AddHeroes(heroes);
        return Optional<UniquePtr<User>>(new Player(pl));
    }

    return Optional<UniquePtr<User>>();
}

void SHGSystem::AddPlayer() const
{
    while (!CreatePlayer())
    {
        static String response = "";

        std::cout << "\nTry again?  ";

        std::cin >> response;
        response.ToLower();

        if (response != "yes" || response != "y")
        {
            Logger::LogMessage("Aborting creating new player!");
            break;
        }
    }
}
void SHGSystem::DeletePlayer(String username) const
{
    if (username == "")
    {
        std::cout << "Enter username that you want to delete: ";
        String::GetLine(std::cin, &username);
    }
    String hashedUsername = Utils::HashString(username);

    std::ifstream file("usernames.dat", std::ios::binary);

    if (!file.is_open())
    {
        Logger::LogError("Users file not found!");
        return;
    }

    Array<Pair<UserType, String>> usernameHashes;
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

        if (split[0] == "P" && split[1] == hashedUsername)
            continue;

        usernameHashes.Add(Pair(split[0] == "A" ? UserType::Admin : UserType::Player, std::move(split[1])));
    }

    file.close();

    if (usernameHashes.IsEmpty())
        return;

    std::ofstream f("usernames.dat", std::ios::binary);

    for (int i = 0; i < usernameHashes.GetLength(); ++i)
    {
        String line;
        line.Append(usernameHashes[i].GetFirst() == UserType::Admin ? "A|" : "P|");

        line.Append(usernameHashes[i].GetSecond());

        size_t len = line.GetSize() + 1; // null terminator

        f.write((const char *) &len, sizeof(size_t));
        f.write(line.c_str(), len);
    }

    f.close();
}
void SHGSystem::Market() const
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
        const Superhero &hero = SuperheroHolder::GetInstance().GetSuperhero(heroes[i]);
        std::cout << "\t" << "[" << (hero.GetElement() == SuperheroElements::Earth ?
                                     "Earth" : hero.GetElement() == SuperheroElements::Fire ?
                                               "Fire" : "Water") << "] " <<
                  hero.GetNickname() << " | $" << hero.GetCost() << "\n";
    }
    std::cout << "\n";

    if (m_loggedUser && m_loggedUser->GetUserType() == UserType::Admin) // Ask to add new heroes.
    {
        std::cout << "Would you like to add a new hero to the market? ";
        String response;
        String::GetLine(std::cin, &response);
        response.ToLower();

        if (response == "y" || response == "yes")
        {
            Optional<Superhero> hero = CreateHero();
            while (!hero.HasValue())
            {
                std::cout << "\nTry again? ";
                String::GetLine(std::cin, &response);

                response.ToLower();

                if (response == "n" || response == "no")
                    return;

                hero = CreateHero();
            }

            size_t id = hero.GetData().GetUniqueID();
            SuperheroHolder::GetInstance().AddSuperhero(std::move(hero.LendData()));
            SuperheroHolder::GetInstance().AddHeroIDToMarket(id);

            SuperheroHolder::GetInstance().SaveSuperheroes();
        }
    }

}
void SHGSystem::ShowPlayers() const
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

        Optional<UniquePtr<User>> user = ParseUser(userFile);
        if (user.HasValue())
            users.Add(user.LendData().Release());

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

                    Array<Pair<size_t, SuperheroMode>> heroes = ((Player *) users[i])->GetSuperheroes();
                    if (heroes.IsEmpty())
                    {
                        std::cout << "No Heroes ]";
                        continue;
                    }

                    for (int j = 0; j < heroes.GetLength(); ++j)
                    {
                        std::cout <<
                                  SuperheroHolder::GetInstance().GetSuperhero(heroes[i].GetFirst()).GetNickname() <<
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
                              ((Player *) users[i])->GetMoney() <<
                              "\n\t\t[ ";

                Array<Pair<size_t, SuperheroMode>> heroes = ((Player *) users[i])->GetSuperheroes();
                if (heroes.IsEmpty())
                {
                    std::cout << "No Heroes ]";
                    continue;
                }

                for (int j = 0; j < heroes.GetLength(); ++j)
                {
                    std::cout <<
                              SuperheroHolder::GetInstance().GetSuperhero(heroes[i].GetFirst()).GetNickname() <<
                              (j == heroes.GetLength() - 1 ? " " : ", ");
                }
                std::cout << "]";
            }
            std::cout << "\n";
            break;
        default:
            Logger::LogError("Unknown user type!");
            break;
    }

    for (int i = 0; i < users.GetLength(); ++i)
        delete users[i];
}
void SHGSystem::BuyHero() const
{
    Array<size_t> heroes = SuperheroHolder::GetInstance().GetMarketHeroes();
    for (int i = 0; i < heroes.GetLength(); ++i)
    {
        const Superhero &hero = SuperheroHolder::GetInstance().GetSuperhero(heroes[i]);
        std::cout << "\t" << i + 1 << ". [" << (hero.GetElement() == SuperheroElements::Earth ?
                                                "Earth" : hero.GetElement() == SuperheroElements::Fire ?
                                                          "Fire" : "Water") << "] " <<
                  hero.GetNickname() << " | $" << hero.GetCost() << "\n";
    }
    std::cout << "\nEnter the number of hero you want to buy. ";

    size_t num;
    std::cin >> num;

    if (num == 0 || num > heroes.GetLength())
    {
        Logger::LogError("Invalid number!");
        return;
    }

    num--;
    Player *pl = dynamic_cast<Player *>(m_loggedUser);
    if (!pl)
    {
        Logger::LogError("Not a player?!?!?!?");
        return;
    }

    if (pl->HasSuperhero(heroes[num]))
    {
        Logger::LogError("You already have this hero!");
        return;
    }

    pl->AddHero(heroes[num], SuperheroMode::Attack);
    SuperheroHolder::GetInstance().RemoveHeroIDFromMarket(heroes[num]);
}

void SHGSystem::ChangeStance() const
{
    Player *player = dynamic_cast<Player *>( m_loggedUser);
    if (!player)
    {
        Logger::LogError("Entered `ChangeStance` but user wasn't player!?!?!?");
        return;
    }

    Array<Pair<size_t, SuperheroMode>> sh = player->GetSuperheroes();
    for (size_t i = 0; i < sh.GetLength(); ++i)
    {
        std::cout << "\t" << i + 1 << ". "
                  << SuperheroHolder::GetInstance().GetSuperhero(sh[i].GetFirst()).GetNickname() <<
                  " | " << (sh[i].GetSecond() == SuperheroMode::Attack ? "Attack\n" : "Defence\n");
    }

    std::cout << "\nType the number of the hero you would like to change the stance of: ";
    size_t id;
    std::cin >> id; // here id will be [1; sh.Length()]

    if (id == 0 || id > sh.GetLength())
    {
        Logger::LogError(String("Invalid position specified! Must be between 1 and ") + sh.GetLength() + "!");
        return;
    }

    std::cout << "Stance: [Attack/Defence]";
    String option;
    String::GetLine(std::cin, &option);

    option.ToLower();

    SuperheroMode mode;
    if (!Utils::ParseSuperHeroMode(option, &mode))
    {
        Logger::LogError(R"(Cannot parse given stance option! Expected "attack" or "defence", received: )" + option);
        return;
    }

    id--;
    player->ChangeHeroStance(sh[id].GetFirst(), mode);
}

static bool IsDominant(SuperheroElements element1, SuperheroElements element2)
{
    switch (element1)
    {
        case SuperheroElements::Fire:
            if (element2 == SuperheroElements::Earth)
                return true;
            else
                return false;
        case SuperheroElements::Earth:
            if (element2 == SuperheroElements::Water)
                return true;
            else
                return false;
        case SuperheroElements::Water:
            if (element2 == SuperheroElements::Fire)
                return true;
            else
                return false;
        default :
            return false;
    }
}

/*WIP*/ void SHGSystem::AttackPlayer() const
{
    std::cout << "Enter player's enemyUsername that you want to attack: ";
    String enemyUsername;
    String::GetLine(std::cin, &enemyUsername);

    String hashedEnemyUsername = Utils::HashString(enemyUsername);

    if (!HasUserInUsersFile(hashedEnemyUsername))
    {
        Logger::LogError("This player doesn't exist!");
        return;
    }

    std::ifstream enemyUserfile(("./users/" + hashedEnemyUsername + ".dat").c_str(), std::ios::binary);

    if (!enemyUserfile.is_open())
    {
        Logger::LogError("Cannot create player enemyUserfile for enemyOptional \"" + enemyUsername + "\"!");
        return;
    }

    Optional<UniquePtr<User>> enemyOptional = ParseUser(enemyUserfile);
    enemyUserfile.close();

    if (!enemyOptional.HasValue())
    {
        Logger::LogError("Invalid enemyOptional user file!");
        return;
    }

    if (enemyOptional.GetData()->GetUserType() != UserType::Player)
    {
        Logger::LogError("Only players can be attacked!");
        return;
    }

    std::cout << "Enter your hero that you want to attack with: ";

    String yourHeroNickname;
    String::GetLine(std::cin, &yourHeroNickname);

    Player *loggedUser = dynamic_cast<Player *>(m_loggedUser);
    if (!loggedUser)
        return;

    size_t yourHeroID = 0;

    try
    {
        yourHeroID = SuperheroHolder::GetInstance().GetSuperhero(yourHeroNickname).GetUniqueID();
    }
    catch (std::exception &ex)
    {
        Logger::LogError(ex.what());
        return;
    }

    if (!loggedUser->HasSuperhero(yourHeroID))
    {
        Logger::LogError("You do not have this hero!");
        return;
    }

    Player *enemy = dynamic_cast<Player *>(enemyOptional.LendData().Release());

    if (!enemy)
    {
        Logger::LogError("Cannot cast `enemy` to player!");
        return;
    }

    if (enemy->GetSuperheroes().GetLength() == 0) // enemy has no heroes => you win.
    {
        enemy->DecreaseMoney(SuperheroHolder::GetInstance().GetSuperhero(yourHeroID).GetPower());
        loggedUser->AddMoney(15);

        SavePlayer(*enemy);
        SavePlayer(*loggedUser);

        delete enemy;
        return;
    }

    std::cout << "Enter the hero's name that you want to attack: ";
    String enemyHeroNickname;
    String::GetLine(std::cin, &enemyHeroNickname);

    size_t enemyHeroID = 0;

    try
    {
        enemyHeroID = SuperheroHolder::GetInstance().GetSuperhero(enemyHeroNickname).GetUniqueID();
    }
    catch (std::exception &ex)
    {
        Logger::LogError(ex.what());
        return;
    }

    if (!enemy->HasSuperhero(enemyHeroID))
    {
        Logger::LogError("This hero is not bought by " + enemyUsername + "!");
        return;
    }

    unsigned yourPower;
    unsigned enemyPower;

    if (SuperheroHolder::GetInstance().GetSuperhero(yourHeroID).GetElement() ==
        SuperheroHolder::GetInstance().GetSuperhero(enemyHeroID).GetElement()) // equal elements.
    {
        yourPower = SuperheroHolder::GetInstance().GetSuperhero(yourHeroID).GetPower();
        enemyPower = SuperheroHolder::GetInstance().GetSuperhero(enemyHeroID).GetPower();
    } else // not equal elements
    {
        if (IsDominant(SuperheroHolder::GetInstance().GetSuperhero(yourHeroID).GetElement(),
                       SuperheroHolder::GetInstance().GetSuperhero(enemyHeroID).GetElement()))
        {
            yourPower = SuperheroHolder::GetInstance().GetSuperhero(yourHeroID).GetPower() * 2;
            enemyPower = SuperheroHolder::GetInstance().GetSuperhero(enemyHeroID).GetPower();
        } else
        {
            yourPower = SuperheroHolder::GetInstance().GetSuperhero(yourHeroID).GetPower();
            enemyPower = SuperheroHolder::GetInstance().GetSuperhero(enemyHeroID).GetPower() * 2;
        }
    }

    const Array<Pair<size_t, SuperheroMode>> &enemyHeroes = enemy->GetSuperheroes();

    for (int i = 0; i < enemyHeroes.GetLength(); ++i)
    {
        if (enemyHeroes[i].GetFirst() == enemyHeroID)
        {
            if (enemyHeroes[i].GetSecond() == SuperheroMode::Attack)
            {
                if (yourPower > enemyPower) // you win, bigger power
                {
                    enemy->DecreaseMoney(yourPower - enemyPower);
                    loggedUser->AddMoney(yourPower - enemyPower);

                    enemy->DeleteHero(enemyHeroID);
                } else if (enemyPower > yourPower) // you lose, smaller power
                {
                    enemy->AddMoney(25);
                    loggedUser->DecreaseMoney(2 * (enemyPower - yourPower));

                    loggedUser->DeleteHero(yourHeroID);
                } else // equal
                {
                    loggedUser->DecreaseMoney(20);
                }
            } else // defence
            {
                if (yourPower > enemyPower) // you win, bigger power
                {
                    loggedUser->AddMoney(yourPower - enemyPower);

                    enemy->DeleteHero(enemyHeroID);
                } else if (enemyPower > yourPower) // you lose, smaller power
                {
                    enemy->AddMoney(25);
                    loggedUser->DecreaseMoney(2 * (enemyPower - yourPower));

                    loggedUser->DeleteHero(yourHeroID);
                } else // equal
                {
                    loggedUser->DecreaseMoney(20);
                }
            }

            SavePlayer(*enemy);
            SavePlayer(*loggedUser);

            delete enemy;
            return;
        }
    }
}

/*WIP*/ void SHGSystem::UpgradeHero() const
{
    Logger::LogMessage("[Work in progress] Not implemented yet!");
}
void SHGSystem::Baltop() const
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

        Optional<UniquePtr<User>> user = ParseUser(userFile);
        if (user.HasValue() && user.GetData()->GetUserType() == UserType::Player) // Get only players.
            users.Add(user.LendData().Release());

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

    for (int i = 0; i < users.GetLength(); ++i) // clean up users
        delete users[i];
}
bool SHGSystem::HasUserInUsersFile(const String &hashedUsername) const
{
    std::ifstream file("usernames.dat", std::ios::binary);

    if (!file.is_open())
    {
        Logger::LogError("Users file not found!");
        return false;
    }

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

        if (split[1] == hashedUsername)
            return true;
    }

    file.close();

    return false;
}

size_t SHGSystem::GetPlayersCount() const
{
    std::ifstream file("usernames.dat", std::ios::binary);

    if (!file.is_open())
    {
        Logger::LogError("Users file not found!");
        return false;
    }

    size_t count = 0;

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

        if (split[0] == "P")
        {
            count++;
        }
    }

    file.close();

    return count;
}

void SHGSystem::SavePlayer(const Player &pl) const
{
    std::ofstream file(("./users/" + Utils::HashString(pl.GetUsername()) + ".dat").c_str(), std::ios::binary);
    if (!file.is_open())
        return;

    size_t usernameLen = pl.GetUsername().GetSize() + 1;
    size_t passLen = pl.GetPassword().GetSize() + 1;
    size_t fnLen = pl.GetFirstName().GetSize() + 1;
    size_t lnLen = pl.GetLastName().GetSize() + 1;
    size_t emLen = pl.GetEmail().GetSize() + 1;

    UserType type = UserType::Player;

    file.write((const char *) &type, sizeof(UserType));

    file.write((const char *) &passLen, sizeof(size_t));
    file.write(pl.GetPassword().c_str(), passLen);

    file.write((const char *) &usernameLen, sizeof(size_t));
    file.write(pl.GetUsername().c_str(), usernameLen);

    file.write((const char *) &fnLen, sizeof(size_t));
    file.write(pl.GetFirstName().c_str(), fnLen);

    file.write((const char *) &lnLen, sizeof(size_t));
    file.write(pl.GetLastName().c_str(), lnLen);

    file.write((const char *) &emLen, sizeof(size_t));
    file.write(pl.GetEmail().c_str(), emLen);

    double money = pl.GetMoney();
    file.write((const char *) &money, sizeof(double));

    size_t heroesCount = pl.GetSuperheroes().GetLength();
    file.write((const char *) &heroesCount, sizeof(size_t));

    for (int i = 0; i < heroesCount; ++i)
    {
        size_t id = pl.GetSuperheroes()[i].GetFirst();
        SuperheroMode mode = pl.GetSuperheroes()[i].GetSecond();

        file.write((const char *) &id, sizeof(size_t));
        file.write((const char *) &mode, sizeof(SuperheroMode));
    }

    file.close();
}
