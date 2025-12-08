#include "Api.h"
#include "EmailDao.h"
#include "EmailRepository.h"
#include "EmailService.h"

void Add()
{
    auto conn_sptr = std::make_shared<SQLiteConn>("addressbook.db");
    auto dao = std::make_shared<EmailDao>(conn_sptr);
    auto repo_sptr = std::make_shared<EmailRepository>();
    if (repo_sptr)
    {
        repo_sptr->SetEmailDao(dao);
        EmailService s(repo_sptr);
        s.AddEmail(EmailDto("1", "2"));
    }
}