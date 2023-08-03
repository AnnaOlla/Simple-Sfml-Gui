#ifndef SFMLGUI_HPP
#define SFMLGUI_HPP

#include <SFML/Graphics.hpp>

#include <vector>
#include <functional>
#include <cmath>

namespace SfGui
{
    enum class WidgetState
    {
        Idle,
        Hovered,
        Pressed
    };

    class ColorSettings
    {
        public:
            ColorSettings(const sf::Color fillColor = sf::Color::White,
                          const sf::Color textColor = sf::Color::Black,
                          const float outlineThickness = 1.0f,
                          const sf::Color outlineColor = sf::Color::Black,
                          const sf::Texture* backgroundTexture = nullptr);
            virtual ~ColorSettings();

            sf::Color getFillColor() const;
            sf::Color getTextColor() const;
            sf::Color getOutlineColor() const;
            float getOutlineThickness() const;
            const sf::Texture* getBackgroundTexture() const;

        private:
            const sf::Color m_fillColor;
            const sf::Color m_textColor;
            const float m_outlineThickness;
            const sf::Color m_outlineColor;
            const sf::Texture* m_backgroundTexture;
    };

    struct FontMetrics
    {
        float ascenderLine;
        float baseLine;
        float descenderLine;
        float fullHeight;
    };

    enum class TextHorizontalAlignment
    {
        Left,
        Center,
        Right
    };

    enum class TextVerticalAlignment
    {
        Top,
        Center,
        Bottom
    };

    class TextSettings
    {
        public:
            TextSettings(const sf::Font& font,
                         const unsigned int characterSize = 30,
                         const TextHorizontalAlignment horizontalAlignment = TextHorizontalAlignment::Center,
                         const TextVerticalAlignment verticalAlignment = TextVerticalAlignment::Center);
            virtual ~TextSettings();

            const sf::Font& getFont() const;
            unsigned int getCharacterSize() const;
            TextHorizontalAlignment getHorizontalAlignment() const;
            TextVerticalAlignment getVerticalAlignment() const;
            const FontMetrics& getFontMetrics() const;

        private:
            const sf::Font& m_font;
            const unsigned int m_characterSize;
            const TextHorizontalAlignment m_horizontalAlignment;
            const TextVerticalAlignment m_verticalAlignment;
            const FontMetrics m_fontMetrics;

            FontMetrics calculateFontMetrics();
    };

    struct Theme
    {
        public:
            Theme(const TextSettings& text,
                  const ColorSettings& idle,
                  const ColorSettings& hover,
                  const ColorSettings& press);
            virtual ~Theme();

            const TextSettings& getTextSettings() const;
            const ColorSettings& getIdleColorSettings() const;
            const ColorSettings& getHoveredColorSettings() const;
            const ColorSettings& getPressedColorSettings() const;

        private:
            const TextSettings& m_textSettings;
            const ColorSettings& m_idleColorSettings;
            const ColorSettings& m_hoverColorSettings;
            const ColorSettings& m_pressColorSettings;
    };

    class Widget;

    class WidgetPool
    {
        friend class Widget;

        public:
            virtual ~WidgetPool();
            WidgetPool(WidgetPool const&) = delete;
            void operator=(WidgetPool const&) = delete;

            static WidgetPool& getInstance();
            void initialize(const sf::RenderWindow* window);
            void processEvent(const sf::Event event);

        private:
            WidgetPool();
            Widget* getActiveWidget() const;
            void addWidget(Widget* widget);

            const sf::RenderWindow* m_window;
            std::vector <Widget*> m_widgets;
            Widget* m_lastHoveredWidget;
            Widget* m_lastClickedWidget;
    };

    class Widget : public sf::Drawable
    {
        friend class WidgetPool;

        public:
            Widget();
            virtual ~Widget();

            virtual void refreshStyles();

            virtual void setPosition(const sf::Vector2f& position);
            virtual void setSize(const sf::Vector2f& size);
            void setTheme(const Theme& theme);
            void setAction(const std::function <void()> doActionOnButtonRelease);

            sf::Vector2f getPosition() const;
            sf::Vector2f getSize() const;
            const Theme& getTheme() const;
            sf::FloatRect getLocalBounds() const;
            sf::FloatRect getGlobalBounds() const;

        protected:
            sf::RectangleShape m_rectangle;
            const Theme* m_theme;
            sf::Vector2f m_padding;
            WidgetState m_state;
            std::function <void()> m_doActionOnClick;

        private:
            void changeState(const WidgetState state);

            // Inherited from sf::Drawable
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    };

    class TextBasedWidget : public Widget
    {
        public:
            TextBasedWidget();
            virtual ~TextBasedWidget();

            virtual void setPosition(const sf::Vector2f& position) override;
            virtual void refreshStyles() override;

            sf::String getString(const sf::String separator = sf::String(L" ")) const;
            void setString(const sf::String& text, const bool isMultiline = false);

        protected:
            std::vector <sf::Text> m_lines;

        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void splitTextToLines(const sf::String& text);
            void placeText();
    };

    class PushButton : public TextBasedWidget
    {
        public:
            PushButton();
            virtual ~PushButton();
    };
}

#endif // SFMLGUI_HPP
