#include "SfmlGui.hpp"
#include <iostream>

namespace SfGui
{

DecorationSettings::DecorationSettings(const sf::Color fillColor,
                                       const sf::Color textColor,
                                       const sf::Uint32 textStyle,
                                       const float outlineThickness,
                                       const sf::Color outlineColor,
                                       const sf::Texture* backgroundTexture) :
    m_fillColor(fillColor),
    m_textColor(textColor),
    m_textStyle(textStyle),
    m_outlineThickness(outlineThickness > 0 ? -outlineThickness : outlineThickness),
    m_outlineColor(outlineColor),
    m_backgroundTexture(backgroundTexture)
{
    //ctor
}

DecorationSettings::~DecorationSettings()
{
    //dtor
}

sf::Color DecorationSettings::getFillColor() const
{
    return m_fillColor;
}

sf::Color DecorationSettings::getTextColor() const
{
    return m_textColor;
}

sf::Uint32 DecorationSettings::getTextStyle() const
{
    return m_textStyle;
}

sf::Color DecorationSettings::getOutlineColor() const
{
    return m_outlineColor;
}

float DecorationSettings::getOutlineThickness() const
{
    return m_outlineThickness;
}

const sf::Texture* DecorationSettings::getBackgroundTexture() const
{
    return m_backgroundTexture;
}

void DecorationSettings::setFillColor(const sf::Color fillColor)
{
    m_fillColor = fillColor;
}

void DecorationSettings::setTextColor(const sf::Color textColor)
{
    m_textColor = textColor;
}

void DecorationSettings::setTextStyle(const sf::Uint32 textStyle)
{
    m_textStyle = textStyle;
}

void DecorationSettings::setOutlineThickness(const float outlineThickness)
{
    m_outlineThickness = outlineThickness > 0 ? -outlineThickness : outlineThickness;
}

void DecorationSettings::setOutlineColor(const sf::Color outlineColor)
{
    m_outlineColor = outlineColor;
}

void DecorationSettings::setBackgroundTexture(const sf::Texture* backgroundTexture)
{
    m_backgroundTexture = backgroundTexture;
}

TextSettings::TextSettings(const sf::Font& font,
                           const unsigned int characterSize,
                           const TextHorizontalAlignment horizontalAlignment,
                           const TextVerticalAlignment verticalAlignment) :
    m_font(&font),
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
    return *m_font;
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

void TextSettings::setFont(const sf::Font& font)
{
    m_font = &font;
    m_fontMetrics = calculateFontMetrics();
}

void TextSettings::setCharacterSize(const unsigned int characterSize)
{
    m_characterSize = characterSize;
    m_fontMetrics = calculateFontMetrics();
}

void TextSettings::setHorizontalAlignment(const TextHorizontalAlignment horizontalAlignment)
{
    m_horizontalAlignment = horizontalAlignment;
}

void TextSettings::setVerticalAlignment(const TextVerticalAlignment verticalAlignment)
{
    m_verticalAlignment = verticalAlignment;
}

FontMetrics TextSettings::calculateFontMetrics()
{
    FontMetrics metrics;

    sf::Text temp;
    temp.setFont(*m_font);
    temp.setCharacterSize(m_characterSize);

    // Just some magic to apply vertical alignment correctly
    // (some fonts will have it a little incorrect)

    // We calculate the highest line and the line which all letters stand on.
    // We use "A" as the standard
    temp.setString(L"A");
    metrics.ascenderLine = temp.getLocalBounds().top;
    metrics.baseLine = temp.getLocalBounds().height;

    // We calculate the lowest line of letters and, at last, maximum possible height
    // We use "j" as the standard
    temp.setString(L"j");
    metrics.descenderLine = temp.getLocalBounds().height - metrics.baseLine;
    metrics.fullHeight = metrics.ascenderLine + metrics.baseLine + metrics.descenderLine;

    return metrics;
}

Theme::Theme(const TextSettings& textSettings,
             const DecorationSettings& idleColorSettings,
             const DecorationSettings& hoverColorSettings,
             const DecorationSettings& pressColorSettings) :
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

const DecorationSettings& Theme::getIdleColorSettings() const
{
    return m_idleColorSettings;
}

const DecorationSettings& Theme::getHoveredColorSettings() const
{
    return m_hoverColorSettings;
}

const DecorationSettings& Theme::getPressedColorSettings() const
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

Widget::Widget() : m_theme(nullptr), m_padding(5.0f, 10.0f), m_state(WidgetState::Idle), m_contentNeedsUpdate(true)
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
    m_contentNeedsUpdate = true;
}

void Widget::setSize(const sf::Vector2f& size)
{
    m_rectangle.setSize(size);
    m_contentNeedsUpdate = true;
}

