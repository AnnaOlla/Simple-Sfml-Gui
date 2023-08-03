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

Theme::Theme(const TextSettings& textSettings,
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

Theme::~Theme()
{
    //dtor
}

const TextSettings& Theme::getTextSettings() const
{
    return m_textSettings;
}

const ColorSettings& Theme::getIdleColorSettings() const
{
    return m_idleColorSettings;
}

const ColorSettings& Theme::getHoveredColorSettings() const
{
    return m_hoverColorSettings;
}

const ColorSettings& Theme::getPressedColorSettings() const
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
                m_lastHoveredWidget->changeState(WidgetState::Idle);
                m_lastHoveredWidget = nullptr;
            }

            break;
        }

        case sf::Event::MouseButtonPressed:
        {
            if (activeWidget == nullptr)
                break;

            activeWidget->changeState(WidgetState::Pressed);

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
                activeWidget->changeState(WidgetState::Hovered);
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
                    m_lastHoveredWidget->changeState(WidgetState::Idle);
            }

            // Change focus
            else if (m_lastHoveredWidget != nullptr)
            {
                if (m_lastHoveredWidget != activeWidget)
                {
                    m_lastHoveredWidget->changeState(WidgetState::Idle);
                    activeWidget->changeState(WidgetState::Hovered);
                }
            }

            // Set focus
            else
                activeWidget->changeState(WidgetState::Hovered);

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

Widget::Widget() : m_theme(nullptr), m_padding(5.0f, 10.0f), m_state(WidgetState::Idle)
{
    auto& ui = WidgetPool::getInstance();
    ui.addWidget(this);
}

Widget::~Widget()
{
    // dtor
}

void Widget::setPosition(const sf::Vector2f& position)
{
    m_rectangle.setPosition(position);
}

void Widget::setSize(const sf::Vector2f& size)
{
    m_rectangle.setSize(size);
}

void Widget::setTheme(const Theme& theme)
{
    m_theme = &theme;
    refreshStyles();
}

void Widget::setAction(const std::function <void()> doActionOnButtonRelease)
{
    m_doActionOnClick = doActionOnButtonRelease;
}

sf::FloatRect Widget::getLocalBounds() const
{
    return m_rectangle.getLocalBounds();
}

sf::FloatRect Widget::getGlobalBounds() const
{
    return m_rectangle.getGlobalBounds();
}

void Widget::refreshStyles()
{
    const ColorSettings* colorSettings;
    switch (m_state)
    {
        case WidgetState::Idle:
            colorSettings = &(m_theme->getIdleColorSettings());
            break;

        case WidgetState::Hovered:
            colorSettings = &(m_theme->getHoveredColorSettings());
            break;

        case WidgetState::Pressed:
            colorSettings = &(m_theme->getPressedColorSettings());
            break;
    }

    m_rectangle.setFillColor(colorSettings->getFillColor());
    m_rectangle.setOutlineThickness(colorSettings->getOutlineThickness());
    m_rectangle.setOutlineColor(colorSettings->getOutlineColor());
    m_rectangle.setTexture(colorSettings->getBackgroundTexture());
}

void Widget::changeState(const WidgetState state)
{
    m_state = state;
    refreshStyles();
}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // pure virtual
}

TextBasedWidget::TextBasedWidget() : Widget()
{
    //ctor
}

TextBasedWidget::~TextBasedWidget()
{
    //dtor
}

void TextBasedWidget::setPosition(const sf::Vector2f& position)
{
    Widget::setPosition(position);
    placeText();
}

void TextBasedWidget::refreshStyles()
{
    const auto& textSettings = m_theme->getTextSettings();

    const ColorSettings* colorSettings;
    switch (m_state)
    {
        case WidgetState::Idle:
            colorSettings = &(m_theme->getIdleColorSettings());
            break;

        case WidgetState::Hovered:
            colorSettings = &(m_theme->getHoveredColorSettings());
            break;

        case WidgetState::Pressed:
            colorSettings = &(m_theme->getPressedColorSettings());
            break;
    }

    m_rectangle.setFillColor(colorSettings->getFillColor());
    m_rectangle.setOutlineThickness(colorSettings->getOutlineThickness());
    m_rectangle.setOutlineColor(colorSettings->getOutlineColor());
    m_rectangle.setTexture(colorSettings->getBackgroundTexture());

    for (auto& line : m_lines)
    {
        line.setFont(textSettings.getFont());
        line.setCharacterSize(textSettings.getCharacterSize());
        line.setFillColor(colorSettings->getTextColor());
    }

    placeText();
}

sf::String TextBasedWidget::getString(const sf::String separator) const
{
    sf::String result;

    for (size_t i = 0; i < m_lines.size(); i++)
    {
        result += m_lines[i].getString();
        if (i < m_lines.size() - 1)
            result += separator;
    }

    return result;
}

void TextBasedWidget::setString(const sf::String& text, const bool isMultiline)
{
    m_lines.clear();

    if (isMultiline)
        splitTextToLines(text);
    else
    {
        m_lines.emplace_back();
        m_lines.back().setString(text);
    }

    placeText();
    refreshStyles();
}

void TextBasedWidget::splitTextToLines(const sf::String& text)
{
    /*const auto maxWidth = m_rectangle.getSize().x - 2 * m_padding.x;
    sf::String text = text.getString();

    size_t wordStartPosition = 0;
    sf::Text textLine = m_text;
    sf::String line;

    while (m_text.getLocalBounds().width > maxWidth)
    {
        auto wordEndPosition = text.find(L" ", wordStartPosition);

        // It may happen that the word is too long to fit the widget
        // And it happens. TODO: split long words
        if (wordEndPosition == sf::String::InvalidPos)
            break;

        line += text.substring(wordStartPosition, wordEndPosition - wordStartPosition + 1);
        textLine.setString(line);
        std::cout << (std::string)line << '\n';

        if (textLine.getLocalBounds().width > maxWidth)
        {
            m_lines.emplace_back();
            m_lines.back().setString()

            line.clear();
        }
        else
            wordStartPosition = wordEndPosition + 1;
    }*/
}

void TextBasedWidget::placeText()
{
    const auto position = m_rectangle.getPosition();
    const auto size = m_rectangle.getSize();

    const auto& textSettings = m_theme->getTextSettings();
    const auto& metrics = textSettings.getFontMetrics();

    for (auto& line : m_lines)
    {
        sf::Vector2f textPosition;

        switch (textSettings.getHorizontalAlignment())
        {
            case TextHorizontalAlignment::Left:
                textPosition.x = position.x + m_padding.x;
                break;

            case TextHorizontalAlignment::Center:
                textPosition.x = position.x + (size.x - line.getLocalBounds().width) / 2.0f - line.getLocalBounds().left;
                break;

            case TextHorizontalAlignment::Right:
                textPosition.x = position.x + size.x - m_padding.x - line.getLocalBounds().width;
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

        // If coordinates are not integer, the text gets blurred
        textPosition.x = std::round(textPosition.x);
        textPosition.y = std::round(textPosition.y);

        line.setPosition(textPosition);
    }
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

    const auto oldView = target.getView();
    target.setView(view);

    for (const auto& line : m_lines)
        target.draw(line);

    target.setView(oldView);
}

PushButton::PushButton()
{
    //ctor
}

PushButton::~PushButton()
{
    //dtor
}

}
