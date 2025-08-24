#pragma once

#include "CoreMinimal.h"

namespace Core
{

class API Exception
{
public:
    
    template<USIZE uReasonLen>
    inline Exception(const CHAR8 (&pszReason)[uReasonLen]) noexcept
        : Core::Exception(pszReason, uReasonLen)
    { }

    Exception(const CHAR8* szReason, USIZE uReasonLen) noexcept;

    ~Exception() noexcept = default;

    Exception(const Exception&) = default;
    Exception(Exception&&) = default;

public:

    const CHAR8* GetReason() const noexcept
    { return m_pszReason; }

    USIZE GetReasonLen() const noexcept
    { return m_uReasonLen; }

private:

    const CHAR8* m_pszReason;
    const USIZE m_uReasonLen;

};

} // !Core
