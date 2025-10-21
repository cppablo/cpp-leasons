#include <iostream>
#include <iomanip>
#include "BitcoinConverter.h"

int main() {
    BitcoinConverter bc;

    double preco = bc.obterCotacaoBitcoin();
    if (preco <= 0) {
        std::cerr << "Não foi possível obter a cotação do Bitcoin." << std::endl;
        return 1;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Cotação atual do Bitcoin: $" << preco << " USD" << std::endl;

    int opcao;
    std::cout << "\nEscolha uma opção:\n";
    std::cout << "1 - Converter BTC para USD\n";
    std::cout << "2 - Converter USD para BTC\n";
    std::cout << "Opção: ";
    std::cin >> opcao;

    if (opcao == 1) {
        double btc;
        std::cout << "Digite a quantidade de BTC: ";
        std::cin >> btc;
        std::cout << btc << " BTC = $" << bc.btcParaUsd(btc, preco) << " USD\n";
    } 
    else if (opcao == 2) {
        double usd;
        std::cout << "Digite a quantidade de USD: ";
        std::cin >> usd;
        std::cout << "$" << usd << " USD = " << bc.usdParaBtc(usd, preco) << " BTC\n";
    } 
    else {
        std::cout << "Opção inválida.\n";
    }

    return 0;
}
