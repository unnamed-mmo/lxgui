#ifndef LXGUI_INPUT_SOURCE_HPP
#define LXGUI_INPUT_SOURCE_HPP

#include <lxgui/lxgui.hpp>
#include "lxgui/utils.hpp"
#include "lxgui/utils_string.hpp"
#include "lxgui/gui_vector2.hpp"
#include "lxgui/gui_event.hpp"
#include "lxgui/input_keys.hpp"

#include <string>
#include <vector>
#include <array>

namespace lxgui {
namespace input
{
    constexpr std::size_t MOUSE_BUTTON_NUMBER = 3u;
    constexpr std::size_t KEY_NUMBER = static_cast<uint>(key::K_MAXKEY);

    /// The base class for input source implementation
    /** \note In case you want to share the same source
    *         for multiple input::manager, you have to call
    *         set_manually_updated(true), and update the
    *         source yourself. Else, it will be updated
    *         by each input::manager (which may not be
    *         desirable).
    */
    class source
    {
    public :

        struct key_state
        {
            std::array<bool, KEY_NUMBER> lKeyState = {};
        };

        struct mouse_state
        {
            std::array<bool, MOUSE_BUTTON_NUMBER> lButtonState = {};
            float fAbsX = 0.0f, fAbsY = 0.0f, fDX = 0.0f, fDY = 0.0f;
            float fRelX = 0.0f, fRelY = 0.0f, fRelDX = 0.0f, fRelDY = 0.0f;
            float fRelWheel = 0.0f;
            bool  bHasDelta = false;
        };

        /// Constructor.
        source() = default;

        /// Destructor.
        virtual ~source() = default;

        /// Updates this input source.
        void update();

        /// Checks if this source is manually updated.
        /** \return 'true' if this source is manually updated
        *   \note See set_manually_updated().
        */
        bool is_manually_updated() const;

        /// Marks this source as manually updated.
        /** \param bManuallyUpdated 'true' if this source is manually updated
        *   \note In case you want to share the same source
        *         for several input::manager, you have to call
        *         set_manually_updated(true), and update the
        *         source yourself. Else, it will be updated
        *         by each input::manager (which may not be
        *         desirable).
        */
        void set_manually_updated(bool bManuallyUpdated);

        /// Returns the keyboard state of this input source.
        const key_state& get_key_state() const;

        /// Returns the unicode characters that have been entered.
        /** \return The unicode characters entered with the keyboard
        */
        const std::vector<char32_t>& get_chars() const;

        /// Return the accumulated events since last frame and clear the cache in the input source.
        std::vector<gui::event> poll_events();

        /// Returns the mouse state of this input source.
        const mouse_state& get_mouse_state() const;

        /// Toggles mouse grab.
        /** When the mouse is grabbed, it is confined to the borders
        *   of the main window. The actual cursor behavior when reaching
        *   those borders is of no importance : what matters is that
        *   relative mouse movement is always aquired, i.e. the mouse is never
        *   blocked.
        *   The mouse is not grabbed by default.
        */
        virtual void toggle_mouse_grab() = 0;

        /// Checks if window has been resized.
        /** \return true if the window has been resized
        */
        bool has_window_resized() const;

        /// Resets the "window resized" flag.
        void reset_window_resized();

        /// Get the window width (in pixels)
        /** \return The window width
        */
        uint get_window_width() const;

        /// Get the window height (in pixels)
        /** \return The window height
        */
        uint get_window_height() const;

        /// Sets the double click maximum time.
        /** \param dDoubleClickTime Maximum amount of time between two clicks in a double click
        */
        void set_doubleclick_time(double dDoubleClickTime);

        /// Returns the double click maximum time.
        /** \return The double click maximum time
        */
        double get_doubleclick_time() const;

        /// Retrieve a copy of the clipboard content.
        /** \return A copy of the clipboard content (empty string is clipboard is empty).
        */
        virtual utils::ustring get_clipboard_content() = 0;

        /// Replace the content of the clipboard.
        /** \param sContent The new clipboard content
        */
        virtual void set_clipboard_content(const utils::ustring& sContent) = 0;

        /// Sets the mouse cursor to a given image on disk.
        /** \param sFileName The cursor image
        *   \param mHotSpot The pixel position of the tip of the pointer in the image
        *   \note Use reset_mouse_cursor() to set the cursor back to the default.
        */
        virtual void set_mouse_cursor(const std::string& sFileName, const gui::vector2i& mHotSpot) = 0;

        /// Return the interface scaling factor suggested by the operating system.
        /** \return The interface scaling factor suggested by the operating system
        *   \note This is implementation-dependent; not all input implementations are able
        *         to produce this hint, in which case the function always returns 1.
        *         Consequently, it is recommended to not rely blindly on this hint, and
        *         to offer a way for the user to change the scaling factor.
        */
        virtual float get_interface_scaling_factor_hint() const;

        /// Sets the mouse cursor back to the default (arrow).
        virtual void reset_mouse_cursor() = 0;

    protected:

        /// Updates this implementation input source.
        virtual void update_() = 0;

        key_state   mKeyboard_;
        mouse_state mMouse_;

        std::vector<char32_t> lChars_;
        std::vector<char32_t> lCharsCache_;

        std::vector<gui::event> lEvents_;

        bool bManuallyUpdated_ = false;

        bool bWindowResized_ = false;
        uint uiWindowHeight_ = 0u;
        uint uiWindowWidth_ = 0u;

        double dDoubleClickTime_ = 0.25;
    };
}
}

#endif