void Widget::setTheme(const Theme& theme)
{
    m_theme = &theme;
    m_contentNeedsUpdate = true;
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

void Widget::forceStylesUpdate() const
{
    m_contentNeedsUpdate = true;
}

void Widget::refreshStyles() const
{
    const DecorationSettings* colorSettings;
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

TextBasedWidget::TextBasedWidget() : Widget(), m_isMultiline(false)
{
    //ctor
}

TextBasedWidget::~TextBasedWidget()
{
    //dtor
}

void TextBasedWidget::setPosition(const sf::Vector2f& position)
{
    m_rectangle.setPosition(position);
    m_contentNeedsUpdate = true;
}

void TextBasedWidget::refreshStyles() const
{
    const auto& textSettings = m_theme->getTextSettings();

    const DecorationSettings* colorSettings;
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
        line.setStyle(colorSettings->getTextStyle());
    }
}

sf::String TextBasedWidget::getString() const
{
    return m_string;
}

bool TextBasedWidget::getMultiline() const
{
    return m_isMultiline;
}

void TextBasedWidget::setString(const sf::String& text)
{
    m_string = text;
    m_contentNeedsUpdate = true;
}

void TextBasedWidget::setMultiline(bool isMultiline)
{
    m_isMultiline = isMultiline;
    m_contentNeedsUpdate = true;
}

void TextBasedWidget::updateTextSplitting() const
{
    m_lines.clear();
    if (!m_isMultiline)
    {
        m_lines.emplace_back();
        m_lines.back().setString(m_string);
        return;
    }

    const auto maxWidth = m_rectangle.getSize().x - 2 * m_padding.x;

    const auto& textSettings = m_theme->getTextSettings();
    sf::Text textLine;
    textLine.setFont(textSettings.getFont());
    textLine.setCharacterSize(textSettings.getCharacterSize());

    size_t wordStartPosition = 0;

    sf::String line;

    while (true)
    {
        auto wordEndPosition = m_string.find(L" ", wordStartPosition);

        if (wordEndPosition == sf::String::InvalidPos)
        {
            line += m_string.substring(wordStartPosition);
            textLine.setString(line);

            // TODO: split words that do not fit in the borders
            // Notice: if the line has words before this, start a new line
            /*while (textLine.getLocalBounds().width > maxWidth)
            {

            }*/

            m_lines.emplace_back();
            m_lines.back().setString(line);

            return;
        }

        line += m_string.substring(wordStartPosition, wordEndPosition - wordStartPosition + 1);
        textLine.setString(line);
        //std::cout << (std::string)line << '\n';

        if (textLine.getLocalBounds().width > maxWidth)
        {
            // Delete the last word and space before it
            line = line.substring(0, line.getSize() - (wordEndPosition - wordStartPosition + 1) - 1);
            m_lines.emplace_back();
            m_lines.back().setString(line);
            line.clear();
        }
        else
            wordStartPosition = wordEndPosition + 1;
    }
    for (const auto& line : m_lines)
        std::cout << '!' << (std::string)line.getString() << '!' << '\n';
}

void TextBasedWidget::placeText() const
{
    const auto position = m_rectangle.getPosition();
    const auto size = m_rectangle.getSize();

    const auto& textSettings = m_theme->getTextSettings();
    const auto horizontalAlignment = textSettings.getHorizontalAlignment();
    const auto verticalAlignment = textSettings.getVerticalAlignment();
    const auto& metrics = textSettings.getFontMetrics();

    sf::Vector2f textPosition;

    for (size_t i = 0; i < m_lines.size(); i++)
    {
        switch (horizontalAlignment)
        {
            case TextHorizontalAlignment::Left:
                textPosition.x = position.x + m_padding.x;
                break;

            case TextHorizontalAlignment::Center:
                textPosition.x = position.x + (size.x - m_lines[i].getLocalBounds().width) / 2.0f - m_lines[i].getLocalBounds().left;
                break;

            case TextHorizontalAlignment::Right:
                textPosition.x = position.x + size.x - m_padding.x - m_lines[i].getLocalBounds().width;
                break;
        }

        switch (verticalAlignment)
        {
            case TextVerticalAlignment::Top:
                textPosition.y = position.y + m_padding.y + i * metrics.fullHeight;
                break;

            case TextVerticalAlignment::Center:
                textPosition.y = position.y + (size.y - m_lines.size() * metrics.fullHeight - metrics.ascenderLine) / 2.0f + i * metrics.fullHeight;
                break;

            case TextVerticalAlignment::Bottom:
                textPosition.y = position.y + size.y - m_padding.y - (m_lines.size() - i) * metrics.fullHeight;
                break;
        }

        // If coordinates are not integer, the text gets blurred
        textPosition.x = std::round(textPosition.x);
        textPosition.y = std::round(textPosition.y);
        m_lines[i].setPosition(textPosition);
    }
}

void TextBasedWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_contentNeedsUpdate)
    {
        updateTextSplitting();
        refreshStyles();
        placeText();
        m_contentNeedsUpdate = false;
    }

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
