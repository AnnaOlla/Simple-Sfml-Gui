#ifndef SMALLSFMLGUI_HPP
#define SMALLSFMLGUI_HPP

#include <SFML/Graphics.hpp>

#include <vector>
#include <list>
#include <functional>
#include <cmath>

namespace SmallGui
{
    /////////////////////////////////////////////////
    /// \brief Contains size-independent info about appearance of the widget
    ///
    /////////////////////////////////////////////////
    class DecorationSettings
    {
        public:
            /////////////////////////////////////////////////
            /// \brief Default constructor
            ///
            /// \param fillColor: the fill color for the background of the widget, can be (semi-)transparent. Multiplies with the texture
            /// \param textColor: the fill color for text, can be (semi-)transparent
            /// \param textStyle: style of the text (regular, italic, bold...), can be combined
            /// \param outlineThickness: thickness of the border, can be zero. Border is drawn only inside the widget
            /// \param outlineColor: color of the border
            /// \param backgroundTexture: texture for the background, 'nullptr' means no texture
            ///
            /////////////////////////////////////////////////
            DecorationSettings(const sf::Color fillColor = sf::Color::White,
                               const sf::Color textColor = sf::Color::Black,
                               const sf::Uint32 textStyle = sf::Text::Style::Regular,
                               const float outlineThickness = 1.0f,
                               const sf::Color outlineColor = sf::Color::Black,
                               const sf::Texture* backgroundTexture = nullptr);

            /////////////////////////////////////////////////
            /// \brief Default destructor
            ///
            /////////////////////////////////////////////////
            virtual ~DecorationSettings();

            /////////////////////////////////////////////////
            /// \brief Gets the background fill color
            ///
            /// \return Current background fill color
            ///
            /////////////////////////////////////////////////
            sf::Color getFillColor() const;

            /////////////////////////////////////////////////
            /// \brief Gets the text fill color
            ///
            /// \return Current text fill color
            ///
            /////////////////////////////////////////////////
            sf::Color getTextColor() const;

            /////////////////////////////////////////////////
            /// \brief Gets the combination of text styles
            ///
            /// \return Current combination of text styles
            ///
            /////////////////////////////////////////////////
            sf::Uint32 getTextStyle() const;

            /////////////////////////////////////////////////
            /// \brief Gets the outline color
            ///
            /// \return Current outline color
            ///
            /////////////////////////////////////////////////
            sf::Color getOutlineColor() const;

            /////////////////////////////////////////////////
            /// \brief Gets the outline thickness. Note that it is a non-positive number meaning that the border is drawn inside borders
            ///
            /// \return Current outline thickness
            ///
            /////////////////////////////////////////////////
            float getOutlineThickness() const;

            /////////////////////////////////////////////////
            /// \brief Gets the current background texture
            ///
            /// \return Current background texture
            ///
            /////////////////////////////////////////////////
            const sf::Texture* getBackgroundTexture() const;

            /////////////////////////////////////////////////
            /// \brief Sets the current background texture
            ///
            /// \param fillColor: new background texture
            ///
            /////////////////////////////////////////////////
            void setFillColor(const sf::Color fillColor);

            /////////////////////////////////////////////////
            /// \brief Sets the text fill color
            ///
            /// \param textColor: new text fill color
            ///
            /////////////////////////////////////////////////
            void setTextColor(const sf::Color textColor);

            /////////////////////////////////////////////////
            /// \brief Sets the combination of text styles
            ///
            /// \param textStyle: new combination of text styles
            ///
            /////////////////////////////////////////////////
            void setTextStyle(const sf::Uint32 textStyle);

            /////////////////////////////////////////////////
            /// \brief Sets the outline thickness. Note that the value will be changed to negative to draw the border inside widgets
            ///
            /// \param outlineThickness: new combination of text styles
            ///
            /////////////////////////////////////////////////
            void setOutlineThickness(const float outlineThickness);

