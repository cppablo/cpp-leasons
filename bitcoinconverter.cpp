#include "BitcoinConverter.h"
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Implementação da função que obtém a cotação
double BitcoinConverter::obterCotacaoBitcoin() {
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

// Implementação das conversões
double BitcoinConverter::btcParaUsd(double btc, double precoBTC) {
    return btc * precoBTC;
}

double BitcoinConverter::usdParaBtc(double usd, double precoBTC) {
    return usd / precoBTC;
}
