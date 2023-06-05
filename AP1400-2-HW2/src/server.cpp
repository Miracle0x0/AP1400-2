#include "server.h"
#include "client.h"

// global variables should be defined in the source file
std::vector<std::string> pending_trxs;

const double initial_coins = 5.0, awarded_coins = 6.25;

Server::Server() {}

std::shared_ptr<Client> Server::add_client(std::string id) {
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 9);
    std::string tail;
    for (auto i = 0; i < 4; i++)
        tail += std::to_string(u(e));
    for (const auto &cli : clients) {
        if (cli.first->get_id() == id)
            id += tail; // id already exists
    }
    auto new_cli = std::make_shared<Client>(id, *this);
    clients[new_cli] = initial_coins;
    return new_cli;
}

std::shared_ptr<Client> Server::get_client(std::string id) const {
    for (const auto &cli : clients) {
        if (cli.first->get_id() == id)
            return cli.first;
    }
    return nullptr;
}

double Server::get_wallet(std::string id) const {
    for (const auto &cli : clients) {
        if (cli.first->get_id() == id)
            return cli.second;
    }
    return 0.0;
}

bool Server::parse_trx(const std::string &trx, std::string &sender,
                       std::string &receiver, double &value) {
    std::string pattern = "(\\w+)-(\\w+)-(\\d+\\.\\d+)";
    std::regex r(pattern);
    std::smatch results;
    if (std::regex_match(trx, results, r)) {
        sender = results.str(1);
        receiver = results.str(2);
        value = std::stod(results.str(3));
        return true;
    } else
        throw std::runtime_error("trx format error");
    return false;
}

bool Server::add_pending_trx(std::string trx, std::string signature) const {
    std::string sender, receiver;
    double value;
    Server::parse_trx(trx, sender, receiver, value);

    auto p_receiver = get_client(receiver);
    if (p_receiver == nullptr)
        return false;
    auto p_sender = get_client(sender);
    bool auth =
        crypto::verifySignature(p_sender->get_publickey(), trx, signature);
    if (auth && value <= clients.at(p_sender)) {
        pending_trxs.push_back(trx);
        return true;
    }
    return false;
}

inline bool has_consecutive_zeros(const std::string &str,
                                  int consecutive_count = 3) {
    const std::string zeros = std::string(consecutive_count, '0');
    return str.substr(0, 10).find(zeros) != str.npos;
}

size_t Server::mine() {
    std::string mempool;
    for (const auto &trx : pending_trxs)
        mempool += trx;
    size_t nonce = 0;
    bool mined_flag = false, mined_success = false;
    while (!mined_flag) {
        for (auto &cli : clients) {
            nonce = cli.first->generate_nonce();
            mined_success = has_consecutive_zeros(
                crypto::sha256(mempool + std::to_string(nonce)));
            if (mined_success) {
                mined_flag = true;
                cli.second += awarded_coins;
                for (const auto &trx : pending_trxs) {
                    std::string sender, receiver;
                    double value;
                    Server::parse_trx(trx, sender, receiver, value);
                    clients[get_client(sender)] -= value;
                    clients[get_client(receiver)] += value;
                }
                pending_trxs.clear();
                return nonce;
            }
        }
    }
    return nonce;
}

void show_wallets(const Server &server) {
    std::cout << std::string(20, '*') << std::endl;
    for (const auto &client : server.clients)
        std::cout << client.first->get_id() << " : " << client.second
                  << std::endl;
    std::cout << std::string(20, '*') << std::endl;
}