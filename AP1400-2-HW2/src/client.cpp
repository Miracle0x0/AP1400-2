#include "client.h"
#include "server.h"

Client::Client(std::string id, const Server &server) : id(id), server(&server) {
    crypto::generate_key(public_key, private_key);
}

std::string Client::get_id() { return id; }

std::string Client::get_publickey() const { return public_key; }

double Client::get_wallet() const { return server->get_wallet(id); }

std::string Client::sign(std::string txt) const {
    return crypto::signMessage(private_key, txt);
}

bool Client::transfer_money(std::string receiver, double value) const {
    std::string trx = id + "-" + receiver + "-" + std::to_string(value);
    return server->add_pending_trx(trx, sign(trx));
}

size_t Client::generate_nonce() {
    static std::default_random_engine e;
    static std::uniform_int_distribution<int> u(0, INT_MAX);
    return u(e);
}