            /////////////////////////////////////////////////
            /// \brief Sets the outline color
            ///
            /// \param outlineColor: new outline color
            ///
            /////////////////////////////////////////////////
            void setOutlineColor(const sf::Color outlineColor);

            /////////////////////////////////////////////////
            /// \brief Sets the background texture. Will be multiplied by fillColor
            ///
            /// \param backgroundTexture: new background texture
            ///
            /////////////////////////////////////////////////
            void setBackgroundTexture(const sf::Texture* backgroundTexture);

        private:
            /////////////////////////////////////////////////
            /// Member data
            /////////////////////////////////////////////////
            sf::Color m_fillColor;                      //!< Fill color for the background of widgets
            sf::Color m_textColor;                      //!< Fill color for text
            sf::Uint32 m_textStyle;                     //!< Combination of styles of the text
            float m_outlineThickness;                   //!< Thickness of the border: zero or negative to draw it inside the widget
            sf::Color m_outlineColor;                   //!< Color of the border
            const sf::Texture* m_backgroundTexture;     //!< Texture for the background
    };

    /////////////////////////////////////////////////
    /// \brief Contains additional info about fonts to align text. Only for internal use
    ///
    /////////////////////////////////////////////////
    struct FontMetrics
    {
        // You can find descriptions with pictures of what these are in the internet
        // For example: https://en.wikipedia.org/wiki/Typeface_anatomy
        float ascenderLine;
        float baseLine;
        float descenderLine;
        float fullHeight;       // Well, maybe it is my invention :D
    };

    /////////////////////////////////////////////////
    /// \brief Contains options of horizontal alignment
    ///
    /////////////////////////////////////////////////
    enum class TextHorizontalAlignment
    {
        Left,
        Center,
        Right
    };

    /////////////////////////////////////////////////
    /// \brief Contains options of vertical alignment
    ///
    /////////////////////////////////////////////////
    enum class TextVerticalAlignment
    {
        Top,
        Center,
        Bottom
    };

    /////////////////////////////////////////////////
    /// \brief Contains size-dependent info about appearance of the text in the widget
    ///
    /////////////////////////////////////////////////
    class TextSettings
    {
        public:
            /////////////////////////////////////////////////
            /// \brief Default constructor
            ///
            /// \param font: font for text
            /// \param characterSize: character size for text
            /// \param TextHorizontalAlignment: where the text should be drawn horizontally?
            /// \param TextVerticalAlignment: where the text should be drawn vertically?
            ///
            /////////////////////////////////////////////////
            TextSettings(const sf::Font& font,
                         const unsigned int characterSize = 30,
                         const TextHorizontalAlignment horizontalAlignment = TextHorizontalAlignment::Center,
                         const TextVerticalAlignment verticalAlignment = TextVerticalAlignment::Center);

            /////////////////////////////////////////////////
            /// \brief Default destructor
            ///
            /////////////////////////////////////////////////
            virtual ~TextSettings();

            /////////////////////////////////////////////////
            /// \brief Gets the current font
            ///
            /// \return Current font
            ///
            /////////////////////////////////////////////////
            const sf::Font& getFont() const;

            /////////////////////////////////////////////////
            /// \brief Gets current character size
            ///
            /// \return Current character size
            ///
            /////////////////////////////////////////////////
            unsigned int getCharacterSize() const;

            /////////////////////////////////////////////////
            /// \brief Gets current horizontal alignment
            ///
            /// \return Current horizontal alignment
            ///
            /////////////////////////////////////////////////
            TextHorizontalAlignment getHorizontalAlignment() const;

            /////////////////////////////////////////////////
            /// \brief Gets current vertical alignment
            ///
            /// \return Current vertical alignment
            ///
            /////////////////////////////////////////////////
            TextVerticalAlignment getVerticalAlignment() const;

