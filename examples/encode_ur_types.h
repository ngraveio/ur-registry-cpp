#pragma once

#include "../src/coin_identity.h"
#include "../src//keypath.h" 
#include "../src//coininfo.h" 
#include "../src/hdkey.h"
#include "../src//output_descriptor.h"
#include "../src//detailed_account.h"
#include "../src//portfolio_coin.h"
#include "../src//portfolio_metadata.h"
#include "../src//portfolio.h"
#include "../src//sign_response.h"
#include "../src//eth_signature.h"

#include "../common/utils.h"  

CoinIdentity encodeCoinIdentity();
Keypath encodeKeypath();
CoinInfo encodeCoinInfo();
HDKey encodeHDKey();
OutputDescriptor encodeOutputDescriptor();
DetailedAccount encodeDetailedAccount();
PortfolioCoin encodePortfolioCoin();
PortfolioMetadata encodePortfolioMetadata();
Portfolio encodePortfolio();
SignResponse encodeSignResponse();
EthSignature encodeEthSignature();
