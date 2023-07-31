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
                          const sf::Texture* texture = nullptr);
            virtual ~ColorSettings();

            sf::Color getFillColor() const;
            sf::Color getTextColor() const;
            sf::Color getOutlineColor() const;
            float getOutlineThickness() const;
            const sf::Texture* getTexture() const;

        private:
            const sf::Color m_fillColor;
            const sf::Color m_textColor;
            const float m_outlineThickness;
            const sf::Color m_outlineColor;
            const sf::Texture* m_texture;
    };

    class TextSettings
    {
        public:
            struct FontMetrics
            {
                float ascenderLine;
                float baseLine;
                float descenderLine;
                float fullHeight;
            };

            enum class HorizontalAlignment
            {
                Left,
                Center,
                Right
            };

            enum class VerticalAlignment
            {
                Top,
                Center,
                Bottom
            };

            TextSettings(const sf::Font& font,
                         const unsigned int characterSize,
                         const HorizontalAlignment horizontalAlignment,
                         const VerticalAlignment verticalAlignment);
            virtual ~TextSettings();

            const sf::Font& getFont() const;
            unsigned int getCharacterSize() const;
            HorizontalAlignment getHorizontalAlignment() const;
            VerticalAlignment getVerticalAlignment() const;
            const FontMetrics& getFontMetrics() const;

        private:
            const sf::Font& m_font;
            const unsigned int m_characterSize;
            const HorizontalAlignment m_horizontalAlignment;
            const VerticalAlignment m_verticalAlignment;
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
            void initialize(sf::RenderWindow* window);
            void processEvent(sf::Event event);

        private:
            WidgetPool();
            Widget* getActiveWidget() const;
            void addWidget(Widget* widget);

            const sf::RenderWindow* m_window;
            std::vector <Widget*> m_widgets;
            Widget* m_lastHoveredWidget;
    };

    class Widget : public sf::Drawable
    {
        friend class WidgetPool;

        public:
            Widget(const sf::Vector2f& position, const sf::String& text, const Style& style);
            Widget(const sf::Vector2f& position, const sf::Vector2f& size, const sf::String& text, const Style& style);
            virtual ~Widget();

            sf::String getLabel() const;
            sf::FloatRect getLocalBounds() const;
            sf::FloatRect getGlobalBounds() const;

        private:
            Widget(const sf::String& text, const Style& style);

            sf::RectangleShape m_rectangle;
            sf::Text m_label;
            const Style& m_style;
            const sf::Vector2f m_padding;

            const std::function <void()> m_doActionOnClick;
            void press();
            void release();
            void hover();

            // Inherited from sf::Drawable
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };
}

#endif // SFMLGUI_HPP