            /////////////////////////////////////////////////
            /// \brief Gets calculated metrics of the font for current character size
            ///
            /// \return Structure with font metrics
            ///
            /////////////////////////////////////////////////
            const FontMetrics& getFontMetrics() const;

            /////////////////////////////////////////////////
            /// \brief Changes the font
            ///
            /// \param font: new font
            ///
            /////////////////////////////////////////////////
            void setFont(const sf::Font& font);

            /////////////////////////////////////////////////
            /// \brief Changes character size
            ///
            /// \param font: new character size
            ///
            /////////////////////////////////////////////////
            void setCharacterSize(const unsigned int characterSize);

            /////////////////////////////////////////////////
            /// \brief Changes horizontal alignment
            ///
            /// \param font: new horizontal alignment
            ///
            /////////////////////////////////////////////////
            void setHorizontalAlignment(const TextHorizontalAlignment horizontalAlignment);

            /////////////////////////////////////////////////
            /// \brief Changes vertical alignment
            ///
            /// \param font: new vertical alignment
            ///
            /////////////////////////////////////////////////
            void setVerticalAlignment(const TextVerticalAlignment verticalAlignment);

        private:
            /////////////////////////////////////////////////
            /// Member data
            /////////////////////////////////////////////////
            const sf::Font* m_font;                         //!< Font for all text
            unsigned int m_characterSize;                   //!< Character size for all text
            TextHorizontalAlignment m_horizontalAlignment;  //!< Horizontal alignment for all text
            TextVerticalAlignment m_verticalAlignment;      //!< Vertical alignment for all text
            FontMetrics m_fontMetrics;                      //!<

            /////////////////////////////////////////////////
            /// \brief Calculates font metrics based on character size. Required to align text
            ///
            /// \return Font metrics
            ///
            /////////////////////////////////////////////////
            FontMetrics calculateFontMetrics();
    };

    /////////////////////////////////////////////////
    /// \brief Contains complete info about appearance of the widget
    ///
    /////////////////////////////////////////////////
    struct Theme
    {
        public:
            Theme(const TextSettings& text,
                  const DecorationSettings& idle,
                  const DecorationSettings& hover,
                  const DecorationSettings& press);
            virtual ~Theme();

            const TextSettings& getTextSettings() const;
            const DecorationSettings& getIdleColorSettings() const;
            const DecorationSettings& getHoveredColorSettings() const;
            const DecorationSettings& getPressedColorSettings() const;

        private:
            const TextSettings& m_textSettings;
            const DecorationSettings& m_idleColorSettings;
            const DecorationSettings& m_hoverColorSettings;
            const DecorationSettings& m_pressColorSettings;
    };

    class Widget;

    /////////////////////////////////////////////////
    /// \brief Contains possible states of the widget
    ///
    /////////////////////////////////////////////////
    enum class WidgetState
    {
        Idle,
        Hovered,
        Pressed,
        Hidden
    };

    /////////////////////////////////////////////////
    /// \brief A singleton that contains all created widgets
    ///
    /////////////////////////////////////////////////
    class WidgetPool : public sf::Drawable
    {
        friend class Widget;

        public:
            /////////////////////////////////////////////////
            /// \brief A destructor to clean up
            ///
            /////////////////////////////////////////////////
            virtual ~WidgetPool();

            /////////////////////////////////////////////////
            /// \brief Prohibition of creation another instance. It is not acceptable by the singleton pattern
            ///
            /////////////////////////////////////////////////
            WidgetPool(WidgetPool const&) = delete;

            /////////////////////////////////////////////////
            /// \brief Prohibition of copying. It is not acceptable by the singleton pattern
            ///
            /////////////////////////////////////////////////
            void operator=(WidgetPool const&) = delete;

            /////////////////////////////////////////////////
            /// \brief Gets or creates an instance of the singleton
            ///
            /// \return Returns the instance
            ///
            /////////////////////////////////////////////////
            static WidgetPool& getInstance();

