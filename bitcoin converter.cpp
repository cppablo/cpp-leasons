//version 0.1
//objetives: (i) conversion BTC in USD; (ii) conversion USD in BTC
//next version 0.1.a: (a) Automatically updates the quote each time the user performs a conversion.
//next version 0.1.b: (b) Supports multiple consecutive conversions without restarting the program.
//be paceful with this noob;

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iomanip>


using json = nlohmann::json;


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "Erro ao buscar cotação: " << curl_easy_strerror(res) << std::endl;
            return 1;
        }

        curl_easy_cleanup(curl);

        // Interpretar JSON
        double precoBTC;
        try {
            auto j = json::parse(readBuffer);
            precoBTC = j["bitcoin"]["usd"];
        } catch (std::exception& e) {
            std::cerr << "Erro ao interpretar JSON: " << e.what() << std::endl;
            return 1;
        }

        std::cout << "Cotação atual do Bitcoin: $" << precoBTC << std::endl;

        // Menu interativo
        int opcao;
        std::cout << "Escolha uma opção:\n";
        std::cout << "1 - Converter BTC para USD\n";
        std::cout << "2 - Converter USD para BTC\n";
        std::cout << "Opção: ";
        std::cin >> opcao;

        if (opcao == 1) {
            double btc;
            std::cout << "Digite a quantidade de BTC: ";
            std::cin >> btc;
            std::cout << std::fixed << std::setprecision(2);
            std::cout << btc << " BTC = $" << btc * precoBTC << " USD" << std::endl;

        } else if (opcao == 2) {
            double usd;
            std::cout << "Digite o valor em USD: ";
            std::cin >> usd;
           std::cout << std::fixed << std::setprecision(6);
           std::cout << "$" << usd << " USD = " << usd / precoBTC << " BTC" << std::endl;
            std::cout << "Opção inválida!" << std::endl;
        }

    } else {
        std::cerr << "Erro ao inicializar cURL" << std::endl;
        return 1;
    }

    return 0;
}
