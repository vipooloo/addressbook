#ifndef TRANSACTIONGUARD_H
#define TRANSACTIONGUARD_H

namespace addrbook {
class TransactionGuard
{
  public:
    TransactionGuard();
    ~TransactionGuard();
    bool Commit();

    TransactionGuard(const TransactionGuard&) = delete;
    TransactionGuard& operator=(const TransactionGuard&) = delete;
    TransactionGuard(TransactionGuard&&) noexcept = delete;
    TransactionGuard& operator=(TransactionGuard&&) noexcept = delete;

  private:
    bool Execute(const char* sql);

  private:
    bool m_bCommited;
};
}  // namespace addrbook

#endif  // TRANSACTIONGUARD_H