            /////////////////////////////////////////////////
            /// \brief Binds widgets to the window. Multiple windows are not supported
            ///
            /////////////////////////////////////////////////
            void initialize(const sf::RenderWindow* window);

            /////////////////////////////////////////////////
            /// \brief Processes user actions on widgets
            ///
            /////////////////////////////////////////////////
            void processEvent(const sf::Event event);

            /////////////////////////////////////////////////
            /// \brief Forces appearance of all widgets to be updated on the current frame. Must be used if any style is changed
            ///
            /////////////////////////////////////////////////
            void forceStylesUpdate() const;

        private:
            /////////////////////////////////////////////////
            /// \brief Constructs the object. Privateness allows to follow the singleton pattern
            ///
            /////////////////////////////////////////////////
            WidgetPool();

            /////////////////////////////////////////////////
            /// \brief Finds a widget hovered by mouse
            ///
            /// \param mousePosition: current mouse position in the world coordinates
            ///
            /// \return 'nullptr' if nothing is hovered, else the pointer to the hovered widget
            ///
            /////////////////////////////////////////////////
            Widget* getActiveWidget(const sf::Vector2f& mousePosition) const;

            /////////////////////////////////////////////////
            /// \brief Adds the widget to the pool
            ///
            /// \param widget: a widget to add
            ///
            /////////////////////////////////////////////////
            void addWidget(Widget* widget);

            /////////////////////////////////////////////////
            /// \brief Draws all widgets on the window. Inherited from sf::Drawable
            ///
            /////////////////////////////////////////////////
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

            /////////////////////////////////////////////////
            /// Member data
            /////////////////////////////////////////////////
            const sf::RenderWindow* m_window;   //!< The window which widgets belong to
            std::list <Widget*> m_widgets;      //!< The container for all created widgets
            Widget* m_activeWidget;             //!< The widget that is hovered or clicked by mouse
            Widget* m_lastHoveredWidget;        //!< The widget that was hovered on the previous frame
            Widget* m_lastClickedWidget;        //!< The widget that was clicked last
    };

    /////////////////////////////////////////////////
    /// \brief Base class for all widgets
    ///
    /////////////////////////////////////////////////
    class Widget : public sf::Drawable
    {
        friend class WidgetPool;

        public:
            /////////////////////////////////////////////////
            /// \brief Default constructor to create an empty object
            ///
            /////////////////////////////////////////////////
            Widget();

            /////////////////////////////////////////////////
            /// \brief All widgets are constructed using WidgetPool only through the default constructor
            ///
            /////////////////////////////////////////////////
            Widget(Widget const&) = delete;

            /////////////////////////////////////////////////
            /// \brief All widgets are constructed using WidgetPool only through the default constructor
            ///
            /////////////////////////////////////////////////
            void operator=(Widget const&) = delete;

            /////////////////////////////////////////////////
            /// \brief Default destructor
            ///
            /////////////////////////////////////////////////
            virtual ~Widget() = 0;

            /////////////////////////////////////////////////
            /// \brief Changes position of the widget
            ///
            /// \param position: new position of the widget
            ///
            /////////////////////////////////////////////////
            virtual void setPosition(const sf::Vector2f& position);

            /////////////////////////////////////////////////
            /// \brief Changes size of the widget
            ///
            /// \param size: new size of the widget
            ///
            /////////////////////////////////////////////////
            virtual void setSize(const sf::Vector2f& size);

            /////////////////////////////////////////////////
            /// \brief Changes theme of the widget
            ///
            /// \param theme: new theme of the widget
            ///
            /////////////////////////////////////////////////
            void setTheme(const Theme& theme);

            /////////////////////////////////////////////////
            /// \brief Changes padding (borders inside the widget)
            ///
            /// \param padding: new padding of the widget
            ///
            /////////////////////////////////////////////////
            void setPadding(const sf::Vector2f& padding);

