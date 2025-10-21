#ifndef BITCOINCONVERTER_H
#define BITCOINCONVERTER_H

#include <string>

class BitcoinConverter {
public:
    // Função que obtém o preço do Bitcoin em USD (via API)
    double obterCotacaoBitcoin();

    // Converte BTC → USD
    double btcParaUsd(double btc, double precoBTC);

    // Converte USD → BTC
    double usdParaBtc(double usd, double precoBTC);
};

#endif
