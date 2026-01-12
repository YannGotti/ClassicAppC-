#include "IUInventory.h"
#include "Inventory.h"
#include "FastFunctions.h"

using namespace Gdiplus;
using namespace FastFunctions;

// ======================================================
// НАСТРОЙКИ ХОТБАРА (под ассеты 1:1)
// ======================================================

constexpr int   MAX_ITEMS = 9;

// ассеты
constexpr float ICON_SIZE = 32.0f;   // размер иконки (реальный)
constexpr float SLOT_SIZE = 64.0f;   // размер слота (по твоему PNG)
constexpr float SLOT_GAP = 4.0f;

// панель
constexpr float PANEL_PADDING = 8.0f;
constexpr float PANEL_MARGIN_BOTTOM = 20.0f;

constexpr float PANEL_WIDTH =
MAX_ITEMS * SLOT_SIZE +
(MAX_ITEMS - 1) * SLOT_GAP +
PANEL_PADDING * 2.0f;

constexpr float PANEL_HEIGHT =
SLOT_SIZE + PANEL_PADDING * 2.0f;

// ======================================================
// ОТРИСОВКА
// ======================================================

void IHotBarInvetory::DrawScreen(Graphics& g, const RECT& clientRect)
{
    auto slots = _parentInv->GetSlots();

    // --- координаты панели ---
    const float panelX = (clientRect.right - PANEL_WIDTH) * 0.5f;
    const float panelY = clientRect.bottom - PANEL_HEIGHT - PANEL_MARGIN_BOTTOM;

    // --- фон панели ---
    

    // --- слоты ---
    for (int i = 0; i < MAX_ITEMS; ++i)
    {
        const float slotX =
            panelX + PANEL_PADDING +
            i * (SLOT_SIZE + SLOT_GAP);

        const float slotY =
            panelY + PANEL_PADDING;

        // ==========================================
        // 1. ИКОНКА (ВСЕГДА ПЕРВАЯ)
        // ==========================================
        

        // ==========================================
        // 2. РАМКА СЛОТА (ПОВЕРХ)
        // ==========================================
        /*if (frame)
        {
            g.DrawImage(
                frame,
                slotX,
                slotY,
                SLOT_SIZE,
                SLOT_SIZE
            );
        }*/


        if (i < static_cast<int>(slots.size()))
        {
            Item* item = slots[i].item.get();
            if (item)
            {
                Image* icon = item->GetIcon();
                if (icon && icon->GetLastStatus() == Ok)
                {
                    const float iconX = slotX + (SLOT_SIZE - ICON_SIZE) * 0.5f;
                    const float iconY = slotY + (SLOT_SIZE - ICON_SIZE) * 0.5f;

                    g.DrawImage(
                        icon,
                        iconX,
                        iconY,
                        ICON_SIZE,
                        ICON_SIZE
                    );
                }
            }
        }

        // ==========================================
        // 3. АКТИВНЫЙ СЛОТ (САМЫЙ ВЕРХ)
        // ==========================================
        if (i == currentSlot && cell)
        {
            g.DrawImage(
                cell,
                slotX,
                slotY,
                SLOT_SIZE,
                SLOT_SIZE
            );
        }

        
    }

    

#ifdef _DEBUG
    // отладочная рамка панели
    Pen pen(Color(180, 255, 255, 255), 1.5f);
    g.DrawRectangle(&pen, panelX, panelY, PANEL_WIDTH, PANEL_HEIGHT);
#endif
}

// ======================================================
// ЛОГИКА
// ======================================================

void IHotBarInvetory::Update(float /*deltaTime*/)
{
}

void IHotBarInvetory::HandlerInputCellInventory(
    const PlayerSwitchInventoryEvent& e)
{
    if (e.slot >= 0 && e.slot < MAX_ITEMS)
    {
        currentSlot = e.slot;

#ifdef _DEBUG
        OutputDebugStringW(
            (L"[HotBar] Switched to slot: " +
                std::to_wstring(currentSlot) + L"\n").c_str()
        );
#endif
    }
}
