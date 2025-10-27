#pragma once

#include "../src/sign_request.h"
#include "../src/eth_sign_request.h"
#include "../src/keypath.h"
#include "../src/coin_identity.h"
#include "../src/batch_sign_request.h"
#include "../src/intent.h"

void displayIntent(const Intent &req);
void displaySignRequest(const SignRequest &req);
void displayEthSignRequest(const EthSignRequest &req);
void displayBatchSignRequest(const BatchSignRequest &req);
void displayCoinIdentity(const CoinIdentity &req);
void displayKeypath(const Keypath &req);