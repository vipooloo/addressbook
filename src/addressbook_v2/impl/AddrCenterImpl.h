#ifndef ADDRCENTERIMPL_H
#define ADDRCENTERIMPL_H

#include "EmailService.h"

class AddrCenterImpl
{
  public:
    static AddrCenterImpl& GetInstance()
    {
        static AddrCenterImpl instance;
        return instance;
    }

    EmailService& GetEmailService()
    {
        return m_email_srv;
    }

  private:
    AddrCenterImpl()
      : m_email_srv{}
    {}
    ~AddrCenterImpl() {}
    AddrCenterImpl(const AddrCenterImpl&) = delete;
    AddrCenterImpl& operator=(const AddrCenterImpl&) = delete;
    AddrCenterImpl(AddrCenterImpl&&) = delete;
    AddrCenterImpl& operator=(AddrCenterImpl&&) = delete;

  private:
    EmailService m_email_srv;
};

#endif  // ADDRCENTERIMPL_H