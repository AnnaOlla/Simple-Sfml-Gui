#include "SfmlGui.hpp"
#include <iostream>

namespace SfGui
{

ColorSettings::ColorSettings(const sf::Color fillColor,
                             const sf::Color textColor,
                             const float outlineThickness,
                             const sf::Color outlineColor,
                             const sf::Texture* backgroundTexture) :
    m_fillColor(fillColor),
    m_textColor(textColor),
    m_outlineThickness(outlineThickness > 0 ? -outlineThickness : outlineThickness),
    m_outlineColor(outlineColor),
    m_backgroundTexture(backgroundTexture)
{
    //ctor
}

ColorSettings::~ColorSettings()
{
    //dtor
}

sf::Color ColorSettings::getFillColor() const
{
    return m_fillColor;
}

sf::Color ColorSettings::getTextColor() const
{
    return m_textColor;
}

sf::Color ColorSettings::getOutlineColor() const
{
    return m_outlineColor;
}

float ColorSettings::getOutlineThickness() const
{
    return m_outlineThickness;
}

const sf::Texture* ColorSettings::getBackgroundTexture() const
{
    return m_backgroundTexture;
}

TextSettings::TextSettings(const sf::Font& font,
                           const unsigned int characterSize,
                           const TextHorizontalAlignment horizontalAlignment,
                           const TextVerticalAlignment verticalAlignment) :
    m_font(font),
    m_characterSize(characterSize),
    m_horizontalAlignment(horizontalAlignment),
    m_verticalAlignment(verticalAlignment),
    m_fontMetrics(calculateFontMetrics())
{
    //ctor
}

TextSettings::~TextSettings()
{
    //dtor
}

const sf::Font& TextSettings::getFont() const
{
    return m_font;
}

unsigned int TextSettings::getCharacterSize() const
{
    return m_characterSize;
}

TextHorizontalAlignment TextSettings::getHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

TextVerticalAlignment TextSettings::getVerticalAlignment() const
{
    return m_verticalAlignment;
}

const FontMetrics& TextSettings::getFontMetrics() const
{
    return m_fontMetrics;
}

FontMetrics TextSettings::calculateFontMetrics()
{
    FontMetrics metrics;

    sf::Text temp;
    temp.setFont(m_font);
    temp.setCharacterSize(m_characterSize);

    // Just some magic to apply vertical alignment correctly
    // (some fonts will have it a little incorrect)

    temp.setString(L"A");
    metrics.ascenderLine = temp.getLocalBounds().top;
    metrics.baseLine = temp.getLocalBounds().height;

    temp.setString(L"j");
    metrics.descenderLine = temp.getLocalBounds().height - metrics.baseLine;
    metrics.fullHeight = metrics.ascenderLine + metrics.baseLine + metrics.descenderLine;

    return metrics;
}

Style::Style(const TextSettings& textSettings,
             const ColorSettings& idleColorSettings,
             const ColorSettings& hoverColorSettings,
             const ColorSettings& pressColorSettings) :
    m_textSettings(textSettings),
    m_idleColorSettings(idleColorSettings),
    m_hoverColorSettings(hoverColorSettings),
    m_pressColorSettings(pressColorSettings)
{
    //ctor
}

Style::~Style()
{
    //dtor
}

const TextSettings& Style::getTextSettings() const
{
    return m_textSettings;
}

const ColorSettings& Style::getIdleColorSettings() const
{
    return m_idleColorSettings;
}

const ColorSettings& Style::getHoverColorSettings() const
{
    return m_hoverColorSettings;
}

const ColorSettings& Style::getPressColorSettings() const
{
    return m_pressColorSettings;
}

WidgetPool::WidgetPool() : m_window(nullptr), m_lastHoveredWidget(nullptr), m_lastClickedWidget(nullptr)
{
    //ctor
}

WidgetPool::~WidgetPool()
{
    //dtor
}

WidgetPool& WidgetPool::getInstance()
{
    static WidgetPool instance;
    return instance;
}

void WidgetPool::initialize(const sf::RenderWindow* window)
{
    m_window = window;
}

void WidgetPool::addWidget(Widget* widget)
{
    m_widgets.push_back(widget);
}

void WidgetPool::processEvent(const sf::Event event)
{
    // Supported events;
    if (event.type != sf::Event::MouseButtonPressed && event.type != sf::Event::MouseButtonReleased &&
        event.type != sf::Event::MouseMoved && event.type != sf::Event::MouseLeft &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        return;
    }

    Widget* activeWidget = getActiveWidget();

    switch (event.type)
    {
        case sf::Event::MouseLeft:
        {
            if (m_lastHoveredWidget != nullptr)
            {
                m_lastHoveredWidget->setIdleStyle();
                m_lastHoveredWidget = nullptr;
            }

            break;
        }

        case sf::Event::MouseButtonPressed:
        {
            if (activeWidget == nullptr)
                break;

            activeWidget->setPressedStyle();

            m_lastHoveredWidget = activeWidget;
            m_lastClickedWidget = activeWidget;
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            if (activeWidget == nullptr)
                break;

            // Check whether focus had moved to other element before mouse was released
            // We do not need to activate element that was not clicked
            if (m_lastClickedWidget == activeWidget)
            {
                activeWidget->setHoverStyle();
                if (activeWidget->m_doActionOnClick != nullptr)
                    activeWidget->m_doActionOnClick();
            }

            m_lastHoveredWidget = activeWidget;
            break;
        }

        case sf::Event::MouseMoved:
        {
            // Unfocus element
            if (activeWidget == nullptr)
            {
                if (m_lastHoveredWidget != nullptr)
                    m_lastHoveredWidget->setIdleStyle();
            }

            // Change focus
            else if (m_lastHoveredWidget != nullptr)
            {
                if (m_lastHoveredWidget != activeWidget)
                {
                    m_lastHoveredWidget->setIdleStyle();
                    activeWidget->setHoverStyle();
                }
            }

            // Set focus
            else
                activeWidget->setHoverStyle();

            m_lastHoveredWidget = activeWidget;
            break;
        }

        default:
            break;
    }
}

Widget* WidgetPool::getActiveWidget() const
{
    auto position = m_window->mapPixelToCoords((sf::Mouse::getPosition(*m_window)));
    auto isSelected = [&position](Widget* w) { return w->getGlobalBounds().contains(position); };

    // Widgets are drawn upon each other. The later we draw them, the bigger index they have
    // We need to select the top widget
    auto it = std::find_if(std::reverse_iterator(m_widgets.cend()), std::reverse_iterator(m_widgets.cbegin()), isSelected);
    return it != std::reverse_iterator(m_widgets.cbegin()) ? *it : nullptr;
}

Widget::Widget(const sf::Vector2f& position, const Style& style, const std::function <void()> onButtonRelease)
    : m_style(style), m_padding(5.0f, 10.0f), m_doActionOnClick(onButtonRelease)
{
    auto& ui = WidgetPool::getInstance();
    ui.addWidget(this);

    const auto& colorSettings = m_style.getIdleColorSettings();

    m_rectangle.setPosition(position);
    m_rectangle.setFillColor(colorSettings.getFillColor());
    m_rectangle.setOutlineThickness(colorSettings.getOutlineThickness());
    m_rectangle.setOutlineColor(colorSettings.getOutlineColor());
    m_rectangle.setTexture(colorSettings.getBackgroundTexture());
}

Widget::~Widget()
{
    // dtor
}

sf::FloatRect Widget::getLocalBounds() const
{
    return m_rectangle.getLocalBounds();
}

sf::FloatRect Widget::getGlobalBounds() const
{
    return m_rectangle.getGlobalBounds();
}

void Widget::setPressedStyle()
{
    // pure virtual
}

void Widget::setIdleStyle()
{
    // pure virtual
}

void Widget::setHoverStyle()
{
    // pure virtual
}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // pure virtual
}