            /////////////////////////////////////////////////
            /// \brief Changes background texture rectangle
            ///
            /// \param rectangle: new rectangle for the texture
            ///
            /////////////////////////////////////////////////
            void setBackgroundTextureRect(const sf::IntRect& rectangle);

            /////////////////////////////////////////////////
            /// \brief Sets the function to be called after the widget is released by the left mouse button
            ///
            /// \param doActionOnButtonRelease: a function to call
            ///
            /////////////////////////////////////////////////
            void setAction(const std::function <void()> doActionOnButtonRelease);

            /////////////////////////////////////////////////
            /// \brief Gets the current position of the widget
            ///
            /// \return Current position of the widget
            ///
            /////////////////////////////////////////////////
            sf::Vector2f getPosition() const;

            /////////////////////////////////////////////////
            /// \brief Gets the current size of the widget
            ///
            /// \return Current size of the widget
            ///
            /////////////////////////////////////////////////
            sf::Vector2f getSize() const;

            /////////////////////////////////////////////////
            /// \brief Gets the current theme of the widget
            ///
            /// \return Current theme of the widget
            ///
            /////////////////////////////////////////////////
            const Theme& getTheme() const;

            /////////////////////////////////////////////////
            /// \brief Gets the current padding of the widget
            ///
            /// \return Current padding of the widget
            ///
            /////////////////////////////////////////////////
            sf::Vector2f getPadding() const;

            /////////////////////////////////////////////////
            /// \brief Gets the current texture rectangle of the widget
            ///
            /// \return Current texture rectangle of the widget
            ///
            /////////////////////////////////////////////////
            sf::IntRect getBackgroundTextureRect() const;

            /////////////////////////////////////////////////
            /// \brief Gets the local bounds of the rectangle of the widget
            ///
            /// \return Current local bounds of the widget
            /////////////////////////////////////////////////
            sf::FloatRect getLocalBounds() const;

            /////////////////////////////////////////////////
            /// \brief Gets the global bounds of the rectangle of the widget
            ///
            /// \return Current global bounds of the widget
            ///
            /////////////////////////////////////////////////
            sf::FloatRect getGlobalBounds() const;

            /////////////////////////////////////////////////
            /// \brief Allows the widget to be drawn
            ///
            /////////////////////////////////////////////////
            void show();

            /////////////////////////////////////////////////
            /// \brief Prohibits the widget to be drawn
            ///
            /////////////////////////////////////////////////
            void hide();

            /////////////////////////////////////////////////
            /// \brief Checks if the widget is hidden
            ///
            /// \return 'true' if the widget is hidden, 'false' if it is shown
            ///
            /////////////////////////////////////////////////
            bool isHidden() const;

        protected:
            /////////////////////////////////////////////////
            /// Member data
            /////////////////////////////////////////////////
            mutable sf::RectangleShape m_rectangle;             //!< Rectangle that represents background and borders
            const Theme* m_theme;                               //!< Pointer to the theme using which the widget is stylized
            sf::Vector2f m_padding;                             //!< Padding for the content
            WidgetState m_state;                                //!< Current state of the widget
            mutable bool m_contentNeedsUpdate;                  //!< Does the content need to be updated?
            std::function <void()> m_doActionOnButtonRelease;   //!< Function to be called on the left mouse button release

            /////////////////////////////////////////////////
            /// \brief Changes styles when the state changes
            ///
            /////////////////////////////////////////////////
            virtual void refreshStyles() const;

            /////////////////////////////////////////////////
            /// \brief Changes state and calls refreshStyles()
            ///
            /// \param state: new state of the widget
            ///
            /////////////////////////////////////////////////
            void changeState(const WidgetState state);

        private:
            /////////////////////////////////////////////////
            /// \brief Forces appearance to be updated on the current frame. Should be used if the style is changed
            ///
            /////////////////////////////////////////////////
            void forceStylesUpdate() const;

            /////////////////////////////////////////////////
            /// \brief Draws the widget on the window. Inherited from sf::Drawable
            ///
            /////////////////////////////////////////////////
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

