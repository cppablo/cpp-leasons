//Version 0.1a
//Updates: 
//(i) clean code; 
//(ii) supports multiple consecutive conversions without restarting the program; 
//(iii) Automatically updates the quote each time the user performs a conversion.


#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iomanip>

using json = nlohmann::json;

// Função de callback usada pelo cURL
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Função que obtém a cotação atual do Bitcoin em USD
double obterCotacaoBitcoin() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Erro ao inicializar cURL" << std::endl;
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Erro ao buscar cotação: " << curl_easy_strerror(res) << std::endl;
        return -1;
    }

    try {
        auto j = json::parse(readBuffer);
        return j["bitcoin"]["usd"];
    } catch (std::exception& e) {
        std::cerr << "Erro ao interpretar JSON: " << e.what() << std::endl;
        return -1;
    }
}

// Função que converte BTC → USD
double btcParaUsd(double btc, double precoBTC) {
    return btc * precoBTC;
}

// Função que converte USD → BTC
double usdParaBtc(double usd, double precoBTC) {
    return usd / precoBTC;
}

// Função principal
int main() {
    while (true) {
        double precoBTC = obterCotacaoBitcoin();
        if (precoBTC < 0) {
            std::cerr << "Erro ao obter cotação. Tente novamente mais tarde." << std::endl;
            break;
        }

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n===============================" << std::endl;
        std::cout << "Cotação atual do Bitcoin: $" << precoBTC << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "1 - Converter BTC para USD\n";
        std::cout << "2 - Converter USD para BTC\n";
        std::cout << "3 - Sair\n";
        std::cout << "Escolha uma opção: ";

        int opcao;
        std::cin >> opcao;

        if (opcao == 1) {
            double btc;
            std::cout << "Digite a quantidade de BTC: ";
            std::cin >> btc;
            std::cout << btc << " BTC = $" << btcParaUsd(btc, precoBTC) << " USD" << std::endl;
        }
        else if (opcao == 2) {
            double usd;
            std::cout << "Digite o valor em USD: ";
            std::cin >> usd;
            std::cout << std::setprecision(6);
            std::cout << "$" << usd << " USD = " << usdParaBtc(usd, precoBTC) << " BTC" << std::endl;
        }
        else if (opcao == 3) {
            std::cout << "Encerrando o programa.../!" << std::endl;
            break;
        }
        else {
            std::cout << "Opção inválida! Tente novamente." << std::endl;
        }
    }

    return 0;
}
