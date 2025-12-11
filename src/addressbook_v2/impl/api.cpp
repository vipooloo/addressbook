#include "AddrCenter.h"
#include "api.h"

void Add()
{
    AddrCenter::GetInstance().GetEmailService().AddEmail(EmailDto{"123", "Test Email", {}});
}