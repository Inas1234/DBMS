#include "loginSystem.h"

bool LoginSystem::authenticateAdmin(const std::string& inputUsername, const std::string& inputPassword) {
    std::ifstream adminFile("admin.txt");

    if (!adminFile.is_open()) {
        std::cerr << "Greska prilikom otvaranja admin.txt fajla.\n";
        return false;
    }

    std::string line;

    while (std::getline(adminFile, line)) {
        std::istringstream iss(line);
        std::string storedUsername, storedPassword;

        try {
            if (!(iss >> storedUsername >> storedPassword)) {
                throw std::runtime_error("Greska pri citanju podataka!");
            }

            if (storedUsername == inputUsername && storedPassword == inputPassword) {
                std::cout << "Uspesno ste se ulogovali kao administrator.\n";
                return true;
            }
        } catch (const std::exception& e) {
            std::cerr << "Greska: " << e.what() << std::endl;
            continue;
        }
    }

    std::cout << "Neuspesno logovanje kao administrator.\n";
    return false;
}

bool LoginSystem::authenticateUser(const std::string& inputUsername, const std::string& inputPassword) {
    std::ifstream userFile("users.txt");

    if (!userFile.is_open()) {
        std::cerr << "Greska prilikom otvaranja users.txt fajla.\n";
        return false;
    }

    std::string line;

    while (std::getline(userFile, line)) {
        std::istringstream iss(line);
        std::string storedUsername, storedPassword;

        try {
            if (!(iss >> storedUsername >> storedPassword)) {
                throw std::runtime_error("Greska pri citanju podataka!");
            }

            if (storedUsername == inputUsername && storedPassword == inputPassword) {
                std::cout << "Uspesno ste se ulogovali kao korisnik.\n";
                return true;
            }
        } catch (const std::exception& e) {
            std::cerr << "Greska: " << e.what() << std::endl;
            continue;
        }
    }

    std::cout << "Neuspesno logovanje kao korisnik.\n";
    return false;
}

void LoginSystem::afterSuccessfulAdminLogin() {
    // Dodajte ovde kôd koji treba izvršiti nakon uspešnog logovanja administratora
    std::cout << "Dobrodosli, administrator!\n";
}

void LoginSystem::afterFailedAdminLogin() {
    // Dodajte ovde kôd koji treba izvršiti nakon neuspešnog logovanja administratora
    std::cout << "Pokusajte ponovo ili kontaktirajte podrsku.\n";
}

void LoginSystem::afterSuccessfulUserLogin() {
    // Dodajte ovde kôd koji treba izvršiti nakon uspešnog logovanja korisnika
    std::cout << "Dobrodosli!\n";
}

void LoginSystem::afterFailedUserLogin() {
    // Dodajte ovde kôd koji treba izvršiti nakon neuspešnog logovanja korisnika
    std::cout << "Pokusajte ponovo ili registrujte novi nalog.\n";
}

int main() {
    std::string username, password;

    std::cout << "Unesite korisnicko ime: ";
    std::cin >> username;

    std::cout << "Unesite lozinku: ";
    std::cin >> password;

    if (LoginSystem::authenticateAdmin(username, password)) {
        LoginSystem::afterSuccessfulAdminLogin();
    } else if (LoginSystem::authenticateUser(username, password)) {
        LoginSystem::afterSuccessfulUserLogin();
    } else {
        LoginSystem::afterFailedUserLogin();
    }

    return 0;
}
