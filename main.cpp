#include <iostream> // For input/output
#include <vector> // For using vector
#include <ctime> // For handling time
#include <iomanip> // For manipulating output
#include <chrono> // For handling time
#include <cctype> // For using isdigit
#include <sstream> // For using stringstream

// Define some ANSI escape codes for colors
#define RESET "\033[0m" // Reset the color
#define RED "\033[31m"  // Red color
#define GREEN "\033[32m" // Green color
#define YELLOW "\033[33m" // Yellow color
#define BLUE "\033[34m" // Blue color
#define MAGENTA "\033[35m" // Magenta color
#define CYAN "\033[36m" // Cyan color

// Article class
class Article {
private:
    long long code_barre;
    std::string designation;
    double prixHT; // Price without tax
    std::string horaire_scan;

public:
    // Default constructor
    Article() {}

    // Parameterized constructor
    Article(int code, std::string name, double price, std::string time) : code_barre(code), designation(name), prixHT(price), horaire_scan(time) {}

    // Getter methods
    int getCodeBarre() const {
        return code_barre;
    }

    std::string getDesignation() const {
        return designation;
    }

    double getPrixHT() const {
        return prixHT;
    }

    // Calculate the price with tax
    double getPrixTTC() const {
        return prixHT * 1.2; // Add 20% tax
    }


    std::string getHoraireScan() const {
        return horaire_scan;
    }
};

// Panier class
class Panier {
private:
    std::string date;
    std::vector<Article> articles;

public:
    // Default constructor
    Panier() {}

    // Setter for date
    void setDate(const std::string& date) {
        this->date = date;
    }

    // Getter for date
    std::string getDate() const {
        return date;
    }

    // Getter for articles
    const std::vector<Article>& getArticles() const {
        return articles;
    }

    // Check if the basket is empty
    bool estVide() const {
        return articles.empty(); // Return true if the basket is empty
    }

    // Add an article to the basket and display the number of articles and total price
    void ajouterArticle(const Article& article) {
        articles.push_back(article);
        std::cout << "\nArticle added: " << article.getDesignation() << std::endl; // Display the name of the added article
        std::cout << "Nombre d'articles in the basket : " << articles.size() << std::endl; // Number of articles in Panier
        std::cout << "Total price of the basket (TTC) : " << prixTotal() << " DH\n" << std::endl; // Total price of the basket
    }

    // Calculate the total price of the basket
    double prixTotal() const {
        double total = 0.0;
        // Calculate the total price of the articles in the basket
        for (const auto& article : articles) {
            total += article.getPrixTTC();
        }
        return total;
    }

    // Print the receipt
    void ticket() const {
        std::cout << BLUE << "\nMarjane market Receipt of " << date << " :\n";
        // Display the name and price of each article in the basket
        for (const auto& article : articles) {
            std::cout << "Article :" << article.getDesignation() << " | Price : " << article.getPrixTTC() << " DH\n";
        }
        std::cout << "Articles number : " << articles.size() << "\n";
        std::cout << "Total Amount : " << prixTotal() << " DH\n" << RESET;
    }

    // Calculate the duration of shopping
    std::string dureeCourses() const {
        // Check if the basket is empty
        if (estVide()) {
            return "\033[31mNo articles scanned!\033[0m"; // Duration of shopping is 0 (empty basket)
        } else {
            std::string debut = articles.front().getHoraireScan();
            std::string fin = articles.back().getHoraireScan();

            int h1, m1, s1, h2, m2, s2;
            sscanf(debut.c_str(), "%d:%d:%d", &h1, &m1, &s1); // Extract the hours, minutes and seconds
            sscanf(fin.c_str(), "%d:%d:%d", &h2, &m2, &s2); // Extract the hours, minutes and seconds

            int debutSec = h1 * 3600 + m1 * 60 + s1; // Convert the time to seconds
            int finSec = h2 * 3600 + m2 * 60 + s2; // Convert the time to seconds
            int dureeSec = finSec - debutSec; // Calculate the duration in seconds

            int dureeH = dureeSec / 3600; // Convert the duration to hours
            dureeSec %= 3600; // Calculate the remaining seconds
            int dureeM = dureeSec / 60; // Convert the remaining seconds to minutes
            dureeSec %= 60; // Calculate the remaining seconds
            int dureeS = dureeSec; // Remaining seconds

            char buffer[100]; // Buffer to store the formatted string
            sprintf(buffer, "\033[34mShopping Duration : %02d:%02d:%02d\033[0m", dureeH, dureeM, dureeS); // Format the string
            return buffer;
        }
    }
};