TextBasedWidget::TextBasedWidget(const sf::Vector2f& position,
                                 const sf::String& text,
                                 const Style& style,
                                 const std::function <void()> onButtonRelease) : Widget(position, style, onButtonRelease)
{
    const auto& textSettings = m_style.getTextSettings();
    const auto& colorSettings = m_style.getIdleColorSettings();

    m_text.setString(text);
    m_text.setFont(textSettings.getFont());
    m_text.setCharacterSize(textSettings.getCharacterSize());
    m_text.setFillColor(colorSettings.getTextColor());

    m_text.setPosition(position.x + m_padding.x, position.y + m_padding.y);
    m_rectangle.setSize(sf::Vector2f(m_text.getLocalBounds().left + m_text.getLocalBounds().width + 2 * m_padding.x,
                                     style.getTextSettings().getFontMetrics().fullHeight + 2 * m_padding.y));
}

TextBasedWidget::TextBasedWidget(const sf::Vector2f& position,
                                 const sf::Vector2f& size,
                                 const sf::String& text,
                                 const Style& style,
                                 const std::function <void()> onButtonRelease) : Widget(position, style, onButtonRelease)
{
    const auto& textSettings = m_style.getTextSettings();
    const auto& metrics = textSettings.getFontMetrics();
    const auto& colorSettings = m_style.getIdleColorSettings();

    m_text.setString(text);
    m_text.setFont(textSettings.getFont());
    m_text.setCharacterSize(textSettings.getCharacterSize());
    m_text.setFillColor(colorSettings.getTextColor());

    sf::Vector2f textPosition;

    switch (textSettings.getHorizontalAlignment())
    {
        case TextHorizontalAlignment::Left:
            textPosition.x = position.x + m_padding.x;
            break;

        case TextHorizontalAlignment::Center:
            textPosition.x = position.x + (size.x - m_text.getLocalBounds().width) / 2.0f - m_text.getLocalBounds().left;
            break;

        case TextHorizontalAlignment::Right:
            textPosition.x = position.x + size.x - m_padding.x - m_text.getLocalBounds().width;
            break;
    }

    switch (textSettings.getVerticalAlignment())
    {
        case TextVerticalAlignment::Top:
            textPosition.y = position.y + m_padding.y;
            break;

        case TextVerticalAlignment::Center:
            textPosition.y = position.y + (size.y - metrics.baseLine) / 2.0f - metrics.ascenderLine;
            break;

        case TextVerticalAlignment::Bottom:
            textPosition.y = position.y + size.y - m_padding.y - metrics.fullHeight;
            break;
    }

    m_text.setPosition(textPosition);

    m_rectangle.setSize(size);
}

