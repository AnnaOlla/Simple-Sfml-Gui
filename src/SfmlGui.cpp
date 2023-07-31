#include "SfmlGui.hpp"
#include <iostream>

namespace SfGui
{

ColorSettings::ColorSettings(const sf::Color fillColor,
                             const sf::Color textColor,
                             const float outlineThickness,
                             const sf::Color outlineColor,
                             const sf::Texture* texture) :
    m_fillColor(fillColor),
    m_textColor(textColor),
    m_outlineThickness(outlineThickness > 0 ? -outlineThickness : outlineThickness),
    m_outlineColor(outlineColor),
    m_texture(texture)
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

const sf::Texture* ColorSettings::getTexture() const
{
    return m_texture;
}

TextSettings::TextSettings(const sf::Font& font,
                           const unsigned int characterSize,
                           const HorizontalAlignment horizontalAlignment,
                           const VerticalAlignment verticalAlignment) :
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

TextSettings::HorizontalAlignment TextSettings::getHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

TextSettings::VerticalAlignment TextSettings::getVerticalAlignment() const
{
    return m_verticalAlignment;
}

const TextSettings::FontMetrics& TextSettings::getFontMetrics() const
{
    return m_fontMetrics;
}

TextSettings::FontMetrics TextSettings::calculateFontMetrics()
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

WidgetPool::WidgetPool() : m_window(nullptr), m_lastHoveredWidget(nullptr)
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

void WidgetPool::initialize(sf::RenderWindow* window)
{
    m_window = window;
}

void WidgetPool::addWidget(Widget* widget)
{
    m_widgets.push_back(widget);
}

void WidgetPool::processEvent(sf::Event event)
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
                m_lastHoveredWidget->release();
                m_lastHoveredWidget = nullptr;
            }

            break;
        }

        case sf::Event::MouseButtonPressed:
        {
            if (activeWidget == nullptr)
                break;

            activeWidget->press();

            m_lastHoveredWidget = activeWidget;
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            if (activeWidget == nullptr)
                break;

            activeWidget->hover();

            if (activeWidget->m_doActionOnClick != nullptr)
            {
                // Check whether focus had moved to other element before mouse was released
                // We do not need to activate element that was not clicked
                if (m_lastHoveredWidget == activeWidget || m_lastHoveredWidget == nullptr)
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
                    m_lastHoveredWidget->release();
            }

            // Change focus
            else if (m_lastHoveredWidget != nullptr)
            {
                if (m_lastHoveredWidget != activeWidget)
                {
                    m_lastHoveredWidget->release();
                    activeWidget->hover();
                }
            }

            // Set focus
            else
                activeWidget->hover();

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

    auto it = std::find_if(std::reverse_iterator(m_widgets.cend()), std::reverse_iterator(m_widgets.cbegin()), isSelected);
    return it != std::reverse_iterator(m_widgets.cbegin()) ? *it : nullptr;
}

Widget::Widget(const sf::String& text, const Style& style) : m_style(style), m_padding(5.0f, 10.0f)
{
    auto& ui = WidgetPool::getInstance();
    ui.addWidget(this);

    const auto& textSettings = m_style.getTextSettings();
    const auto& colorSettings = m_style.getIdleColorSettings();

    m_label.setString(text);
    m_label.setFont(textSettings.getFont());
    m_label.setCharacterSize(textSettings.getCharacterSize());
    m_label.setFillColor(colorSettings.getTextColor());

    m_rectangle.setFillColor(colorSettings.getFillColor());
    m_rectangle.setOutlineThickness(colorSettings.getOutlineThickness());
    m_rectangle.setOutlineColor(colorSettings.getOutlineColor());
    m_rectangle.setTexture(colorSettings.getTexture());
}

Widget::Widget(const sf::Vector2f& position, const sf::String& text, const Style& style) : Widget(text, style)
{
    m_label.setPosition(position.x + m_padding.x, position.y + m_padding.y);

    m_rectangle.setPosition(position);
    m_rectangle.setSize(sf::Vector2f(m_label.getLocalBounds().left + m_label.getLocalBounds().width + 2 * m_padding.x,
                                     style.getTextSettings().getFontMetrics().fullHeight + 2 * m_padding.y));
}

Widget::Widget(const sf::Vector2f& position, const sf::Vector2f& size, const sf::String& text, const Style& style) : Widget(text, style)
{
    const auto& textSettings = m_style.getTextSettings();
    const auto& metrics = textSettings.getFontMetrics();

    sf::Vector2f textPosition;

    switch (textSettings.getHorizontalAlignment())
    {
        case TextSettings::HorizontalAlignment::Left:
            textPosition.x = position.x + m_padding.x;
            break;

        case TextSettings::HorizontalAlignment::Center:
            textPosition.x = position.x + (size.x - m_label.getLocalBounds().width) / 2.0f - m_label.getLocalBounds().left;
            break;

        case TextSettings::HorizontalAlignment::Right:
            textPosition.x = position.x + size.x - m_padding.x - m_label.getLocalBounds().width;
            break;
    }

    switch (textSettings.getVerticalAlignment())
    {
        case TextSettings::VerticalAlignment::Top:
            textPosition.y = position.y + m_padding.y;
            break;

        case TextSettings::VerticalAlignment::Center:
            textPosition.y = position.y + (size.y - metrics.baseLine) / 2.0f - metrics.ascenderLine;
            break;

        case TextSettings::VerticalAlignment::Bottom:
            textPosition.y = position.y + size.y - m_padding.y - metrics.fullHeight;
            break;
    }

    m_label.setPosition(textPosition);

    m_rectangle.setSize(size);
    m_rectangle.setPosition(position);
}

Widget::~Widget()
{
    // dtor
}

sf::String Widget::getLabel() const
{
    return m_label.getString();
}

sf::FloatRect Widget::getLocalBounds() const
{
    return m_rectangle.getLocalBounds();
}

sf::FloatRect Widget::getGlobalBounds() const
{
    return m_rectangle.getGlobalBounds();
}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const
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

    target.draw(m_label);

    target.setView(target.getDefaultView());
}

void Widget::press()
{
    const auto& style = m_style.getPressColorSettings();

    m_rectangle.setFillColor(style.getFillColor());
    m_rectangle.setOutlineThickness(style.getOutlineThickness());
    m_rectangle.setOutlineColor(style.getOutlineColor());
    m_rectangle.setTexture(style.getTexture());

    m_label.setFillColor(style.getTextColor());
}

void Widget::release()
{
    const auto& style = m_style.getIdleColorSettings();

    m_rectangle.setFillColor(style.getFillColor());
    m_rectangle.setOutlineThickness(style.getOutlineThickness());
    m_rectangle.setOutlineColor(style.getOutlineColor());
    m_rectangle.setTexture(style.getTexture());

    m_label.setFillColor(style.getTextColor());
}

void Widget::hover()
{
    const auto& style = m_style.getHoverColorSettings();

    m_rectangle.setFillColor(style.getFillColor());
    m_rectangle.setOutlineThickness(style.getOutlineThickness());
    m_rectangle.setOutlineColor(style.getOutlineColor());
    m_rectangle.setTexture(style.getTexture());

    m_label.setFillColor(style.getTextColor());
}

}