bool isValidPrice(const std::string& price) {
    for (char c : price) {
        if (!isdigit(c) && c != '.') return false;
    }
    return true;
}

bool isValidCode(const std::string& code) {
    for (char c : code) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Function to check if a string is a valid integer
bool isValidNumber(const std::string& number) {
    for (char c : number) {
        if (!isdigit(c)) return false;
    }
    return true;
}

int main() {
    Panier panier; // Create a Panier object

    // Print the title
    std::cout << YELLOW << "============================================================================\n";
    std::cout << "                      Caisse automatique\n";
    std::cout << "============================================================================\n";
    std::cout << "***************This console app is programmed by ASR Group******************\n" << RESET;

    // Get the current date
    auto now = std::chrono::system_clock::now(); // Get the current time
    std::time_t now_c = std::chrono::system_clock::to_time_t(now); // Convert the current time to a time_t object
    std::tm* now_tm = std::localtime(&now_c); // Convert the time_t object to a tm struct

    // Format the current date as a string
    std::stringstream ss;
    ss << std::put_time(now_tm, "%Y-%m-%d");
    std::string date = ss.str();

    panier.setDate(date);

    // Print the purchase date
    std::cout <<  "\nPurchase date: " << date << "\n";

    std::string num_articles_str;
    do {
        std::cout << YELLOW << "\nEnter the number of articles: " << RESET;
        std::cin >> num_articles_str;
        if (!isValidNumber(num_articles_str)) {
            std::cout << RED << "Error: Invalid number. Please try again.\n" << RESET;
        }
    } while (!isValidNumber(num_articles_str));
    int num_articles = std::stoi(num_articles_str);

    for (int i = 0; i < num_articles; ++i) {
        std::string code;
        std::string name;
        std::string price;

        do {
            std::cout << YELLOW << "\nEnter the barcode for article " << i+1 << ": " << RESET;
            std::cin >> code;
            if (!isValidCode(code)) {
                std::cout << RED << "Error: Invalid barcode. Please try again.\n" << RESET;
            }
        } while (!isValidCode(code));

        std::cout << YELLOW << "Enter the name for article " << i+1 << ": " << RESET;
        std::cin.ignore(); // Ignore the newline character in the input buffer
        std::getline(std::cin, name);

        do {
            std::cout << YELLOW << "Enter the price HT for article " << i+1 << ": " << RESET;
            std::cin >> price;
            if (!isValidPrice(price)) {
                std::cout << RED << "Error: Invalid price. Please try again.\n" << RESET;
            }
        } while (!isValidPrice(price));

        // Get the current time
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_c);

        // Format the current time as a string
        char time[9];
        std::strftime(time, sizeof(time), "%H:%M:%S", now_tm);

        std::cout << GREEN << "The article " << name << " is scanned at " << time << RESET << std::endl;

        Article article(std::stoll(code), name, std::stod(price), time); // Create an article object
        panier.ajouterArticle(article);

        std::string exit_code;
        if (i < num_articles - 1) { // Ask the user if they want to exit
            do {
                std::cout << YELLOW << "\nIf you want to exit, enter 00. To continue, enter Y or y key: " << RESET;
                std::cin >> exit_code;
                if (exit_code != "Y" && exit_code != "y" && exit_code != "00") {
                    std::cout << RED << "Error: Invalid input. Please try again.\n" << RESET;
                }
            } while (exit_code != "Y" && exit_code != "y" && exit_code != "00");
            if (exit_code == "00") {
                std::cout << RED << "Exiting...\n" << RESET;
                break;
            }
        }
    }

    panier.ticket(); // Print the receipt

    if (panier.getArticles().size() > 1) {
        std::cout << panier.dureeCourses() << std::endl;
    }
    else if (panier.getArticles().size() == 1){
        std::cout << RED << "Warning ! You have just one article. The duration of shopping cannot be calculated.\n" << RESET;
    }
    else {
        std::cout << panier.dureeCourses() << std::endl;
    }

    return 0;
}