            /////////////////////////////////////////////////
            /// \brief Processes the event sent by the instance of WidgetPool. Changes styles and controls behavior
            ///
            /// \param event: event caught by the window
            /// \param mousePosition: current mouse position in the world coordinates
            ///
            /////////////////////////////////////////////////
            virtual void processEvent(const sf::Event event, const sf::Vector2f& mousePosition);
    };

    /////////////////////////////////////////////////
    /// \brief Widget that contains text
    ///
    /////////////////////////////////////////////////
    class TextBasedWidget : public Widget
    {
        public:
            /////////////////////////////////////////////////
            /// \brief Default constructor to create an empty object
            ///
            /////////////////////////////////////////////////
            TextBasedWidget();

            /////////////////////////////////////////////////
            /// \brief Default destructor
            ///
            /////////////////////////////////////////////////
            virtual ~TextBasedWidget() = 0;

            /////////////////////////////////////////////////
            /// \brief Calculates the size of the widget based on the text. Requires the string and the style to be called before
            ///
            /////////////////////////////////////////////////
            void setSizeFitToText();

            /////////////////////////////////////////////////
            /// \brief Sets the string inside the widget
            ///
            /// \param text: text to draw, non-printable characters are not affected
            ///
            /////////////////////////////////////////////////
            void setString(const sf::String& text);

            /////////////////////////////////////////////////
            /// \brief Allows the widget to split text in several lines, or forces to keep all the string in one line
            ///
            /// \param isMultiline: should the text be drawn in multiple lines?
            ///
            /////////////////////////////////////////////////
            void setMultilined(bool isMultiline);

            /////////////////////////////////////////////////
            /// \brief Changes position of the widget
            ///
            /// \param position: new position of the widget
            ///
            /////////////////////////////////////////////////
            virtual void setPosition(const sf::Vector2f& position) override;

            /////////////////////////////////////////////////
            /// \brief Changes styles when the state changes
            ///
            /////////////////////////////////////////////////
            virtual void refreshStyles() const override;

            /////////////////////////////////////////////////
            /// \brief Gets the string kept inside the widget
            ///
            /// \return The string that is drawn on the widget
            ///
            /////////////////////////////////////////////////
            sf::String getString() const;

            /////////////////////////////////////////////////
            /// \brief Gets the flag allowing the text to be split to multiple lines
            ///
            /// \return 'true' if the text is allowed to be split to multiple lines, else 'false'
            ///
            /////////////////////////////////////////////////
            bool isMultiline() const;

        protected:
            /////////////////////////////////////////////////
            /// Member data
            /////////////////////////////////////////////////
            mutable std::vector <sf::Text> m_lines;     //!< Lines of the text to be drawn
            sf::String m_string;                        //!< The whole text put in the widget
            bool m_isMultiline;                         //!< Is the text allowed to be split in multiple lines?

            /////////////////////////////////////////////////
            /// \brief Converts 'm_string' to 'm_lines' by pushing to one or splitting to several lines
            ///
            /////////////////////////////////////////////////
            void updateTextSplitting() const;

            /////////////////////////////////////////////////
            /// \brief Calculates position of each line
            ///
            /////////////////////////////////////////////////
            void placeText() const;

        private:
            /////////////////////////////////////////////////
            /// \brief Draws the widget on the window. Inherited from sf::Drawable
            ///
            /////////////////////////////////////////////////
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

    /////////////////////////////////////////////////
    /// \brief Clickable and labeled widget
    ///
    /////////////////////////////////////////////////
    class PushButton : public TextBasedWidget
    {
        public:
            /////////////////////////////////////////////////
            /// \brief Default constructor to create an empty object
            ///
            /////////////////////////////////////////////////
            PushButton();

            /////////////////////////////////////////////////
            /// \brief Default destructor
            ///
            /////////////////////////////////////////////////
            virtual ~PushButton();
    };

