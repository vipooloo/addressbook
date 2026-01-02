#ifndef TRANSACTIONGUARD_H
#define TRANSACTIONGUARD_H

#include <mutex>

class TransactionGuard
{
  public:
    TransactionGuard();
    ~TransactionGuard();
    bool Commit();

    TransactionGuard(const TransactionGuard&) = delete;
    TransactionGuard& operator=(const TransactionGuard&) = delete;
    TransactionGuard(TransactionGuard&&) = delete;
    TransactionGuard& operator=(TransactionGuard&&) = delete;

  private:
    bool Execute(const char* sql);

  private:
    bool m_bCommited;
    std::mutex m_mutex;
};

#endif  // TRANSACTIONGUARD_H