TextBasedWidget::~TextBasedWidget()
{
    //dtor
}

sf::String TextBasedWidget::getText() const
{
    return m_text.getString();
}

void TextBasedWidget::setPressedStyle()
{
    const auto& style = m_style.getPressColorSettings();

    m_rectangle.setFillColor(style.getFillColor());
    m_rectangle.setOutlineThickness(style.getOutlineThickness());
    m_rectangle.setOutlineColor(style.getOutlineColor());
    m_rectangle.setTexture(style.getBackgroundTexture());

    m_text.setFillColor(style.getTextColor());
}

void TextBasedWidget::setIdleStyle()
{
    const auto& style = m_style.getIdleColorSettings();

    m_rectangle.setFillColor(style.getFillColor());
    m_rectangle.setOutlineThickness(style.getOutlineThickness());
    m_rectangle.setOutlineColor(style.getOutlineColor());
    m_rectangle.setTexture(style.getBackgroundTexture());

    m_text.setFillColor(style.getTextColor());
}

void TextBasedWidget::setHoverStyle()
{
    const auto& style = m_style.getHoverColorSettings();

    m_rectangle.setFillColor(style.getFillColor());
    m_rectangle.setOutlineThickness(style.getOutlineThickness());
    m_rectangle.setOutlineColor(style.getOutlineColor());
    m_rectangle.setTexture(style.getBackgroundTexture());

    m_text.setFillColor(style.getTextColor());
}

void TextBasedWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_rectangle);

    // Clip text
    auto bounds = m_rectangle.getGlobalBounds();
    bounds.left += m_padding.x;
    bounds.top += m_padding.y;
    bounds.width -= m_padding.x * 2;
    bounds.height -= m_padding.y * 2;

    const auto size = target.mapPixelToCoords(static_cast <sf::Vector2i>(target.getSize()));
    sf::View view(bounds);
    view.setViewport(sf::FloatRect(bounds.left / size.x, bounds.top / size.y,
                                   bounds.width / size.x, bounds.height / size.y));
    target.setView(view);

    target.draw(m_text);

    target.setView(target.getDefaultView());
}

PushButton::PushButton(const sf::Vector2f& position,
                       const sf::Vector2f& size,
                       const sf::String& text,
                       const Style& style,
                       std::function <void()> onButtonRelease) :
    TextBasedWidget(position, size, text, style, onButtonRelease)
{
    //ctor
}

PushButton::PushButton(const sf::Vector2f& position,
                       const sf::String& text,
                       const Style& style,
                       std::function <void()> onButtonRelease) :
    TextBasedWidget(position, text, style, onButtonRelease)
{
    //ctor
}

PushButton::~PushButton()
{
    //dtor
}

}