    /////////////////////////////////////////////////
    /// \brief A clickable widget that is represented by an icon
    ///
    /////////////////////////////////////////////////
    class IconButton : public Widget
    {
        public:
            /////////////////////////////////////////////////
            /// \brief Default constructor to create an empty object
            ///
            /////////////////////////////////////////////////
            IconButton();

            /////////////////////////////////////////////////
            /// \brief Default constructor to create an empty object
            ///
            /////////////////////////////////////////////////
            virtual ~IconButton();

            /////////////////////////////////////////////////
            /// \brief Changes position of the widget
            ///
            /// \param position: new position of the widget
            ///
            /////////////////////////////////////////////////
            virtual void setPosition(const sf::Vector2f& position) override;

            /////////////////////////////////////////////////
            /// \brief Changes size of the widget
            ///
            /// \param size: new size of the widget
            ///
            /////////////////////////////////////////////////
            virtual void setSize(const sf::Vector2f& size) override;

            /////////////////////////////////////////////////
            /// \brief Changes texture of the icon
            ///
            /// \param texture: new texture of the icon
            ///
            /////////////////////////////////////////////////
            void setIconTexture(const sf::Texture& texture);

            /////////////////////////////////////////////////
            /// \brief Changes texture rectangle of the icon
            ///
            /// \param texture: new texture rectangle of the icon
            ///
            /////////////////////////////////////////////////
            void setIconTextureRect(const sf::IntRect& rectangle);

            /////////////////////////////////////////////////
            /// \brief Gets texture of the icon
            ///
            /// \return Current texture of the icon
            ///
            /////////////////////////////////////////////////
            const sf::Texture* getIconTexture() const;

            /////////////////////////////////////////////////
            /// \brief Gets texture rectangle of the icon
            ///
            /// \return Current texture rectangle of the icon
            ///
            /////////////////////////////////////////////////
            sf::IntRect getIconTextureRect() const;

        private:
            /////////////////////////////////////////////////
            /// Member date
            /////////////////////////////////////////////////
            mutable sf::Sprite m_icon;      //!< Icon to be drawn

            /////////////////////////////////////////////////
            /// \brief Changes the size of the icon
            ///
            /////////////////////////////////////////////////
            void updateSpriteSize() const;

            /////////////////////////////////////////////////
            /// \brief Draws the widget on the window. Inherited from sf::Drawable
            ///
            /////////////////////////////////////////////////
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

    /////////////////////////////////////////////////
    /// \brief A clickable widget that is represented by a box and shows the list of items when clicked. Can be used as a drop down menu
    ///
    /////////////////////////////////////////////////
    class DropDownList : public TextBasedWidget
    {
        public:
            /////////////////////////////////////////////////
            /// \brief Default constructor to create an empty object
            ///
            /////////////////////////////////////////////////
            DropDownList();

            /////////////////////////////////////////////////
            /// \brief Default destructor
            ///
            /////////////////////////////////////////////////
            virtual ~DropDownList();

            /////////////////////////////////////////////////
            /// \brief Add new item to the list
            ///
            /// \param label: text to be shown on the new list item
            /// \param doAction: function to be called after the item receives 'MouseButtonRelease'
            ///
            /////////////////////////////////////////////////
            void addListItem(const sf::String& label, const std::function <void()> doAction);

        private:
            /////////////////////////////////////////////////
            /// Member data
            /////////////////////////////////////////////////
            std::list <PushButton> m_items;     //!< List of items. May cause the program crash if some magic moves the item in the memory: WidgetPool will have an outdated pointer
            bool m_isOpened;                    //!< Are the items shown?

            /////////////////////////////////////////////////
            /// \brief Allows all items to be shown
            ///
            /////////////////////////////////////////////////
            void showItems();

            /////////////////////////////////////////////////
            /// \brief Prohibites all items from being shown
            ///
            /////////////////////////////////////////////////
            void hideItems();

