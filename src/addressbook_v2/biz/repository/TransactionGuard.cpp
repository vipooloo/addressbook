#include "AddrCenterLog.h"
#include "TransactionGuard.h"

TransactionGuard::~TransactionGuard()
{
    if (m_has_error)
    {
        AB_LOG_E("TransactionGuard: Transaction rollback");
        m_transaction.rollback();
    }
    else
    {
        m_transaction.commit();
    }
}