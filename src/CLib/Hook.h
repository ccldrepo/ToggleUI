#pragma once

#include <PCH.h>

namespace CLib
{
    // The call hook base class.
    template <class T>
    struct CallHook
    {
        static void Install()
        {
            SKSE::AllocTrampoline(14);
            REL::Relocation<std::uintptr_t> target{ T::id, T::offset };
            T::func = target.write_call<5>(T::thunk);
            SKSE::log::info("Hooked 0x{:016X} call ({}).", target.address(), T::name);
        }
    };
}
