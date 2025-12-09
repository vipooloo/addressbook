#ifndef ADDRCENTER_H
#define ADDRCENTER_H

#include "EmailService.h"

class AddrCenter
{
  public:
    static AddrCenter& GetInstance()
    {
        static AddrCenter instance;
        return instance;
    }

    EmailService& GetEmailService()
    {
        return m_email_srv;
    }

  private:
    AddrCenter()
      : m_email_srv{}
    {}
    ~AddrCenter() {}
    AddrCenter(const AddrCenter&) = delete;
    AddrCenter& operator=(const AddrCenter&) = delete;
    AddrCenter(AddrCenter&&) = delete;
    AddrCenter& operator=(AddrCenter&&) = delete;

  private:
    EmailService m_email_srv;
};

#endif  // ADDRCENTER_H