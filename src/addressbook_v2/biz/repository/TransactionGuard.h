#ifndef TRANSACTIONGUARD_H
#define TRANSACTIONGUARD_H

#include "AbstractDao.h"

class TransactionGuard
{
  public:
    TransactionGuard()
      : TransactionGuard(false)
    {
    }
    explicit TransactionGuard(bool has_error)
      : m_transaction{AbstractDao::GetDb()}
      , m_has_error{has_error}
    {
    }
    ~TransactionGuard();
    void SetError(bool has_error)
    {
        m_has_error = has_error;
    }

    TransactionGuard(const TransactionGuard&) = delete;
    TransactionGuard& operator=(const TransactionGuard&) = delete;
    TransactionGuard(TransactionGuard&&) = delete;
    TransactionGuard& operator=(TransactionGuard&&) = delete;

  private:
    SQLite::Transaction m_transaction;
    bool m_has_error;
};

#endif  // TRANSACTIONGUARD_H