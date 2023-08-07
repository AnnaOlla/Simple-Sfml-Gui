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

    class DecorationSettings
    {
        public:
            DecorationSettings(const sf::Color fillColor = sf::Color::White,
                               const sf::Color textColor = sf::Color::Black,
                               const sf::Uint32 textStyle = sf::Text::Style::Regular,
                               const float outlineThickness = 1.0f,
                               const sf::Color outlineColor = sf::Color::Black,
                               const sf::Texture* backgroundTexture = nullptr);
            virtual ~DecorationSettings();

            sf::Color getFillColor() const;
            sf::Color getTextColor() const;
            sf::Uint32 getTextStyle() const;
            sf::Color getOutlineColor() const;
            float getOutlineThickness() const;
            const sf::Texture* getBackgroundTexture() const;

            void setFillColor(const sf::Color fillColor);
            void setTextColor(const sf::Color textColor);
            void setTextStyle(const sf::Uint32 textStyle);
            void setOutlineThickness(const float outlineThickness);
            void setOutlineColor(const sf::Color outlineColor);
            void setBackgroundTexture(const sf::Texture* backgroundTexture);

        private:
            sf::Color m_fillColor;
            sf::Color m_textColor;
            sf::Uint32 m_textStyle;
            float m_outlineThickness;
            sf::Color m_outlineColor;
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

            void setFont(const sf::Font& font);
            void setCharacterSize(const unsigned int characterSize);
            void setHorizontalAlignment(const TextHorizontalAlignment horizontalAlignment);
            void setVerticalAlignment(const TextVerticalAlignment verticalAlignment);

        private:
            const sf::Font* m_font;
            unsigned int m_characterSize;
            TextHorizontalAlignment m_horizontalAlignment;
            TextVerticalAlignment m_verticalAlignment;
            FontMetrics m_fontMetrics;

            FontMetrics calculateFontMetrics();
    };

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
            virtual ~Widget() = 0;

            virtual void setPosition(const sf::Vector2f& position);
            virtual void setSize(const sf::Vector2f& size);
            void setTheme(const Theme& theme);
            void setPadding(const sf::Vector2f& padding);
            void setBackgroundTextureRect(const sf::IntRect& rectangle);

            sf::Vector2f getPosition() const;
            sf::Vector2f getSize() const;
            const Theme& getTheme() const;
            sf::Vector2f getPadding() const;
            sf::IntRect getBackgroundTextureRect() const;

            sf::FloatRect getLocalBounds() const;
            sf::FloatRect getGlobalBounds() const;
            void forceStylesUpdate() const;

        protected:
            mutable sf::RectangleShape m_rectangle;
            const Theme* m_theme;
            sf::Vector2f m_padding;

            WidgetState m_state;
            mutable bool m_contentNeedsUpdate;

            virtual void refreshStyles() const;

        private:
            void changeState(const WidgetState state);

            // Inherited from sf::Drawable
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    };

    class TextBasedWidget : public Widget
    {
        public:
            TextBasedWidget();
            virtual ~TextBasedWidget() = 0;

            virtual void setPosition(const sf::Vector2f& position) override;
            virtual void refreshStyles() const override;

            sf::String getString() const;
            bool getMultiline() const;
            void setString(const sf::String& text);
            void setMultiline(bool isMultiline);

        protected:
            mutable std::vector <sf::Text> m_lines;
            sf::String m_string;
            bool m_isMultiline;

        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void updateTextSplitting() const;
            void placeText() const;
    };

    class Clickable
    {
        friend class WidgetPool;

        public:
            Clickable();
            virtual ~Clickable() = 0;
            void setAction(const std::function <void()> doActionOnButtonRelease);

        private:
            std::function <void()> m_doActionOnButtonRelease;
    };

    class PushButton : public TextBasedWidget, public Clickable
    {
        public:
            PushButton();
            virtual ~PushButton();
    };

    class IconButton : public Widget, public Clickable
    {
        public:
            IconButton();
            virtual ~IconButton();

            virtual void setPosition(const sf::Vector2f& position) override;
            virtual void setSize(const sf::Vector2f& size) override;
            void setIconTexture(const sf::Texture& texture);
            void setIconTextureRect(const sf::IntRect& rectangle);

            const sf::Texture* getIconTexture() const;
            sf::IntRect getIconTextureRect() const;

        private:
            mutable sf::Sprite m_icon;

            void updateSpriteSize() const;
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}

#endif // SFMLGUI_HPP