            /////////////////////////////////////////////////
            /// \brief Processes the event sent by the instance of WidgetPool. Changes styles and controls behavior
            ///
            /// \param event: event caught by the window
            /// \param mousePosition: current mouse position in the world coordinates
            ///
            /////////////////////////////////////////////////
            virtual void processEvent(const sf::Event event, const sf::Vector2f& mousePosition) override;
    };

    /////////////////////////////////////////////////
    /// \brief A gui element that allows the user to input text. Does not allow to input multilined text, but can show it if it is too long
    ///
    /////////////////////////////////////////////////
    class TextBox : public TextBasedWidget
    {
        public:
            /////////////////////////////////////////////////
            /// \brief Default constructor to create an empty object
            ///
            /////////////////////////////////////////////////
            TextBox();

            /////////////////////////////////////////////////
            /// \brief Default destructor
            ///
            /////////////////////////////////////////////////
            virtual ~TextBox();

            /////////////////////////////////////////////////
            /// \brief Gets the value showing how many symbols user can input
            ///
            /// \return Current maximum allowed length in characters
            ///
            /////////////////////////////////////////////////
            size_t getMaxInputLength() const;

            /////////////////////////////////////////////////
            /// \brief Sets the value showing how many symbols user can input
            ///
            /// \param New maximum allowed length in characters
            ///
            /////////////////////////////////////////////////
            void setMaxInputLength(const size_t maxInputLength = sf::String::InvalidPos);

        private:
            /////////////////////////////////////////////////
            /// Member data
            /////////////////////////////////////////////////
            size_t m_maxInputLength;        //!< Maximum allowed length in characters. Initially set to the maximum size of sf::String

            /////////////////////////////////////////////////
            /// \brief Processes the event sent by the instance of WidgetPool. Changes styles and controls behavior
            ///
            /// \param event: event caught by the window
            /// \param mousePosition: current mouse position in the world coordinates
            ///
            /////////////////////////////////////////////////
            virtual void processEvent(const sf::Event event, const sf::Vector2f& mousePosition) override;
    };

    /////////////////////////////////////////////////
    /// \brief A gui element that allows to set a tick or unset it. The checkmark can be any string or symbol that the font allows
    ///
    /////////////////////////////////////////////////
    class CheckBox : public TextBasedWidget
    {
        public:
            /////////////////////////////////////////////////
            /// \brief Default constructor to create an empty object
            ///
            /////////////////////////////////////////////////
            CheckBox();

            /////////////////////////////////////////////////
            /// \brief Default destructor
            ///
            /////////////////////////////////////////////////
            virtual ~CheckBox();

            /////////////////////////////////////////////////
            /// \brief Checks if the checkbox is checked with a check
            ///
            /// \return 'true' if checked, 'false' if unchecked
            ///
            /////////////////////////////////////////////////
            bool isChecked() const;

            /////////////////////////////////////////////////
            /// \brief Checks if the checkbox is checked with a check
            ///
            /// \return 'true' if checked, 'false' if unchecked
            ///
            /////////////////////////////////////////////////
            void setChecked(const bool isChecked);

        private:
            /////////////////////////////////////////////////
            /// Member data
            /////////////////////////////////////////////////
            bool m_isChecked;   //!< Is the checkmark set?

            /////////////////////////////////////////////////
            /// \brief Processes the event sent by the instance of WidgetPool. Changes styles and controls behavior
            ///
            /// \param event: event caught by the window
            /// \param mousePosition: current mouse position in the world coordinates
            ///
            /////////////////////////////////////////////////
            virtual void processEvent(const sf::Event event, const sf::Vector2f& mousePosition) override;

            /////////////////////////////////////////////////
            /// \brief Draws the widget on the window. Inherited from sf::Drawable
            ///
            /////////////////////////////////////////////////
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}   // namespace SmallGui

#endif // SMALLSFMLGUI_HPP
