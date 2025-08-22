#include "Exception.h"

#include "Console.h"



namespace Core
{

// Exception // --------------------------------------------------------------------------------------------------------
Exception::Exception(const CHAR8* pszReason, USIZE uReasonLen) noexcept
    : m_pszReason(pszReason)
    , m_uReasonLen(uReasonLen)
{
    const CHAR8 szExceptPrefix[] = "Exception!!!\n   Reason: ";

    WriteToConsoleN(szExceptPrefix, sizeof(szExceptPrefix));
    WriteToConsole(m_pszReason, m_uReasonLen);
}

} // !Core

