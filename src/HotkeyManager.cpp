#include "HotkeyManager.h"

#include "Application.h"
#include "CLib/Key.h"
#include "Configuration.h"

namespace
{
    class HotkeyContext
    {
    public:
        HotkeyContext(const Configuration* config) :
            hotkey(config->iHotkey, config->iModifier), hotkeyCompass(config->iHotkeyCompass, config->iModifierCompass),
            hotkeyPlayerBar(config->iHotkeyPlayerBar, config->iModifierPlayerBar),
            hotkeySubtitle(config->iHotkeySubtitle, config->iModifierSubtitle)
        {}

        void Update(const RE::ButtonEvent* a_button)
        {
            if (!a_button->HasIDCode()) {
                return;
            }

            if (a_button->IsPressed()) {
                auto key = CLib::ParseKey(a_button->GetIDCode(), a_button->GetDevice());

                hotkey.UpdatePressed(key);
                hotkeyCompass.UpdatePressed(key);
                hotkeyPlayerBar.UpdatePressed(key);
                hotkeySubtitle.UpdatePressed(key);

                if (a_button->IsDown()) {
                    hotkey.UpdateDown(key);
                    hotkeyCompass.UpdateDown(key);
                    hotkeyPlayerBar.UpdateDown(key);
                    hotkeySubtitle.UpdateDown(key);
                }
            }
        }

        void Finalize(Application* app)
        {
            for (std::uint32_t count = 2; count > 0; --count) {
                bool done = false;

                if (hotkey.Count() == count && hotkey.IsActive()) {
                    app->ToggleUI();
                    done = true;
                }

                if (hotkeyCompass.Count() == count && hotkeyCompass.IsActive()) {
                    app->ToggleCompass();
                    done = true;
                }

                if (hotkeyPlayerBar.Count() == count && hotkeyPlayerBar.IsActive()) {
                    app->TogglePlayerBar();
                    done = true;
                }

                if (hotkeySubtitle.Count() == count && hotkeySubtitle.IsActive()) {
                    app->ToggleSubtitle();
                    done = true;
                }

                if (done) {
                    break;
                }
            }
        }

    private:
        CLib::KeyCombo hotkey;
        CLib::KeyCombo hotkeyCompass;
        CLib::KeyCombo hotkeyPlayerBar;
        CLib::KeyCombo hotkeySubtitle;
    };
}

void HotkeyManager::Process(const RE::InputEvent* const* a_event)
{
    const auto config = Configuration::GetSingleton();

    HotkeyContext ctx{ config };

    for (auto event = *a_event; event; event = event->next) {
        if (auto button = event->AsButtonEvent()) {
            ctx.Update(button);
        }
    }

    auto app = Application::GetSingleton();

    ctx.Finalize(app);
}
