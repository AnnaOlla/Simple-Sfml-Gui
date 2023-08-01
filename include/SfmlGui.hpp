#ifndef SFMLGUI_HPP
#define SFMLGUI_HPP

#include <SFML/Graphics.hpp>

#include <vector>
#include <functional>

namespace SfGui
{
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
                         const unsigned int characterSize,
                         const TextHorizontalAlignment horizontalAlignment,
                         const TextVerticalAlignment verticalAlignment);
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

    struct Style
    {
        public:
            Style(const TextSettings&, const ColorSettings&, const ColorSettings&, const ColorSettings&);
            virtual ~Style();

            const TextSettings& getTextSettings() const;
            const ColorSettings& getIdleColorSettings() const;
            const ColorSettings& getHoverColorSettings() const;
            const ColorSettings& getPressColorSettings() const;

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
            Widget(const sf::Vector2f& position,
                   const Style& style,
                   const std::function <void()> doActionOnButtonRelease);
            virtual ~Widget();

            sf::FloatRect getLocalBounds() const;
            sf::FloatRect getGlobalBounds() const;

        protected:
            sf::RectangleShape m_rectangle;
            const Style& m_style;
            const sf::Vector2f m_padding;

            const std::function <void()> m_doActionOnClick;
            virtual void setPressedStyle() = 0;
            virtual void setIdleStyle() = 0;
            virtual void setHoverStyle() = 0;

        private:
            // Inherited from sf::Drawable
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    };

    class TextBasedWidget : public Widget
    {
        public:
            TextBasedWidget(const sf::Vector2f& position,
                            const sf::String& text,
                            const Style& style,
                            const std::function <void()> doActionOnButtonRelease);
            TextBasedWidget(const sf::Vector2f& position,
                            const sf::Vector2f& size,
                            const sf::String& text,
                            const Style& style,
                            const std::function <void()> doActionOnButtonRelease);
            virtual ~TextBasedWidget();

            sf::String getText() const;

        protected:
            sf::Text m_text;

            virtual void setPressedStyle() override;
            virtual void setIdleStyle() override;
            virtual void setHoverStyle() override;

        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

    class PushButton : public TextBasedWidget
    {
        public:
            PushButton(const sf::Vector2f& position,
                       const sf::String& text,
                       const Style& style,
                       const std::function <void()> onButtonRelease = nullptr);
            PushButton(const sf::Vector2f& position,
                       const sf::Vector2f& size,
                       const sf::String& text,
                       const Style& style,
                       const std::function <void()> onButtonRelease = nullptr);
            virtual ~PushButton();
    };
}

#endif // SFMLGUI_HPP
